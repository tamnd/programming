---
tag: EAEHE8
kind: pattern
slug: monitor-object
name: Monitor Object
aliases: [monitor object, monitor, thread-safe passive object]
area: software-design/concurrency-patterns
level: core
summary: >-
  Monitor Object makes an object safe for many threads. Each method holds the object's lock while it runs, and a method that cannot go on waits on a condition that releases the lock.
family: concurrency
polarity: positive
intent: Synchronize method execution so that only one method runs within an object at a time, and let the methods of the object cooperatively schedule their execution sequences.
also_known_as: [Thread-safe Passive Object]
history:
  year: 1974
  people: [Per Brinch Hansen, C. A. R. Hoare, Douglas C. Schmidt]
  note: Brinch Hansen and Hoare developed monitors in the early 1970s, and Hoare's monitor paper appeared in Communications of the ACM in October 1974. Schmidt wrote Monitor Object as a pattern, and POSA2 (2000) has it with the concurrency patterns.
see_also: [active-object, scoped-locking, strategized-locking, thread-safe-interface, mutual-exclusion, deadlock, threads, channels]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/monitor.pdf
    title: "Douglas C. Schmidt, Monitor Object: An Object Behavioral Pattern for Concurrent Programming"
    locator: 1 Intent; 2 Also Known As; 5 Problem; 6 Solution; 7 Structure; 11 Variants; 12 Known Uses; 13 Consequences; 14 See Also
  - url: https://en.wikipedia.org/wiki/Monitor_(synchronization)
    title: Monitor (synchronization), Wikipedia
    locator: lead; Implicit condition variable monitors; History
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: Condition objects
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Event_loop
    title: JavaScript execution model, MDN Web Docs
    locator: Run-to-completion
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Overview, the 17 patterns
  - url: https://github.com/python/cpython/blob/main/Lib/queue.py
    title: CPython source, Lib/queue.py
    locator: class Queue, __init__, put, get
---

## Intent

The Monitor Object pattern synchronizes method execution to ensure only one method runs within an object at a time. It also allows an object's methods to cooperatively schedule their execution sequences.[^1] It is also known as Thread-safe Passive Object.[^1]

## Problem

Many applications contain objects that are accessed concurrently by multiple client threads, so it is often necessary to synchronize and schedule access to these objects.[^1] The paper's example is the message queue of a consumer handler in a communication Gateway. Its put and get operations must let multiple threads insert and remove messages simultaneously without corrupting the queue's internal state.[^1] The paper states these requirements:[^1]

- Synchronization boundaries should correspond to object methods.[^1]
- Objects, not clients, should be responsible for their own method synchronization. Concurrent applications are harder to program if clients must explicitly acquire and release low-level synchronization mechanisms, such as semaphores, mutexes, or condition variables.[^1]

The same queue must also block a get on an empty queue and a put on a full queue until another thread changes the queue.[^1]

## Solution

Define each object that client threads access concurrently as a monitor object. Clients access its services only through its synchronized methods, and only one synchronized method at a time can run within the object.[^1] The participants are:[^1]

- The **monitor object** exports one or more methods to clients. Each method executes in the thread of the client that invokes it, because a monitor object does not have its own thread of control.[^1]
- The **synchronized methods** implement the thread-safe services of the object.[^1]
- The **monitor lock** serializes method invocations on a per-object basis. Each synchronized method acquires it when the method enters the object and releases it when the method exits.[^1]
- The **monitor conditions** let synchronized methods in separate threads cooperatively schedule their execution sequences, by waiting for and notifying each other.[^1]

When a synchronized method cannot make progress, it waits on a monitor condition. The monitor lock is released automatically and the client thread is suspended. When another method notifies the condition, the lock is reacquired before the thread reenters the object and resumes.[^1]

```text
producer thread                 consumer thread
put(m):                         get():
  acquire monitor lock            acquire monitor lock
  while full: wait not_full       while empty: wait not_empty
  add m                           (wait releases the lock)
  notify not_empty  ----------->  wakes, reacquires the lock
  release monitor lock            remove m, notify not_full
                                  release monitor lock
```

A thread that wakes from a wait must check its condition again, because other threads can run between the notify and the wakeup.[^2] Python's documentation gives the same rule: wait() can return after an arbitrary long time, and the condition which prompted the notify() call may no longer hold true.[^3]

## Example

The example files implement a bounded message queue with room for 2 messages as a monitor object. One lock guards the queue, and two conditions, not full and not empty, let put and get wait. Four producer threads put 250 messages each, and two consumer threads get 500 messages each. Every message is a different number from 0 to 999, so the sum shows that no message was lost or read twice. The JavaScript and TypeScript versions use async functions on one thread: each job runs to completion before any other job is processed,[^4] so the methods need no lock, and a waiting method awaits a promise. The program prints:

