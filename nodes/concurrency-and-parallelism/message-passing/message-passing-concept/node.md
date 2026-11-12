---
tag: KR08XP
kind: concept
slug: message-passing-concept
name: Message passing
aliases: [message passing, message-passing concurrency, send and receive, mailbox, inbox, producer-consumer queue, share memory by communicating, inter-process communication, IPC, postMessage]
area: concurrency-and-parallelism/message-passing
level: core
summary: >-
  In message passing, parts of a program work together by sending each other messages instead of changing shared variables. Each part owns its own state and changes it only when a message arrives.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [Tony Hoare, Carl Hewitt]
  note: Hoare described communicating sequential processes in 1978, five years after the actor formalism of Hewitt, Bishop and Steiger. CSP influenced occam, Erlang and Go, and the first version of the MPI standard for parallel programs appeared in 1994.
see_also: [channels, actor-model, mutual-exclusion, data-races, threads, objects-and-messages, deadlock]
same_as:
  wikidata: Q1859284
  wikipedia: Message_passing
sources:
- url: https://en.wikipedia.org/wiki/Message_passing
  title: Message passing, Wikipedia
  locator: Introduction; Synchronous versus asynchronous message passing; Mathematical models; References
- url: https://go.dev/doc/effective_go
  title: Effective Go
  locator: Concurrency, Share by communicating
- url: https://doc.rust-lang.org/book/ch16-02-message-passing.html
  title: Transfer Data Between Threads with Message Passing, The Rust Programming Language
  locator: Chapter 16.2
- url: https://www.erlang.org/doc/system/conc_prog.html
  title: Concurrent Programming, Erlang System Documentation
  locator: Message Passing
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Channel types; Making slices, maps and channels
- url: https://docs.python.org/3/library/multiprocessing.html
  title: multiprocessing, Process-based parallelism, Python documentation
  locator: Exchanging objects between processes; Sharing state between processes
- url: https://developer.mozilla.org/en-US/docs/Web/API/Worker/postMessage
  title: Worker postMessage() method, MDN Web Docs
  locator: Introduction
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-17.html
  title: Structure and Interpretation of Computer Programs, 2nd edition
  locator: 2.4.3 Data-Directed Programming and Additivity, Message passing
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/BlockingQueue.html
  title: Interface BlockingQueue, Java SE 25 API
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/core/extensions/channels
  title: System.Threading.Channels library, .NET documentation
  locator: Producer/consumer conceptual programming model
- url: https://en.wikipedia.org/wiki/Message_Passing_Interface
  title: Message Passing Interface, Wikipedia
  locator: Introduction; History
- url: https://en.wikipedia.org/wiki/Communicating_sequential_processes
  title: Communicating sequential processes, Wikipedia
  locator: Introduction
---

## Idea

When two threads change the same variable, they need a lock, and a missing lock is a bug that shows up only sometimes. Message passing removes the shared variable. One part of the program owns the data. Other parts send it messages, such as "add 1" or "tell me the total", and the owner handles one message at a time. Because only the owner touches the data, no lock protects it.

```python run id=idea
import queue
import threading


# The total lives in one thread. Other code changes it only by sending messages.
def counter(inbox):
    total = 0
    while True:
        message = inbox.get()
        if message[0] == "add":
            total += message[1]
        elif message[0] == "get":
            message[1].put(total)
        elif message[0] == "stop":
            return


inbox = queue.Queue()
worker = threading.Thread(target=counter, args=(inbox,))
worker.start()


# Four threads send 1000 messages each. They share no variable, so they need no lock.
def sender():
    for _ in range(1000):
        inbox.put(("add", 1))


senders = [threading.Thread(target=sender) for _ in range(4)]
for t in senders:
    t.start()
for t in senders:
    t.join()

# To read the total, send a request that carries a queue for the reply.
reply = queue.Queue()
inbox.put(("get", reply))
print("total:", reply.get())

inbox.put(("stop",))
worker.join()
print("worker stopped:", not worker.is_alive())
```

```output for=idea
total: 4000
worker stopped: True
```

