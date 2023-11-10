#ifndef SCHEDULER_GRAPH_NODE_H
#define SCHEDULER_GRAPH_NODE_H

#include <memory>
#include <utility>
#include <vector>

#include "Scheduler/Task/Task.h"

namespace Scheduler {

class Node {
 public:
  Node(std::string name, std::shared_ptr<Task> task)
      : name_(std::move(name)), task_(std::move(task)) {}

  auto get_name() const -> std::string { return name_; }

  void execute_task() const {
    if (task_) {
      task_->execute();
    }
  }

  void add_upstream_node(std::shared_ptr<Node> node) {
    upstream_nodes_.push_back(node);
  }

  auto get_upstream_nodes() const -> const std::vector<std::shared_ptr<Node>>& {
    return upstream_nodes_;
  }

  void add_downstream_node(std::shared_ptr<Node> node) {
    downstream_nodes_.push_back(node);
  }

  auto get_downstream_nodes() const
      -> const std::vector<std::shared_ptr<Node>>& {
    return downstream_nodes_;
  }

 private:
  // 节点名。
  std::string name_;
  // 任务。
  std::shared_ptr<Task> task_;
  // 上游节点。
  std::vector<std::shared_ptr<Node>> upstream_nodes_;
  // 下游节点。
  std::vector<std::shared_ptr<Node>> downstream_nodes_;
};

}  // namespace Scheduler

#endif  // SCHEDULER_GRAPH_NODE_H