This area is about programs that do more than one thing at a time. Concurrency is about structure: tasks that overlap in time. Parallelism is about speed: work that runs at the same time on more than one core. The area covers the models, the language features and the bugs of both.

## What it holds

- **Concurrency basics.** The words and models that the other sub-areas use: concurrency, parallelism, interleaving, safety and liveness, and the limits of speed-up.
- **Processes and threads.** Processes and threads as units of execution, how a program starts and joins them, and the lightweight forms.
- **Shared memory and locks.** Shared memory between threads and the locks that protect it.
- **Condition synchronization.** Waiting until a condition holds: condition variables, semaphores, monitors, barriers, latches and one-time initialization.
- **Atomics and memory models.** Atomic operations, the memory models of languages, and the orderings that make lock-free code correct.
- **Thread safety.** What makes a type or a module safe to use from many threads, and the designs that avoid shared mutable state.
- **Message passing.** Tasks that share data only by sending messages, and the queues and pipelines built from them.
- **CSP and channels.** Communicating sequential processes and channels, as in Go, occam, Clojure core.async and Rust.
- **Actors.** Actor runtimes and libraries, and the practices of building systems from actors. The actor model as a paradigm is in paradigms.
- **Async and event loops.** Async and await, event loops, non-blocking I/O and the patterns of event-driven servers.
- **Futures and promises.** Values that are not ready yet, and how to combine, wait for and fail them.
- **Structured concurrency.** Task lifetimes that follow the structure of the code, and cancellation.
- **Timers and rate limits.** Running work later or at a rate, and limiting the rate of work inside one program.
- **Races and deadlocks.** The bugs of concurrent code: data races, race conditions, deadlocks, livelocks, starvation and priority inversion.
- **Data parallelism.** The same operation on many items at once, and task parallelism with fork and join.
- **GPU programming.** Programming a GPU from a host language: the model, the memory and the APIs. CUDA and HPC in depth are in scientific-and-hpc.

## What it does not hold

- Concurrent algorithms and lock-free data structures as algorithms. They are on [the algorithm site](https://algorithm.tamnd.com/).
- Distributed systems, where tasks run on different machines. They are in [Distributed and cloud programming](/distributed-and-cloud/).
- OS processes and signals as an interface. They are in [Systems programming](/systems-programming/).
- CUDA and HPC accelerators in depth. They are in [Scientific and high-performance computing](/scientific-and-hpc/).
