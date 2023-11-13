#include "Meepo/Graph/Graph.h"

#include <gtest/gtest.h>

#include "Meepo/Aspect/LogAspect.h"
#include "Meepo/Aspect/TimeAspect.h"

namespace Meepo::Unittests {

TEST(GraphTest, DAGExecution) {
  auto task1 =
      make_function_serial_task({[] { std::cout << "task1.1" << '\n'; },
                                 [] { std::cout << "task1.2" << '\n'; }});

  auto task2 =
      make_function_serial_task({[] { std::cout << "task2.1" << '\n'; },
                                 [] { std::cout << "task2.2" << '\n'; }});

  auto task3 =
      make_function_parallel_task({[] { std::cout << "task3.1" << '\n'; },
                                   [] { std::cout << "task3.2" << '\n'; }});

  auto node1 = std::make_shared<Node>("node1", task1);
  auto node2 = std::make_shared<Node>("node2", task2);
  auto node3 = std::make_shared<Node>("node3", task3);

  node1->add_downstream_node(node3);
  node2->add_downstream_node(node3);
  node3->add_upstream_node(node1);
  node3->add_upstream_node(node2);

  auto time_aspect = std::make_unique<TimeAspect>();
  auto log_aspect = std::make_unique<LogAspect>();

  node1->set_aspect(std::move(time_aspect));
  node2->set_aspect(std::move(log_aspect));

  Graph graph;
  graph.add_node(node1);
  graph.add_node(node2);
  graph.add_node(node3);

  graph.execute_tasks();
}

}  // namespace Meepo::Unittests