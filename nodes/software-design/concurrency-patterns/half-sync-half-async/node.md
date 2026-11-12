---
tag: QGA6WS
kind: pattern
slug: half-sync-half-async
name: Half-Sync/Half-Async
aliases: [half-sync/half-async, half sync half async, hsha]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  Half-Sync/Half-Async splits a concurrent system into layers. Short handlers react to events and never block, threads run the long tasks with blocking calls, and a queue joins the two layers.
family: concurrency
polarity: positive
intent: Decouple synchronous I/O from asynchronous I/O in a system to simplify concurrent programming effort without degrading execution efficiency.
also_known_as: []
history:
  year: 1996
  people: [Douglas C. Schmidt, Charles D. Cranor]
  note: An earlier version of the paper appeared in Pattern Languages of Program Design 2 (1996). POSA2 (2000) has the pattern with the concurrency patterns.
see_also: [active-object, reactor, monitor-object, leader-followers, event-driven-programming, async-await, threads, channels]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/PLoP-95.pdf
    title: "Douglas C. Schmidt and Charles D. Cranor, Half-Sync/Half-Async: An Architectural Pattern for Efficient and Well-structured Concurrent I/O"
    locator: Abstract; 1 Intent; 2 Motivation; 3 Solution; 5 Structure and Participants; 7 Consequences; 8 Implementation; 10 Related Patterns; 11 Known Uses
  - url: https://docs.python.org/3/library/queue.html
    title: queue, A synchronized queue class, Python documentation
    locator: introduction
  - url: https://docs.python.org/3/library/asyncio-eventloop.html
    title: Event Loop, asyncio, Python documentation
    locator: Scheduling callbacks; Executing code in thread or process pools
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Overview, the 17 patterns
---

## Intent

The Half-Sync/Half-Async pattern decouples synchronous I/O from asynchronous I/O in a system to simplify concurrent programming effort without degrading execution efficiency.[^1] Higher-level tasks use a synchronous I/O model, which simplifies concurrent programming. Lower-level tasks use an asynchronous I/O model, which enhances execution efficiency.[^1]

## Problem

The paper's example is the BSD UNIX networking subsystem. The kernel performs I/O asynchronously in response to device interrupts, and user-level applications perform I/O synchronously.[^1] Two forces pull in opposite directions:[^1]

- **Programming simplicity.** Asynchrony can cause subtle timing problems and race conditions when an interrupt handler preempts the current thread of control. Programs that use synchronous I/O can block awaiting the completion of I/O operations and keep their state in a runtime stack of activation records, rather than in separate data structures.[^1]
- **Execution efficiency.** The asynchronous I/O model maps efficiently onto hardware devices that are driven by interrupts. A completely synchronous model may be inefficient if each source of events is associated with a separate process or thread, because each one needs a stack and a set of registers to block.[^1]

## Solution

Higher-level tasks, such as database queries or file transfers, use a synchronous I/O model. Lower-level tasks, such as servicing interrupts from network controllers, use an asynchronous I/O model. A Queueing layer mediates communication between the two layers.[^1] There are usually more high-level tasks than low-level tasks in a system, so the pattern localizes the complexity of asynchronous processing within a single layer.[^1] The participants are:[^1]

- The **Synchronous task layer** performs high-level I/O operations. Its tasks are active objects that have their own run-time stack and registers, so they can block. In BSD UNIX, this layer is the user processes.[^1]
- The **Queueing layer** provides a synchronization and buffering point between the two task layers. In BSD UNIX, it is the Socket layer.[^1]
- The **Asynchronous task layer** handles lower-level events from multiple external event sources. Its tasks are passive objects without their own run-time stack, so they cannot block indefinitely on any single source of events. In BSD UNIX, it is the kernel.[^1]
- The **External event sources**, such as network interfaces and disk controllers, generate the events.[^1]

```text
sync layer:      worker 1   worker 2   worker 3   (threads that block)
                     \          |          /
queueing layer:     [ message queue: get blocks ]
                                |
async layer:     event loop: on_event(e) -> put e, return at once
                                |
event sources:   network interfaces, timers, devices
```

The two layers communicate in a "producer/consumer" manner by passing messages.[^1]

## Example

The example files receive 100 events in the asynchronous layer. Each handler only puts a request on the queue and returns. Four worker threads form the synchronous layer: each one blocks on the queue and then runs a blocking call for the request. The demo holds the workers at a gate until the event source ends, so the program can count the requests that wait in the queueing layer. The Python version uses an asyncio event loop for the asynchronous layer and `queue.Queue` for the queueing layer, a queue that is useful when information must be exchanged safely between multiple threads.[^2] The program prints:

