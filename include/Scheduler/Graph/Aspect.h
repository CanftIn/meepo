#ifndef SCHEDULER_GRAPH_ASPECT_H
#define SCHEDULER_GRAPH_ASPECT_H

namespace Scheduler {

class Node;

/// 切面类，针对于节点之间数据上的动作。
class Aspect {
 public:
  virtual ~Aspect() = default;

  virtual void before(const Node& node) = 0;
  virtual void after(const Node& node) = 0;
};

}  // namespace Scheduler

#endif  // SCHEDULER_GRAPH_ASPECT_H