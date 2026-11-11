This area is about how to measure the cost of a program and how to make it smaller. It covers measurement first, then the main sources of cost in real programs: memory layout, allocation, the compiler, concurrency and I/O.

## What it holds

- Measuring: benchmarks, profilers and the statistics of a measurement.
- Complexity in practice: Big-O cost as a programmer meets it, and the constant factors that it hides.
- Caches and memory layout: CPU caches, data layout and locality.
- Allocation cost: the cost of allocation and how to reduce it.
- Vectorization: SIMD instructions and loops that the compiler can vectorize.
- Compiler optimizations: what optimizing compilers do, and how to help them.
- Concurrency for speed: using more cores to finish sooner, and the limits of it.
- I/O performance: buffering, batching and the cost of system calls.
- Latency and throughput: the two main measures of speed, and how they trade.

## What it does not hold

- The analysis of algorithms. It is on [the algorithm site](https://algorithm.tamnd.com/). This area links to it.
- Profiling to find a bug. It is in [Debugging and observability](/debugging-and-observability/).
- Web page performance. It is in [Web development](/web-development/).
- HPC and numerical speed. They are in [Scientific and high-performance computing](/scientific-and-hpc/).
