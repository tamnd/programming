---
tag: K75AMM
kind: concept
slug: actor-model
name: Actor model
aliases: [actors, actor-oriented programming, mailboxes, message passing concurrency, let it crash, supervision trees]
area: paradigms/actor-oriented
level: core
summary: >-
  The actor model builds a concurrent program from actors that each own private state and a mailbox, handle one message at a time, and interact only by sending messages.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1973
  people: [Carl Hewitt, Peter Bishop, Richard Steiger, Joe Armstrong]
  note: Hewitt, Bishop and Steiger proposed actors in 1973, and Erlang made isolated processes with supervision the base of a language from 1986.
see_also: [channels, threads, data-races, deadlock, mutual-exclusion, futures, event-driven-programming, object-oriented-programming]
same_as:
  wikidata: Q344524
  wikipedia: Actor_model
sources:
- url: https://www.ijcai.org/Proceedings/73/Papers/027B.pdf
  title: A Universal Modular ACTOR Formalism for Artificial Intelligence, Carl Hewitt, Peter Bishop and Richard Steiger, IJCAI 1973
  locator: introduction
- url: https://doc.akka.io/libraries/akka-core/current/general/actors.html
  title: What is an Actor?, Akka documentation
  locator: What is an Actor?
- url: https://erlang.org/download/armstrong_thesis_2003.pdf
  title: Making reliable distributed systems in the presence of software errors, Joe Armstrong, PhD thesis, 2003
  locator: 2.4 Concurrency oriented programming; 4.4 Let it crash
- url: https://www.erlang.org/doc/system/ref_man_processes.html
  title: Processes, Erlang System Documentation
  locator: Processes
---

## Idea

The actor model builds a concurrent program from actors: independent units that each have private state and a mailbox, and that interact only by sending messages to each other. An actor handles one message at a time. To handle it, an actor can change its own state, send messages to actors it knows, and create new actors. It cannot read or write the state of another actor. Carl Hewitt, Peter Bishop and Richard Steiger introduced the model in 1973 and claimed that all computation can be "defined in terms of one kind of behavior: sending messages to actors."[^1] The example below is a counter actor in Python. Four threads send it 4000 messages at the same time, and no lock protects the count:

```python run id=idea
import queue
import threading


class Actor:
    def __init__(self):
        self.mailbox = queue.Queue()
        self.thread = threading.Thread(target=self._loop)
        self.thread.start()

    def send(self, message):
        self.mailbox.put(message)

    def _loop(self):
        while (message := self.mailbox.get()) is not None:
            self.receive(message)

    def stop(self):
        self.send(None)
        self.thread.join()


class Counter(Actor):
    def __init__(self):
        self.count = 0  # private state: only the actor's own thread uses it
        super().__init__()

    def receive(self, message):
        match message:
            case ("add", n):
                self.count += n
            case ("get", reply_to):
                reply_to.put(self.count)


counter = Counter()
clients = [threading.Thread(target=lambda: [counter.send(("add", 1)) for _ in range(1000)]) for _ in range(4)]
for c in clients:
    c.start()
for c in clients:
    c.join()

reply = queue.Queue()
counter.send(("get", reply))
print("count:", reply.get())
counter.stop()
```

```output for=idea
count: 4000
```

The count is correct because only the actor's own thread reads and writes `self.count`. The mailbox is the only shared object, and it is a thread-safe queue. The clients do not call a method on the counter. They put a message in its mailbox and continue. To read the count, a client sends a `get` message with a place for the answer, and waits for the reply.

## How it works

**Mailboxes and asynchronous sends.** Each actor has an address and a mailbox. A send puts a message in the mailbox of the receiver and returns at once. The sender does not wait for the receiver to handle the message, and the receiver handles its messages one after another. The order of messages between one sender and one receiver is kept in most implementations, but there is no order between messages from different senders.

**The three actions.** The Akka documentation lists what an actor can do when it receives a message: "send a finite number of messages to Actors it knows", "create a finite number of new Actors" and "designate the behavior to be applied to the next message".[^2] The last action is how an actor changes state. In Erlang, an actor is a function that loops with its state as an argument, and it calls itself with new state after each message. In object-oriented libraries, an actor is an object whose fields only its own handler can change.

**Isolation.** Actors share nothing. Joe Armstrong, a designer of Erlang, wrote that processes "should share no data, and only communicate by message passing", because as soon as two processes share memory, "a software error in one of the processes will corrupt the shared resource."[^3] Erlang copies each message into the heap of the receiver. Languages without this guarantee rely on a rule: send only immutable data, or data that the sender will not touch again. Rust checks the rule with the `Send` trait. Python and Java do not check it, and the wrong block in the Python file shows a message that changes after the send.

**Addresses and location.** An actor knows other actors only by their addresses. An address can come from the creation of the actor, from a message, or from a registry of names. Because all interaction is by message, an address can refer to an actor on another machine with no change to the code that sends to it. Erlang and Akka use this to spread a system over many machines.

