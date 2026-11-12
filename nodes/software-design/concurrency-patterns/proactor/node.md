---
tag: QGN129
kind: pattern
slug: proactor
name: Proactor
aliases: [proactor pattern]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  A proactor starts asynchronous operations and, when each one completes, calls its completion handler, so the program reacts to finished work and not to readiness.
family: concurrency
polarity: positive
intent: Let an application start many asynchronous operations at once and dispatch a completion handler for each operation when the operation finishes.
also_known_as: []
history:
  year: 1997
  people: [Irfan Pyarali, Tim Harrison, Douglas C. Schmidt, Thomas D. Jordan]
  note: The pattern paper appeared at the 4th Pattern Languages of Programming conference in September 1997. POSA2 (2000) put Proactor in its event handling patterns.
see_also: [reactor, asynchronous-completion-token, acceptor-connector, callback-pattern, futures, async-await, event-driven-programming, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/proactor.pdf
    title: Irfan Pyarali, Tim Harrison, Douglas C. Schmidt and Thomas D. Jordan, Proactor, An Object Behavioral Pattern for Demultiplexing and Dispatching Handlers for Asynchronous Events
    locator: Intent; Structure; Consequences
  - url: https://en.wikipedia.org/wiki/Proactor_pattern
    title: Proactor pattern, Wikipedia
    locator: Introduction; Implementations
  - url: https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/overview/core/async.html
    title: The Proactor Design Pattern, Concurrency Without Threads, Boost.Asio 1.86.0 documentation
    locator: Implementation Using Reactor; Implementation Using Windows Overlapped I/O; Advantages; Disadvantages
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 3 Event Handling Patterns
  - url: https://docs.python.org/3/library/asyncio-eventloop.html
    title: Event Loop, asyncio, Python documentation
    locator: loop.run_in_executor; asyncio.ProactorEventLoop
  - url: https://man7.org/linux/man-pages/man7/io_uring.7.html
    title: io_uring(7), Linux manual page
    locator: DESCRIPTION
---

## Intent

The Proactor pattern supports the demultiplexing and dispatching of multiple event handlers, which are triggered by the completion of asynchronous events.[^1] It can be considered to be an asynchronous variant of the synchronous reactor pattern.[^2]

## Problem

A server must start many reads and writes, and it must not block a thread for each one. A Reactor tells the program that a handle is ready, and the program then does the operation itself. Some operating systems can do the whole operation: they provide asynchronous I/O subsystems within the kernel.[^1] The program then needs a way to learn which operation finished and which code must handle the result.

## Solution

The paper names these participants.[^1]

- **Proactive Initiator.** It is any entity in the application that initiates an Asynchronous Operation.[^1]
- **Asynchronous Operation Processor.** It runs the operations to completion. This component is typically implemented by the OS.[^1]
- **Completion Dispatcher.** It is responsible for calling back to the application's Completion Handlers when Asynchronous Operations complete.[^1]
- **Completion Handler.** It holds the application code that uses the result.

```text
initiator --start(op, handler)--> Asynchronous Operation Processor
                                     | runs op (OS, kernel, thread)
                                     v
                              completion queue
                                     |
              Completion Dispatcher: take (handler, result)
                                     |
                                     v
                              handler(result)
```

The initiator gives the handler at the start of the operation. The dispatcher calls the handler later, with the result, in a thread that it controls.

## Example

The example files show one program in each language. A worker thread is the Asynchronous Operation Processor: it reads two files from a table and puts each completion on a queue. The main thread is the Completion Dispatcher: it takes each completion from the queue and calls its handler. The worker does the operations in order, so the output is the same on each run:

```text
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

## Consequences

Benefits:[^1]

- **Separation of concerns.** The Proactor pattern decouples application-independent asynchrony mechanisms from application-specific functionality.[^1]
- **Portability.** It improves application portability by allowing its interface to be reused independently of the underlying OS calls that perform event demultiplexing.[^1]
- **Simpler synchronization.** As long as Completion Handlers do not spawn additional threads of control, application logic can be written with little or no regard to synchronization issues.[^1]

Drawbacks:

- **Hard to debug.** The inverted flow of control oscillates between the framework infrastructure and the method callbacks on application-specific handlers.[^1]
- **Little control of order.** Proactive Initiators may have no control over the order in which Asynchronous Operations are executed.[^1]
- **Memory use.** Buffer space must be committed for the duration of a read or write operation, and a separate buffer is required for each concurrent operation.[^3]

The last drawback is a common mistake. Here two reads share one buffer. The second read writes over the first one before its handler runs:

```python wrong id=shared-buffer
files = {"a.txt": b"hello", "b.txt": b"abc"}
completions = []


def start_read(name, buffer, handler):
    data = files[name]
    buffer[: len(data)] = data  # the processor fills the caller's buffer
    completions.append((handler, name, len(data)))


def show(name, size):
    print(name, "got", bytes(buffer[:size]))


buffer = bytearray(5)
start_read("a.txt", buffer, show)
start_read("b.txt", buffer, show)  # same buffer while a.txt is not handled
for handler, name, size in completions:
    handler(name, size)
```

```output for=shared-buffer
a.txt got b'abclo'
b.txt got b'abc'
```

The fix is one buffer for each operation that is in progress.[^3]

## When not to use it

- The platform has no asynchronous I/O, and a Reactor or blocking threads are simpler.
- The program does few operations at a time. The separation in time and space between operation initiation and completion makes the code more difficult to develop.[^3]
- Memory for buffers is small, and many connections are idle. The Reactor pattern does not require buffer space until a socket is ready for reading or writing.[^3]

## Variants and related patterns

- **Proactor on top of a Reactor.** On many platforms, Boost.Asio implements the Proactor design pattern in terms of a Reactor, such as select, epoll or kqueue.[^3]
- **Asynchronous Completion Token.** The paper uses it to simplify the asynchronous application programming model: the token tells the handler which request finished.[^1]
- **Event handling patterns.** POSA2 puts Proactor with Reactor, Asynchronous Completion Token and Acceptor-Connector.[^4]
- **Executors and an event loop.** In Python, `loop.run_in_executor` will arrange for func to be called in the specified executor.[^5] The coroutine that waits for the result resumes in the thread of the event loop, which acts as the completion dispatcher:

```python run id=executor
import asyncio
import threading


def read_file(name):  # the asynchronous operation, run in a pool thread
    return name, threading.current_thread() is not threading.main_thread()


async def main():
    loop = asyncio.get_running_loop()
    name, in_pool = await loop.run_in_executor(None, read_file, "a.txt")
    print("operation ran in a pool thread:", in_pool)
    print("handler runs in the loop thread:", threading.current_thread() is threading.main_thread())
    print("result:", name)


asyncio.run(main())
```

```output for=executor
operation ran in a pool thread: True
handler runs in the loop thread: True
result: a.txt
```

## Known uses

- **Boost.Asio.** Its asynchronous support is based on the Proactor design pattern.[^3] On Windows it takes advantage of overlapped I/O to provide an efficient implementation of the Proactor design pattern.[^3]
- **ACE.** The Adaptive Communication Environment is a C++ implementation.[^2]
- **Python asyncio.** `ProactorEventLoop` is a subclass of `AbstractEventLoop` for Windows that uses "I/O Completion Ports" (IOCP).[^5]
- **Linux io_uring.** io_uring is a Linux-specific API for asynchronous I/O. It allows the user to submit one or more I/O requests, which are processed asynchronously without blocking the calling process.[^6]

## References
