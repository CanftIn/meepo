#ifndef MEEPO_GRAPH_GRAPH_H
#define MEEPO_GRAPH_GRAPH_H

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "Meepo/Graph/Node.h"
#include "Meepo/Executor/Executor.h"

namespace Meepo {

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

  /// 执行图中所有节点的任务
  void execute_tasks() {
    std::lock_guard<std::mutex> lock(mutex_);
    // 检测循环依赖
    if (detect_cycle()) {
      throw std::runtime_error("Detected circular dependencies in the graph.");
    }

    Executor& executor = Executor::get_instance();
    for (const auto& node : nodes_) {
      executor.submit(node);
    }
  }

  /// 打印图的形状
  void print() const {
    // TODO
  }

 private:
  /// 检测循环依赖。
  /// 通过迭代遍历图中的每个节点，如果发现某个节点尚未被访问，
  /// 则调用 has_cycle_by_dfs 函数进行深度优先搜索检测循环依赖。
  auto detect_cycle() const -> bool {
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, bool> recursion_stack;

    for (const auto& node : nodes_) {
      if (!visited[node->get_name()]) {
        if (has_cycle_by_dfs(node->get_name(), visited, recursion_stack)) {
          return true;
        }
      }
    }

    return false;
  }

  /// 检测循环依赖的核心部分。
  auto has_cycle_by_dfs(
      const std::string& node_name,
      std::unordered_map<std::string, bool>& visited,
      std::unordered_map<std::string, bool>& recursion_stack) const -> bool {
    // 当前节点标记为已访问。
    visited[node_name] = true;
    recursion_stack[node_name] = true;

    // 遍历所有下游节点，检查它们是否依赖于当前节点，并且没有被访问过。
    for (const auto& downstream : nodes_) {
      if (is_dependent(node_name, downstream->get_name())) {
        if (!visited[downstream->get_name()] &&
            has_cycle_by_dfs(downstream->get_name(), visited,
                             recursion_stack)) {
          return true;
        } else if (recursion_stack[downstream->get_name()]) {
          return true;
        }
      }
    }

    recursion_stack[node_name] = false;
    return false;
  }

  /// 检查两个节点之间是否存在依赖关系。
  auto is_dependent(const std::string& source, const std::string& target) const
      -> bool {
    // 遍历图中的所有节点，查找名称为 target 的节点，然后检查它是否有一个
    // 上游节点的名称与 source 相匹配
    for (const auto& node : nodes_) {
      if (node->get_name() == target) {
        for (const auto& upstream : node->get_upstream_nodes()) {
          if (upstream->get_name() == source) {
            return true;
          }
        }
      }
    }
    return false;
  }

  mutable std::mutex mutex_;
  std::vector<std::shared_ptr<Node>> nodes_;
  std::vector<std::thread> threads_;
};

}  // namespace Meepo

#endif  // MEEPO_GRAPH_GRAPH_H