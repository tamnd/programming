---
tag: M8ZWG3
kind: concept
slug: event-driven-programming
name: Event-driven programming
aliases: [event loop, event handlers, callbacks on events, listeners, reactor pattern, GUI event programming]
area: paradigms/event-driven
level: core
summary: >-
  Event-driven programming structures a program as handlers that an event loop calls when events happen, such as input, timers or ready sockets, so the events decide the order of work.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1984
  people: [John Ousterhout]
  note: Graphical systems such as the Macintosh and the X Window System put an event loop at the center of each application, and Ousterhout argued in 1995 that events are better than threads for most programs.
see_also: [async-await, futures, threads, closures, higher-order-functions, channels, memory-leaks, imperative-programming]
same_as:
  wikidata: Q1135914
  wikipedia: Event-driven_programming
sources:
- url: https://web.stanford.edu/~ouster/cgi-bin/papers/threads.pdf
  title: Why Threads Are A Bad Idea (for most purposes), John Ousterhout, 1995
  locator: slides 8, 10 and 11
- url: https://html.spec.whatwg.org/multipage/webappapis.html
  title: HTML Living Standard, WHATWG
  locator: 8.1.7 Event loops
- url: https://nodejs.org/en/learn/asynchronous-work/event-loop-timers-and-nexttick
  title: The Node.js Event Loop, Node.js documentation
  locator: What is the Event Loop?
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Event_loop
  title: JavaScript execution model, MDN Web Docs
  locator: Run-to-completion
---

## Idea

Event-driven programming structures a program as a set of handlers that run when events happen. An event is a fact that the program must react to: a key press, a click, data on a socket, a timer that expires or a message from another part of the program. An event loop waits for events and calls the handlers that the program registered for them. John Ousterhout summarized the model in 1995 in a list: "One execution stream: no CPU concurrency. Register interest in events (callbacks). Event loop waits for events, invokes handlers. No preemption of event handlers. Handlers generally short-lived."[^1] The example below is a small event loop with a simulated clock:

```python run id=idea
import heapq
import itertools
from collections import defaultdict

handlers = defaultdict(list)
queue = []
order = itertools.count()
now = 0


def on(event, handler):
    handlers[event].append(handler)


def emit(event, data=None, delay=0):
    heapq.heappush(queue, (now + delay, next(order), event, data))


def run():
    global now
    while queue:
        now, _, event, data = heapq.heappop(queue)
        for handler in handlers[event]:
            handler(data)


def on_click(button):
    print(f"t={now} click on {button}")
    if button == "save":
        emit("saved", "report.txt", delay=30)


on("click", on_click)
on("click", lambda button: print(f"t={now} log: {button}"))
on("saved", lambda name: print(f"t={now} saved {name}"))
on("tick", lambda n: print(f"t={now} tick {n}"))

emit("click", "save", delay=10)
emit("tick", 1, delay=20)
emit("click", "open", delay=25)
emit("tick", 2, delay=40)
run()
```

```output for=idea
t=10 click on save
t=10 log: save
t=20 tick 1
t=25 click on open
t=25 log: open
t=40 tick 2
t=40 saved report.txt
```

The program does not decide the order of its work. The events decide it. The main part of the program only registers handlers and starts the loop. A handler can emit new events, as `on_click` does when it schedules the `saved` event, and the loop delivers them later. Two handlers for the same event run in the order of registration.

## How it works

**The event loop.** At its center, an event-driven program has a loop: wait for the next event, find the handlers for it, call them, and repeat. The loop is often in a library or in the runtime, not in the program. The HTML standard requires it of browsers: "To coordinate events, user interaction, scripts, rendering, networking, and so forth, user agents must use event loops as described in this section."[^2] Node.js uses the same model for servers, and the Node.js documentation says that the event loop "is what allows Node.js to perform non-blocking I/O operations", even though one JavaScript thread runs the code by default.[^3]

**Registration.** A program connects a handler to an event source: `addEventListener` in the browser, `on` in Node.js, `+=` on a C# event, a listener object in Java, a callback stored with a file descriptor in Python's `selectors`. The source keeps a list of handlers and calls each one when the event happens. This is the observer pattern, with the loop as the part that decides when.

**Waiting for many sources.** A loop that serves sockets must wait for all of them at once. The operating system provides calls for this: `select` and `poll` on POSIX systems, `epoll` on Linux, `kqueue` on the BSDs and macOS, and I/O completion ports on Windows. The loop gives the kernel a list of descriptors, sleeps until one is ready, and then calls the handler for each ready descriptor. Timers are kept in a priority queue sorted by due time, and the loop uses the time to the next timer as the limit of its wait.

**Run to completion.** In most event loops, a handler runs until it returns, and no other handler starts in between. MDN describes the result for JavaScript: "whenever a function runs, it cannot be preempted and will run entirely before any other code runs".[^4] A handler can therefore update shared data without locks. The cost is that a slow handler delays every other event. MDN states the downside: "if a job takes too long to complete, the web application is unable to process user interactions like click or scroll."[^4]

