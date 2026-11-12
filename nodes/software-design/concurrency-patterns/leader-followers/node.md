---
tag: 66RDDY
kind: pattern
slug: leader-followers
name: Leader/Followers
aliases: [leader/followers, leader followers, leader-follower thread pool]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  Leader/Followers lets a pool of threads take turns on one set of event sources. The leader waits for an event, promotes a follower to lead, and then handles the event in its own thread.
family: concurrency
polarity: positive
intent: Provide an efficient concurrency model where multiple threads take turns sharing a set of event sources to detect, demultiplex, dispatch, and process service requests that occur on these event sources.
also_known_as: []
history:
  year: 2000
  people: [Douglas C. Schmidt, Carlos O'Ryan, Michael Kircher, Irfan Pyarali, Frank Buschmann]
  note: The pattern paper is dated June 2000, and POSA2 (2000) has the pattern with the concurrency patterns.
see_also: [half-sync-half-async, reactor, proactor, active-object, monitor-object, thread-specific-storage, threads, event-driven-programming]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/lf.pdf
    title: "Douglas C. Schmidt, Carlos O'Ryan, Michael Kircher, Irfan Pyarali, and Frank Buschmann, Leader/Followers"
    locator: Example; Context; Problem; Solution; Structure; Implementation; Variants; Known Uses; Consequences; See Also
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Overview, the 17 patterns
---

## Intent

The Leader/Followers architectural pattern provides an efficient concurrency model where multiple threads take turns sharing a set of event sources in order to detect, demultiplex, dispatch, and process service requests that occur on these event sources.[^1] POSA2 lists it among its 17 patterns.[^2]

## Problem

The paper's example is a high-volume, on-line transaction processing (OLTP) server.[^1] One way to multi-thread it is a thread pool based on the Half-Sync/Half-Reactive variant of Half-Sync/Half-Async. A network I/O thread waits in `select()`, reads each request, and inserts it into a synchronized message queue that a pool of worker threads services.[^1] Even with a light workload, this design incurs a dynamic memory allocation, multiple synchronization operations, and a context switch to pass each request between the network I/O thread and a worker thread.[^1] The design must resolve these forces:[^1]

- Associating a thread with each event source may be infeasible, because it does not scale when the number of connections increases significantly.[^1]
- Context switching, synchronization, and cache coherency management must be minimized to maximize performance.[^1]
- Multiple threads that demultiplex events on a shared set of event sources must coordinate to prevent race conditions. The operating system will erroneously notify more than one thread calling `select()` on the same set of socket handles, and multiple threads that call `read()` on one TCP socket handle will corrupt or lose data.[^1]

## Solution

Structure a pool of threads to share a set of event sources by taking turns demultiplexing events and synchronously dispatching them to application services.[^1] One thread at a time, the leader, waits for an event to occur from a set of event sources. The other threads, the followers, queue up waiting their turn to become the leader.[^1] After the leader detects an event, it first promotes a follower to become the new leader. Then it plays the role of a processing thread and dispatches the event to an event handler in its own thread. After handling its event, a processing thread reverts to a follower role.[^1]

The participants are **handles** and a **handle set**, the **event handler** and **concrete event handlers**, and a **thread pool**. The thread pool is a group of threads that share a synchronizer, such as a semaphore or condition variable, and implement a protocol for their transition between the roles.[^1]

```text
followers:  T2 T3 T4  wait on the synchronizer
leader:     T1        waits on the handle set
event e1 arrives:
  T1: promote_new_leader()  -> T2 becomes the leader
  T1: handle_event(e1) in T1, no hand-off to another thread
  T1: join() again          -> T1 is a follower
```

When an event arrives, the leader deactivates the handle in the handle set temporarily and then promotes a follower. Otherwise the new leader could demultiplex the same event a second time.[^1]

## Example

The example files run a pool of 4 threads over 100 events. A queue stands for the handle set, and the pool lets only the leader wait on it. The leader takes an event, promotes a follower, and processes the event in its own thread, with a 1 ms blocking call. The pool counts the threads that wait on the handle set at one time. The program prints:

```text
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

## Consequences

Benefits, compared with the Half-Sync/Half-Reactive thread pool:[^1]

- **Performance enhancements.** The pattern enhances CPU cache affinity and eliminates the need for dynamic allocation and data buffer sharing between threads. It minimizes locking overhead by not exchanging data between threads. It can minimize priority inversion because no extra queueing is introduced in the server. It does not require a context switch to handle each event, but promoting a follower to fulfill the leader role does require a context switch.[^1]
- **Programming simplicity.** The pattern simplifies concurrency models where multiple threads receive requests, process responses, and demultiplex connections using a shared handle set.[^1]

Liabilities:[^1]

- **Implementation complexity.** The advanced variants are harder to implement than Half-Sync/Half-Reactive thread pools. The set of followers changes concurrently, in an unpredictable order, when a follower is promoted and when a thread rejoins the pool.[^1]
- **Lack of flexibility.** It is harder to discard or reorder events because there is no explicit queue.[^1]
- **Network I/O bottlenecks.** Only a single thread at a time waits on the handle set, and in some environments this design could become a bottleneck.[^1]

The leader must promote a follower before it processes the event. In the block below, the leader keeps the role while it processes, so the pool handles one event at a time, and four threads take as long as one thread:

```python wrong id=promote-late
import threading
import time

events = list(range(20))
synchronizer = threading.Lock()  # held by the leader
busy = 0
most_busy = 0
count_lock = threading.Lock()


def join():
    global busy, most_busy
    while True:
        with synchronizer:  # wrong: the leader role ends only after processing
            if not events:
                return
            events.pop()
            with count_lock:
                busy += 1
                most_busy = max(most_busy, busy)
            time.sleep(0.01)
            with count_lock:
                busy -= 1


start = time.monotonic()
threads = [threading.Thread(target=join) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("most events in processing at once:", most_busy)
print("20 events of 10 ms took at least 0.2 s:", time.monotonic() - start >= 0.2)
```

```output for=promote-late
most events in processing at once: 1
20 events of 10 ms took at least 0.2 s: True
```

## When not to use it

- Events need extra synchronization or ordering constraints before threads in the pool can process them. Half-Sync/Half-Async and Active Object are the alternatives then.[^1]
- The events must be discarded or reordered by priority. A queue makes that easier, and Leader/Followers has no explicit queue.[^1]
- The operating system supports asynchronous I/O efficiently. Then Proactor can be used as an alternative.[^1]
- The event sources cannot be waited for by a single event demultiplexer.[^1]

## Variants and related patterns

- **Bound handle/thread associations.** Each thread is bound to its own handle. This is common on the client side, when a thread waits on a socket handle for a response to a two-way request it sent.[^1]
- **Relaxing serialization constraints.** On some platforms, multiple leader threads can wait on a handle set simultaneously, for example with `WaitForMultipleObjects()` on Win32.[^1]
- **Hybrid thread associations.** Some applications use bound and unbound handle/thread associations simultaneously.[^1]
- **Half-Sync/Half-Reactive.** A network I/O thread passes every request through a queue to a worker thread.[^1] The block below counts these hand-offs, which Leader/Followers does not make:

```python run id=handoffs
import queue
import threading

request_queue = queue.Queue()  # the queueing layer of Half-Sync/Half-Reactive
handed_off = 0
lock = threading.Lock()


def worker():
    global handed_off
    while True:
        request, io_thread = request_queue.get()
        if request is None:
            return
        with lock:
            handed_off += threading.current_thread() is not io_thread


workers = [threading.Thread(target=worker) for _ in range(4)]
for w in workers:
    w.start()
io_thread = threading.current_thread()  # the network I/O thread reads every event
for event_id in range(100):
    request_queue.put((event_id, io_thread))
for _ in workers:
    request_queue.put((None, None))
for w in workers:
    w.join()
print("Half-Sync/Half-Reactive: events handed to another thread:", handed_off, "of 100")
```

```output for=handoffs
Half-Sync/Half-Reactive: events handed to another thread: 100 of 100
```

## Known uses

- **ACE Thread Pool Reactor.** `ACE_TP_Reactor` demultiplexes events to event handlers within a pool of threads. One thread becomes the leader and waits for an event, then promotes an arbitrary thread to become the next leader.[^1]
- **CORBA ORBs.** Chorus COOL ORB and TAO use the pattern for their client-side connection model and the server-side concurrency model.[^1]
- **Web servers.** The JAWS Web server uses the Leader/Followers thread pool model on platforms that do not allow multiple threads to simultaneously call `accept()` on a passive-mode socket handle.[^1]
- **Transaction monitors.** Implementations of the CORBA Transaction Service employ bound Leader/Followers associations between threads and transactions.[^1]
- **Taxi stands.** Many airport taxi stands work this way. The first taxi cab in line is the leader, the other cabs are the followers, and arriving passengers are the events.[^1]

## References
