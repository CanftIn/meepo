#include <gtest/gtest.h>

#include "Meepo/Graph/Graph.h"

namespace Meepo::Unittests {

TEST(GraphTest, Parameter) {
  auto task1 = std::make_shared<FunctionTask>(FunctionTask::ParamReturnCallback(
      [](const Parameter& params) -> Parameter {
        Parameter new_params;
        new_params.set("data", std::string("Node1"));
        return new_params;
      }));

  auto task2 = std::make_shared<FunctionTask>(
      FunctionTask::ParamCallback([](const Parameter& params) {
        try {
          auto data = params.get<std::string>("data");
          std::cout << "Received in Node2: " << data << '\n';
        } catch (const std::exception& e) {
          std::cerr << "Error: " << e.what() << '\n';
        }
      }));

  auto node1 = std::make_shared<Node>("node1", task1);
  auto node2 = std::make_shared<Node>("node2", task2);

  node1->add_downstream_node(node2);
  node2->add_upstream_node(node1);

  Graph graph;
  graph.add_node(node1);
  graph.add_node(node2);

  graph.execute_tasks();
}

}  // namespace Meepo::Unittests