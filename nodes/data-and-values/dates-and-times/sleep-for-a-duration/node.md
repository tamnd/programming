---
tag: W7XAT5
kind: task
slug: sleep-for-a-duration
name: Sleep for a duration
aliases: [sleep, pause the program, wait for a number of seconds, delay, time.sleep, Thread.sleep, Task.Delay, setTimeout, nanosleep, sleep_for, asyncio.sleep, sleep in async code]
area: data-and-values/dates-and-times
level: intro
summary: Pause the program for at least a given time. A sleep can end late or early, so loop to a deadline on a monotonic clock. In async code, use the sleep of the event loop.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/time.html
    title: The Python Standard Library, time
    locator: time.sleep
  - url: https://docs.python.org/3/library/asyncio-task.html
    title: The Python Standard Library, Coroutines and Tasks
    locator: Sleeping
  - url: https://nodejs.org/api/timers.html
    title: Node.js documentation, Timers
    locator: setInterval and setTimeout; Timers Promises API
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Window/setTimeout
    title: MDN, setTimeout() global function
    locator: delay; Maximum delay value
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
    title: Java SE 21 API, java.lang.Thread
    locator: sleep(long millis)
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/TimeUnit.html
    title: Java SE 21 API, java.util.concurrent.TimeUnit
    locator: sleep(long timeout)
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/System.html
    title: Java SE 21 API, java.lang.System
    locator: nanoTime()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.thread.sleep
    title: .NET API reference, Thread.Sleep
    locator: Sleep(Int32), Exceptions and Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.task.delay
    title: .NET API reference, Task.Delay
    locator: Delay(Int32), Exceptions and Remarks
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func Sleep
  - url: https://doc.rust-lang.org/std/thread/fn.sleep.html
    title: The Rust Standard Library, std::thread::sleep
    locator: the description and Platform-specific behavior
  - url: https://man7.org/linux/man-pages/man2/nanosleep.2.html
    title: Linux manual page, nanosleep(2)
    locator: DESCRIPTION and NOTES
  - url: https://man7.org/linux/man-pages/man2/clock_nanosleep.2.html
    title: Linux manual page, clock_nanosleep(2)
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/thread/sleep_for
    title: cppreference.com, std::this_thread::sleep_for
    locator: the description of the function
  - url: https://en.cppreference.com/w/cpp/thread/sleep_until
    title: cppreference.com, std::this_thread::sleep_until
    locator: the description of the function
  - url: https://man7.org/linux/man-pages/man1/sleep.1.html
    title: Linux manual page, sleep(1)
    locator: DESCRIPTION
---

## Task

Pause the program for at least a given duration, for example 20 milliseconds, and then go on. In async code, pause only the current task and let the other tasks run.

Each language has a sleep function. Each one has the same three problems:

- **It can end late.** The operating system wakes the thread when the time is up, but the thread can wait more before it runs. The Linux manual says that the interval "will be rounded up to the next multiple" of the granularity of the clock, and that "there may still be a delay before the CPU becomes free to once again execute the calling thread". So a sleep gives a minimum time, not an exact time.
- **It can end early.** In C, a signal ends `nanosleep` early, and it "returns -1, sets errno to EINTR, and writes the remaining time" into its second argument. Some functions also take a smaller unit or a smaller range than the duration that you need, so one call cannot sleep for all of it.
- **It blocks.** A sleep stops the whole thread. In async code, the thread also runs the other tasks, so they stop too.

The answer to the first two problems is a loop to a deadline. Read a monotonic clock, and add the duration to get the deadline. Then sleep for the rest of the time until the clock reaches the deadline:

```text
now = clock()
deadline = now + duration
while now < deadline:
    sleep(deadline - now)
    now = clock()
```

A sleep that ends early is followed by a sleep for the rest of the time. A sleep that ends late ends the loop. Use a monotonic clock, which never goes back. A wall clock can jump when the system sets the time, and then the sleep is too short or too long. See [[measure-elapsed-time]].

A real sleep takes real time, so the contract tests the loop on a fake clock. `sleep_requests(duration_ns, start_ns, readings)` runs the loop with a clock that gives `start_ns` first and then the `readings`, one after each sleep. It returns the list of durations that the loop asks the sleep for, in nanoseconds. For example, a sleep of 1 ms from 0 that wakes at 0.6 ms asks for `[1000000, 400000]`. A duration of zero or less gives an empty list, with no sleep.

