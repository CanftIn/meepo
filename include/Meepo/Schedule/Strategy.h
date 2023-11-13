#ifndef MEEPO_SCHEDULE_STRATEGY_H
#define MEEPO_SCHEDULE_STRATEGY_H

#include <memory>
#include <vector>

#include "Meepo/Graph/Node.h"

namespace Meepo {

class Strategy {
 public:
  virtual ~Strategy() = default;
  virtual void execute(const std::vector<std::shared_ptr<Node>>& nodes) = 0;
};

class PriorityStrategy : public Strategy {
 public:
  void execute(const std::vector<std::shared_ptr<Node>>& nodes) override {
    // 对节点按名称排序
    auto sorted_nodes = nodes;
    std::sort(
        sorted_nodes.begin(), sorted_nodes.end(),
        [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
          return a->get_name() < b->get_name();
        });

    for (auto& node : sorted_nodes) {
      node->execute_task();
    }
  }
};

class RoundRobinStrategy : public Strategy {
 public:
  void execute(const std::vector<std::shared_ptr<Node>>& nodes) override {
    // 简单的轮询调度
    for (const auto& node : nodes) {
      node->execute_task();
    }
  }
};

}  // namespace Meepo

#endif  // MEEPO_SCHEDULE_STRATEGY_H