#ifndef SCHEDULER_TASK_TASK_H
#define SCHEDULER_TASK_TASK_H

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

namespace Scheduler {

class Task {
 public:
  virtual ~Task() = default;

  virtual void process() const = 0;
};

class SerialTask : public Task {
 public:
  SerialTask(const std::vector<std::shared_ptr<Task>>& tasks)  // NOLINT(*)
      : tasks_(tasks) {}

  void process() const override {
    for (const auto& task : tasks_) {
      task->process();
    }
  }

 private:
  std::vector<std::shared_ptr<Task>> tasks_;
};

class ParallelTask : public Task {
 public:
  ParallelTask(const std::vector<std::shared_ptr<Task>>& tasks)  // NOLINT(*)
      : tasks_(tasks) {}

  void process() const override {
    std::vector<std::thread> threads;
    threads.reserve(tasks_.size());
    for (const auto& task : tasks_) {
      threads.emplace_back([&task]() { task->process(); });
    }

    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

 private:
  std::vector<std::shared_ptr<Task>> tasks_;
};

class FunctionTask : public Task {
 public:
  explicit FunctionTask(std::function<void()> callback)
      : callback_(std::move(callback)) {}

  void process() const override {
    if (callback_) {
      callback_();
    }
  }

 private:
  std::function<void()> callback_;
};

inline auto make_function_task(
    const std::initializer_list<std::function<void()>>& callbacks)
    -> std::shared_ptr<Task> {
  std::vector<std::shared_ptr<Task>> tasks;
  for (const auto& callback : callbacks) {
    tasks.push_back(std::make_shared<FunctionTask>(callback));
  }
  return std::make_shared<SerialTask>(tasks);
}

}  // namespace Scheduler

#endif  // SCHEDULER_TASK_TASK_H