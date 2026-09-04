#ifndef THREADPOOL_H__
#define THREADPOOL_H__

#include "taskqueue.h"
#include <functional>
#include <thread>

class ThreadPool {
private:
  TaskQueue m_queue;
  std::vector<std::jthread> m_workers;

public:
  ThreadPool(size_t num_threads);
  ~ThreadPool();

  void submit(std::function<void()> task);
};

#endif // THREADPOOL_H__