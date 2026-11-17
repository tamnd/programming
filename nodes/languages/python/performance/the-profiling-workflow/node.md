---
tag: 7XYQE7
kind: feature
owner: python
slug: the-profiling-workflow
name: The profiling workflow
aliases: [profiling workflow, find the hot path, profiling.sampling, Tachyon, statistical profiler, flame graph, collapsed stacks, line_profiler, kernprof, Scalene, tracemalloc, wall-clock time, CPU time]
area: languages/python/performance
level: core
summary: >-
  Find the hot path with a profiler before you change code. Use a sampling profiler for most work, a tracing profiler for exact call counts, and a memory profiler for allocations. Then measure again.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/profiling.html", locator: "profiling, Python profilers"}
see_also: [python/performance-measurement, python/optimization-guidance, python/object-sizes-and-tracemalloc, cpu-profiling]
sources:
  - url: https://docs.python.org/3/library/profiling.html
    title: profiling, Python profilers
    locator: Choosing a profiler; When to use statistical sampling; When to use deterministic tracing
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/profiling.sampling.html
    title: profiling.sampling, Statistical profiler
    locator: What is statistical profiling?; When to use a different approach; Profiling modes; Output formats
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/time.html
    title: time, Time access and conversions
    locator: time.perf_counter; time.process_time
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/tracemalloc.html
    title: tracemalloc, Trace memory allocations
    locator: introduction; Compute differences
    accessed: 2026-11-16
  - url: https://github.com/pyutils/line_profiler
    title: line_profiler and kernprof
    locator: README, Quick Start
    accessed: 2026-11-16
  - url: https://github.com/plasma-umass/scalene
    title: "Scalene: a Python CPU+GPU+memory profiler"
    locator: README, About Scalene
    accessed: 2026-11-16
  - url: https://pkg.go.dev/runtime/pprof
    title: Package pprof
    locator: Profiling a Go program
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.15.html
    title: What’s new in Python 3.15
    locator: profiling module
    accessed: 2026-11-16
---

## Synopsis

```python run id=workflow
import cProfile
import pstats

def parse(lines):
    return [line.split(',') for line in lines]

def dedupe_slow(rows):
    out = []
    for r in rows:
        if r not in out:
            out.append(r)
    return out

def dedupe_fast(rows):
    seen, out = set(), []
    for r in rows:
        key = tuple(r)
        if key not in seen:
            seen.add(key)
            out.append(r)
    return out

lines = [f'{i % 1500},x' for i in range(3000)]

def hottest(dedupe):
    prof = cProfile.Profile()
    prof.enable()
    result = dedupe(parse(lines))
    prof.disable()
    stats = pstats.Stats(prof).get_stats_profile().func_profiles
    name = max(stats, key=lambda n: stats[n].tottime)
    return name, result

name, before = hottest(dedupe_slow)
print('hot spot:', name)
name, after = hottest(dedupe_fast)
print('same result:', before == after, len(after))
```

```output for=workflow
hot spot: dedupe_slow
same result: True 1500
```

Profiling has four steps. First, run the program on a realistic input under a profiler. Second, find the function or the line that takes the most time: the hot path. Third, change that code, and keep the result the same. Fourth, profile and measure again, to see if the change helped. In the example, the profile points at `dedupe_slow`, which searches a list for each row. The fix uses a set, and the result stays the same.

## Semantics

**r1.** Python has two kinds of profilers. "For most performance analysis, use the statistical profiler (profiling.sampling). It has minimal overhead, works for both development and production, and provides rich visualization options including flame graphs, heatmaps, GIL analysis, and more."[^1] "Use the deterministic profiler (profiling.tracing) when you need exact call counts and cannot afford to miss any function calls."[^1] Python 3.15 adds the `profiling` package. It has "profiling.tracing: deterministic function-call tracing (relocated from cProfile)" and "profiling.sampling: a new statistical sampling profiler (named Tachyon)".[^8] "The cProfile module remains as an alias for backwards compatibility."[^8] The example uses `cProfile`, so it also runs on earlier versions.

