#ifndef SCHEDULER_TASK_TASK_H
#define SCHEDULER_TASK_TASK_H

#include <iostream>
#include <string>
#include <utility>

namespace Scheduler {

class Task {
 public:
  virtual ~Task() = default;

  virtual void execute() const = 0;
};

class SimpleTask : public Task {
 public:
  explicit SimpleTask(std::string message) : message_(std::move(message)) {}

  void execute() const override {
    std::cout << "Executing SimpleTask: " << message_ << '\n';
  }

 private:
  std::string message_;
};

class ComplexTask : public Task {
 public:
  void execute() const override {
    std::cout << "Executing ComplexTask" << '\n';
  }
};

}  // namespace Scheduler

#endif  // SCHEDULER_TASK_TASK_H