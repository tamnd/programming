This area covers how to find the cause of a defect and how to see what a running program does. It holds methods and tools for both: debuggers and sanitizers for a single run, and logs, traces and metrics for a program in production.

## What it holds

- **Debugging fundamentals.** What a bug is, the chain from defect to failure, and the general process of debugging.
- **Debugging methods.** Systematic ways to find the cause of a bug, from explaining the code aloud to automatic isolation of failure causes.
- **Debuggers.** Interactive debuggers, what they can do, how they work, and the debugger of each main platform.
- **Core dumps and crash analysis.** Saving the state of a crashed process and reading it later, and collecting crashes from the field.
- **Logging.** Log messages from programs: what to log, levels, formats, structured logs, libraries and the handling of logs.
- **Tracing.** Following a request through a program and between services with traces and spans. The sub-area id changes so that the core slug tracing can be the concept page.
- **Observability fundamentals.** The ideas that join logs, metrics, traces and profiles into a way to ask questions about a running system.
- **Runtime metrics.** Numbers that a running program exposes: counters, gauges and histograms, how to collect them and which ones to watch.
- **Profiling for diagnosis.** Profiling to find the cause of a problem: a slow path, a leak, lock contention or high memory use. Optimization is in performance.
- **Sanitizers and dynamic checkers.** Tools that add checks to a build or a run to catch memory errors, data races and undefined behaviour when they happen.
- **System and network debugging.** Watching what a process asks of the operating system and the network: system call tracers, dynamic tracing, packet capture and process inspection.
- **Kinds of bugs.** Named kinds of bugs by how they behave and how hard they are to find, with the debugging approach for each.
- **Bug reports and tracking.** How to write a bug report that another person can reproduce, and how bugs move through tracking and triage.
- **Post-mortems.** Reviews after a failure or incident that find causes and lessons without blame.
- **Debugging in specific settings.** How debugging changes in production, in distributed systems, in front ends, on mobile and embedded targets, and for concurrent code.

## What it does not hold

- Profiling and benchmarks to make code faster. They are in [Performance](/performance/). This area holds profiling for diagnosis.
- Monitoring and alerts of a service as an operations practice. They are in [Distributed and cloud programming](/distributed-and-cloud/), under site reliability.
- Static analysis. It is in [Program analysis and verification](/program-analysis-and-verification/).
