---
tag: MDN62G
kind: feature
owner: python
slug: tracing-and-monitoring
name: Tracing and monitoring
aliases: [sys.settrace, sys.setprofile, sys.monitoring, trace function, profile function, PEP 669, low impact monitoring, monitoring events, tool id, sys.monitoring.DISABLE]
area: languages/python/metaprogramming
level: research
summary: >-
  sys.settrace and sys.setprofile call a Python function on each call, line or return, for debuggers and profilers. sys.monitoring, from PEP 669, sends only the events a tool asks for, at a lower cost.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/sys.monitoring.html", locator: "sys.monitoring — Execution event monitoring"}
proposals: [pep-669]
see_also: [interactive-debugging, cpu-profiling, python/code-and-frame-objects, python/bytecode-and-dis, python/introspection-with-inspect]
sources:
  - url: https://docs.python.org/3.15/library/sys.html#sys.settrace
    title: sys — System-specific parameters and functions, Python 3.15 documentation
    locator: sys.settrace(); sys.setprofile()
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0669/
    title: PEP 669 – Low Impact Monitoring for CPython
    locator: Abstract; Performance
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.monitoring.html
    title: sys.monitoring — Execution event monitoring, Python 3.15 documentation
    locator: Tool identifiers; Events; Callback function arguments; Disabling events
    accessed: 2026-11-16
---

## Synopsis

```python run id=settrace
import sys

def tracer(frame, event, arg):
    if frame.f_code.co_name != "total":
        return None
    first = frame.f_code.co_firstlineno
    print(event, frame.f_lineno - first, arg)
    return tracer

def total(items):
    s = 0
    for x in items:
        s += x
    return s

sys.settrace(tracer)
total([5, 7])
sys.settrace(None)
```

```output for=settrace
call 0 None
line 1 None
line 2 None
line 3 None
line 2 None
line 3 None
line 2 None
line 4 None
return 4 12
```

## Semantics

**r1.** `sys.settrace` sets "the system’s trace function, which allows you to implement a Python source code debugger in Python."[^1] A trace function takes "three arguments: frame, event, and arg."[^1]

**r2.** The global trace function runs "whenever a new local scope is entered; it should return a reference to a local trace function to be used for the new scope, or None if the scope shouldn’t be traced."[^1] Example: [`settrace`](#settrace).

**r3.** The `'line'` event means "The interpreter is about to execute a new line of code or re-execute the condition of a loop."[^1]

**r4.** The profile function of `sys.setprofile` "isn’t called for each executed line of code (only on call and return".[^1] It also gets `'c_call'` when "A C function is about to be called."[^1] Example: [`profile`](#profile).

**r5.** `settrace` "is intended only for implementing debuggers, profilers, coverage tools and the like. Its behavior is part of the implementation platform, rather than part of the language definition".[^1]

**r6.** PEP 669 starts from this problem: "Using a profiler or debugger in CPython can have a severe impact on performance. Slowdowns by an order of magnitude are common."[^2] It "proposes an API for monitoring Python programs running on CPython that will enable monitoring at low cost."[^2]

**r7.** In `sys.monitoring`, a tool first takes an identifier. "Identifiers are integers in the range 0 to 5 inclusive."[^3] `use_tool_id` "Must be called before tool_id can be used" and "Raises a ValueError if tool_id is in use."[^3] Example: [`monitor`](#monitor).

**r8.** "In order to monitor an event, it must be turned on and a corresponding callback must be registered. Events can be turned on or off by setting the events either globally and/or for a particular code object."[^3]

**r9.** "Local events can be disabled for a specific code location by returning sys.monitoring.DISABLE from a callback function."[^3] Example: [`disable`](#disable).

## Examples

```python run id=profile
import sys

calls = []

def profiler(frame, event, arg):
    if event == "call":
        calls.append(frame.f_code.co_name)
    elif event == "c_call":
        calls.append(arg.__name__)

def area(w, h):
    return abs(w * h)

def main():
    return area(3, -4) + area(1, 2)

sys.setprofile(profiler)
main()
sys.setprofile(None)
print(calls)
```

```output for=profile
['main', 'area', 'abs', 'area', 'abs', 'setprofile']
```

The profile function sees the calls of Python functions and of the C function `abs`, but not the lines (rule r4). The last item is the call to `sys.setprofile(None)`, which is also a C call.

```python run id=monitor
import sys
from collections import Counter

mon = sys.monitoring
TOOL = mon.PROFILER_ID
counts = Counter()

def on_start(code, offset):
    counts[code.co_name] += 1

def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)

mon.use_tool_id(TOOL, "counter")
mon.register_callback(TOOL, mon.events.PY_START, on_start)
mon.set_events(TOOL, mon.events.PY_START)
fib(10)
mon.set_events(TOOL, mon.events.NO_EVENTS)
mon.free_tool_id(TOOL)
print(counts["fib"])
```

```output for=monitor
177
```

The tool turns on one event, `PY_START`, and counts the start of each Python function. `fib(10)` makes 177 calls in total (rules r7 and r8).

```python run id=disable
import sys

mon = sys.monitoring
TOOL = mon.COVERAGE_ID
seen = []

def on_line(code, line):
    seen.append(line - code.co_firstlineno)
    return mon.DISABLE

def clamp(x):
    if x < 0:
        return 0
    return x

mon.use_tool_id(TOOL, "coverage")
mon.register_callback(TOOL, mon.events.LINE, on_line)
mon.set_local_events(TOOL, clamp.__code__, mon.events.LINE)
for v in [5, 6, 7]:
    clamp(v)
mon.free_tool_id(TOOL)
print(seen)
```

```output for=disable
[1, 3]
```

The `LINE` event is on for the code of `clamp` only. Each line reports once, because the callback returns `DISABLE` for that location (rule r9). A coverage tool needs only this first event for each line.

## Versions

> [!since v="3.12"]
> `sys.monitoring` is new, from PEP 669.[^3][^2]

## Pitfalls

```python wrong id=no-local-tracer
import sys

events = []

def tracer(frame, event, arg):
    if frame.f_code.co_name == "work":
        events.append(event)

def work():
    a = 1
    b = 2
    return a + b

sys.settrace(tracer)
work()
sys.settrace(None)
print(events)
```

```output for=no-local-tracer
['call']
```

The trace function returns `None`, so the new scope has no local trace function and gives no `'line'` or `'return'` events (rule r2). Return the trace function itself to trace the lines.

```python fails id=tool-in-use expect=exception:ValueError
import sys

sys.monitoring.use_tool_id(3, "first")
sys.monitoring.use_tool_id(3, "second")
```

Each tool needs its own identifier (rule r7). Call `free_tool_id` when the tool stops, or use `get_tool` to see if an identifier is free.

```python fails id=import-namespace expect=exception:ModuleNotFoundError
import sys.monitoring
```

"sys.monitoring is a namespace within the sys module, not an independent module, and import sys.monitoring would fail with a ModuleNotFoundError."[^3] Write `import sys` and use `sys.monitoring`.

## Comparison

A trace function runs Python code for each event, so the program runs much slower. With `sys.monitoring`, "If a small set of events are active, e.g. for a debugger, then the overhead of callbacks will be orders of magnitudes less than for sys.settrace()".[^2] See [[interactive-debugging]] and [[cpu-profiling]] for the general tools, and [[python/code-and-frame-objects]] for the frames that a trace function gets.
