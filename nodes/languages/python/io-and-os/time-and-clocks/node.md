---
tag: 5M1F44
kind: feature
owner: python
slug: time-and-clocks
name: Time and clocks
aliases: [time module, time.time, time.monotonic, time.perf_counter, time.process_time, time.sleep, epoch, wall clock, monotonic clock]
area: languages/python/io-and-os
level: core
summary: >-
  The time module reads the clocks of the OS. time() is the wall clock in seconds since the epoch. monotonic() and perf_counter() measure durations, process_time() counts CPU time, and sleep() waits.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/time.html", locator: "time module"}
see_also: [python/dates-and-times, python/signals, python/operating-system-interface]
sources:
  - url: https://docs.python.org/3.15/library/time.html
    title: time, Python 3.15 documentation
    locator: module introduction; get_clock_info(); gmtime(); monotonic(); perf_counter(); process_time(); sleep(); strftime(); struct_time; time(); time_ns()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/time
    title: Package time, Go standard library
    locator: Monotonic Clocks; func Since; func Sleep
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import time

start = time.monotonic()
time.sleep(0.2)
elapsed = time.monotonic() - start
print(0.2 <= elapsed < 5)

print(time.gmtime(0))
print(time.strftime('%Y-%m-%d %H:%M:%S', time.gmtime(1_000_000_000)))
```

```output for=synopsis
True
time.struct_time(tm_year=1970, tm_mon=1, tm_mday=1, tm_hour=0, tm_min=0, tm_sec=0, tm_wday=3, tm_yday=1, tm_isdst=0)
2001-09-09 01:46:40
```

The program times a sleep with the monotonic clock, then turns two counts of seconds since the epoch into calendar fields and text.

## Semantics

**r1.** "The epoch is the point where the time starts, the return value of time.gmtime(0). It is January 1, 1970, 00:00:00 (UTC) on all platforms."[^1] `time()` will "Return the time in seconds since the epoch as a floating-point number."[^1]

**r2.** The wall clock can go back. `time()` "normally returns non-decreasing values, it can return a lower value than a previous call if the system clock has been set back between the two calls."[^1]

**r3.** `monotonic()` reads "a monotonic clock, i.e. a clock that cannot go backwards. The clock is not affected by system clock updates. The reference point of the returned value is undefined, so that only the difference between the results of two calls is valid."[^1]

**r4.** `perf_counter()` reads "a clock with the highest available resolution to measure a short duration. It does include time elapsed during sleep."[^1] `process_time()` is "the sum of the system and user CPU time of the current process. It does not include time elapsed during sleep."[^1] Example: [`cpu`](#cpu).

**r5.** `get_clock_info(name)` will "Get information on the specified clock as a namespace object."[^1] Its `adjustable` is "True if the clock can be set to jump forward or backward in time", and `monotonic` is "True if the clock cannot go backward".[^1] Example: [`clocks`](#clocks).

**r6.** `sleep(seconds)` will "Suspend execution of the calling thread for the given number of seconds. The argument may be a non-integer to indicate a more precise sleep time."[^1] "The suspension time may be longer than requested by an arbitrary amount, because of the scheduling of other activity in the system."[^1]

**r7.** `gmtime()` converts seconds since the epoch "to a struct_time in UTC", and `localtime()` is "Like gmtime() but converts to local time."[^1] `struct_time` "is an object with a named tuple interface: values can be accessed by index and by attribute name."[^1] `strftime()` will "Convert a tuple or struct_time representing a time as returned by gmtime() or localtime() to a string as specified by the format argument."[^1]

**r8.** The `_ns` functions return integers. `time_ns()` is "Similar to time() but returns time as an integer number of nanoseconds since the epoch."[^1] The docs advise them "to avoid the precision loss caused by the float type."[^1] Example: [`ns`](#ns).

## Examples

```python run id=cpu
import time

wall = time.perf_counter()
cpu = time.process_time()
time.sleep(0.3)
sum(range(100_000))
print(time.perf_counter() - wall >= 0.3)
print(time.process_time() - cpu < 0.2)
```

```output for=cpu
True
True
```

The sleep counts on the performance counter but not as CPU time (rule r4).

```python run id=clocks
import time

for name in ('time', 'monotonic', 'perf_counter', 'process_time'):
    info = time.get_clock_info(name)
    print(f'{name:13} monotonic={info.monotonic} adjustable={info.adjustable}')
```

```output for=clocks
time          monotonic=False adjustable=True
monotonic     monotonic=True adjustable=False
perf_counter  monotonic=True adjustable=False
process_time  monotonic=True adjustable=False
```

Only the wall clock can be set, so only it can jump (rules r2 and r5). The output is from Linux.

```python run id=ns
import math
import time

now = time.time()
print(type(now).__name__, type(time.time_ns()).__name__)
print(math.ulp(now) > 1e-7)
t = time.gmtime(now)
print(t.tm_year == t[0], len(t))
```

```output for=ns
float int
True
True 9
```

A float near the current count of seconds has steps of more than 100 nanoseconds, so `time()` cannot hold nanoseconds (rule r8). A `struct_time` reads by name or by index (rule r7).

## Pitfalls

`strftime()` takes a `struct_time`, not the float from `time()`:

```python fails id=strftime-float expect=exception:TypeError
import time

print(time.strftime('%H:%M', time.time()))
```

Pass `time.localtime(seconds)` or `time.gmtime(seconds)` (rule r7).

Do not measure a duration with `time()`. The wall clock can be set back between the two calls (rule r2), and the difference can then be negative. Use `monotonic()` or `perf_counter()` (rules r3 and r4).

The values of `monotonic()`, `perf_counter()` and `process_time()` have no fixed start. Only the difference of two calls means anything (rule r3). Do not store them or compare them across machines.

`sleep()` can last longer than asked (rule r6). A loop that sleeps a fixed time each round adds those extra delays up, so it falls behind a fixed schedule. Compute the next deadline from the monotonic clock instead.

## Comparison

Go keeps both clocks in one value. "Operating systems provide both a "wall clock," which is subject to changes for clock synchronization, and a "monotonic clock," which is not."[^2] In Go, "the Time returned by time.Now contains both a wall clock reading and a monotonic clock reading; later time-telling operations use the wall clock reading, but later time-measuring operations, specifically comparisons and subtractions, use the monotonic clock reading."[^2] Python has separate functions, so the program chooses: `time()` to tell time, `monotonic()` to measure it (rules r1 and r3). Go's `time.Since` "returns the time elapsed since t."[^2]
