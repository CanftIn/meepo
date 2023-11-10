#ifndef SCHEDULER_GRAPH_NODE_H
#define SCHEDULER_GRAPH_NODE_H

#include <condition_variable>
#include <memory>
#include <mutex>
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
    wait_upstreams();
    if (task_) {
      task_->execute();
    }
    notify_downstreams();
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

  /// 通知所有下游节点。
  void notify_downstreams() const {
    for (const auto& downstream : downstream_nodes_) {
      std::unique_lock<std::mutex> lock(downstream->mutex_);
      downstream->upstream_completed_++;
      downstream->cv_.notify_one();
    }
  }

  /// 等待所有上游节点完成。
  void wait_upstreams() const {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock,
             [this] { return upstream_completed_ == upstream_nodes_.size(); });
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
  // 同步相关变量。
  mutable std::mutex mutex_;
  mutable std::condition_variable cv_;
  // 已完成的上游节点。
  mutable int upstream_completed_ = 0;
};

}  // namespace Scheduler

#endif  // SCHEDULER_GRAPH_NODE_H