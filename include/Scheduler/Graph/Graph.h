#ifndef SCHEDULER_GRAPH_GRAPH_H
#define SCHEDULER_GRAPH_GRAPH_H

#include <memory>
#include <mutex>
#include <vector>

#include "Scheduler/Graph/Node.h"

namespace Scheduler {

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
  void add_node(std::shared_ptr<Node> node) {
    std::lock_guard<std::mutex> lock(mutex_);
    nodes_.push_back(std::move(node));
  }

  /// 获取图的大小（节点数量）
  auto size() const -> std::size_t {
    std::lock_guard<std::mutex> lock(mutex_);
    return nodes_.size();
  }

  // 执行图中所有节点的任务
  void execute_tasks() const {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& node : nodes_) {
      execute_node_task(node.get());
    }
  }

  // 打印图的形状
  void print() const {
    // TODO
  }

 private:
  // 递归执行节点及其下游节点的任务
  void execute_node_task(Node* node) const {
    if (node) {
      for (const auto& downstream : node->get_downstream_nodes()) {
        execute_node_task(downstream.get());
      }
      node->execute_task();
    }
  }

  mutable std::mutex mutex_;
  std::vector<std::shared_ptr<Node>> nodes_;
};

}  // namespace Scheduler

#endif  // SCHEDULER_GRAPH_GRAPH_H