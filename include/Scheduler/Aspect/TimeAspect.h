#ifndef SCHEDULER_ASPECT_TIME_ASPECT_H
#define SCHEDULER_ASPECT_TIME_ASPECT_H

#include <chrono>
#include <iostream>
#include <map>

#include "Scheduler/Aspect/Aspect.h"
#include "Scheduler/Graph/Node.h"

namespace Scheduler {

class TimeAspect : public Aspect {
 public:
  void before(const Node& node) override {
    start_times_[&node] = std::chrono::high_resolution_clock::now();
  }

  void after(const Node& node) override {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto start_time = start_times_[&node];
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time)
                        .count();
    std::cout << "Node " << node.get_name() << " took " << duration
              << " ms to execute.\n";
  }

 private:
  std::map<const Node*, std::chrono::high_resolution_clock::time_point>
      start_times_;
};

}  // namespace Scheduler

#endif  // SCHEDULER_ASPECT_TIME_ASPECT_H