```text
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

## Consequences

Benefits:[^1]

- **Simplify synchronization of methods invoked concurrently on an object.** Clients need not be concerned with concurrency control when they call a monitor object. If a language does not support monitor objects, idioms like Scoped Locking can automate the acquisition and release of monitor locks.[^1]
- **Synchronized methods can cooperatively schedule their order of execution.** Methods can suspend themselves and wait to be notified when arbitrarily complex conditions occur, without inefficient polling.[^1]

Liabilities:[^1]

- **Tight coupling between object functionality and synchronization mechanisms.** A monitor object's synchronization and scheduling logic is often closely coupled with its methods' functionality. This makes monitor objects more efficient than active objects, but it may be hard to change their synchronization policies without changing the method implementations.[^1]
- **Nested monitor lockout.** This problem can occur when a monitor object is nested within another monitor object. In the paper's Java example, a thread waits in the inner monitor and keeps the lock of the outer monitor, so no other thread can enter the outer monitor to make the condition true, and the first thread blocks forever.[^1]

The block below shows nested monitor lockout. `Outer.process` holds the outer lock and waits on the inner condition. `Outer.set` must take the outer lock first, so it cannot run until the wait times out:

```python wrong id=nested-lockout
import threading


class Inner:
    def __init__(self):
        self.cv = threading.Condition()
        self.cond = False

    def await_condition(self):
        with self.cv:
            # The wait releases the inner lock only. The timeout keeps the demo from hanging.
            return self.cv.wait_for(lambda: self.cond, timeout=0.3)

    def notify_condition(self, value):
        with self.cv:
            self.cond = value
            self.cv.notify_all()


class Outer:
    def __init__(self):
        self.lock = threading.Lock()
        self.inner = Inner()
        self.entered = threading.Event()
        self.order = []

    def process(self):
        with self.lock:
            self.entered.set()
            result = self.inner.await_condition()
            self.order.append("process gave up")
            return result

    def set(self, value):
        with self.lock:
            self.order.append("set ran")
            self.inner.notify_condition(value)


outer = Outer()
results = []
t1 = threading.Thread(target=lambda: results.append(outer.process()))
t1.start()
outer.entered.wait()
t2 = threading.Thread(target=outer.set, args=(True,))
t2.start()
t1.join()
t2.join()
print("process saw the condition:", results[0])
print("order:", outer.order)
```

```output for=nested-lockout
process saw the condition: False
order: ['process gave up', 'set ran']
```

## When not to use it

- The callers must not block in the object at all. Each synchronized method runs in the client thread,[^1] so a long wait in the method stops the client. Active Object runs the methods in a different thread than the clients.[^1]
- The synchronization policy must change often. The policy is coupled with the methods, so a change can mean a change to each method.[^1]
- Methods of one monitor must wait on a condition of another monitor. That is the nested monitor lockout that the block above shows.[^1]

## Variants and related patterns

- **Timed synchronized method invocations.** Clients can bound the amount of time they are willing to wait for a synchronized method to enter its monitor object's critical section.[^1]
- **Strategized locking.** The Strategized Locking pattern can make a monitor object more flexible and reusable, for example by a template parameter that selects the lock and condition types of a Message Queue.[^1] Then the Thread-safe Interface idiom keeps intra-object calls from self-deadlock.[^1]
- **Active Object.** Both patterns can synchronize and schedule methods invoked concurrently on an object. An active object executes its methods in a different thread than its clients, whereas a monitor object executes its methods in its client threads. Active objects can do more sophisticated, albeit more expensive, scheduling.[^1] POSA2 lists Monitor Object among its 17 patterns, next to Active Object.[^5]

Python's `queue.Queue` is a monitor object with timed methods. Its source creates one lock, `self.mutex`, and the conditions `not_empty` and `not_full` on that lock.[^6] With a timeout, `get` blocks at most that many seconds and then raises the `Empty` exception, and `put` raises `Full`:[^6]

```python run id=timed
import queue

q = queue.Queue(maxsize=1)
try:
    q.get(timeout=0.1)
except queue.Empty:
    print("get on an empty queue: timed out")
q.put("m1")
try:
    q.put("m2", timeout=0.1)
except queue.Full:
    print("put on a full queue: timed out")
```

```output for=timed
get on an empty queue: timed out
put on a full queue: timed out
```

## Known uses

- **Dijkstra and Hoare monitors.** Dijkstra and Hoare defined programming language features called monitors. A monitor contains a lock that allows only one function at a time to be active, and functions can block on a condition variable.[^1] Brinch Hansen created the first implementation of monitors, in Concurrent Pascal.[^2]
- **Java objects.** Each Java object is implicitly a monitor that contains a monitor lock and a single monitor condition. Threads coordinate via explicit wait, notify, and notifyAll operations.[^1] Methods that need mutual exclusion are marked with the synchronized keyword.[^2]
- **ACE Gateway.** The Message Queues used by Consumer Handlers in the Gateway are reusable ACE components implemented as monitor objects.[^1]
- **Languages.** Languages that support monitors include C#, Go, Java, Mesa, Modula-3, Ruby, and Python through its `threading.Condition` object.[^2]

## References
