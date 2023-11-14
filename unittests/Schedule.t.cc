#include "Meepo/Schedule/Schedule.h"

#include <gtest/gtest.h>

namespace Meepo::Unittests {

TEST(GraphTest, Pipeline) {
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

  auto pipeline = std::make_shared<Pipeline>();
  pipeline->add_node(node1);
  pipeline->add_node(node2);
  pipeline->add_node(node3);

  auto schedule =
      std::make_shared<Schedule>(std::make_shared<PriorityStrategy>());
  schedule->add_pipeline(pipeline);

  schedule->execute();
}

}  // namespace Meepo::Unittests