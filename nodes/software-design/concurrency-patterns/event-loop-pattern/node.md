---
tag: 18YDA2
kind: pattern
slug: event-loop-pattern
name: Event loop
aliases: [main loop, main event loop, message loop, message pump, message dispatcher, run loop]
area: software-design/concurrency-patterns
level: core
summary: >-
  An event loop is one thread that takes events and callbacks from a queue and runs each one to the end. It handles many tasks without locks, if no callback blocks the loop.
family: concurrency
polarity: positive
intent: Handle many independent events on one thread by putting their handlers on a queue and running the handlers one at a time, each to its end, so that the handlers do not need locks.
also_known_as: [Main loop, Message loop, Message pump]
see_also: [reactor, proactor, event-driven-programming, async-await, callback-pattern, thread-confinement, half-sync-half-async, futures]
sources:
  - url: https://en.wikipedia.org/wiki/Event_loop
    title: Event loop, Wikipedia
    locator: lead; Examples
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Execution_model
    title: JavaScript execution model, MDN Web Docs
    locator: Job queue and event loop; Run-to-completion; Never blocking
  - url: https://nodejs.org/en/learn/asynchronous-work/event-loop-timers-and-nexttick
    title: The Node.js Event Loop, Node.js documentation
    locator: What is the Event Loop?; Event Loop Explained; Phases Overview
  - url: https://docs.python.org/3/library/asyncio-eventloop.html
    title: Event loop, asyncio, Python documentation
    locator: Preface; loop.call_soon; loop.call_later; loop.call_soon_threadsafe; loop.run_in_executor
---

## Intent

An event loop is an algorithm that continually dispatches control flow for events: the loop requests the next event from an event provider, and when an event is received, invokes its associated event handler.[^1] When the event loop is the central event dispatcher of a program, it is called the main loop or main event loop.[^1] Because one thread runs all the handlers, the handlers do not need locks to share data.

## Problem

A program waits for many things at the same time: user input, timers, network replies and files. A thread for each wait uses memory, and threads that share data need locks. A program that waits for one thing at a time cannot respond to the others while it waits.

## Solution

Put the code that handles each event in a callback, and put the callbacks on a queue. One thread runs a loop. In each turn, it takes the callbacks that are ready and runs each one to its end. When no callback is ready, the loop waits for the next event or the next timer.

```text
        ┌──────────────── one thread ────────────────┐
        │  ┌─► wait for I/O or the next timer         │
 timers ┼─►│   move due timers to the ready queue     │
 I/O    ┼─►│   run each ready callback to its end ──┐ │
        │  └────────────── next turn ◄──────────────┘ │
        └─────────────────────────────────────────────┘
```

In JavaScript, the code that handles the completion of an asynchronous action is a callback, and this callback defines a job, which gets placed into a job queue once the action is completed.[^2] When a job is executed, it may create more jobs, which are added to the end of the queue.[^2] Each job is processed completely before any other job is processed, so a function cannot be preempted and runs entirely before any other code runs.[^2] In asyncio, callbacks that `loop.call_soon()` schedules are called in the order in which they are registered, at the next iteration of the event loop.[^4]

The loop does not do slow I/O itself. Node.js performs non-blocking I/O operations, although a single JavaScript thread is used by default, by offloading operations to the system kernel whenever possible; when one of these operations completes, the kernel tells Node.js so that the callback can be added to the poll queue.[^3]

## Example

The program builds a small event loop with a queue of ready callbacks and a list of timers. It adds two timers, then two callbacks. The first callback adds a third callback, which waits for the next turn. When no callback is ready, the loop moves its clock to the next timer. A real loop sleeps or waits for I/O at that point; this clock is a counter, so the output does not depend on the speed of the machine.

```text
order of the callbacks:
  time 0: first callback
  time 0: second callback
  time 0: callback that the first callback added
  time 10: timer of 10
  time 20: timer of 20
turns of the loop: 4
```

The examples for each language show the same program.

## Consequences

Benefits:

