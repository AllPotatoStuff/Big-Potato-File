#ifndef SCANNER_H__
#define SCANNER_H__

#include "threadpool/concurrentqueue.h"
#include "index/indexer.h" // for FileInfo
#include "threadpool/threadpool.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

/**
 * @struct ResultEndScan
 * @brief Structure to hold the result of scanning a directory.
 * @param totalMb The total size of files in the directory in megabytes.
 * @param fileCount The number of files in the directory.
 */
struct ResultEndScan {
  double totalMb;
  int fileCount;
  int folderCount;

  ResultEndScan() : totalMb(0.0), fileCount(0), folderCount(0) {}
};

/**
 * @struct Node
 * @brief Structure to represent a node in the directory tree.
 * @param dir The directory path.
 * @param fileCount The number of files in the directory.
 * @param sizeMb The total size of files in the directory in megabytes.
 * @param left Pointer to the left child node.
 * @param right Pointer to the right child node.
 */
struct Node {
  std::string dir;
  int fileCount;
  double sizeMb;
  std::vector<Node *> children;

  Node() : dir(""), fileCount(0), sizeMb(0.0) {}
};

/**
 * @class Scanner
 * @brief Class to scan directories and build a directory tree.
 */
class Scanner {
private:
  Node *m_root;

  std::mutex m_resMtx;
  std::mutex m_doneMtx;
  std::condition_variable m_doneCv;
  std::atomic<int> m_pendingTasks{0};

private:
  void insertNode(Node *&node, ResultEndScan &res);
  // Private member function to free the memory allocated for the directory
  // tree.
  void freeTree(Node *node);

  void scanDirTask(const std::string &dir, ResultEndScan &globalRes,
                   ThreadPool &pool, ConcurrentQueue<FileInfo> &outQueue);

public:
  Scanner();
  ~Scanner();

  ResultEndScan PathScannerSingleThread(std::string dir = "C:\\");

  ResultEndScan PathScannerMultiThread(const std::string &dir, ThreadPool &pool,
                                       ConcurrentQueue<FileInfo> &outQueue);
};

#endif // SCANNER_H__