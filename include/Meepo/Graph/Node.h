#ifndef MEEPO_GRAPH_NODE_H
#define MEEPO_GRAPH_NODE_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "Meepo/Aspect/Aspect.h"
#include "Meepo/Graph/Parameter.h"
#include "Meepo/Task/Task.h"

namespace Meepo {

class Node {
 public:
  Node(std::string name, std::shared_ptr<Task> task)
      : name_(std::move(name)), task_(std::move(task)) {}

  auto get_name() const -> std::string { return name_; }

  void execute_task() {
    wait_upstreams();

    Parameter params_copy;

    // 合并上游节点的参数
    for (const auto& upstream : upstream_nodes_) {
      const auto& upstream_params = upstream->get_parameter();
      for (const auto& param : upstream_params) {
        params_copy.set(param.first, param.second);
      }
    }

    if (aspect_) {
      aspect_->before(*this);
    }

    if (task_) {
      task_->process(params_copy);
    }

    if (aspect_) {
      aspect_->after(*this);
    }

    params_ = params_copy;

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

  void set_aspect(std::unique_ptr<Aspect> aspect) {
    aspect_ = std::move(aspect);
  }

  auto get_parameter() const -> Parameter {
    std::lock_guard<std::mutex> guard(mutex_);
    return params_;
  }

  void set_parameter(const Parameter& params) {
    std::lock_guard<std::mutex> guard(mutex_);
    params_ = params;
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
  // 切面动作
  std::unique_ptr<Aspect> aspect_;
  // 节点参数
  Parameter params_;
  // 同步相关变量。
  mutable std::mutex mutex_;
  mutable std::condition_variable cv_;
  // 已完成的上游节点。
  mutable int upstream_completed_ = 0;
};

}  // namespace Meepo

#endif  // MEEPO_GRAPH_NODE_H