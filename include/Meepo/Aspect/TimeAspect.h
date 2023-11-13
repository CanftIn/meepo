#ifndef MEEPO_ASPECT_TIME_ASPECT_H
#define MEEPO_ASPECT_TIME_ASPECT_H

#include <chrono>
#include <iostream>
#include <map>

#include "Meepo/Aspect/Aspect.h"
#include "Meepo/Graph/Node.h"

namespace Meepo {

class TimeAspect : public Aspect {
 public:
  void before(const Node& node) override {
    std::lock_guard<std::mutex> lock(aspect_mutex);
    start_times_[&node] = std::chrono::high_resolution_clock::now();
  }

  void after(const Node& node) override {
    std::lock_guard<std::mutex> lock(aspect_mutex);
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

}  // namespace Meepo

#endif  // MEEPO_ASPECT_TIME_ASPECT_H