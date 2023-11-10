#include "Scheduler/Graph/Graph.h"

#include <gtest/gtest.h>

namespace Scheduler::Unittests {

TEST(GraphTest, GraphExecution) {
  std::shared_ptr<Task> simple_task =
      std::make_shared<SimpleTask>("SimpleTask");
  std::shared_ptr<Task> complex_task = std::make_shared<ComplexTask>();

  std::unique_ptr<Node> node1 = std::make_unique<Node>(1, simple_task);
  std::unique_ptr<Node> node2 = std::make_unique<Node>(2, complex_task);

  Graph graph;
  graph.add_node(std::move(node1));
  graph.add_node(std::move(node2));

  graph.execute_tasks();
}

}  // namespace Scheduler::Unittests