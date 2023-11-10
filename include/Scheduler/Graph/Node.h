#ifndef SCHEDULER_GRAPH_NODE_H
#define SCHEDULER_GRAPH_NODE_H

#include <memory>

#include "Scheduler/Task/Task.h"

namespace Scheduler {

class Node {
 public:
  Node(int id, std::shared_ptr<Task> task) : id_(id), task_(std::move(task)) {}

  auto get_id() const -> int { return id_; }

  void execute_task() const {
    if (task_) {
      task_->execute();
    }
  }

 private:
  int id_;
  std::shared_ptr<Task> task_;
};

}  // namespace Scheduler

#endif  // SCHEDULER_GRAPH_NODE_H