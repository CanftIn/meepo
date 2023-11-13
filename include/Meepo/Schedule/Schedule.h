#ifndef MEEPO_SCHEDULE_SCHEDULE_H
#define MEEPO_SCHEDULE_SCHEDULE_H

#include "Meepo/Schedule/Pipeline.h"
#include "Meepo/Schedule/Strategy.h"

namespace Meepo {

class Schedule {
 public:
  Schedule(std::shared_ptr<Strategy> strategy)  // NOLINT(*)
      : strategy_(std::move(strategy)) {}

  void add_pipeline(std::shared_ptr<Pipeline> pipeline) {
    pipelines_.push_back(pipeline);
  }

  void execute() {
    std::vector<std::shared_ptr<Node>> nodes;
    for (const auto& pipeline : pipelines_) {
      auto pipeline_nodes = pipeline->get_nodes();
      nodes.insert(nodes.end(), pipeline_nodes.begin(), pipeline_nodes.end());
    }
    strategy_->execute(nodes);
  }

 private:
  std::shared_ptr<Strategy> strategy_;
  std::vector<std::shared_ptr<Pipeline>> pipelines_;
};

}  // namespace Meepo

#endif  // MEEPO_SCHEDULE_SCHEDULE_H