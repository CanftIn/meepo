#ifndef MEEPO_EXECUTOR_EXECUTOR_H
#define MEEPO_EXECUTOR_EXECUTOR_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Meepo/Graph/Node.h"

namespace Meepo {

class Executor {
 public:
  static auto get_instance(size_t thread_count = 5) -> Executor& {
    static Executor instance(thread_count);
    return instance;
  }

  void submit(std::shared_ptr<Node> node) {
    auto task_function = [node]() { node->execute_task(); };
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      if (stop_) {
        throw std::runtime_error("Executor is stopped");
      }
      tasks_.emplace(task_function);
    }
    condition_.notify_one();
  }

 private:
  Executor(size_t thread_count) : stop_(false) {  // NOLINT(*)
    for (size_t i = 0; i < thread_count; ++i) {
      workers_.emplace_back([this] {
        // 不断地检查是否有新任务需要执行
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(this->queue_mutex_);
            this->condition_.wait(
                lock, [this] { return this->stop_ || !this->tasks_.empty(); });
            if (this->stop_ && this->tasks_.empty()) {
              return;
            }
            task = std::move(this->tasks_.front());
            this->tasks_.pop();
          }
          task();
        }
      });
    }
  }

  Executor(const Executor&) = delete;
  auto operator=(const Executor&) -> Executor& = delete;

  ~Executor() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      stop_ = true;
    }
    condition_.notify_all();
    for (std::thread& worker : workers_) {
      worker.join();
    }
  }

  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

}  // namespace Meepo

#endif  // MEEPO_EXECUTOR_EXECUTOR_H