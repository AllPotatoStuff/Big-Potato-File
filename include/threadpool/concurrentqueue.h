#ifndef CONCURRENTQUEUE_H__
#define CONCURRENTQUEUE_H__

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class ConcurrentQueue {
private:
  std::queue<T> m_items;
  std::mutex m_mtx;
  std::condition_variable m_cv;
  bool m_isStopping = false;

public:
  ConcurrentQueue() = default;
  ~ConcurrentQueue() { stop(); }

  ConcurrentQueue(const ConcurrentQueue &) = delete;
  ConcurrentQueue &operator=(const ConcurrentQueue &) = delete;

  void push(T item) {
    {
      std::lock_guard<std::mutex> lock(m_mtx);
      m_items.push(std::move(item));
    }
    m_cv.notify_one();
  }

  bool pop(T &item) {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this]() { return !m_items.empty() || m_isStopping; });
    if (m_items.empty() && m_isStopping)
      return false;
    item = std::move(m_items.front());
    m_items.pop();
    return true;
  }

  void stop() {
    {
      std::lock_guard<std::mutex> lock(m_mtx);
      m_isStopping = true;
    }
    m_cv.notify_all();
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex &>(m_mtx));
    return m_items.size();
  }
};

#endif // CONCURRENTQUEUE_H__