```text
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

## Consequences

Benefits:[^1]

- **Higher-level tasks are simplified** because they are shielded from lower-level asynchronous I/O. Complex concurrency control, interrupt handling, and timing issues are delegated to the Asynchronous task layer.[^1]
- **Synchronization policies in each layer are decoupled.** In the single-threaded BSD UNIX kernel, the Asynchronous layer controls concurrency by raising and lowering CPU interrupt levels, while user processes use semaphores, message queues, condition variables, and record locks.[^1]
- **Inter-layer communication is localized at a single point**, because the Queueing layer mediates all interaction.[^1]
- **Performance is improved on multi-processors.** One processor can be dedicated to a thread that transfers a large stream of data.[^1]

Liabilities:[^1]

- **A boundary-crossing penalty** may be incurred from synchronization, data copying, and context switching overhead when data moves through the Queueing layer. The socket layer in BSD UNIX accounts for a large percentage of the overall TCP/IP networking overhead.[^1]
- **Asynchronous I/O for higher-level tasks is lacking.** The system I/O structure may prevent applications from utilizing the hardware efficiently, even if external devices support asynchronous overlap of computation and communication.[^1]

The asynchronous layer must not block. The asyncio documentation warns that file operations can block the event loop, and runs them in a thread pool.[^3] In the block below, four handlers make a blocking call on the event loop thread, so a timer that is due after 10 ms runs only after all of them:

```python wrong id=blocking-handler
import asyncio
import time


async def main():
    loop = asyncio.get_running_loop()
    order = []
    done = loop.create_future()
    start = time.monotonic()

    def slow_handler():
        time.sleep(0.05)  # a blocking call in the async layer
        order.append("slow")

    def timer():
        order.append("timer")
        done.set_result(time.monotonic() - start)

    loop.call_later(0.01, timer)
    for _ in range(4):
        loop.call_soon(slow_handler)
    elapsed = await done
    print("order:", order)
    print("timer was late by at least 0.15 s:", elapsed - 0.01 >= 0.15)


asyncio.run(main())
```

```output for=blocking-handler
order: ['slow', 'slow', 'slow', 'slow', 'timer']
timer was late by at least 0.15 s: True
```

## When not to use it

- Every task is short and never blocks. Then the asynchronous layer can do all the work, and the queue only adds the boundary-crossing penalty.[^1]
- Higher-level tasks must use asynchronous I/O to reach the full speed of the devices. The pattern hides that I/O from them.[^1]
- The queue has no bound and events can arrive faster than workers finish them. Systems cannot devote an unlimited amount of resources to buffer messages in the Queueing layer, so the layer needs flow control.[^1]

## Variants and related patterns

- **Active Object.** The Synchronous task layer uses the Active Object pattern.[^1]
- **Reactor.** The Asynchronous task layer may use the Reactor pattern to demultiplex events from multiple sources of events.[^1]
- **Facade and Mediator.** The Queueing layer provides a Facade that simplifies the interface to the Asynchronous task layer, and it is also a Mediator that coordinates the exchange of data between the two task layers.[^1]
- **Thread per handle.** In the Asynchronous layer of MT-Orbix, a separate thread is associated with each handle that is connected to a client, and each thread blocks synchronously reading CORBA requests.[^1]
- **Leader/Followers.** POSA2 lists Half-Sync/Half-Async and Leader/Followers among its 17 patterns.[^4]

The data can also go from the asynchronous layer to a synchronous call and back. In asyncio, `loop.run_in_executor` arranges for a function to be called in an executor, and the default executor is a `ThreadPoolExecutor`.[^3] The coroutine waits for the result and the event loop keeps running other callbacks:

```python run id=executor
import asyncio
import threading
import time


def blocking_lookup(n):
    time.sleep(0.1)  # a synchronous call in a pool thread
    return n * n, threading.current_thread() is threading.main_thread()


async def main():
    loop = asyncio.get_running_loop()
    ticks = 0

    async def ticker():
        nonlocal ticks
        while True:
            ticks += 1
            await asyncio.sleep(0.01)

    task = asyncio.create_task(ticker())
    result, on_main = await loop.run_in_executor(None, blocking_lookup, 12)
    task.cancel()
    print("result:", result)
    print("blocking call ran on the event loop thread:", on_main)
    print("event loop ran other work during the call:", ticks > 1)


asyncio.run(main())
```

```output for=executor
result: 144
blocking call ran on the event loop thread: False
event loop ran other work during the call: True
```

## Known uses

- **UNIX kernels.** The BSD UNIX networking subsystem and the original System V UNIX STREAMS framework use the pattern. The Queueing layer is the Socket layer in BSD and the STREAM heads in System V STREAMS.[^1]
- **MT-Orbix.** The multi-threaded version of Orbix 1.3 uses several variations of the pattern to dispatch CORBA remote operations in a concurrent server.[^1]
- **Motorola Iridium.** An application-level Gateway that routes messages between satellites and ground control stations uses the pattern. The ASX Reactor handles events asynchronously, the ASX Message Queue is the Queueing layer, and the ASX Task class implements the synchronous active objects.[^1]
- **Operating systems.** The pattern is widely used in operating systems such as UNIX, Mach, Windows NT, and VMS.[^1]

## References
