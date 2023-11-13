#include "Scheduler/Graph/Graph.h"

#include <gtest/gtest.h>

#include <thread>

namespace Scheduler::Unittests {

TEST(GraphTest, DAGExecution) {
  auto task1 = make_function_task({[] { std::cout << "task1.1" << '\n'; },
                                   [] { std::cout << "task1.2" << '\n'; }});

  auto task2 = make_function_task({[] { std::cout << "task2.1" << '\n'; },
                                   [] { std::cout << "task2.2" << '\n'; }});

  auto task3 = make_function_task({[] { std::cout << "task3.1" << '\n'; },
                                   [] { std::cout << "task3.2" << '\n'; }});

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