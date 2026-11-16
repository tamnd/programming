---
tag: ZZG79R
kind: feature
owner: python
slug: performance-measurement
name: Performance measurement
aliases: [timeit, cProfile, profiling.tracing, profiling.sampling, pstats, benchmark, profiler, deterministic profiling, statistical profiling]
area: languages/python/performance
level: core
summary: >-
  timeit measures the time of a small piece of code. cProfile, also named profiling.tracing, counts the calls of each function, and pstats reads the result. Measure before you optimize.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/timeit.html", locator: "timeit, Measure execution time of small code snippets"}
see_also: [cpu-profiling]
sources:
  - url: https://docs.python.org/3/tutorial/stdlib.html#performance-measurement
    title: Brief Tour of the Standard Library
    locator: 10.10. Performance measurement
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/timeit.html
    title: timeit, Measure execution time of small code snippets
    locator: introduction; Timer.timeit; Timer.repeat; Command-line interface
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/profiling.html
    title: profiling, Python profilers
    locator: Introduction to profiling; Legacy compatibility
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/profiling.tracing.html
    title: profiling.tracing, Deterministic profiler
    locator: introduction; What is deterministic profiling?
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/pstats.html
    title: pstats, Statistics for profilers
    locator: introduction; get_stats_profile
    accessed: 2026-11-16
  - url: https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up
    title: Programming FAQ
    locator: My program is too slow. How do I speed it up?
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing
    title: Package testing
    locator: Benchmarks
    accessed: 2026-11-16
---

## Synopsis

```python run id=timeit
import timeit

t = timeit.Timer('a, b = b, a', setup='a = 1; b = 2')
times = t.repeat(repeat=5, number=100_000)
print(len(times), all(x > 0 for x in times))
best = min(times) / 100_000
print('best time per loop is under 1 ms:', best < 0.001)
```

```output for=timeit
5 True
best time per loop is under 1 ms: True
```

`timeit` runs a statement many times and returns the time in seconds. `repeat` does this a number of times and returns a list of times, and the smallest time is the best estimate. The time changes from one run to the next, so the example prints only facts about the times. A profiler, such as `cProfile`, measures a whole program and tells which functions use the time. The module `pstats` reads and sorts the results of a profiler.

## Semantics

**r1.** The tutorial says: "Python provides a measurement tool that answers those questions immediately."[^1] "In contrast to timeit’s fine level of granularity, the profile and pstats modules provide tools for identifying time critical sections in larger blocks of code."[^1]

**r2.** "This module provides a simple way to time small bits of Python code. It has both a Command-line interface as well as a callable one. It avoids a number of common traps for measuring execution times."[^2] Example: [`timeit`](#timeit).

**r3.** `Timer.timeit(number=1000000)` "executes the setup statement once, and then returns the time it takes to execute the main statement a number of times. The default timer returns seconds as a float."[^2] "The default timer, which is always time.perf_counter(), returns float seconds."[^2]

**r4.** "By default, timeit() temporarily turns off garbage collection during the timing."[^2] "If so, GC can be re-enabled as the first statement in the setup string."[^2] Example: [`gc-off`](#gc-off).

**r5.** For the list that `repeat` returns, the documentation says: "So the min() of the result is probably the only number you should be interested in."[^2] Example: [`timeit`](#timeit).

**r6.** The command line form is `python -m timeit [-n N] [-r N] [-u U] [-s S] [-p] [-v] [-h] [statement ...]`.[^2] The option `-r N` tells "how many times to repeat the timer (default 5)".[^2] "If -n is not given, a suitable number of loops is calculated".[^2]

**r7.** "A profile is a set of statistics that describes how often and for how long various parts of a program execute."[^3] The documentation of Python 3.15 describes two profilers. `profiling.sampling` is "A statistical profiler that periodically samples the call stack."[^3] `profiling.tracing` is "A deterministic profiler that traces every function call, return, and exception event."[^3]

**r8.** For `profiling.tracing`, "This module is also available as cProfile for backward compatibility. The cProfile name will continue to work in all future Python versions."[^4] Deterministic profiling gives "exact call counts rather than statistical approximations."[^4] Example: [`profile-calls`](#profile-calls).

**r9.** The call count of a recursive function is shown as two numbers. "When a function recurses, the total call count includes recursive invocations, but primitive calls counts only the initial entry. Displayed as total/primitive (for example, 3/1 means three total calls, one primitive)."[^3] Example: [`profile-calls`](#profile-calls).

**r10.** "The pstats module provides tools for reading, manipulating, and displaying profiling statistics generated by Python’s profilers."[^5] `get_stats_profile()` gives "function names mapped to FunctionProfile objects containing timing and call count information."[^5]

## Examples

`timeit` turns off the garbage collector while it measures. A setup of `gc.enable()` turns it on again (rule r4):

```python run id=gc-off
import gc
import timeit

seen = []
timeit.timeit('seen.append(gc.isenabled())', number=2, globals={'seen': seen, 'gc': gc})
timeit.timeit('seen.append(gc.isenabled())', setup='gc.enable()', number=2,
              globals={'seen': seen, 'gc': gc})
print(seen)
```

```output for=gc-off
[False, False, True, True]
```

`cProfile` counts each call. The recursive function `fib` is called 1973 times, and 1 of the calls is primitive (rules r8, r9 and r10):

```python run id=profile-calls
import cProfile
import pstats

def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)

profiler = cProfile.Profile()
profiler.enable()
fib(15)
profiler.disable()

stats = pstats.Stats(profiler).get_stats_profile()
print(stats.func_profiles['fib'].ncalls)
```

```output for=profile-calls
1973/1
```

The times in a profile change from one run to the next, but the call counts do not.

## Pitfalls

Find the slow part of the program before you change it. The FAQ says: "You should always find the hot spots in your program before attempting to optimize any code (see the profile module)."[^6]

Do not use a profiler to compare the speed of two pieces of code. "The profiler modules are designed to provide an execution profile for a given program, not for benchmarking purposes. For benchmarking, use the timeit module, which provides reasonably accurate timing measurements."[^3]

Other programs on the computer change the times. "default_timer() measurements can be affected by other programs running on the same machine, so the best thing to do when accurate timing is necessary is to repeat the timing a few times and use the best time."[^2]

A function that makes much garbage can be faster in `timeit` than in the program, because `timeit` turns off the garbage collector (rule r4). Add `gc.enable()` to the setup when the collector is a part of the cost.

## Comparison

In Go, "Functions of the form func BenchmarkXxx(*testing.B) are considered benchmarks".[^7] The command `go test -bench` runs them.[^7] In a benchmark, "Only the body of the loop is timed, so benchmarks may do expensive setup before calling b.Loop, which will not be counted toward the benchmark measurement".[^7] The setup argument of `timeit` does the same job in Python (rule r3). Go runs its benchmarks with the go test command, and Python runs `timeit` from the command line or from a program (rule r6).

## References

[^1]: [Brief Tour of the Standard Library](https://docs.python.org/3/tutorial/stdlib.html#performance-measurement)
[^2]: [timeit, Measure execution time of small code snippets](https://docs.python.org/3/library/timeit.html)
[^3]: [profiling, Python profilers](https://docs.python.org/3/library/profiling.html)
[^4]: [profiling.tracing, Deterministic profiler](https://docs.python.org/3/library/profiling.tracing.html)
[^5]: [pstats, Statistics for profilers](https://docs.python.org/3/library/pstats.html)
[^6]: [Programming FAQ](https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up)
[^7]: [Package testing](https://pkg.go.dev/testing)
