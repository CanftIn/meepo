#ifndef SCHEDULER_ASPECT_ASPECT_H
#define SCHEDULER_ASPECT_ASPECT_H

#include <mutex>

namespace Scheduler {

class Node;

/// 切面类，针对于节点之间数据上的动作。
class Aspect {
 public:
  virtual ~Aspect() = default;

  virtual void before(const Node& node) = 0;
  virtual void after(const Node& node) = 0;
};


std::mutex aspect_mutex;

}  // namespace Scheduler

#endif  // SCHEDULER_ASPECT_ASPECT_H