- One thread handles many events, and the callbacks do not need locks, because a callback cannot be stopped in the middle to run another callback.[^2]
- The order is easy to follow. In the example, the callbacks run in the order they were added, and the timers run in the order of their due times.
- The program stays responsive while it waits. When the application is waiting for a query or a `fetch()` request to return, it can still process other things like user input.[^2]

Liabilities:

- A slow callback stops the whole program. If a job takes too long to complete, the web application is unable to process user interactions like click or scroll.[^2]
- The loop uses one processor. Work that needs a lot of processing must go to other threads or processes.
- Other threads must not touch the loop directly. In asyncio, `call_soon()` is not thread-safe, and another thread must use `loop.call_soon_threadsafe()` to schedule a callback.[^4]
- Code that waits must be split into callbacks, or written with `async` and `await`, which the loop runs as a series of callbacks.

In this program, a callback calls `time.sleep`, so the loop cannot run the timer when it is due:

```python wrong id=blocking-callback
import asyncio
import time


def blocking_work():
    time.sleep(0.3)  # wrong: this blocks the one thread that runs the loop


async def main():
    loop = asyncio.get_running_loop()
    start = time.monotonic()
    ran_at = []
    loop.call_later(0.05, lambda: ran_at.append(time.monotonic() - start))
    loop.call_soon(blocking_work)
    await asyncio.sleep(0.4)
    print("timer due after 0.05 s")
    print("timer ran only after the blocking work, at 0.3 s or later:", ran_at[0] >= 0.3)


asyncio.run(main())
```

```output for=blocking-callback
timer due after 0.05 s
timer ran only after the blocking work, at 0.3 s or later: True
```

## When not to use it

Do not use an event loop for work that is mostly computation. One thread does not use the other processors, and long callbacks make the loop late for every other event. A [thread pool](thread-pool) or [fork-join](fork-join) fits that work. Do not use an event loop when the program must call libraries that block and cannot be changed, unless the loop can send those calls to other threads.

## Variants and related patterns

- **Blocking work on a thread pool.** asyncio's `loop.run_in_executor()` arranges for a function to be called in an executor,[^4] which is a thread pool by default. The loop keeps running while the function runs:

```python run id=offload-the-blocking-work
import asyncio
import time


def blocking_work():
    time.sleep(0.3)  # runs on a worker thread, not on the loop thread


async def main():
    loop = asyncio.get_running_loop()
    start = time.monotonic()
    ran_at = []
    loop.call_later(0.05, lambda: ran_at.append(time.monotonic() - start))
    await loop.run_in_executor(None, blocking_work)  # the loop keeps running meanwhile
    print("timer due after 0.05 s")
    print("timer ran before the blocking work ended:", ran_at[0] < 0.3)


asyncio.run(main())
```

```output for=offload-the-blocking-work
timer due after 0.05 s
timer ran before the blocking work ended: True
```

- **Phases.** The Node.js event loop has phases, such as timers, poll and check, and each phase has a FIFO queue of callbacks to execute.[^3]
- **Tasks and microtasks.** HTML event loops split jobs into two categories, and the microtask queue is drained first before the task queue is pulled.[^2]
- **[Reactor](reactor)** is the event loop for I/O: it waits on many handles and dispatches the handler of each handle that is ready. **[Proactor](proactor)** dispatches the handler when an asynchronous operation completes.
- **[Half-sync/half-async](half-sync-half-async)** joins an event loop for I/O with threads for the slow work, as the run block does.
- **[Thread confinement](thread-confinement)** is the reason that the loop needs no locks: only the loop thread touches the data of the callbacks.

## Known uses

- Web browsers: a web page and its JavaScript typically run in a single-threaded web browser process, which deals with messages from a queue one at a time.[^1]
- Node.js runs JavaScript callbacks on its event loop.[^3]
- Python: the event loop is the core of every asyncio application, and runs asynchronous tasks and callbacks, network I/O operations and subprocesses.[^4]
- Windows: a process that interacts with the user must accept and react to incoming messages, which is almost inevitably done by a message loop in that process.[^1]
- X Window System: applications that use Xlib directly are built around the `XNextEvent` family of functions.[^1]
- GLib: the GLib event loop was originally created for use in GTK, but it is now used in non-GUI applications as well, such as D-Bus.[^1]

## References
