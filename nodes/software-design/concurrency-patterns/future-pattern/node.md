---
tag: 2GNXEZ
kind: pattern
slug: future-pattern
name: Future as a design pattern
aliases: [future pattern, future, promise, placeholder for a later result]
area: software-design/concurrency-patterns
level: core
summary: >-
  A call starts slow work and returns a future at once. The caller does other work and asks the future for the value when it needs it, and it waits only if the value is not ready.
family: concurrency
polarity: positive
intent: Let a caller start a computation without waiting for it, and give the caller an object that delivers the result, or the error, when the computation completes.
also_known_as: [Promise, Deferred]
history:
  year: 1977
  people: [Henry G. Baker, Jr., Carl Hewitt]
  note: Baker and Hewitt introduced the future in a 1977 paper on call-by-future. Wikipedia notes that Daniel P. Friedman and David Wise proposed the term promise in 1976.
see_also: [futures, thread-pool, active-object, asynchronous-completion-token, async-await, callback-pattern, fork-join, proxy-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Futures_and_promises
    title: Futures and promises, Wikipedia
    locator: lead; Implicit vs. explicit; Promise pipelining; Blocking vs non-blocking semantics
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Future.html
    title: Java SE 21 API, java.util.concurrent.Future
    locator: the interface description; cancel
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: concurrent.futures, Launching parallel tasks, Python documentation
    locator: Future Objects; ThreadPoolExecutor
  - url: https://plover.com/~mjd/misc/hbaker-archive/Futures.html
    title: Henry G. Baker, Jr. and Carl Hewitt, The Incremental Garbage Collection of Processes (1977)
    locator: abstract
---

## Intent

A future is an object that acts as a proxy for a result that is initially unknown, usually because the computation of its value is not yet complete.[^1] As a design pattern, a call starts the computation and returns a future at once. The caller continues with other work and asks the future for the value only when it needs it.

This page states the pattern. The page [futures](futures) describes the mechanism and how each language provides it.

## Problem

A caller needs the results of several slow calls, for example three price lookups on other machines. If each call blocks until its result is ready, the calls run one after another, and the caller can do nothing else while it waits. If the calls run on other threads with no return value, the caller needs a separate way to collect each result and each error.

## Solution

Each call starts the work, for example on a [thread pool](thread-pool), and returns a future. A future represents the result of an asynchronous computation, and it has methods to check if the computation is complete, to wait for its completion, and to retrieve the result.[^2] The result can only be retrieved when the computation has completed, and the call that retrieves it blocks if necessary until it is ready.[^2]

```text
 caller                       future                 worker thread
   │ submit(lookup, "apple")    │                        │
   ├──────────────────────────► │ ── start lookup ─────► │
   │ ◄── future (not done) ──── │                        │
   │ (other work)               │                        │ (runs lookup)
   │                            │ ◄── set result 12 ──── │
   │ result() ────────────────► │                        │
   │ ◄── 12 ─────────────────── │                        │
```

When the two sides are distinguished, the future is a read-only placeholder view of a variable, and the promise is a writable, single assignment container which sets the value of the future.[^1] The worker holds the promise side, and the caller holds the future side.

## Example

The caller starts three price lookups, and each call returns a future at once. No future is done right after the calls, because each lookup waits at a barrier until all three lookups and the caller arrive. That shows that the three lookups ran at the same time while the caller was free. Then the caller reads the three results in the order of the calls.

```text
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

The examples for each language show the same program.

## Consequences

Benefits:

- The caller does not wait at the call. In the example of the Java `Future` documentation, the caller submits a search and then has the comment "do other things while searching" before it calls `get`.[^2]
- The result and the error travel together. In Python, if the call raised an exception, `result()` will raise the same exception.[^3]
- The caller decides when to wait. Python's `result(timeout)` waits up to the timeout and then raises a `TimeoutError`.[^3]

Liabilities:

- Deadlocks can occur when the callable associated with a future waits on the results of another future.[^3] In a pool with a fixed number of threads, a task that waits for a future of the same pool can block a thread that the other task needs.
- Work can become useless. Baker and Hewitt describe futures which were thought to be relevant when they were created but become irrelevant because the program ignores them.[^4] In Java, cancellation is performed by the `cancel` method, but once a computation has completed, the computation cannot be cancelled.[^2]
- A blocking wait can hide where the work runs. In C++, if the future came from `std::async()`, a blocking wait without a timeout may run the function on the waiting thread.[^1]

This program waits for each future right after the call that makes it. The results are correct, but the lookups never overlap:

```python wrong id=wait-right-after-the-call
import threading
import time
from concurrent.futures import ThreadPoolExecutor

PRICES = {"apple": 12, "bread": 7, "cheese": 30}
lock = threading.Lock()
running = 0
most_running = 0


def lookup(item):
    global running, most_running
    with lock:
        running += 1
        most_running = max(most_running, running)
    time.sleep(0.05)  # a slow lookup
    with lock:
        running -= 1
    return PRICES[item]


with ThreadPoolExecutor(max_workers=3) as pool:
    # Wrong: wait for each future right after the call that makes it.
    prices = [pool.submit(lookup, item).result() for item in PRICES]

print("prices:", prices)
print("most lookups running at once:", most_running)
```

```output for=wait-right-after-the-call
prices: [12, 7, 30]
most lookups running at once: 1
```

Start all the calls first, keep the futures in a list, and then read the results, as the example does.

## When not to use it

Do not use a future when the caller needs the value at once and has no other work to do. A plain call gives the same result with less code. Do not use one future for a stream of many values. A queue in the [producer-consumer](producer-consumer) pattern, or a channel, fits that case. Do not let tasks in a small, fixed pool wait on futures of the same pool, because of the deadlock above. The [fork-join](fork-join) page shows a pool for tasks that wait for their subtasks.

## Variants and related patterns

- **Explicit and implicit futures.** With an explicit future, the user must call a function to obtain the value, such as the `get` method of `Future` in Java.[^1] With an implicit future, any use of the future obtains its value, as if it were an ordinary reference.[^1] The original Baker and Hewitt paper described implicit futures.[^1] With an explicit future, the call that reads the value is also where the error of the computation appears:

```python run id=future-carries-the-error
from concurrent.futures import ThreadPoolExecutor

PRICES = {"apple": 12}


def lookup(item):
    return PRICES[item]  # raises KeyError for an unknown item


with ThreadPoolExecutor(max_workers=2) as pool:
    ok = pool.submit(lookup, "apple")
    bad = pool.submit(lookup, "caviar")
    print("price of apple:", ok.result())
    try:
        bad.result()  # the exception of the call comes back here
    except KeyError as e:
        print("lookup of caviar raised:", repr(e))
```

```output for=future-carries-the-error
price of apple: 12
lookup of caviar raised: KeyError('caviar')
```

- **Callbacks on completion.** The caller attaches the next step to the future, and it does not wait. Python's `add_done_callback(fn)` attaches the callable `fn` to the future, and `fn` is called when the future is cancelled or finishes running.[^3] See the [callback pattern](callback-pattern) and [async/await](async-await).
- **Promise pipelining.** The caller sends calls to a future before its value is known. The use of futures can dramatically reduce latency in distributed systems, for example with promise pipelining, as implemented in the languages E and Joule.[^1]

An [active object](active-object) returns a future for each request that it puts on its queue. An [asynchronous completion token](asynchronous-completion-token) is the other way to match a result to its request: the caller passes a token, and the completion event brings it back.

## Known uses

- Java `java.util.concurrent.Future`, which an executor's `submit` returns.[^2]
- Python `concurrent.futures.Future`, which `Executor.submit()` creates.[^3]
- C++ `std::future`, which `std::async()` returns, with the `wait()` and `get()` member functions.[^1]
- Promise pipelining in E and Joule, which the language Argus called call-stream.[^1]

## References
