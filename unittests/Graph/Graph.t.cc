#include "Scheduler/Graph/Graph.h"

#include <gtest/gtest.h>

#include <thread>

namespace Scheduler::Unittests {

TEST(GraphTest, DAGExecution) {
  std::shared_ptr<Task> simple_task1 =
      std::make_shared<SimpleTask>("SimpleTask1");
  std::shared_ptr<Task> simple_task2 =
      std::make_shared<SimpleTask>("SimpleTask2");
  std::shared_ptr<Task> complex_task = std::make_shared<ComplexTask>();

  auto node1 = std::make_shared<Node>(1, simple_task1);
  auto node2 = std::make_shared<Node>(2, simple_task2);
  auto node3 = std::make_shared<Node>(3, complex_task);

  node1->add_downstream_node(node3);
  node2->add_downstream_node(node3);
  node3->add_upstream_node(node1);
  node3->add_upstream_node(node2);

  Graph graph;
  graph.add_node(node1);
  graph.add_node(node2);
  graph.add_node(node3);

  std::thread thread1([&graph] { graph.execute_tasks(); });
  std::thread thread2([&graph] { graph.execute_tasks(); });

  thread1.join();
  thread2.join();
}

}  // namespace Scheduler::Unittests