**r2.** A sampling profiler reads the call stack at intervals. "This approach rests on a simple principle: functions that consume significant CPU time will appear frequently in the collected samples."[^2] "The time values shown in Tachyon’s output are estimates derived from sample counts, not direct measurements."[^2] Example: [`collapsed`](#collapsed).

**r3.** "For very short scripts that complete in under one second, the profiler may not collect enough samples for reliable results."[^2] "When you need exact call counts, sampling cannot provide them."[^2] "When comparing two implementations where the difference might be only 1-2%, sampling noise can obscure real differences. Use timeit for micro-benchmarks or profiling.tracing for precise measurements."[^2]

**r4.** Wall-clock time and CPU time are different. "In wall-clock mode, samples are recorded whether the thread is actively executing Python code, waiting for I/O, blocked on a lock, or sleeping."[^2] In CPU mode, "Samples taken while the thread is sleeping, blocked on I/O, or waiting for a lock are discarded."[^2] "If a function is high in wall-clock mode but low or absent in CPU mode, it is I/O-bound or waiting."[^2] The clocks `time.perf_counter` and `time.process_time` show the same difference: `perf_counter` "does include time elapsed during sleep", and `process_time` "does not include time elapsed during sleep".[^3] Example: [`wall-cpu`](#wall-cpu).

**r5.** A flame graph shows the samples as boxes. "The flame graph visualization shows call stacks as nested rectangles, with width proportional to time spent."[^2] The collapsed format "produces one line per unique call stack, with a count of how many times that stack was sampled".[^2] Other flame graph tools read this format. Example: [`collapsed`](#collapsed).

**r6.** To compare two versions, the sampling profiler makes a differential flame graph. "Differential flame graphs compare two profiling runs to highlight where performance changed."[^2]

**r7.** For the time of each line, the sampling profiler has a heatmap: "Heatmap format (--heatmap) generates an interactive HTML visualization showing sample counts at the source line level".[^2] The third-party package `line_profiler` does line-by-line profiling with instrumentation: "Use it when function-level profiling identifies a slow function, but you need to see which individual source lines account for the time."[^5]

**r8.** For memory, "The tracemalloc module is a debug tool to trace memory blocks allocated by Python."[^4] It gives "Statistics on allocated memory blocks per filename and per line number" and can "Compute the differences between two snapshots to detect memory leaks".[^4] Example: [`memory`](#memory).

**r9.** The third-party profiler Scalene samples CPU time and memory together. Its README says: "It uses sampling instead of instrumentation or relying on Python's tracing facilities. Its overhead is typically no more than 10-20% (and often less)."[^6]

**r10.** A profile is not a benchmark. "The profiler modules are designed to provide an execution profile for a given program, not for benchmarking purposes. For benchmarking, use the timeit module, which provides reasonably accurate timing measurements."[^1]

## Examples

A sleep adds to the wall-clock time, but not to the CPU time (rule r4). A profile in wall-clock mode shows the wait, and a profile in CPU mode does not:

```python run id=wall-cpu
import time

def measure(work):
    w0, c0 = time.perf_counter(), time.process_time()
    work()
    return time.perf_counter() - w0, time.process_time() - c0

wall, cpu = measure(lambda: time.sleep(0.3))
print('sleep: wall >= 0.3:', wall >= 0.3, 'cpu < 0.1:', cpu < 0.1)
wall, cpu = measure(lambda: sum(i * i for i in range(2_000_000)))
print('compute: cpu > 0:', cpu > 0)
```

```output for=wall-cpu
sleep: wall >= 0.3: True cpu < 0.1: True
compute: cpu > 0: True
```

A model of a sampling profiler. Each sample is a call stack. The profiler counts equal stacks and writes them in the collapsed format. The share of the samples estimates the share of the time (rules r2 and r5):

```python run id=collapsed
from collections import Counter

samples = (
    [('main', 'load', 'parse')] * 6
    + [('main', 'load', 'read')] * 2
    + [('main', 'report')] * 2
)

for stack, n in sorted(Counter(samples).items()):
    print(';'.join(stack), n)

self_time = Counter(stack[-1] for stack in samples)
for name, n in self_time.most_common():
    print(f'{name}: {n * 100 // len(samples)}%')
```

```output for=collapsed
main;load;parse 6
main;load;read 2
main;report 2
parse: 60%
read: 20%
report: 20%
```

`tracemalloc` finds the line that allocates the most memory (rule r8):

```python run id=memory
import tracemalloc

def build():
    return [str(i) * 10 for i in range(50_000)]

tracemalloc.start()
data = build()
snapshot = tracemalloc.take_snapshot()
tracemalloc.stop()

top = snapshot.statistics('lineno')[0]
frame = top.traceback[0]
print('line in build:', frame.lineno == build.__code__.co_firstlineno + 1)
print('more than 1 MB:', top.size > 1_000_000)
```

```output for=memory
line in build: True
more than 1 MB: True
```

The sampling profiler of Python 3.15 runs from the command line. Its documentation gives these commands, among others:[^2]

- `python -m profiling.sampling run script.py` profiles a script and prints a table.
- `python -m profiling.sampling run --mode=cpu script.py` records only the samples in which the thread runs on a CPU.
- `python -m profiling.sampling run --flamegraph -o profile.html script.py` writes a flame graph.
- `python -m profiling.sampling run --heatmap script.py` writes the samples of each source line.
- `python -m profiling.sampling attach 12345` profiles the running process 12345.

## Pitfalls

Do not optimize before you profile. The slow part of a program is often not the part that you expect.

Profile a realistic input that runs long enough. A sampling profiler needs many samples, and a short run gives few (rule r3).

Do not read a wall-clock profile as a CPU profile. A function that waits for the network is high in wall-clock mode and low in CPU mode, and a faster algorithm does not help it (rule r4).

A tracing profiler adds a cost to each Python call, but not to code in C. This can make Python code look slower than C code by more than it is (rule r10). Use `timeit` to compare two versions.

To attach the sampling profiler to a running process, the operating system must give permission to read the memory of that process. On Linux, this can need root, `CAP_SYS_PTRACE` or a change to the ptrace scope.[^2]

## Comparison

Go has the profiler in the toolchain. The `pprof` documentation says: "Support for profiling benchmarks built with the standard testing package is built into go test."[^7] The command `go test -cpuprofile cpu.prof -memprofile mem.prof -bench .` writes a CPU profile and a memory profile.[^7] Python has the profilers in the standard library, and runs them with `python -m` (rule r1).

## References

[^1]: [profiling, Python profilers](https://docs.python.org/3/library/profiling.html)
[^2]: [profiling.sampling, Statistical profiler](https://docs.python.org/3/library/profiling.sampling.html)
[^3]: [time, Time access and conversions](https://docs.python.org/3/library/time.html)
[^4]: [tracemalloc, Trace memory allocations](https://docs.python.org/3/library/tracemalloc.html)
[^5]: [line_profiler and kernprof](https://github.com/pyutils/line_profiler)
[^6]: [Scalene: a Python CPU+GPU+memory profiler](https://github.com/plasma-umass/scalene)
[^7]: [Package pprof](https://pkg.go.dev/runtime/pprof)
[^8]: [What’s new in Python 3.15](https://docs.python.org/3/whatsnew/3.15.html)
