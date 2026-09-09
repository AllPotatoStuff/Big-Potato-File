#ifndef INDEXER_H__
#define INDEXER_H__

#include "database/database.h"
#include "index/fileinfo.h"
#include "threadpool/concurrentqueue.h"
#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>

/**
 * @class Indexer
 * @brief Consume a ConcurrentQueue<FileInfo> produced by Scanner
 *        and insert them by transactionnal batchs.
 *        Run on its own thread to never block scanning thread on the disk
 */
class Indexer {
private:
  ConcurrentQueue<FileInfo> &m_queue;
  IndexDatabase m_db;
  std::thread m_worker;
  std::atomic<uint64_t> m_indexedCount{0};

  static constexpr size_t kBatchSize = 1000;

  void run();
  void flushBatch(std::vector<FileInfo> &batch);

public:
  explicit Indexer(ConcurrentQueue<FileInfo> &queue, const std::string &dbPath);
  ~Indexer();

  Indexer(const Indexer &) = delete;
  Indexer &operator=(const Indexer &) = delete;

  void start();
  void join();

  uint64_t indexedCount() const { return m_indexedCount.load(); }
};

#endif // INDEXER_H__