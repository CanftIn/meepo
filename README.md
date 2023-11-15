# Meepo

> 地卜师米波, 是一个 dota 英雄。因为其对微操的高度依赖而成为游戏中最难的核心之一。 米波的大招<分则能成>，允许米波为自己创造复制体。

实现基于图的任务流调度框架，图中每一个节点类似于一个米波分身，项目则以米波命名。

## 设计：

1. Task

设计 SerialTask 和 ParallelTask，允许混合串行和并行的执行模式。
Task需要具备单一职责的模式，一种Task只做一种类型的事情。FunctionTask只执行lambda类型的函数。
Task内部不拥有策略，只根据具体的Task类型按模式执行任务。

2. Aspect
Aspect 类设计为一个接口，用于在任务执行前后执行某些动作。

2. Node

一个节点对应一个任务，节点之间存在上下游关系。

3. Parameter

作为节点之间调度执行时的数据传输。

4. Graph

节点构建好依赖关系后，存在图中，由Graph来调度，Graph调度Schedule，Schedule编排Pipeline，Pipeline进行串并联的Node的编排。

5. Executor

多线程管理执行Node的执行器。

6. Pipeline

进行串并行Node的编排。Pipeline 类负责按顺序执行一组节点，其中节点线性排列，每个节点只依赖于前一个节点。但Pipeline可以相互组合形成串并行pipeline。

7. Schedule

Pipeline的编排。

8. Stategy
Pipeline和Schedule的编排策略。