Each solution also has `sleep_for(duration_ns)`, which runs the same loop with the real monotonic clock and the real sleep of the language. The test of each language sleeps for 20 ms and checks that at least 20 ms went by on the monotonic clock.

## Solutions

- **Python.** `time.monotonic_ns()` is the clock, and `time.sleep(ns / 1e9)` sleeps. The function sleeps at least the given time even if a signal interrupts it, unless the signal handler raises an exception. In async code, `sleep_for_async` uses `await asyncio.sleep(...)`. The documentation says that "sleep() always suspends the current task, allowing other tasks to run".
- **JavaScript.** There is no sleep that blocks. `setTimeout` from `node:timers/promises` returns a promise, and `await` suspends only the current function. `process.hrtime.bigint()` is the clock. The delay is in whole milliseconds, so the solution rounds the rest up, and it cuts the delay to 2147483647 ms. See the pitfalls.
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** `System.nanoTime()` is the clock, and `TimeUnit.NANOSECONDS.sleep(ns)` sleeps. `Thread.sleep` throws `InterruptedException` if another thread interrupts the thread, and the solution passes it on to the caller. The documentation of `nanoTime` says to compare the difference of two readings, `System.nanoTime() - startTime >= timeoutNanos`, "because of the possibility of numerical overflow", so the loop compares `deadline - now` with zero.
- **C#.** `Stopwatch.GetTimestamp()` is the clock. `Thread.Sleep` and `Task.Delay` take whole milliseconds, so the solution rounds the rest up and cuts it to `int.MaxValue`. `SleepForAsync` uses `await Task.Delay(ms, cancel)`, which frees the thread and can be canceled.
- **Go.** `time.Sleep` "pauses the current goroutine for at least the duration d". `time.Since` uses the monotonic reading of a `time.Time`. A goroutine that sleeps does not stop the other goroutines. `SleepContext` waits on a timer and on `ctx.Done()` with `select`, so a caller can cancel it.
- **Rust.** `Instant` is the clock. `std::thread::sleep` "will never sleep less" than the duration. On Unix, "this function may invoke that system call multiple times". It "should not be used in async functions". The standard library has no async runtime, so the solution has no async version.
- **C.** `clock_gettime(CLOCK_MONOTONIC, ...)` is the clock, and `nanosleep` sleeps. If a signal ends `nanosleep` early, the loop reads the clock and sleeps for the rest. On Linux, `clock_nanosleep` with `TIMER_ABSTIME` takes the deadline itself, and "If t is less than or equal to the current value of the clock, then clock_nanosleep() returns immediately".
- **C++.** `steady_clock` is the clock, and `std::this_thread::sleep_for` "Blocks the execution of the current thread for at least the specified sleep_duration". `std::this_thread::sleep_until(deadline)` takes the deadline itself and "Blocks the execution of the current thread until specified sleep_time has been reached".
- **Bash.** GNU `sleep` takes a number of seconds that can have a fraction: "NUMBER is an integer or floating-point". The function writes the nanoseconds as seconds with nine decimal places. Bash has no monotonic clock, so the function makes one call of `sleep`, and `sleep_requests` only shows the loop.

Most languages also have a sleep that a caller can cancel. In Go, `select` waits for the first of a timer and a context:

```go run id=go-sleep-or-cancel
package main

import (
	"context"
	"fmt"
	"time"
)

func main() {
	ctx, cancel := context.WithTimeout(context.Background(), 50*time.Millisecond)
	defer cancel()
	start := time.Now()
	select {
	case <-time.After(time.Hour):
		fmt.Println("slept for an hour")
	case <-ctx.Done():
		fmt.Println(ctx.Err())
	}
	fmt.Println(time.Since(start) < time.Second)
}
```

