#include "filesystem/scanner.h"

namespace fs = std::filesystem;

namespace {
// Convert a file_time_type into Unix timestamp (int64_t).
int64_t toUnixTime(fs::file_time_type ft) {
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(
      ft - fs::file_time_type::clock::now() + system_clock::now());
  return duration_cast<seconds>(sctp.time_since_epoch()).count();
}
} // namespace

Scanner::Scanner() : m_root(nullptr) {}

Scanner::~Scanner() { freeTree(m_root); }

void Scanner::insertNode(Node *&node, ResultEndScan &res) {
  if (node == nullptr)
    return;

  try {
    fs::path p(node->dir);
    if (!fs::exists(p) || !fs::is_directory(p))
      return;

    for (const auto &entry : fs::directory_iterator(p)) {
      if (fs::is_regular_file(entry.path())) {
        node->fileCount++;
        node->sizeMb += static_cast<double>(fs::file_size(entry.path())) /
                        (1024.0 * 1024.0);
      } else if (fs::is_directory(entry.path())) {
        Node *childNode = new Node();
        childNode->dir = entry.path().string();
        node->children.push_back(childNode);
      }
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Access denied or error: " << e.what() << "\n";
  }

  res.totalMb += node->sizeMb;
  res.fileCount += node->fileCount;
  res.folderCount++;

  for (Node *child : node->children)
    insertNode(child, res);
}

ResultEndScan Scanner::PathScannerSingleThread(std::string dir) {
  if (m_root != nullptr) {
    freeTree(m_root);
    m_root = nullptr;
  }

  dir = dir.empty() ? "C:\\" : dir;

  ResultEndScan res;
  m_root = new Node();
  m_root->dir = dir;

  insertNode(m_root, res);

  return res;
}

ResultEndScan
Scanner::PathScannerMultiThread(const std::string &dir, ThreadPool &pool,
                                ConcurrentQueue<FileInfo> &outQueue) {
  ResultEndScan res;
  m_pendingTasks = 1;

  pool.submit([this, dir, &res, &pool, &outQueue]() {
    scanDirTask(dir, res, pool, outQueue);
  });

  std::unique_lock<std::mutex> lock(m_doneMtx);
  m_doneCv.wait(lock, [this]() { return m_pendingTasks.load() == 0; });

  return res;
}

void Scanner::freeTree(Node *node) {
  if (node == nullptr)
    return;
  for (Node *child : node->children)
    freeTree(child);
  delete node;
}

void Scanner::scanDirTask(const std::string &dir, ResultEndScan &globalRes,
                          ThreadPool &pool,
                          ConcurrentQueue<FileInfo> &outQueue) {
  ResultEndScan localRes;

  try {
    fs::path p(dir);
    if (fs::exists(p) && fs::is_directory(p)) {
      for (const auto &entry : fs::directory_iterator(p)) {
        const auto &path = entry.path();

        if (fs::is_regular_file(path)) {
          uint64_t sizeBytes = fs::file_size(path);

          localRes.fileCount++;
          localRes.totalMb +=
              static_cast<double>(sizeBytes) / (1024.0 * 1024.0);

          FileInfo info;
          info.path = path.string();
          info.name = path.filename().string();
          info.size = sizeBytes;
          info.isDirectory = false;
          try {
            info.modifiedAt = toUnixTime(fs::last_write_time(path));
          } catch (...) {
            info.modifiedAt = 0;
          }
          info.createdAt =
              info.modifiedAt; // std::filesystem doesn't expose the creation
                               // date of the file portably
          outQueue.push(std::move(info));

        } else if (fs::is_directory(path)) {
          localRes.folderCount++;

          FileInfo dirInfo;
          dirInfo.path = path.string();
          dirInfo.name = path.filename().string();
          dirInfo.size = 0;
          dirInfo.isDirectory = true;
          outQueue.push(std::move(dirInfo));

          std::string childDir = path.string();
          m_pendingTasks++;
          pool.submit([this, childDir, &globalRes, &pool, &outQueue]() {
            scanDirTask(childDir, globalRes, pool, outQueue);
          });
        }
      }
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Access denied or error: " << e.what() << "\n";
  }

  {
    std::lock_guard<std::mutex> lock(m_resMtx);
    globalRes.totalMb += localRes.totalMb;
    globalRes.fileCount += localRes.fileCount;
    globalRes.folderCount += localRes.folderCount;
  }

  if (--m_pendingTasks == 0) {
    outQueue.stop();
    std::lock_guard<std::mutex> lock(m_doneMtx);
    m_doneCv.notify_one();
  }
}