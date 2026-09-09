#include "threadpool/taskqueue.h"

TaskQueue::TaskQueue() {}

TaskQueue::~TaskQueue() {}

void TaskQueue::push(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> mutex(m_mtx);
    m_tasks.push(std::move(task));
  }
  m_cv.notify_one();
}

bool TaskQueue::pop(std::function<void()> &task) {
  std::unique_lock<std::mutex> lock(m_mtx);

  m_cv.wait(lock, [this]() { return !m_tasks.empty() || m_isStopping; });

  if (m_tasks.empty() && m_isStopping) {
    return false;
  }

  task = std::move(m_tasks.front());
  m_tasks.pop();
  return true;
}

void TaskQueue::stop() {
  {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_isStopping = true;
  }
  m_cv.notify_all();
}
