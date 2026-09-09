#include "threadpool/threadpool.h"

ThreadPool::ThreadPool(size_t num_threads) {
  for (size_t i = 0; i < num_threads; ++i) {
    m_workers.emplace_back([this]() {
      std::function<void()> task;
      while (m_queue.pop(task)) {
        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() { m_queue.stop(); }

void ThreadPool::submit(std::function<void()> task) {
  m_queue.push(std::move(task));
}
