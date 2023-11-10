#ifndef SCHEDULER_GRAPH_GRAPH_H
#define SCHEDULER_GRAPH_GRAPH_H

#include <memory>
#include <vector>

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

class Graph {
 public:
  Graph() = default;

  Graph(const Graph&) = delete;
  auto operator=(const Graph&) -> Graph& = delete;

  Graph(Graph&& other) noexcept {
    nodes_ = std::move(other.nodes_);
    other.nodes_.clear();
  }
  auto operator=(Graph&& other) noexcept -> Graph& {
    if (this != &other) {
      nodes_ = std::move(other.nodes_);
      other.nodes_.clear();
    }
    return *this;
  }

  /// 添加节点。
  void add_node(std::unique_ptr<Node> node) {
    nodes_.push_back(std::move(node));
  }

  /// 获取图的大小（节点数量）
  auto size() const -> std::size_t { return nodes_.size(); }

  // 执行图中所有节点的任务
  void execute_tasks() const {
    for (const auto& node : nodes_) {
      node->execute_task();
    }
  }

 private:
  std::vector<std::unique_ptr<Node>> nodes_;
};

}  // namespace Scheduler

#endif  // SCHEDULER_GRAPH_GRAPH_H