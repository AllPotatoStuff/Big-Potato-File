#ifndef TASKQUEUE_H__
#define TASKQUEUE_H__

#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>

class TaskQueue {
private:
  std::queue<std::function<void()>> m_tasks;
  std::mutex m_mtx;
  std::condition_variable m_cv;
  bool m_isStopping = false;

public:
  TaskQueue();
  ~TaskQueue();

  void push(std::function<void()> task);
  bool pop(std::function<void()> &task);
  void stop();
};

#endif // TASKQUEUE_H__