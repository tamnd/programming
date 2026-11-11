---
tag: G63EVZ
kind: task
slug: measure-elapsed-time
name: Measure elapsed time
aliases: [time a function, measure how long code takes, use a monotonic clock, stopwatch, measure a duration]
area: data-and-values/dates-and-times
level: core
summary: Measure how long a piece of work takes with a monotonic clock, not the wall clock, and print the duration in milliseconds with integer arithmetic on nanoseconds.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/time.html
    title: The Python Standard Library, time
    locator: time.monotonic, time.monotonic_ns, time.time
  - url: https://nodejs.org/api/process.html#processhrtimebigint
    title: Node.js documentation, process
    locator: process.hrtime([time]); process.hrtime.bigint()
  - url: https://nodejs.org/api/perf_hooks.html#performancenow
    title: Node.js documentation, Performance measurement APIs
    locator: performance.now()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html#nanoTime()
    title: Java SE 25 API, java.lang.System
    locator: nanoTime(); currentTimeMillis()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
    title: Java SE 25 API, java.util.Formatter
    locator: Number Localization Algorithm
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.timeprovider?view=net-10.0
    title: .NET API reference, TimeProvider
    locator: Remarks; Properties (System, TimestampFrequency); Methods (GetElapsedTime, GetTimestamp)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.stopwatch.gettimestamp?view=net-10.0
    title: .NET API reference, Stopwatch.GetTimestamp
    locator: Remarks
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: Overview, Monotonic Clocks; func (Time) String
  - url: https://doc.rust-lang.org/std/time/struct.Instant.html
    title: The Rust Standard Library, std::time::Instant
    locator: Description; duration_since; checked_duration_since
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/clock_gettime.html
    title: The Open Group Base Specifications Issue 8, clock_getres, clock_gettime, clock_settime
    locator: DESCRIPTION, CLOCK_MONOTONIC
  - url: https://en.cppreference.com/w/c/chrono/clock.html
    title: cppreference.com, clock
    locator: the description of clock
  - url: https://en.cppreference.com/w/cpp/chrono/steady_clock.html
    title: cppreference.com, std::chrono::steady_clock
    locator: the description of the class
  - url: https://en.cppreference.com/w/cpp/chrono/high_resolution_clock.html
    title: cppreference.com, std::chrono::high_resolution_clock
    locator: the description of the class
  - url: https://www.gnu.org/software/bash/manual/bash.html#Bash-Variables
    title: GNU Bash Reference Manual
    locator: "5.2 Bash Variables: BASH_MONOSECONDS, EPOCHREALTIME; 6.5 Shell Arithmetic"
---

## Task

Measure how long a piece of work takes, and print the duration. Read a monotonic clock before and after the work, and subtract the two readings. Do not use the wall clock, because a person or a time service can set it forward or back while the work runs.

The real duration changes on each run, so the contract tests the arithmetic and the format. `elapsed_ms(start_ns, end_ns)` takes two readings of a monotonic clock in nanoseconds and returns the difference as `M.UUU ms`: the whole milliseconds, a dot, and three digits of microseconds. The part under one microsecond is dropped, not rounded. The readings can be negative, because the origin of a monotonic clock is arbitrary. They can be as large as a signed 64-bit integer allows.

Each solution also has `measure(work, clock)`. It reads the clock, runs the work once, reads the clock again and returns the difference. Its tests give a fake clock with fixed readings, and they check the order of the calls. One more test uses the real clock and checks only that the result is not negative.

## Solutions

Each solution keeps the readings as integer nanoseconds and uses integer division. A floating-point number of seconds loses nanoseconds when the reading is large (see the pitfalls).

