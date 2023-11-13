1. Task 的设计
设计 SerialTask 和 ParallelTask，允许混合串行和并行的执行模式。

2. Aspect 的设计
Aspect 类设计为一个接口，用于在任务执行前后执行某些动作。

3. Pipeline 的设计
Pipeline 类负责按顺序执行一组节点。它可以被看作是 Graph 的一种特殊情况，其中节点线性排列，每个节点只依赖于前一个节点。