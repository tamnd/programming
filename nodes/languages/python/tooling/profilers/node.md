---
tag: F6KNKK
kind: feature
owner: python
slug: profilers
name: Profilers
aliases: [cProfile, "python -m cProfile", profiling.tracing, profiling.sampling, pstats, deterministic profiler, statistical profiler, sampling profiler, primitive calls, tottime, cumtime, print_callers]
area: languages/python/tooling
level: core
summary: >-
  The standard profilers: running a script under cProfile from the command line, reading the table, primitive calls in recursion, and saving a profile to read callers with pstats.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/profiling.html", locator: "Understanding profile output"}
see_also: [python/the-profiling-workflow, python/performance-measurement, python/object-sizes-and-tracemalloc, cpu-profiling, python/the-pdb-debugger, python/development-mode-and-x-options]
sources:
  - url: https://docs.python.org/3.15/library/profiling.html
    title: profiling, Python 3.15 documentation
    locator: module overview; Choosing a profiler; Understanding profile output; Legacy compatibility
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/profiling.tracing.html
    title: profiling.tracing, Python 3.15 documentation
    locator: Command-line interface; Programmatic usage examples; profiling.tracing.run
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/pstats.html
    title: pstats, Python 3.15 documentation
    locator: Analyzing call relationships; print_callers; get_stats_profile; sort keys
    accessed: 2026-11-18
  - url: https://pkg.go.dev/runtime/pprof
    title: pprof package, Go documentation
    locator: Profiling a Go program
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import pathlib
import subprocess
import sys
import tempfile

script = '''\
def parse(line):
    return [int(x) for x in line.split(',')]


def total(lines):
    return sum(sum(parse(line)) for line in lines)


print(total(['1,2,3'] * 4))
'''
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'work.py').write_text(script)
    result = subprocess.run([sys.executable, '-m', 'cProfile', '-s', 'ncalls', 'work.py'],
                            cwd=tmp, capture_output=True, text=True)
    lines = result.stdout.splitlines()
    print('program output:', lines[0])
    for line in lines[1:]:
        fields = line.split()
        if 'function calls' in line:
            print(line.split(' in ')[0].strip())
        elif fields[:1] == ['ncalls']:
            print(f'{fields[0]:>8}  {fields[-1]}')
        elif 'work.py:' in line or 'builtins.sum' in line or "'split'" in line:
            print(f'{fields[0]:>8}  {" ".join(fields[5:])}')
```

```output for=synopsis
program output: 24
23 function calls (19 primitive calls)
  ncalls  filename:lineno(function)
     5/1  {built-in method builtins.sum}
       5  work.py:6(<genexpr>)
       4  {method 'split' of 'str' objects}
       4  work.py:1(parse)
       1  work.py:1(<module>)
       1  work.py:5(total)
```

`python -m cProfile` runs the script, lets it print its own output, and then prints a table with one row per function. The example keeps only the call counts and the function names, because the times change from run to run. `-s ncalls` sorts the rows by call count. The program is not changed to be profiled. Built-in functions such as `sum` and methods such as `str.split` have rows too.

## Semantics

**r1.** There are two ways to profile. "statistical sampling and deterministic tracing."[^1] The deterministic profiler "traces every function call, return, and exception event. Provides exact call counts and precise timing information, capturing every invocation including very fast functions."[^1] The statistical profiler "periodically samples the call stack."[^1] In the current documentation they are `profiling.tracing` and `profiling.sampling`, and "For backward compatibility, the cProfile module remains available as an alias to profiling.tracing ."[^1] The examples on this page use the name `cProfile`.

**r2.** The command line form is `python -m profiling.tracing [-o output_file] [-s sort_order] (-m module | script.py)`. It "runs the specified script or module under the profiler and prints the results to standard output (or saves them to a file)."[^2] With `-o`, it will "Write the profile results to a file instead of standard output. The output file can be read by the pstats module for later analysis".[^2] The `-s` option "only applies when -o is not specified."[^2]

**r3.** The table has two times for each function. Direct time, "also called self time or tottime", is "Time spent executing code in the function itself, excluding time spent in functions it called."[^1] Cumulative time, "also called total time or cumtime", is "Time spent in the function and all functions it called."[^1]

**r4.** A call count can have two numbers. Primitive calls are "Calls that are not induced by recursion. When a function recurses, the total call count includes recursive invocations, but primitive calls counts only the initial entry. Displayed as total/primitive (for example, 3/1 means three total calls, one primitive)."[^1]

**r5.** A saved profile is read with `pstats`. "The print_callers() method shows which functions called each displayed function", and "The print_callees() method shows the opposite relationship".[^3] "With profiling.tracing (or cProfile ), each caller line shows three numbers: the number of calls from that caller, and the total and cumulative times for those specific calls."[^3] `get_stats_profile()` returns an object "with function names mapped to FunctionProfile objects containing timing and call count information."[^3]

**r6.** A profiler is not a timer. "The profiler modules are designed to provide an execution profile for a given program, not for benchmarking purposes. For benchmarking, use the timeit module".[^1] The tracing profiler adds a cost to each call: "deterministic profilers introduce overhead for Python code but not for C-level functions, which can skew comparisons."[^1]

## Examples

### Primitive calls in recursion

```python run id=recursion
import cProfile
import pstats