- **Python.** `time.monotonic_ns()` returns an `int`. The clock cannot go backwards, and updates of the system clock do not change it. Only the difference of two readings has a meaning. The documentation says to use `monotonic_ns()` to avoid the precision loss of `float`.
- **JavaScript and TypeScript (Node.js).** `process.hrtime.bigint()` returns a `bigint` of nanoseconds. The time is relative to an arbitrary time in the past and is not related to the time of day. The division of two `bigint` values truncates. `performance.now()` returns a `number` of milliseconds. The legacy `process.hrtime()` returns an array of seconds and nanoseconds.
- **Java.** `System.nanoTime()` returns a `long` of nanoseconds from an arbitrary origin, which can be in the future, so a reading can be negative. Only a difference has a meaning. `String.format` gets `Locale.ROOT`, because `Formatter` writes the digits of the locale.
- **C#.** `TimeProvider.System.GetTimestamp()` returns a time stamp from `Stopwatch`, and `GetElapsedTime(start, end)` converts the difference to a `TimeSpan`. A test gives a subclass of `TimeProvider` that overrides `GetTimestamp` and `TimestampFrequency`.
- **Go.** `time.Now()` returns a `Time` with a wall clock reading and a monotonic clock reading. `Sub` and `time.Since` use the monotonic readings when both times have one. The fake clock in the test returns times from `time.Unix`, which have no monotonic reading, so `Sub` uses their wall readings.
- **Rust.** `Instant::now()` reads a monotonic clock. An `Instant` is opaque, so the fake clock adds fixed `Duration` values to one real `Instant`. `duration_since` returns a `Duration`.
- **C.** `clock_gettime(CLOCK_MONOTONIC, &ts)` fills a `struct timespec`. POSIX requires each implementation to support this clock, and `clock_settime` cannot set it. The code puts `tv_sec` and `tv_nsec` into one `int64_t` of nanoseconds, so a subtraction needs no borrow between the two fields. `measure` takes the clock as a function pointer.
- **C++.** `std::chrono::steady_clock` is monotonic and is not related to the wall clock. `measure` takes the clock as a template parameter, and a `static_assert` rejects a clock whose `is_steady` is false. `high_resolution_clock` can be an alias of `system_clock`, so do not use it to measure a duration.
- **Bash.** In Bash 5.3, `BASH_MONOSECONDS` expands to a reading of the monotonic clock of the system. With no monotonic clock, it is the same as `EPOCHSECONDS`. Both give whole seconds, so `monotonic_ns` gives only multiples of 10^9. `EPOCHREALTIME` has microseconds, but it counts from the Unix epoch, so it is the wall clock. `measure` takes the clock as a command. Shell arithmetic uses the largest fixed-width integers and does not check for overflow.

A real measurement prints a different number on each run. A program that must give the same output can print a check:

```python run id=python-measure
import time


def work():
    return sum(range(100_000))


start = time.monotonic_ns()
work()
elapsed = time.monotonic_ns() - start
print("elapsed >= 0:", elapsed >= 0)
```

```output for=python-measure
elapsed >= 0: True
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: two equal readings give `0.000 ms`.
- `under-one-microsecond` and `truncate-not-round`: 999 ns gives `0.000 ms`, and 1999999 ns gives `1.999 ms`. Rounding gives `0.001 ms` and `2.000 ms`.
- `leading-zero-in-fraction` and `exact-ms`: 1005000 ns is `1.005 ms`, not `1.5 ms`. The format pads the microseconds to three digits.
- `one-hour`: 3.6 × 10^12 ns. A 32-bit integer cannot hold it.
- `negative-readings` and `across-zero`: the readings are below zero, or the zero is between them. The Java documentation says that `nanoTime` can be negative.
- `beyond-2-pow-53`: the start is 9007199254740993, which a 64-bit float cannot hold. The true difference is 1000999 ns. A float subtraction gives 1001000 ns, so the result is `1.001 ms`, not `1.000 ms`.
- `near-int64-max` and `near-int64-min`: readings at the ends of the signed 64-bit range. JavaScript needs `bigint` here, and C needs `int64_t`.

## Pitfalls

- **A floating-point reading loses nanoseconds.** A 64-bit float holds each integer up to 2^53 exactly, but not each integer above it. In JavaScript, keep the `bigint` from `hrtime.bigint()`, and do not convert it to a `number` before the subtraction:

```javascript wrong id=js-number-precision
const start = 9007199254740993n;
const end = 9007199255741992n;
console.log(end - start);
console.log(Number(end) - Number(start));
```

```output for=js-number-precision
1000999n
1001000
```

- **Compare a `nanoTime` difference, not two readings.** The origin of `nanoTime` is arbitrary, so a reading can be near the end of the range of `long`, and a sum can wrap to a negative value. The Java documentation says to write `System.nanoTime() - startTime >= timeoutNanos`, not `System.nanoTime() >= startTime + timeoutNanos`:

```java wrong id=java-nanotime-timeout
public class Main {
    public static void main(String[] args) {
        long startTime = Long.MAX_VALUE - 1_000;
        long now = startTime + 2_000;
        long timeoutNanos = 500;
        System.out.println("now >= startTime + timeoutNanos: " + (now >= startTime + timeoutNanos));
        System.out.println("now - startTime >= timeoutNanos: " + (now - startTime >= timeoutNanos));
    }
}
```

```output for=java-nanotime-timeout
now >= startTime + timeoutNanos: false
now - startTime >= timeoutNanos: true
```

- **A Go `Time` loses its monotonic reading.** `t.Round(0)` and the marshal methods drop it. `time.Date`, `time.Parse`, `time.Unix` and the unmarshal methods make times without it. `Sub` on such a time uses the wall clock. Keep the `Time` from `time.Now()` in memory, and do not store it and read it back to measure. `String` shows the monotonic reading as `m=±<value>`:

```go wrong id=go-monotonic-strip
package main