The `counter` thread owns `total`. Four other threads send 4000 messages in total, and the queue delivers them one at a time, so the result is always 4000 with no lock in the program. To read the total, the main thread sends a `get` message with a queue for the reply and waits on that queue. The `stop` message tells the worker to end.

## How it works

**Send, receive and the owner.** Wikipedia defines message passing as "a technique for invoking behavior (i.e., running a program) on a computer"[^1]. "The invoking program sends a message to a process (which may be an actor or object) and relies on that process and its supporting infrastructure to then select and run some appropriate code"[^1]. The receiver decides what a message means, so the sender does not need access to the state of the receiver. The Go documentation puts the rule in one line: "Do not communicate by sharing memory; instead, share memory by communicating"[^2]. Values go from one goroutine to another on channels, so "Only one goroutine has access to the value at any given time. Data races cannot occur, by design"[^2].

**Channels and mailboxes.** A message needs a path from the sender to the receiver. There are two common forms:

- A channel is a typed pipe that any holder of one end can use. "A channel is a general programming concept by which data is sent from one thread to another"[^3]. Go, Rust, C# and Kotlin have channels. See [channels](/channels/).
- A mailbox belongs to one process or actor. In Erlang, "Each process has its own input queue for messages it receives. New messages received are put at the end of the queue"[^4], and `receive` takes the first message that matches a pattern. See the [actor model](/actor-model/).

**Synchronous and asynchronous.** In synchronous message passing, "the sending process waits until the receiving process accepts the message"[^1]. A Go channel with no buffer works this way: "communication succeeds only when both a sender and receiver are ready"[^5]. In asynchronous message passing, the message waits in a queue and the sender continues. A queue with a limit makes a choice when it is full. Wikipedia says that "A decision has to be made whether to block the sender or whether to discard future messages. A blocked sender may lead to deadlock"[^1].

**Copy or move.** A message must not leave shared state behind. Processes have separate memory, so a message between processes is a copy. Python `multiprocessing` "supports two types of communication channel between processes"[^6], queues and pipes. "When an object is put on a queue, the object is pickled"[^6], so the receiver gets a copy. A JavaScript worker gets a copy too: "The data may be any JavaScript object that can be handled by the structured clone algorithm"[^7]. Threads share memory, so the language must stop the sender from using the value after the send. Rust does this with ownership: "The send function takes ownership of its parameter, and when the value is moved the receiver takes ownership of it"[^3]. In Java, Go and C#, the sender can keep a reference to a mutable object that it sent, so the rule is a convention. Send immutable values, or do not touch an object after you send it.

**Request and reply.** A message goes in one direction. To get an answer, the request carries a place for the reply, such as a reply channel or the identifier of the sender. The idea example, the Go example and the Rust example all send a reply channel inside the request.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `queue.Queue` between threads; `multiprocessing.Queue` and `Pipe` between processes; `asyncio.Queue` |
| JavaScript | native | `postMessage` to workers and `MessageChannel` ports; messages are structured clones |
| TypeScript | native | the same `postMessage` API, with types for the message data |
| Java | stdlib | `BlockingQueue` implementations in `java.util.concurrent` |
| Kotlin | library | `Channel` in kotlinx.coroutines, with `send` and `receive` |
| C# | stdlib | `System.Threading.Channels`, with bounded and unbounded channels |
| Go | native | `chan` types, the `<-` operator, `select` and `close` |
| Rust | stdlib | `std::sync::mpsc` channels in the standard library; ownership moves the message |
| C | idiom | no language support; POSIX pipes, sockets and message queues, or MPI |
| C++ | idiom | no standard channel; a queue with a mutex and a condition variable, or a library |
| Bash | idiom | a pipeline sends the output of one process to the input of the next; named pipes |
| PowerShell | idiom | the pipeline passes objects from one command to the next; jobs return output |
| SQL | n/a | no messages between parts of a query; some databases add queues and `LISTEN` and `NOTIFY` |
| Regex | n/a | a pattern does not run concurrent parts |
| HTML | n/a | HTML is markup; `postMessage` is part of the JavaScript API |
| CSS | n/a | CSS has no concurrent parts |

