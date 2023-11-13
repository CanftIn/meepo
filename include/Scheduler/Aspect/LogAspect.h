#ifndef SCHEDULER_ASPECT_LOG_ASPECT_H
#define SCHEDULER_ASPECT_LOG_ASPECT_H

#include <chrono>
#include <iostream>
#include <map>

#include "Scheduler/Aspect/Aspect.h"
#include "Scheduler/Graph/Node.h"

namespace Scheduler {

class LogAspect : public Aspect {
 public:
  void before(const Node& node) override {
    std::lock_guard<std::mutex> lock(aspect_mutex);
    std::cout << "Starting node: " << node.get_name() << '\n';
  }

  void after(const Node& node) override {
    std::lock_guard<std::mutex> lock(aspect_mutex);
    std::cout << "Finished node: " << node.get_name() << '\n';
  }
};

}  // namespace Scheduler

#endif  // SCHEDULER_ASPECT_LOG_ASPECT_H