**Supervision.** Actor systems handle failure by structure. A supervisor is an actor that starts other actors, watches them, and restarts them when they crash. Armstrong called the style "let it crash": "in the event of an error, then the program should just crash", and another process fixes the problem.[^3] A crashed actor loses only its own state, which a restart rebuilds from a known start. The Python file shows a supervisor that restarts a worker after bad input.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | a thread with a `queue.Queue` mailbox |
| JavaScript | stdlib | worker threads and web workers with `postMessage` |
| TypeScript | stdlib | as JavaScript |
| Java | library | `BlockingQueue` and virtual threads; Akka, Pekko |
| Kotlin | library | the coroutines `actor` builder; Akka, Pekko |
| C# | library | `System.Threading.Channels`; Orleans, Akka.NET |
| Go | idiom | a goroutine that owns its state and reads a channel |
| Rust | idiom | a thread with an `mpsc` channel; `Send` checks messages; Actix, Ractor |
| C | absent | threads with a hand-made mailbox |
| C++ | idiom | `std::jthread`, a queue of `std::variant`; CAF |
| Bash | idiom | a background process that reads a named pipe |
| PowerShell | absent | jobs and runspaces, no mailboxes |
| SQL | n/a | transactions, not actors |
| Regex | n/a | no concurrency |
| HTML | n/a | no concurrency |
| CSS | n/a | no concurrency |

No core language has actors in its syntax. The languages built on the model are Erlang and Elixir, which run on the BEAM virtual machine, and Pony. The Erlang documentation says that its processes "are lightweight (grow and shrink dynamically) with a small memory footprint, fast to create and terminate, and the scheduling overhead is low."[^4] A BEAM system can run millions of them, each with its own heap and garbage collection.

In the core languages, an actor is a pattern: a thread, goroutine or task with private state and a queue. The examples on this page all have the same shape: a bank account that many clients deposit into at the same time, with the balance owned by one thread and read with a reply message. JavaScript workers are the closest to Erlang processes in isolation, because each has its own heap and the runtime copies each message. Rust is the only core language whose compiler rejects a message that would share memory unsafely, as the second block of the Rust file shows. Go's goroutines and channels come from a different model, communicating sequential processes, in which channels are the named things and not the processes. See [channels](/channels/).

## Variations and related ideas

- **CSP.** In communicating sequential processes, processes send to channels, not to each other, and a send can wait until the receiver is ready. Go and Clojure's core.async use it. See [channels](/channels/).
- **Active objects.** An object whose method calls are turned into messages to its own thread, and whose results come back as futures. See [futures](/futures/).
- **Virtual actors.** In Orleans, actors always exist in concept. The runtime creates one when a message arrives and removes it when it is idle, and it stores its state.
- **Event loops.** Each actor runs a small event loop over its mailbox. See [event-driven programming](/event-driven-programming/).
- **Object-oriented programming.** Alan Kay's idea of objects that send messages is close to actors, and Hewitt cited Smalltalk as an influence. See [object-oriented programming](/object-oriented-programming/).
- **Message queues and microservices.** Services that interact through a broker such as Kafka or RabbitMQ apply the same rules at the scale of a network.

## Pitfalls

- **Shared mutable messages.** Sending a reference to mutable data breaks isolation. The wrong block in the Python file shows a message that the sender changes after the send.
- **Deadlock by request and reply.** Actor A sends a request to B and waits for the reply, while B sends a request to A and waits. Neither handles its mailbox. Avoid blocking waits inside handlers, or use timeouts. See [deadlock](/deadlock/).
- **Unbounded mailboxes.** A fast sender can fill the mailbox of a slow actor until memory runs out. Use bounded mailboxes and back-pressure, or drop messages on purpose.
- **Lost messages.** A message to an actor that has crashed or stopped is lost, and the sender is not told. Use replies, timeouts and monitors when delivery matters.
- **A single actor as a bottleneck.** All work for one actor runs on one thread. A popular actor limits the throughput of the whole system. Split its state over many actors.
- **Hidden ordering assumptions.** Messages from different senders arrive in no fixed order. A protocol that needs an order must enforce it with replies or sequence numbers.

## Cost

A send costs a queue operation and, where messages are copied, a copy of the data. Erlang processes start with a few hundred words of memory, so a system can create one for each connection or request. Actors on threads cost a thread each, and libraries such as Akka run many actors on a small pool of threads, with each actor scheduled only when its mailbox has messages. An actor removes the cost of locks from its own state, but the mailbox itself needs synchronization, and an exchange of a request and a reply costs at least two queue operations and two thread wakeups. The model gains most when the state is naturally split among many actors, so that the work spreads over all cores without contention.

## History

Carl Hewitt, Peter Bishop and Richard Steiger proposed actors at the International Joint Conference on Artificial Intelligence in 1973, as a single concept for the PLANNER language project at MIT.[^1] Irene Greif gave the model an operational semantics in 1975, and Gul Agha's thesis of 1985 developed it as a basis for concurrent programming. Joe Armstrong, Robert Virding and Mike Williams built Erlang at Ericsson from 1986 for telephone switches, with isolated processes, messages and supervision, and Ericsson released it as open source in 1998. Armstrong's thesis of 2003 described the design as concurrency oriented programming.[^3] Scala included an actor library from 2006, Akka followed in 2009, and Microsoft Research released Orleans in 2015. Elixir brought a new syntax to the BEAM in 2012.

## References
