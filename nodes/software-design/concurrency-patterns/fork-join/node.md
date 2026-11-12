---
tag: V37PWB
kind: pattern
slug: fork-join
name: Fork/Join
aliases: [fork-join, fork/join, fork–join model, fork-join parallelism]
area: software-design/concurrency-patterns
level: core
summary: >-
  Fork/Join splits a task into parts that can run in parallel, and then joins them, so that only one thread continues with the combined result. Recursive splitting gives parallel divide and conquer.
family: concurrency
polarity: positive
intent: Run the independent parts of a computation in parallel by forking a subtask for each part and joining all the subtasks before the computation continues.
also_known_as: [Fork-join model, Fork/join framework]
history:
  year: 1963
  people: [Melvin E. Conway]
  note: Wikipedia dates the fork-join model to at least 1963 and cites the paper "A multiprocessor system design" by Melvin E. Conway.
see_also: [thread-pool, futures, threads, producer-consumer, half-sync-half-async, leader-followers, deadlock, async-await]
sources:
  - url: https://en.wikipedia.org/wiki/Fork%E2%80%93join_model
    title: Fork–join model, Wikipedia
    locator: lead; Examples; Implementations
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ForkJoinTask.html
    title: Java SE 21 API, java.util.concurrent.ForkJoinTask
    locator: class description
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ForkJoinPool.html
    title: Java SE 21 API, java.util.concurrent.ForkJoinPool
    locator: class description
  - url: https://doc.rust-lang.org/std/thread/fn.scope.html
    title: The Rust Standard Library, std::thread::scope
    locator: function description
  - url: https://docs.rs/rayon/latest/rayon/fn.join.html
    title: Rayon documentation, rayon::join
    locator: function description
---

## Intent

In the fork-join model, execution branches off in parallel at designated points in the program, and then joins at a subsequent point and resumes sequential execution.[^1] Parallel sections may fork recursively until a certain task granularity is reached.[^1]

## Problem

A computation has parts that do not depend on each other, such as the two halves of a sum or a sort. One thread does the parts one after the other, and the other processors stay idle. A new operating-system thread for each part costs too much, because creating new threads tends to result in too much overhead.[^1]

## Solution

Split the problem. If it is small enough, solve it directly. If not, fork a subtask for each part, join all the subtasks, and combine their results.[^1] By nesting fork-join computations recursively, one gets a parallel version of the divide and conquer paradigm.[^1]

```text
                 solve(1..1000000)
                 /    fork     \
       solve(left half)   solve(right half)   <- run in parallel
          /    \               /     \
        ...    ...           ...    ...       <- split until small
          \    /               \     /
                 \    join     /
                 combine the sums
```

The primary coordination mechanisms in Java are `fork()`, that arranges asynchronous execution, and `join()`, that does not proceed until the result of the task has been computed.[^2] Implementations typically fork tasks or lightweight threads, not operating-system threads, and use a thread pool to execute these tasks.[^1] A work-stealing scheduler often maps the tasks onto the pool.[^1] In a `ForkJoinPool`, all threads attempt to find and execute tasks that were submitted to the pool or created by other active tasks.[^3]

## Example

The example sums the integers from 1 to 1,000,000. A range of at most 10,000 numbers is summed directly, and a larger range is split into two halves. The task forks the left half, solves the right half itself, and then joins the left half. The C# example gives both halves to `Parallel.Invoke` instead. The splits make 128 leaf ranges, and the sum matches the formula n(n+1)/2.

```text
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- The fork primitive lets the programmer specify potential parallelism, and the implementation then maps it onto actual parallel execution.[^1]
- The scheduler can be much simpler than a full operating-system scheduler, because in the fork-join paradigm threads only block at the join point.[^1]
- Huge numbers of tasks and subtasks can run on a small number of actual threads in a `ForkJoinPool`.[^2]

Liabilities:

- The task size matters. If tasks are too big, then parallelism cannot improve throughput. If they are too small, then memory and internal task maintenance overhead may overwhelm processing.[^2] As a very rough rule of thumb, a Java task should perform more than 100 and less than 10000 basic computational steps.[^2]
- Tasks should avoid synchronized methods or blocks, should minimize other blocking synchronization apart from joining other tasks, and should not perform blocking I/O.[^2]
- A plain thread pool that blocks a worker on each join can stall. A `ForkJoinPool` tries to keep enough active threads even if some tasks are stalled waiting to join others, but it does not guarantee this in the face of blocked I/O or other unmanaged synchronization.[^3]

This program forks each half into a plain pool of 2 workers and blocks the worker on the join. Both workers soon wait for subtasks that are still in the queue:

```python wrong id=join-in-fixed-pool
from concurrent.futures import ThreadPoolExecutor, TimeoutError

