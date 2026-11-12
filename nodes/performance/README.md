This area is about how to measure the cost of a program and how to make it smaller. It covers measurement first, then the main sources of cost in real programs: memory layout, allocation, the compiler, concurrency and I/O.

## What it holds

- **Performance principles.** The ideas that decide when and how to optimize: what efficiency means, when to stop, and the trade-offs.
- **Measuring.** Benchmarks and the statistics needed to trust them.
- **Performance analysis.** Methods and tools to find where time and resources go in a running system. The profiler concepts are in debugging-and-observability/profiling; the methods and the performance tools are here.
- **Complexity in practice.** The cost of code on real machines, where constant factors and hidden loops matter. The theory of complexity is on algorithm.tamnd.com.
- **Caches and memory layout.** How the memory hierarchy shapes performance, and how to lay out data for it.
- **CPU microarchitecture for programmers.** The parts of a modern CPU core that change how fast code runs.
- **Allocation cost.** What heap allocation costs, and how programs reduce it.
- **Vectorization.** Using SIMD instructions by hand and through the compiler.
- **Compiler optimizations.** What optimizing compilers do to code, seen from the programmer's side: what to expect, what blocks it and how to check it. The pass implementations are in language-implementation.
- **Concurrency for speed.** Using cores to go faster, and the limits on the speed-up.
- **I/O performance.** The cost of reading, writing and sending data, and how programs reduce it.
- **Latency and throughput.** The two main measures of service performance, the laws that link them and the methods to control them.
- **Runtime performance.** The performance model of each major runtime: start-up, JIT, garbage collection and interpreter overhead.

## What it does not hold

- The analysis of algorithms. It is on [the algorithm site](https://algorithm.tamnd.com/). This area links to it.
- Profiling to find a bug. It is in [Debugging and observability](/debugging-and-observability/).
- Web page performance. It is in [Web development](/web-development/).
- HPC and numerical speed. They are in [Scientific and high-performance computing](/scientific-and-hpc/).
