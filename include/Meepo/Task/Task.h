#ifndef MEEPO_TASK_TASK_H
#define MEEPO_TASK_TASK_H

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <variant>

#include "Meepo/Graph/Parameter.h"

namespace Meepo {

class Task {
 public:
  virtual ~Task() = default;

  virtual void process(Parameter& params) const = 0;
};

class SerialTask : public Task {
 public:
  SerialTask(const std::vector<std::shared_ptr<Task>>& tasks)  // NOLINT(*)
      : tasks_(tasks) {}

  void process(Parameter& params) const override {
    for (const auto& task : tasks_) {
      task->process(params);
    }
  }

 private:
  std::vector<std::shared_ptr<Task>> tasks_;
};

class ParallelTask : public Task {
 public:
  ParallelTask(const std::vector<std::shared_ptr<Task>>& tasks)  // NOLINT(*)
      : tasks_(tasks) {}

  void process(Parameter& params) const override {
    std::vector<std::thread> threads;
    threads.reserve(tasks_.size());
    for (const auto& task : tasks_) {
      threads.emplace_back([&task, &params]() { task->process(params); });
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
  using Callback = std::function<void()>;
  using ParamCallback = std::function<void(const Parameter&)>;
  using ParamReturnCallback = std::function<Parameter(const Parameter&)>;

  using TaskVariant =
      std::variant<Callback, ParamCallback, ParamReturnCallback>;

  explicit FunctionTask(TaskVariant task) : task_(std::move(task)) {}

  void process(Parameter& params) const override {
    std::visit(
        [&params](auto&& callback) {
          using T = std::decay_t<decltype(callback)>;
          if constexpr (std::is_same_v<T, Callback>) {
            callback();
          } else if constexpr (std::is_same_v<T, ParamCallback>) {
            callback(params);
          } else if constexpr (std::is_same_v<T, ParamReturnCallback>) {
            params = callback(params);
          }
        },
        task_);
  }

 private:
  TaskVariant task_;
};

inline auto make_function_serial_task(
    const std::initializer_list<FunctionTask::TaskVariant>& callbacks)
    -> std::shared_ptr<Task> {
  std::vector<std::shared_ptr<Task>> tasks;
  for (const auto& callback : callbacks) {
    tasks.push_back(std::make_shared<FunctionTask>(callback));
  }
  return std::make_shared<SerialTask>(tasks);
}

inline auto make_function_parallel_task(
    const std::initializer_list<FunctionTask::TaskVariant>& callbacks)
    -> std::shared_ptr<Task> {
  std::vector<std::shared_ptr<Task>> tasks;
  for (const auto& callback : callbacks) {
    tasks.push_back(std::make_shared<FunctionTask>(callback));
  }
  return std::make_shared<ParallelTask>(tasks);
}

}  // namespace Meepo

#endif  // MEEPO_TASK_TASK_H