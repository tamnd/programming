---
tag: 2J1DHE
kind: pattern
slug: master-worker-pattern
name: Master-worker
aliases: [master-worker, master-slave, manager-worker, boss-worker, controller-worker, scatter and gather of work]
area: software-design/concurrency-patterns
level: core
summary: >-
  In master-worker, a master splits a job into parts, gives the parts to identical workers that run at the same time, and combines their results into the final result.
family: concurrency
polarity: positive
intent: Split a job into independent parts, give the parts to identical workers that run concurrently, and let one master combine the partial results, so that the client sees one job and one result.
also_known_as: [Master-Slave]
history:
  year: 1996
  people: [Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad, Michael Stal]
  note: Master-Slave is one of the patterns of Pattern-Oriented Software Architecture, Volume 1, which John Wiley and Sons published in 1996. Because of the association of the old name with slavery, many projects use other terms; in 2018 Python replaced slave with worker, child or helper, depending on the context.
see_also: [fork-join, thread-pool, worker-queue, future-pattern, pipeline-concurrency, barrier-pattern, data-races, threads]
sources:
  - url: https://www.cs.uni.edu/~wallingf/teaching/062/sessions/support/pattern-examples.pdf
    title: "Examples to Accompany: Design Patterns, Elements of Reusable Object-Oriented Software, AG Communication Systems, 1999"
    locator: Master-Slave, page A-45
  - book: isbn:9780471958697
    title: "Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad and Michael Stal, Pattern-Oriented Software Architecture, Volume 1: A System of Patterns, John Wiley and Sons"
    locator: Master-Slave
  - url: https://en.wikipedia.org/wiki/Master%E2%80%93slave_(technology)
    title: Master–slave (technology), Wikipedia
    locator: lead; Controversy, Notable events
  - url: https://docs.python.org/3/library/multiprocessing.html
    title: multiprocessing, Process-based parallelism, Python documentation
    locator: Introduction; Pool.map; ThreadPool
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ExecutorService.html
    title: Interface ExecutorService, Java SE 21 API
    locator: the interface description; invokeAll
---

## Intent

A master component distributes work to identical worker components and computes a final result from the results when the workers return.[^1] The pattern is Master-Slave in Pattern-Oriented Software Architecture, Volume 1.[^2] This page uses the name master-worker. The old terminology is a subject of controversy because of its association with slavery, and it has been replaced with other terms in some cases.[^3]

## Problem

A job is too slow for one thread, but it splits into parts that do not depend on each other, for example a count over a large range or a test over many files. The client wants to ask for the job and get one result. It does not want to know how many threads did the work, or how the job was split.

## Solution

The master takes the job from the client and splits it into parts. It gives each part to a worker, and the workers run at the same time. Each worker computes a partial result for its part and gives it back. The master waits for all the workers, then combines the partial results into the final result.

```text
                      ┌─► worker 1: part 1 ─┐
 client ─► master ────┼─► worker 2: part 2 ─┼──► master combines ─► client
          splits      ├─► worker 3: part 3 ─┤    the results
                      └─► worker 4: part 4 ─┘
```

The workers are identical and interchangeable. In the example from the AG Communication Systems booklet, the census bureau is the master, which takes the census by dividing the work into smaller tasks, and the census takers are the workers, which report their work to the bureau and are interchangeable.[^1] Clients deal directly with the master, not with the workers.[^1]

## Example

The master counts the primes below 20000. It splits the range into four equal parts, gives each part to a worker thread, waits for all of them and adds up the four counts. Each worker writes only its own result slot, so the workers share no data that changes. The last line checks the master against one loop.

