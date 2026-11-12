---
tag: 749ESG
kind: pattern
slug: active-object
name: Active Object
aliases: [active object, concurrent object, actor object]
area: software-design/concurrency-patterns
level: core
summary: >-
  Active Object gives an object its own thread. A proxy turns each method call into a request on a queue and returns a future, and a scheduler runs the requests on the object one at a time.
family: concurrency
polarity: positive
intent: Decouple method execution from method invocation, to enhance concurrency and simplify synchronized access to an object that resides in its own thread of control.
also_known_as: [Concurrent Object, Actor]
history:
  year: 1996
  people: [R. Greg Lavender, Douglas C. Schmidt]
  note: An earlier version of the paper appeared in Pattern Languages of Program Design 2 (1996). POSA2 (2000) has the pattern with the concurrency patterns.
see_also: [futures, proxy-pattern, command-pattern, actor-model, reactor, channels, threads, message-passing-concept]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/Act-Obj.pdf
    title: "R. Greg Lavender and Douglas C. Schmidt, Active Object: An Object Behavioral Pattern for Concurrent Programming"
    locator: Abstract; 1 Intent; 2 Also Known As; 5 Problem; 11 Variants; 12 Known Uses; 13 Consequences; 14 See Also
  - url: https://en.wikipedia.org/wiki/Active_object
    title: Active object, Wikipedia
    locator: lead
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 5 Concurrency Patterns
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: concurrent.futures, Launching parallel tasks, Python documentation
    locator: ThreadPoolExecutor; Future Objects
---

## Intent

The Active Object design pattern decouples method execution from method invocation to enhance concurrency and simplify synchronized access to an object that resides in its own thread of control.[^1] It is also known as Concurrent Object and Actor.[^1]

## Problem

Many applications benefit from using concurrent objects to improve their QoS, for example by allowing an application to handle multiple client requests in parallel.[^1] The paper's example is a communication Gateway that routes messages from supplier processes to consumer processes over TCP.[^1] TCP uses flow control to ensure that fast suppliers or Gateways do not produce data more rapidly than slow consumers or congested networks can buffer and process the data.[^1] The connections that are flow controlled must block without impeding the progress of threads whose TCP connections are not flow controlled.[^1] A passive object runs its methods in the thread of the client that calls it, so a slow call blocks that client.

## Solution

An active object consists of these components:[^1]

- A **Proxy** represents the interface of the object and runs in the client thread.[^1]
- A **Servant** provides the object's implementation and runs in a different thread.[^1]
- At run time, the Proxy transforms the client's method invocation into a **Method Request**, which is stored in an **Activation Queue** by a **Scheduler**.[^1]
- The Scheduler runs continuously in the same thread as the servant. It dequeues Method Requests from the Activation Queue when they become runnable and dispatches them on the Servant.[^1]
- Clients can obtain the results of a method's execution via the **Future** returned by the Proxy.[^1]

```text
client thread                     scheduler thread
proxy.deposit(1)
  -> MethodRequest(deposit, 1) -> activation queue
  <- future                       scheduler: dequeue
future.result() ................> servant.deposit(1)
                                  future.set(result)
```

The goal is to introduce concurrency, by using asynchronous method invocation and a scheduler for handling requests.[^2] The client thread appears to invoke an ordinary method.[^1]

## Example

The example files show an account as an active object in each language. The servant keeps a total and has no lock, because only the scheduler thread calls it. Four client threads each call `deposit(1)` 250 times, and each call returns a future at once. The demo holds the scheduler at a gate until the clients end, so no request has run when the program counts the finished futures. Then it opens the gate and waits for the future of `balance`. The JavaScript and TypeScript versions put the servant in a worker thread. The program prints:

```text
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

## Consequences

Benefits:[^1]

- **Enhance application concurrency and simplify synchronization complexity.** Client threads and asynchronous method executions run simultaneously. The Scheduler evaluates synchronization constraints to guarantee serialized access to Servants.[^1]
- **Transparently leverage available parallelism.** If the platform supports multiple CPUs efficiently, multiple active objects can execute in parallel, subject to their synchronization constraints.[^1]
- **Method execution order can differ from method invocation order.** Methods invoked asynchronously are executed based on their synchronization constraints, which may differ from their invocation order.[^1]

Liabilities:[^1]

- **Performance overhead.** Context switching, synchronization, and data movement overhead may occur when scheduling and executing method invocations. The pattern is most applicable on relatively coarse-grained objects.[^1]
- **Complicated debugging.** It may be difficult to debug programs containing active objects due to the concurrency and non-determinism of the Scheduler.[^1]

A servant method must not wait for a future of its own active object. The request it waits for sits in the queue behind the running request, and only the same scheduler thread can run it. In this version, the wait has a timeout, so the program shows the failure instead of hanging:

```python wrong id=self-wait
import queue
import threading
from concurrent.futures import Future, TimeoutError