**State across events.** A handler must return to let the loop continue, so it cannot keep local variables from one event to the next. Ousterhout listed this as a problem of the model: a program "can't maintain local state across events (handler must return)."[^1] Programs keep the state in objects or closures, or in an explicit state machine. Callbacks, promises and [async/await](/async-await/) are steps that make this easier: an `async` function looks like straight-line code, and the compiler splits it into handlers that the loop calls when each awaited result is ready.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `selectors`, `asyncio` event loop, `tkinter` |
| JavaScript | native | the host event loop, `addEventListener`, `EventEmitter` |
| TypeScript | native | as JavaScript, with typed events |
| Java | stdlib | listener interfaces, `java.beans`, the Swing event thread |
| Kotlin | stdlib | Java listeners; `Flow` in the coroutines library |
| C# | native | `event` members of delegate type |
| Go | idiom | a loop over `select` on channels |
| Rust | idiom | a loop over a channel with an enum of events; async runtimes in crates |
| C | idiom | `poll` or `select` with function pointers |
| C++ | idiom | a queue of `std::function` handlers; Boost.Asio, Qt |
| Bash | idiom | `trap` for signals; a `read` loop over a pipe |
| PowerShell | native | `Register-ObjectEvent`, `Register-EngineEvent` |
| SQL | native | triggers on `INSERT`, `UPDATE` and `DELETE` |
| Regex | n/a | no events |
| HTML | native | element events and `on` attributes |
| CSS | n/a | state pseudo-classes, but no handlers |

JavaScript is the language in which event-driven programming is the default. Every host runs scripts as jobs in an event loop, and there is no way to block and wait in the language, so all I/O is done with callbacks, promises or `await`. C# is the only other core language with events in its syntax. An `event` member is a list of delegates that only the declaring class can raise, and the C# file of this page shows the compile error when other code tries to raise it.

Python, Java and Kotlin put event loops in the standard library: `asyncio` and `tkinter` in Python, the Swing and JavaFX event threads in Java. Go, Rust, C and C++ have no event loop in the standard library. Go does not need one for I/O, because goroutines block cheaply and the runtime uses an event loop of the kernel under them. A `for` loop over `select` is still the Go way to react to several sources of events. Rust programs use a channel and an enum of events, or an async runtime from a crate. C and C++ programs use the calls of the operating system directly or through libraries such as libuv, libevent, Boost.Asio and Qt.

SQL triggers and HTML events are event-driven parts of languages that are not general-purpose. A trigger runs when a statement changes a table, and an HTML attribute such as `onclick` connects a handler to an element.

## Variations and related ideas

- **Callbacks.** A handler passed as a function value is a callback. See [higher-order functions](/higher-order-functions/) and [closures](/closures/).
- **Promises and futures.** A promise is an object for a result that an event will deliver later, and handlers attach to it. See [futures](/futures/).
- **Async/await.** The compiler turns sequential code into handlers on an event loop. See [async/await](/async-await/).
- **Observer pattern.** An object keeps a list of listeners and notifies them of changes, as `PropertyChangeSupport` does in the Java file.
- **Publish and subscribe.** Senders publish messages to topics, and a broker delivers them to subscribers. Neither side knows the other.
- **Reactive programming.** Streams of events are values that can be mapped, filtered and joined, as in Rx libraries and Kotlin `Flow`.
- **The actor model.** Each actor has a mailbox and handles one message at a time, which is an event loop for each actor. See [channels](/channels/).
- **Interrupts and signals.** Hardware interrupts and Unix signals are events at the level of the machine and the operating system. Their handlers can run at almost any point and have strict limits on what they may do.
- **Event sourcing.** A system stores the list of events that changed its state, and computes the state by replaying them.

## Pitfalls

- **Blocking the loop.** A handler that waits for I/O or computes for a long time stops all other events. The wrong blocks in the Python and JavaScript files show a timer that fires late. Use asynchronous calls, split the work, or move it to a thread or a worker.
- **Callback nesting.** When each step waits for the last one, callbacks nest deeper and deeper, and error handling repeats at each level. Promises and `async`/`await` flatten the code.
- **Lost errors.** An exception in a handler goes to the event loop, not to the code that registered the handler. A rejected promise without a handler can be lost. Handle errors in each handler, and install a handler for unhandled rejections.
- **Listener leaks.** A listener that is never removed keeps its closure and all that the closure refers to. Remove listeners when the object that registered them goes away. See [memory leaks](/memory-leaks/).
- **Order assumptions.** Synchronous events, such as `emit` in Node.js, run the handlers before `emit` returns. Asynchronous events run later. A program that mixes the two can see events in an order its author did not expect.
- **Reentrancy.** A handler that raises an event can cause another handler to change the same state while the first handler is still running.
- **Starvation by microtasks.** A microtask that always schedules another microtask stops the loop from reaching timers and I/O.

## Cost

An event loop serves many connections with one thread. Each waiting connection costs only its state object and a registration with the kernel, not a thread stack, which is why event-driven servers such as nginx and Node.js handle tens of thousands of connections in little memory. Ousterhout noted that events have "no locking overheads" and "no context switching" on one CPU.[^1] The calls `epoll` and `kqueue` cost time in proportion to the number of ready descriptors, not the number of registered ones. The limit is the CPU: one loop uses one core, so a program runs one loop for each core, or moves CPU-heavy work to other threads. Each event also costs a dispatch: a lookup of the handlers and an indirect call, and for promises an allocation.

## History

Event-driven programs began with interrupts in the hardware of the 1950s, which made the processor jump to a handler when a device needed service. Interactive graphics systems of the 1960s and 1970s, and the Smalltalk environment at Xerox PARC, built programs around events from the keyboard and the mouse. The Apple Macintosh in 1984, the X Window System in 1984 and Microsoft Windows in 1985 made an event loop the center of every graphical application. On servers, the BSD `select` call of 1983 let one process wait for many sockets, and Ousterhout's 1995 talk "Why Threads Are A Bad Idea (for most purposes)" argued that events are better than threads for most programs.[^1] FreeBSD added `kqueue` in 2000 and Linux added `epoll` in 2002. Netscape added JavaScript to its browser in 1995 with event handlers on HTML elements. Node.js brought the browser model to servers in 2009, and promises and `async`/`await` changed how programs in JavaScript, C#, Python and Rust write event-driven code in the 2010s.

## References