```text
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- The workers are exchangeable and extensible.[^1] The master can use more workers without a change to the worker code.
- The code of the client and the workers is separate from the code that splits the work.[^1] Only the master knows how the job is split and combined.
- The pattern allows parallel computing, because all the workers can work at the same time.[^1]

Liabilities:

- The job must split into parts that are independent. If the parts depend on each other, the workers must communicate, and the master is no longer enough.
- The master must split the job correctly and wait for every worker. A part that the split misses, or a result that the master reads too early, gives a wrong result with no error.
- Equal parts do not always take equal time. In the example, a part with larger numbers needs more trial divisions, so the master waits for the slowest worker.
- Splitting, starting the workers and combining the results take time. For a small job, one loop is faster.

In this program, the master splits 1002 items into four parts of `1002 // 4` items, so two items are in no part:

```python wrong id=split-loses-items
import threading

items = list(range(1, 1003))  # 1002 numbers to add up
WORKERS = 4

# Wrong: each part has len // WORKERS items, so the last 1002 % 4 items are in no part.
size = len(items) // WORKERS
parts = [items[i * size:(i + 1) * size] for i in range(WORKERS)]
results = [0] * WORKERS


def worker(i):
    results[i] = sum(parts[i])


threads = [threading.Thread(target=worker, args=(i,)) for i in range(WORKERS)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("items in the parts:", sum(len(p) for p in parts), "of", len(items))
print("total from the master:", sum(results))
print("right total:", sum(items))
```

```output for=split-loses-items
items in the parts: 1000 of 1002
total from the master: 500500
right total: 502503
```

The bounds `n * i // workers` in the example cover every item, because the last bound is `n` itself.

## When not to use it

Do not use master-worker when the parts depend on each other, or when the parts appear over time instead of all at the start. A [work queue](worker-queue) fits tasks that arrive over time, and a [pipeline](pipeline-concurrency) fits work that goes through fixed steps. Do not use it for a job that is cheap, because the threads cost more than they save. When the parts split again into smaller parts, [fork-join](fork-join) is the general form.

## Variants and related patterns

- **More parts than workers.** The master splits the job into many small parts and gives the next part to a worker when the worker is free, so a slow part does not hold up the others. Python's `Pool.map` chops the iterable into a number of chunks which it submits to the pool as separate tasks.[^4] A `ThreadPool` is fully interface compatible with `Pool`, with worker threads in place of processes.[^4]

```python run id=more-parts-than-workers
import threading
from multiprocessing.pool import ThreadPool

seen = set()  # the worker threads that did a part
lock = threading.Lock()


def square_sum(part):
    with lock:
        seen.add(threading.get_ident())
    return sum(n * n for n in part)


items = range(1, 1003)
parts = [items[i:i + 50] for i in range(0, len(items), 50)]  # more parts than workers
with ThreadPool(4) as pool:
    results = pool.map(square_sum, parts)  # a free worker takes the next part
print("parts:", len(parts))
print("worker threads that did a part, at most 4:", len(seen) <= 4)
print("total from the master:", sum(results))
print("same as one loop:", sum(results) == sum(n * n for n in items))
```

```output for=more-parts-than-workers
parts: 21
worker threads that did a part, at most 4: True
total from the master: 335839505
same as one loop: True
```

- **Futures for the results.** Java's `ExecutorService.invokeAll` executes the given tasks and returns a list of futures that hold their status and results when all complete.[^5] The master then reads each [future](future-pattern).
- **[Fork-join](fork-join)** lets a worker split its part again and join the results of its sub-parts. Master-worker splits once, at the master.
- **[Thread pool](thread-pool)** is the usual way to keep the workers. The master sends the parts to the pool instead of starting a thread for each part.
- **[Barrier](barrier-pattern)** replaces the join when the workers run several steps and must wait for each other between steps.

## Known uses

- Python: `multiprocessing.Pool` parallelizes the execution of a function across multiple input values, distributing the input data across processes.[^4]
- Java: `ExecutorService.invokeAll` runs a collection of tasks and waits for all of them to complete.[^5]
- Terminology: in 2018, Python switched to main, parent or server in place of master, and to worker, child or helper in place of slave, depending on context.[^3]

## References
