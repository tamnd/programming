This area is about programs that do more than one thing at a time. Concurrency is about structure: tasks that overlap in time. Parallelism is about speed: work that runs at the same time on more than one core. The area covers the models, the language features and the bugs of both.

## What it holds

- Processes and threads: processes, threads and how a program starts and joins them.
- Shared memory and locks: mutexes, condition variables and other locks.
- Atomics and memory models: atomic operations and the memory model of each language.
- Message passing: tasks that share data only by sending messages.
- Async and event loops: async and await, event loops and non-blocking I/O.
- Futures and promises: values that are not ready yet, and how to combine them.
- Actors: actor runtimes and libraries, such as Erlang processes and Akka.
- CSP and channels: communicating sequential processes and channels, as in Go.
- Structured concurrency: task lifetimes that follow the structure of the code.
- Data parallelism: the same operation on many items at once.
- GPU programming: programming a GPU from a host language.
- Races and deadlocks: data races, race conditions, deadlocks and livelocks.

## What it does not hold

- Concurrent algorithms and lock-free data structures as algorithms. They are on [the algorithm site](https://algorithm.tamnd.com/).
- Distributed systems, where tasks run on different machines. They are in [Distributed and cloud programming](/distributed-and-cloud/).
- OS processes and signals as an interface. They are in [Systems programming](/systems-programming/).
- CUDA and HPC accelerators in depth. They are in [Scientific and high-performance computing](/scientific-and-hpc/).