class Scheduler(threading.Thread):
    def __init__(self):
        super().__init__(daemon=True)
        self.activation_queue = queue.Queue()

    def run(self):
        while True:
            call, future = self.activation_queue.get()
            future.set_result(call())


scheduler = Scheduler()
scheduler.start()


def send(call):
    future = Future()
    scheduler.activation_queue.put((call, future))
    return future


total = 0


def balance():
    return total


def report():
    # Runs in the scheduler thread, then waits for a request that
    # only the same scheduler thread can run.
    try:
        return send(balance).result(timeout=0.2)
    except TimeoutError:
        return "deadlock: the scheduler waits for itself"


print("report:", send(report).result())
```

```output for=self-wait
report: deadlock: the scheduler waits for itself
```

## When not to use it

- The object is very fine-grained. The performance overhead of active objects can be excessive then, compared with other concurrency patterns such as Monitors.[^1]
- Callers need each result at once and have nothing else to do. A future that the caller waits on at once only adds a thread switch to a plain locked call.
- The servant's methods must call each other through the proxy and wait for the results. That makes the scheduler wait for itself, as the block above shows.

## Variants and related patterns

- **Integrated Scheduler.** The roles of the Proxy and Servant are often integrated into the Scheduler component, though servants still execute in a different thread than the proxies.[^1]
- **Message passing.** A further refinement removes the Proxy and Servant altogether and uses direct message passing between the client thread and the Scheduler thread.[^1]
- **Polymorphic futures.** A Polymorphic Future result value provides write-once, read-many synchronization.[^1]
- **Distributed Active Object.** A distribution boundary exists between the Proxy and the Scheduler, rather than a threading boundary, and the client-side Proxy plays the role of a stub.[^1]
- **Thread pool.** A thread pool is a generalization of Active Object that supports multiple Servants per Active Object.[^1]
- **Actors.** The Active Object pattern has been used to implement Actors. Messages sent to an Actor are queued in the Actor's message queue.[^1]
- **Monitor Object.** Monitors are generally more efficient than Active Objects since they incur less context switching and data movement overhead, but it is harder to add a distribution boundary between client and server threads.[^1]
- **Half-Sync/Half-Async.** That pattern typically uses Active Object to implement the synchronous task layer and Reactor to implement the asynchronous task layer.[^1] POSA2 lists Active Object first among its concurrency patterns.[^3]
- **Command Processor.** Its intent is to separate the issuing of requests from their execution, and the command processor corresponds to the scheduler.[^1]

In Python, a `ThreadPoolExecutor` with one worker gives the scheduler and the activation queue in one object. It uses a pool of at most `max_workers` threads to execute calls asynchronously, and `submit` returns a `Future`:[^4]

```python run id=executor
from concurrent.futures import ThreadPoolExecutor


class AccountServant:
    def __init__(self):
        self.total = 0

    def deposit(self, amount):
        self.total += amount
        return self.total


servant = AccountServant()
# One worker thread: the executor is the scheduler and its activation queue.
with ThreadPoolExecutor(max_workers=1) as scheduler:
    futures = [scheduler.submit(servant.deposit, 1) for _ in range(1000)]
    print("last deposit result:", futures[-1].result())
    print("results in call order:", [f.result() for f in futures[:5]])
```

```output for=executor
last deposit result: 1000
results in call order: [1, 2, 3, 4, 5]
```

## Known uses

- **CORBA ORBs.** The pattern has been used to implement concurrent ORB middleware frameworks. The TAO ORB implements it for its default concurrency model: CORBA stubs are the Proxies, the ORB Core's Reactor is the Scheduler, and the socket queues are the Activation Queues.[^1]
- **ACE.** Reusable implementations of the Method Request, Activation Queue, and Future components are provided in the ACE framework.[^1]
- **Siemens MedCom.** The framework for electronic medical systems uses the pattern with the Command Processor pattern.[^1]
- **Siemens Call Center management system.** This system uses the thread pool variant.[^1]
- **Clients.** Client applications, such as windowing systems and network browsers, employ active objects to simplify concurrent, asynchronous network operations.[^1]

## References
