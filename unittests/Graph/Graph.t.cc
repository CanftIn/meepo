#include "Scheduler/Graph/Graph.h"

#include <gtest/gtest.h>

#include <thread>

namespace Scheduler::Unittests {

TEST(GraphTest, DAGExecution) {
  std::shared_ptr<Task> task1 =
      std::make_shared<FunctionTask>([] { std::cout << "task1" << '\n'; });
  std::shared_ptr<Task> task2 =
      std::make_shared<FunctionTask>([] { std::cout << "task2" << '\n'; });
  std::shared_ptr<Task> task3 =
      std::make_shared<FunctionTask>([] { std::cout << "task3" << '\n'; });

  auto node1 = std::make_shared<Node>("node1", task1);
  auto node2 = std::make_shared<Node>("node2", task2);
  auto node3 = std::make_shared<Node>("node3", task3);

  node1->add_downstream_node(node3);
  node2->add_downstream_node(node3);
  node3->add_upstream_node(node1);
  node3->add_upstream_node(node2);

  Graph graph;
  graph.add_node(node1);
  graph.add_node(node2);
  graph.add_node(node3);

  try {
    graph.execute_tasks();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
  }
}

}  // namespace Scheduler::Unittests