#include "index/indexer.h"

Indexer::Indexer(ConcurrentQueue<FileInfo> &queue, const std::string &dbPath)
    : m_queue(queue), m_db(dbPath) {}

Indexer::~Indexer() {
  if (m_worker.joinable())
    m_worker.join();
}

void Indexer::flushBatch(std::vector<FileInfo> &batch) {
  m_db.insertBatch(batch);
  m_indexedCount += batch.size();
  batch.clear();
}

void Indexer::run() {
  std::vector<FileInfo> batch;
  batch.reserve(kBatchSize);
  FileInfo info;

  // pop() only returns false when the queue is stopped AND empty,
  // so this loop absorb everything that Scanner produce, even
  // what happens after stop() until the buffer is empty
  while (m_queue.pop(info)) {
    batch.push_back(std::move(info));
    if (batch.size() >= kBatchSize)
      flushBatch(batch);
  }

  if (!batch.empty())
    flushBatch(batch);
}

void Indexer::start() {
  m_worker = std::thread([this]() { run(); });
}

void Indexer::join() {
  if (m_worker.joinable())
    m_worker.join();
}