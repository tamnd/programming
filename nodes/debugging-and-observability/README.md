This area covers how to find the cause of a defect and how to see what a running program does. It holds methods and tools for both: debuggers and sanitizers for a single run, and logs, traces and metrics for a program in production.

## What it holds

- Debugging methods: systematic ways to find a bug, such as bisection and the scientific method.
- Debuggers: debuggers, breakpoints and watchpoints.
- Core dumps: saving the state of a crashed process and reading it later.
- Logging: log messages, levels and structured logs.
- Tracing: following a request through a program and between services.
- Runtime metrics: counters, gauges and histograms from a running program.
- Profiling as diagnosis: profiling to find the cause of a problem, such as a slow path or a leak.
- Sanitizers: tools that add checks to a build, such as AddressSanitizer.
- Reproducible bug reports: how to write a bug report that another person can reproduce.
- Post-mortems: reviews after an incident, without blame.

## What it does not hold

- Profiling and benchmarks to make code faster. They are in [Performance](/performance/). This area holds profiling for diagnosis.
- Monitoring and alerts of a service as an operations practice. They are in [Distributed and cloud programming](/distributed-and-cloud/), under site reliability.
- Static analysis. It is in [Program analysis and verification](/program-analysis-and-verification/).
