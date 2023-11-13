#ifndef MEEPO_ASPECT_LOG_ASPECT_H
#define MEEPO_ASPECT_LOG_ASPECT_H

#include <chrono>
#include <iostream>
#include <map>

#include "Meepo/Aspect/Aspect.h"
#include "Meepo/Graph/Node.h"

namespace Meepo {

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

}  // namespace Meepo

#endif  // MEEPO_ASPECT_LOG_ASPECT_H