Go is the only language of the set with channels in the syntax. In JavaScript and TypeScript, a worker cannot share ordinary objects, so the message is always a copy. In the other languages, message passing is a library on top of threads that share memory, and the program must not share the data that it sends.

The example files show these differences. Python moves work to another process through a `multiprocessing.Queue`, shows that a list sent through a `Pipe` is a copy, and writes an account in the message-passing style of SICP. JavaScript sends an array to a worker and shows that the worker changed a copy, that a function cannot be cloned and that a `MessageChannel` connects two ports. Java uses a `LinkedBlockingQueue` as an inbox, and shows that a full `ArrayBlockingQueue` and a `SynchronousQueue` with no waiting receiver refuse `offer`. C# uses unbounded and bounded channels with the full modes `Wait` and `DropOldest`. Go uses `select`, a buffered channel, `close` and a pool of workers. Rust has many producers that each clone the sender, and shows that `send` moves the value. C passes numbers to a child process through a pipe. C++ builds a channel from a mutex, a condition variable and a queue.

## Variations and related ideas

- **Messages to objects.** In Smalltalk and in the design of Alan Kay, calling a method is sending a message to an object. Wikipedia says that "Alan Kay has argued that message passing is more important than objects in OOP"[^1]. See [objects and messages](/objects-and-messages/).
- **Dispatch on the operation name.** SICP describes a data object that is a procedure, which takes the name of an operation and runs it. "This style of programming is called message passing"[^8]. The Python example has an account of this form.
- **Producer and consumer queues.** Java `BlockingQueue` is "A Queue that additionally supports operations that wait for the queue to become non-empty when retrieving an element, and wait for space to become available in the queue when storing an element"[^9]. In .NET, "Channels are an implementation of the producer/consumer conceptual programming model"[^10].
- **Message passing between machines.** Sockets, HTTP, message brokers and MPI carry messages over a network. "The Message Passing Interface (MPI) is a portable message-passing standard designed to function on parallel computing architectures"[^11].
- **Shared memory.** The other main model of concurrency uses shared variables with locks and atomic operations. See [mutual exclusion](/mutual-exclusion/). Many programs use both.

## Pitfalls

- **Sharing through the message.** If a thread sends a reference to a mutable object and keeps using it, the two threads share memory again, with the same data races. Send a copy, an immutable value or ownership.
- **Deadlock.** Two parts that each wait for a message from the other wait forever. A full bounded queue that blocks its sender can do the same. See [deadlock](/deadlock/).
- **Queues with no limit.** An unbounded queue hides a consumer that is too slow until the program runs out of memory. Use a bounded queue and decide what happens when it is full.
- **The end of the stream.** A receiver must know when no more messages come. Close the channel, drop the last sender, or send a stop message. A Go receive from a closed channel gives the zero value, so check the second result.
- **Too far.** Effective Go warns that "This approach can be taken too far. Reference counts may be best done by putting a mutex around an integer variable, for instance"[^2]. A lock is simpler for one small counter.

## Cost

A message between threads costs a queue operation and, when the receiver waits, a wake-up of a thread. That is much more than a write to memory, so a message should carry a useful amount of work. A message between processes also pays for the copy and for the system call, and a message over a network pays for the network. In return, the owner of the data needs no locks, and a design with messages often moves from threads to processes or to machines with few changes.

## History

The actor formalism of Carl Hewitt, Peter Bishop and Richard Steiger, "A universal modular ACTOR formalism for artificial intelligence"[^1], appeared in 1973. "CSP was first described by Tony Hoare in a 1978 article"[^12]. CSP is one of the process calculi "based on message passing via channels"[^12], and it "also influenced the design of programming languages such as Limbo"[^12], RaftLib, Erlang, Go, Crystal and Clojure core.async. In parallel computing, the MPI Forum wrote a standard for message passing with library routines for C, C++ and Fortran, and "version 1.0 of MPI was released in June 1994"[^11]. The Go documentation made "share memory by communicating" a rule of style[^2].

## References
