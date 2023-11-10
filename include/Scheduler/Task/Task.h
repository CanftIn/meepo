#ifndef SCHEDULER_TASK_TASK_H
#define SCHEDULER_TASK_TASK_H

#include <functional>
#include <iostream>
#include <string>
#include <utility>

namespace Scheduler {

class Task {
 public:
  virtual ~Task() = default;

  virtual void execute() const = 0;
};

class FunctionTask : public Task {
 public:
  explicit FunctionTask(const std::function<void()>& callback)
      : callback_(callback) {}

  void execute() const override {
    if (callback_) {
      callback_();
    }
  }

 private:
  std::function<void()> callback_;
};

}  // namespace Scheduler

#endif  // SCHEDULER_TASK_TASK_H