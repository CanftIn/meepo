#ifndef MEEPO_SCHEDULE_PIPELINE_H
#define MEEPO_SCHEDULE_PIPELINE_H

#include <memory>
#include <vector>

#include "Meepo/Graph/Node.h"

namespace Meepo {

class Pipeline {
 public:
  void add_node(std::shared_ptr<Node> node) {
    if (!nodes_.empty()) {
      nodes_.back()->add_downstream_node(node);
      node->add_upstream_node(nodes_.back());
    }
    nodes_.push_back(node);
  }

  auto get_nodes() const -> const std::vector<std::shared_ptr<Node>>& {
    return nodes_;
  }

  void execute() {
    if (!nodes_.empty()) {
      nodes_.front()->execute_task();
    }
  }

 private:
  std::vector<std::shared_ptr<Node>> nodes_;
};

}  // namespace Meepo

#endif  // MEEPO_SCHEDULE_PIPELINE_H