```output for=go-sleep-or-cancel
context deadline exceeded
true
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`, `negative` and `most-negative`: a duration of zero or less asks for no sleep. Some sleep functions throw for a negative value, and C# waits forever for -1. See the pitfalls.
- `woke-early`, `several-early-wakes` and `no-progress`: each sleep that ends early is followed by a sleep for the rest. A wake with no progress of the clock asks for the same duration again.
- `woke-late`, `exact-deadline` and `extra-readings`: a reading at or after the deadline ends the loop.
- `one-ns-short`: a reading 1 ns before the deadline asks for one more sleep of 1 ns.
- `negative-origin` and `origin-after-100-days`: the origin of a monotonic clock is arbitrary, so a reading can be negative or larger than 2^53. JavaScript uses `bigint`.
- `one-nanosecond` and `sub-millisecond`: a sleep in whole milliseconds must round up. If it rounds 0.5 ms down to 0, the loop calls it again and again until the time is up.
- `one-hour` and `thirty-days`: 30 days is more than 2147483647 ms, so the JavaScript and C# solutions sleep two times.

## Pitfalls

- **A blocking sleep in async code.** `time.sleep` in a coroutine stops the event loop, so two tasks sleep one after the other:

```python wrong id=python-blocking-sleep-in-async
import asyncio
import time


async def blocking():
    time.sleep(0.1)


async def suspending():
    await asyncio.sleep(0.1)


async def two(job):
    start = time.monotonic()
    await asyncio.gather(job(), job())
    return time.monotonic() - start


print("time.sleep, 0.2 s or more:", asyncio.run(two(blocking)) >= 0.2)
print("asyncio.sleep, 0.2 s or more:", asyncio.run(two(suspending)) >= 0.2)
```

```output for=python-blocking-sleep-in-async
time.sleep, 0.2 s or more: True
asyncio.sleep, 0.2 s or more: False
```

The same is true for `Thread.Sleep` in a C# `async` method, and for `std::thread::sleep` in an async Rust function.

- **A delay larger than a timer can hold.** Node.js says: "When delay is larger than 2147483647 or less than 1 or NaN, the delay will be set to 1." So a timer for 25 days fires at once, and Node.js writes only a warning to stderr:

```javascript wrong id=js-settimeout-overflow
const start = performance.now();
setTimeout(() => {
  console.log("fired in less than 1 s:", performance.now() - start < 1000);
}, 2 ** 31);
```

```output for=js-settimeout-overflow
fired in less than 1 s: true
```

Node.js also says that "Non-integer delays are truncated to an integer", so a delay of 1.9 ms is 1 ms. Round up, and cut a long delay into parts.

- **-1 means forever in C#.** `Timeout.Infinite` is -1, and `Thread.Sleep` throws `ArgumentOutOfRangeException` only if "The time-out value is negative and is not equal to Infinite". A duration that a subtraction makes -1 waits forever:

```csharp wrong id=csharp-sleep-minus-one
using System.Threading;
using System.Threading.Tasks;

Console.WriteLine(Timeout.Infinite);
Console.WriteLine(Task.Delay(-1).Wait(100));
try
{
    Thread.Sleep(-2);
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-sleep-minus-one
-1
False
ArgumentOutOfRangeException
```

Other languages differ for a negative duration too. Go says that "A negative or zero duration causes Sleep to return immediately". Java `Thread.sleep` throws `IllegalArgumentException` "if the value of millis is negative". Python raises `ValueError`:

```python wrong id=python-negative-sleep
import time

try:
    time.sleep(-0.001)
except ValueError as e:
    print(type(e).__name__, e)
```

```output for=python-negative-sleep
ValueError sleep length must be non-negative
```

Check for a duration of zero or less before the call, as the loop does.

- **A sleep of the same length in a periodic loop drifts.** A loop that does work and then sleeps for one period takes the period plus the time of the work on each pass, so it falls behind more and more. The Linux manual says the same of a sleep that restarts after a signal: "the time between the interruptions and restarts of the call will lead to drift in the time when the sleep finally completes. This problem can be avoided by using clock_nanosleep(2) with an absolute time value." Keep a deadline, add the period to it on each pass, and sleep until it.

- **A deadline on the wall clock.** `time.time()`, `Date.now()` and `System.currentTimeMillis()` follow the time of day, which the system can set back or forward. Read a deadline on a monotonic clock.

## Related tasks

- [[measure-elapsed-time]]: the monotonic clock that the loop reads.
- [[format-a-duration]]: show a duration as hours, minutes and seconds.
- [[get-the-current-date-and-time]]: the wall clock, which is the wrong clock for a deadline.

## References