pool = ThreadPoolExecutor(max_workers=2)


def solve(lo, hi):
    if hi - lo <= 10_000:
        return sum(range(lo, hi))
    mid = (lo + hi) // 2
    # Wrong: each task submits its halves to the same small pool and blocks.
    left = pool.submit(solve, lo, mid)
    right = pool.submit(solve, mid, hi)
    return left.result(timeout=1) + right.result(timeout=1)


try:
    print("sum:", pool.submit(solve, 1, 1_000_001).result())
except TimeoutError:
    print("join timed out: both workers wait for subtasks that no worker can run")
pool.shutdown(cancel_futures=True)
```

```output for=join-in-fixed-pool
join timed out: both workers wait for subtasks that no worker can run
```

## When not to use it

Do not use fork/join when the parts depend on each other, or when each part waits for I/O. Subdividable tasks should ideally access variables that are completely independent of those accessed by other running tasks.[^2] Do not use it when the whole problem is smaller than one good leaf task, because the cost of the forks is then larger than the work.

## Variants and related patterns

- **Fork one part, do the other.** In the parallel merge sort of CLRS, the first recursive call is forked, and the main task handles the second recursive call itself. This is intentional, because forking tasks may come at an expense. If both calls were subtasks, the main task would have no additional work to do before it is blocked at the join.[^1] The next program counts the threads that each way starts:

```python run id=fork-one-or-both
import threading

started = {"count": 0}
lock = threading.Lock()


def fork(fn, *args):
    out = {}
    t = threading.Thread(target=lambda: out.update(result=fn(*args)))
    with lock:
        started["count"] += 1
    t.start()
    return t, out


def solve_fork_both(lo, hi):
    if hi - lo <= 10_000:
        return sum(range(lo, hi))
    mid = (lo + hi) // 2
    (t1, a), (t2, b) = fork(solve_fork_both, lo, mid), fork(solve_fork_both, mid, hi)
    t1.join()  # this thread only waits
    t2.join()
    return a["result"] + b["result"]


def solve_fork_one(lo, hi):
    if hi - lo <= 10_000:
        return sum(range(lo, hi))
    mid = (lo + hi) // 2
    t, a = fork(solve_fork_one, lo, mid)
    right = solve_fork_one(mid, hi)  # this thread does the other half
    t.join()
    return a["result"] + right


for solve in (solve_fork_both, solve_fork_one):
    started["count"] = 0
    total = solve(1, 1_000_001)
    print(f"{solve.__name__}: sum {total}, threads started {started['count']}")
```

```output for=fork-one-or-both
solve_fork_both: sum 500000500000, threads started 254
solve_fork_one: sum 500000500000, threads started 127
```

- **Join compared with a barrier.** A join may look like a barrier, but after a barrier the threads continue to work, and after a join only one thread continues.[^1]
- **Scoped threads.** In Rust, `std::thread::scope` creates a scope for spawning threads that can borrow non-`'static` data, because the scope guarantees that all threads are joined at the end of the scope.[^4]
- **Work stealing.** `rayon::join` takes two closures and potentially runs them in parallel. The calling thread runs closure A and advertises closure B to other threads. If B has been stolen, the calling thread looks for other work while it waits for the thief to execute B.[^5]

A [thread pool](thread-pool) runs the tasks of a fork/join framework, and a [future](futures) holds the result that a join waits for.

## Known uses

- Fork-join is the main model of parallel execution in OpenMP.[^1]
- The Java concurrency framework, the Task Parallel Library for .NET and Intel Threading Building Blocks support it.[^1]
- The Cilk language has the `spawn` and `sync` keywords, and Cilk Plus has `cilk_spawn` and `cilk_sync`.[^1]
- Java has `ForkJoinPool` with a static `commonPool()` that is appropriate for most applications.[^3]
- The Rust library Rayon uses a fixed pool of worker threads and work stealing.[^5]

## References
