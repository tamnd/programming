---
tag: VRY55J
kind: pattern
slug: reactor
name: Reactor
aliases: [reactor pattern, dispatcher, notifier]
area: software-design/concurrency-patterns
level: core
summary: >-
  A reactor waits for events on many handles in one thread and calls the handler that is registered for each handle, so one thread serves many connections.
family: concurrency
polarity: positive
intent: Demultiplex the events that arrive on many handles and dispatch each event to the handler that is registered for its handle, with no thread for each connection.
also_known_as: [Dispatcher, Notifier]
history:
  year: 1995
  people: [Douglas C. Schmidt]
  note: Schmidt described the pattern in a chapter of Pattern Languages of Program Design (1995). POSA2 (2000) put it in its event handling patterns.
see_also: [proactor, acceptor-connector, asynchronous-completion-token, async-await, event-driven-programming, observer-pattern, callback-pattern, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/Reactor.pdf
    title: Douglas C. Schmidt, Reactor, An Object Behavioral Pattern for Demultiplexing and Dispatching Handles for Synchronous Events
    locator: Intent; Also Known As; Structure; Known Uses; Consequences
  - url: https://en.wikipedia.org/wiki/Reactor_pattern
    title: Reactor pattern, Wikipedia
    locator: Introduction; Overview; Variants
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 3 Event Handling Patterns
  - url: https://docs.python.org/3/library/selectors.html
    title: selectors, High-level I/O multiplexing, Python documentation
    locator: Introduction
  - url: https://docs.libuv.org/en/v1.x/design.html
    title: Design overview, libuv documentation
    locator: Design overview; The I/O loop
---

## Intent

The Reactor design pattern handles service requests that are delivered concurrently to an application by one or more clients.[^1] It is also known as Dispatcher and Notifier.[^1] The pattern's key component is an event loop, running in a single thread or process, which demultiplexes incoming requests and dispatches them to the correct request handler.[^2]

## Problem

A server has many connections, and on each connection a request can arrive at any time. An iterative server reads one request from start to finish in each turn of its loop, and it will fall behind once it receives multiple requests in quick succession.[^2] A thread for each connection solves the wait, but each new thread or process imposes overhead costs in memory and processing time.[^2] Both approaches also tightly couple the general demultiplexer with specific request handlers.[^2]

## Solution

The paper names five participants.[^1]

- **Handle.** It identifies a resource that is managed by an OS, such as a network connection, an open file or a timer.[^1]
- **Synchronous Event Demultiplexer.** It blocks awaiting events to occur on a set of Handles, and it returns when it is possible to initiate an operation on a Handle without blocking.[^1] A common demultiplexer for I/O events is `select`.[^1]
- **Initiation Dispatcher.** It keeps a table of event handlers, with `register_handler` and `remove_handler`, and its `handle_events` loop calls the demultiplexer.[^1]
- **Event Handler.** It declares the hook method `handle_event`.[^1]
- **Concrete Event Handler.** It gives the application code for one kind of event.

```text
           register_handler(h)
  handler ------------------> Initiation Dispatcher
     ^                           | handle_events():
     |                           |   ready = select(handles)
     |   handle_event(data)      |   for h in ready:
     +---------------------------+     handlers[h].handle_event()
```

The dispatcher calls the handlers one at a time in its thread. The Initiation Dispatcher calls back to the handle event hook method of the Event Handler.[^1] That is an inversion of control: the framework, not the handler, decides when the handler runs.

## Example

The example files show one program in each language. A list of ready events takes the place of `select`, so the output is the same on each run. An acceptor handler is registered for the handle `listener`. Each event on it registers a new echo handler for a new connection. An empty message closes the connection and removes its handler. The program prints:

```text
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

## Consequences

Benefits:[^1]

- **Separation of concerns.** The Reactor pattern decouples application-independent demultiplexing and dispatching mechanisms from application-specific hook method functionality.[^1]
- **Modularity and reuse.** In the paper's logging server, there are two separate classes: one for establishing connections and another for receiving and processing logging records.[^1]
- **Portability.** The Initiation Dispatcher's interface can be reused independently of the OS system calls that perform event demultiplexing.[^1]
- **Coarse-grained concurrency control.** Serialization at the Initiation Dispatcher level often eliminates the need for more complicated synchronization or locking within an application process.[^1]

Liabilities:[^1]

- **Restricted applicability.** The Reactor pattern can only be applied efficiently if the OS supports Handles.[^1]
- **Non-preemptive.** An Event Handler should not perform blocking I/O on an individual Handle since this will block the entire process.[^1]
- **Hard to debug.** The inverted flow of control oscillates between the framework infrastructure and the method callbacks on application-specific handlers.[^1]

The second liability is the common mistake. Here the first handler sleeps, and the second handler, which is ready at the same time, must wait:

```python wrong id=blocking
import time

start = time.monotonic()


def slow(data):
    time.sleep(0.2)  # blocking work inside a handler
    print("slow:", data)


def fast(data):
    waited = time.monotonic() - start
    print("fast:", data, "| waited 0.2 s or more:", waited >= 0.2)


handlers = {"a": slow, "b": fast}
for handle, data in [("a", "report"), ("b", "ping")]:  # both ready together
    handlers[handle](data)
```

```output for=blocking
slow: report
fast: ping | waited 0.2 s or more: True
```

The fix is to give long work to another thread or process, for example with the Active Object pattern.[^1]

## When not to use it

- The work for each request is long and uses the CPU. One thread then serves the requests one after the other.
- There are few connections, and a thread for each one is simpler to write and to read.
- The platform has no demultiplexer for the resources, so the reactor must be emulated with threads.[^1]

## Variants and related patterns

- **Handlers in a thread pool.** One basic modification is to invoke event handlers in their own threads for more concurrency.[^2]
- **Multireactor.** Replicated dispatchers run concurrently, and the dispatcher count matches the available CPU cores of the underlying hardware.[^2]
- **Proactor.** It dispatches the completion of asynchronous operations, not readiness. It still makes use of a reactor subcomponent.[^2]
- **Event handling patterns.** POSA2 puts Reactor with Proactor, Asynchronous Completion Token and Acceptor-Connector.[^3]

The Python module `selectors` allows high-level and efficient I/O multiplexing, built upon the select module primitives.[^4] This reactor waits on two real sockets, and the data of each registration is its handler:

```python run id=selectors
import selectors
import socket

sel = selectors.DefaultSelector()


def make_handler(name):
    def on_readable(sock):
        data = sock.recv(100)
        if data:
            print(name, "read", data.decode())
        else:
            print(name, "closed")
            sel.unregister(sock)
            sock.close()
    return on_readable


ends = {}
for name in ["one", "two"]:
    client, server = socket.socketpair()
    server.setblocking(False)
    sel.register(server, selectors.EVENT_READ, make_handler(name))
    ends[name] = client


def dispatch_once():
    for key, _ in sel.select(timeout=1):
        key.data(key.fileobj)  # call the registered handler


for name, message in [("one", b"hello"), ("two", b"hi")]:
    ends[name].sendall(message)
    dispatch_once()
for name in ["one", "two"]:
    ends[name].close()
    dispatch_once()
```

```output for=selectors
one read hello
two read hi
one closed
two closed
```

## Known uses

- **InterViews.** The InterViews window system implements the pattern under the name Dispatcher.[^1]
- **ACE.** The ACE framework uses the Reactor pattern as its central event demultiplexer and dispatcher.[^1]
- **libuv.** libuv was originally written for Node.js.[^5] Its I/O loop is meant to be tied to a single thread, and it polls non-blocking sockets with epoll on Linux, kqueue on OSX and other BSDs, event ports on SunOS and IOCP on Windows.[^5]
- **Python.** The `selectors` module has concrete implementations such as `KqueueSelector` and `EpollSelector`.[^4]

## References