def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)


with cProfile.Profile() as profiler:
    fib(10)

profile = pstats.Stats(profiler).get_stats_profile()
print('ncalls:', profile.func_profiles['fib'].ncalls)
```

```output for=recursion
ncalls: 177/1
```

`Profile` "also works as a context manager".[^2] The program calls `fib` once, and `fib` calls itself 176 more times. The count `177/1` shows both numbers (r4). The synopsis has the same form for `sum`: `5/1`. The outer `sum` is still running when the generator calls `sum` for each line, so those four calls count as recursive.

### A saved profile and its callers

```python run id=callers
import cProfile
import io
import os
import pstats
import tempfile


def load(n):
    return [str(i) for i in range(n)]


def clean(items):
    return [s.strip() for s in items]


def main():
    clean(load(100))
    clean(load(10))


with tempfile.TemporaryDirectory() as tmp:
    path = os.path.join(tmp, 'run.prof')
    cProfile.run('main()', path)
    report = io.StringIO()
    stats = pstats.Stats(path, stream=report)
    stats.strip_dirs().sort_stats('name').print_callers(r'\((load|clean|main)\)')
    here = os.path.basename(__file__)
    for line in report.getvalue().splitlines():
        if '<-' in line:
            callee, rest = line.split('<-')
            row = f'{callee.split()[0]} <- {rest.split()[0]} call(s) from {rest.split()[-1]}'
            print(row.replace(here, '<this file>'))
```

```output for=callers
<this file>:12(clean) <- 2 call(s) from <this file>:16(main)
<this file>:8(load) <- 2 call(s) from <this file>:16(main)
<this file>:16(main) <- 1 call(s) from <string>:1(<module>)
```

`cProfile.run` "executes the command string using exec() in the __main__ module’s namespace", and with a file name it saves the profile there.[^2] `pstats.Stats` reads the file back, so the run and the reading can be in different processes. The argument to `print_callers` is a regular expression that keeps only three rows. The example prints the caller and the call count from each line (r5) and drops the times. `main` is called from `<string>`, the string given to `run`.

## Pitfalls

- `-s` has no effect together with `-o` (r2). Sort the saved file with `pstats` instead.
- The times from a profiler are not benchmark results (r6). Use the profile to find where the time goes, and `timeit` to measure a change.
- A recursive function shows two counts, such as `177/1`. The first number is the number of calls that were made; the second is the number of times the recursion started from outside (r4).
- The tracing profiler can miss nothing, but the statistical profiler "may undercount short-lived functions that complete between samples."[^1] Use the tracing profiler when the exact count matters.
- "Profiling requires that the profiled code returns normally. If the interpreter terminates (for example, via sys.exit() ) during profiling, no results will be available."[^2]

## Comparison

Go has its profiler in the standard library. "Package pprof writes runtime profiling data in the format expected by the pprof visualization tool."[^4] For tests, "Support for profiling benchmarks built with the standard testing package is built into go test", for example `go test -cpuprofile cpu.prof -memprofile mem.prof -bench .`.[^4] For other programs the docs say: "To add equivalent profiling support to a standalone program, add code like the following to your main function", and the code calls `pprof.StartCPUProfile` and `pprof.StopCPUProfile`.[^4] `python -m cProfile` profiles a script with no change to its code. The Go profile file "can then be visualized with the pprof tool: go tool pprof cpu.prof".[^4] This is like a Python profile saved with `-o` and read with `pstats`.

[^1]: profiling, Python 3.15 documentation.
[^2]: profiling.tracing, Python 3.15 documentation.
[^3]: pstats, Python 3.15 documentation.
[^4]: pprof package, Go documentation.