import (
	"fmt"
	"strings"
	"time"
)

func main() {
	start := time.Now()
	var restored time.Time
	text, _ := start.MarshalText()
	if err := restored.UnmarshalText(text); err != nil {
		panic(err)
	}
	fmt.Println("start has m=:", strings.Contains(start.String(), "m="))
	fmt.Println("restored has m=:", strings.Contains(restored.String(), "m="))
	fmt.Println("Round(0) has m=:", strings.Contains(start.Round(0).String(), "m="))
}
```

```output for=go-monotonic-strip
start has m=: true
restored has m=: false
Round(0) has m=: false
```

- **`clock()` in C measures processor time.** `clock` returns the approximate processor time of the process, not the elapsed time. This program sleeps for 0.2 seconds. Use `clock_gettime(CLOCK_MONOTONIC)`:

```c wrong id=c-clock-cpu-time
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>

static double seconds(struct timespec ts) {
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main(void) {
    struct timespec start, end;
    clock_t cpu_start = clock();
    clock_gettime(CLOCK_MONOTONIC, &start);
    nanosleep(&(struct timespec){.tv_nsec = 200000000}, nullptr);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double cpu = (double)(clock() - cpu_start) / CLOCKS_PER_SEC;
    printf("clock() < 0.1 s: %s\n", cpu < 0.1 ? "true" : "false");
    printf("CLOCK_MONOTONIC >= 0.2 s: %s\n", seconds(end) - seconds(start) >= 0.2 ? "true" : "false");
}
```

```output for=c-clock-cpu-time
clock() < 0.1 s: true
CLOCK_MONOTONIC >= 0.2 s: true
```

- **`duration_since` hides a swapped order.** In Rust, `earlier.duration_since(later)` returns zero, not an error. Use `checked_duration_since` when the order is not certain:

```rust wrong id=rust-swapped
use std::time::{Duration, Instant};

fn main() {
    let start = Instant::now();
    let end = start + Duration::from_millis(5);
    println!("{:?}", end.duration_since(start));
    println!("{:?}", start.duration_since(end));
    println!("{:?}", start.checked_duration_since(end));
}
```

```output for=rust-swapped
5ms
0ns
None
```

- **The wall clock can go back.** Python `time.time()`, Java `System.currentTimeMillis()`, Bash `EPOCHREALTIME` and C `CLOCK_REALTIME` give the time of day. The Python documentation says that `time.time()` can return a lower value than a previous call if the system clock is set back between the two calls. A duration from two such readings can be wrong or negative. If `Stopwatch` in .NET has no high-resolution counter, its time stamp is `DateTime.UtcNow.Ticks`, which also comes from the wall clock.

## Related tasks

- [[parse-and-format-a-date]]: read and write the time of day, which comes from the wall clock.
- [[generate-random-numbers]]: another task whose output changes on each run, and how to test it.
- [[parse-an-integer]]: read a number of nanoseconds from text into a 64-bit integer.

## References
