---
tag: Q4TD5Z
kind: concept
slug: channels
name: Channel
aliases: [CSP channel, message channel]
area: concurrency-and-parallelism/csp-and-channels
level: core
summary: >-
  A channel is a typed conduit between concurrent processes. One process sends a value into it and another receives the value, so the two exchange data and synchronize without shared variables.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [C. A. R. Hoare]
  note: Hoare described communicating sequential processes, where parallel processes exchange values only through input and output commands that wait for each other, with no shared variables and no automatic buffering.
see_also: [threads, data-races, mutual-exclusion, deadlock, async-await, closures, higher-order-functions]
same_as:
  wikidata: Q5072250
  wikipedia: Channel_(programming)
sources:
  - paper: doi:10.1145/359576.359585
    title: C. A. R. Hoare, Communicating Sequential Processes, Communications of the ACM 21(8), 1978, pages 666 to 677
    locator: section 1 (proposals 2 to 6); 5.1 Bounded Buffer; 7.3 Port Names; 7.4 Automatic Buffering; 7.6 Fairness; 7.8 Output Guards
  - url: https://go.dev/doc/effective_go#sharing
    title: Effective Go
    locator: Concurrency, Share by communicating
  - url: https://go.dev/doc/effective_go#channels
    title: Effective Go
    locator: Concurrency, Channels; Channels of channels
  - url: https://go.dev/ref/spec#Channel_types
    title: The Go Programming Language Specification
    locator: Channel types
  - url: https://go.dev/ref/spec#Send_statements
    title: The Go Programming Language Specification
    locator: Send statements
  - url: https://go.dev/ref/spec#Receive_operator
    title: The Go Programming Language Specification
    locator: Receive operator
  - url: https://go.dev/ref/spec#Close
    title: The Go Programming Language Specification
    locator: Built-in functions, Close
  - url: https://go.dev/ref/spec#Select_statements
    title: The Go Programming Language Specification
    locator: Select statements
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
  - url: https://go.dev/ref/mem#chan
    title: The Go Memory Model
    locator: Synchronization, Channel communication
  - url: https://go.dev/doc/faq#csp
    title: Go FAQ
    locator: Why build concurrency on the ideas of CSP?
  - url: https://go.dev/doc/faq#ancestors
    title: Go FAQ
    locator: What are Go's ancestors?
  - url: https://doc.rust-lang.org/std/sync/mpsc/index.html
    title: The Rust Standard Library, Module std::sync::mpsc
    locator: module description; Disconnection; Functions channel and sync_channel
  - url: https://doc.rust-lang.org/std/sync/mpsc/struct.Receiver.html
    title: The Rust Standard Library, Struct std::sync::mpsc::Receiver
    locator: methods recv, recv_timeout, try_recv and iter
  - url: https://doc.rust-lang.org/std/sync/mpmc/index.html
    title: The Rust Standard Library, Module std::sync::mpmc
    locator: module header (nightly-only experimental API, mpmc_channel)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/SynchronousQueue.html
    title: Java SE 25 API, Class SynchronousQueue
    locator: class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/BlockingQueue.html
    title: Java SE 25 API, Interface BlockingQueue
    locator: interface description (method table, close and shutdown, memory consistency effects)
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/channels
    title: .NET documentation, System.Threading.Channels library
    locator: introduction; Bounding strategies; Bounded channels (Full mode behavior); Producer APIs; Consumer APIs
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.concurrent.blockingcollection-1
    title: .NET API reference, BlockingCollection<T> Class
    locator: class description; methods CompleteAdding, GetConsumingEnumerable and TakeFromAny
  - url: https://docs.python.org/3.14/library/queue.html
    title: The Python Standard Library, queue, Python 3.14
    locator: module description; queue.Queue; Queue.task_done; Queue.join; Queue.shutdown
  - url: https://nodejs.org/api/worker_threads.html#class-messagechannel
    title: Node.js documentation, Worker threads
    locator: Class MessageChannel; port.postMessage(value[, transferList]); port.close(); worker_threads.receiveMessageOnPort(port)
  - url: https://kotlinlang.org/docs/channels.html
    title: Kotlin documentation, Channels
    locator: Channel basics; Closing and iteration over channels; Buffered channels
  - url: https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.channels/-channel.html
    title: kotlinx.coroutines API, Channel() factory function
    locator: signature (capacity = RENDEZVOUS)
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.condition.condvar
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.condition.condvar], paragraph 8"
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_cond_wait.html
    title: POSIX.1-2024, pthread_cond_wait
    locator: APPLICATION USAGE and DESCRIPTION (spurious wakeups)
  - url: https://www.erlang.org/doc/system/ref_man_processes.html#signals
    title: Erlang System Documentation, Processes
    locator: Signals
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_09_02
    title: POSIX.1-2024, Shell Command Language
    locator: 2.9.2 Pipelines
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/mkfifo.html
    title: POSIX.1-2024, mkfifo utility
    locator: DESCRIPTION
---

## Idea

Concurrent processes must exchange data. One way is to share variables and protect them with locks (see [[mutual-exclusion]]). A channel is the other way. A channel is a conduit that carries values of one type from one process to another. A process sends a value into the channel, and another process receives it. The send and the receive also synchronize the two processes, so the receiver sees the value only after the sender has made it.

Hoare described this model in 1978 as communicating sequential processes, or CSP. In his proposal, processes do not communicate by updating global variables. They communicate only through input and output commands. An output command in one process and an input command in another process meet, and the value is copied from the first process to the second.[^1] The Go documentation states the same idea as a slogan: "Do not communicate by sharing memory; instead, share memory by communicating."[^2] Only one process owns a value at a time, so two processes cannot race on it (see [[data-races]]).

In Go, `make(chan int)` makes a channel, `ch <- v` sends and `<-ch` receives.[^4] In the example, the main goroutine waits at the receive until the other goroutine sends.

```go run id=idea
package main

import "fmt"

func main() {
	results := make(chan string)
	go func() {
		results <- "hello from a goroutine"
	}()
	fmt.Println(<-results)
}
```

```output for=idea
hello from a goroutine
```

The function literal that starts the goroutine is a closure over `results` (see [[closures]]). A channel is a value, so a function can take it as a parameter, return it, or send it on another channel.[^3]

## How it works

### Send, receive and the buffer

A channel has a capacity. If the capacity is zero, the channel is unbuffered. A send and a receive on it succeed only when both a sender and a receiver are ready.[^4] The two processes meet at that point, which is a rendezvous. If the capacity is greater than zero, the channel holds a buffer. A send does not block while the buffer has room, and a receive does not block while the buffer holds a value. In both cases the channel is a first-in, first-out queue.[^4]

Hoare's processes had no buffer: a command waited until the other process was ready.[^1] He discussed automatic buffering and set it aside, because a buffer can be built from the primitives. His own example is a bounded buffer: a process that holds ten portions and runs between a producer and a consumer.[^1] Most channel libraries offer both forms.

| Capacity | Send blocks when | Receive blocks when | Use |
|---|---|---|---|
| 0 (rendezvous) | no receiver is ready | no sender is ready | hand over a value and synchronize at the same time |
| n > 0 (bounded) | the buffer holds n values | the buffer is empty | smooth out bursts; limit work in progress |
| no limit (unbounded) | never | the buffer is empty | decouple producers from a slow consumer, at the cost of memory |

A bounded channel gives back pressure. When the consumer is slow, the buffer fills and the producers wait. The .NET documentation uses this term for a writer that produces faster than the reader consumes.[^18] An unbounded channel never makes the producer wait, so its memory can grow without limit. Rust calls its unbounded `channel()` asynchronous, with a buffer that is "conceptually infinite".[^13]

### Synchronization and memory

A channel operation is also a memory barrier between the two processes. The Go memory model states that a send is synchronized before the completion of the matching receive. For an unbuffered channel, the receive is also synchronized before the completion of the send.[^10] Java gives the same guarantee for a `BlockingQueue`: the actions of a thread before it puts an object into the queue happen before the actions after another thread takes it.[^17] So the receiver can read the fields of a received object without a lock, as long as the sender does not change the object after the send.

The memory model has one more rule. On a channel with capacity C, the kth receive is synchronized before the completion of the (k+C)th send.[^10] So a buffered channel of capacity C can limit the number of active workers to C. Each worker sends before it starts and receives when it finishes. This is the counting semaphore idiom.[^3]

### Close

A sender can close a channel to say that no more values will come. In Go, a receive on a closed channel proceeds at once. It first yields the values that were sent before the close, and after that the zero value of the element type.[^6] The two-value form `v, ok := <-ch` tells the two cases apart: `ok` is false when the value comes from a closed, empty channel.[^6] A `for v := range ch` loop receives values until the channel is closed.[^9] A close is synchronized before a receive that returns because the channel is closed.[^10]

Close is a message from the sender side. A send on a closed channel causes a run-time panic, and so does a second close.[^5][^7] So the rule is: the sender closes, never the receiver. With several senders, the channel is closed only after all of them have finished (see Pitfalls).

Other libraries signal the end in other ways:

| Library | End of stream |
|---|---|
| Go | `close(ch)`; then `v, ok` gives `ok == false`[^6] |
| Rust `mpsc` | no close; the channel hangs up when all senders are dropped, and `recv` returns `Err`[^13] |
| .NET `Channel<T>` | `Writer.Complete()`; a later write throws `ChannelClosedException`[^18] |
| .NET `BlockingCollection<T>` | `CompleteAdding()`; `GetConsumingEnumerable()` ends[^19] |
| Kotlin `kotlinx.coroutines` | `close()`, which acts like a special close token[^22] |
| Java `BlockingQueue` | no close; the producer sends a "poison" object[^17] |
| Python `queue.Queue` | a sentinel object that the consumer recognizes[^20] |

The Rust rule fits ownership. Each producer holds a clone of the `Sender`. When the last clone is dropped, the receiver iterator ends.[^14] In the Rust example, the main thread drops its own sender before it reads, or the loop would never end.

### Select

A process often waits on more than one channel: a result channel, a cancel channel and a timer. Hoare allowed input commands in guards. When several guards are ready, the choice between them is arbitrary.[^1] Go has the `select` statement. If one or more of its cases can proceed, it picks one by a uniform pseudo-random choice. If none can proceed, it runs the `default` case if there is one; otherwise it blocks.[^8] A `default` case gives a send or receive that does not block. A case that receives from `time.After` gives a timeout, as in the Go example.

A nil channel is never ready. A send or a receive on it blocks forever.[^4][^5][^6] Inside a `select`, this is useful. A loop that sets a channel variable to `nil` turns off that case. Here the loop merges two channels and stops when both are closed.

```go run id=merge
package main

import "fmt"

func numbers(from, to int) <-chan int {
	ch := make(chan int)
	go func() {
		for i := from; i <= to; i++ {
			ch <- i
		}
		close(ch)
	}()
	return ch
}

func main() {
	a, b := numbers(1, 3), numbers(10, 12)
	sum := 0
	for a != nil || b != nil {
		select {
		case v, ok := <-a:
			if !ok {
				a = nil // a is closed: turn off this case
				continue
			}
			sum += v
		case v, ok := <-b:
			if !ok {
				b = nil
				continue
			}
			sum += v
		}
	}
	fmt.Println("sum:", sum)
}
```

```output for=merge
sum: 39
```

The order in which `select` takes the values changes from run to run, so the program prints only the sum. The return type `<-chan int` is a receive-only channel. The compiler rejects a send or a `close` on it.[^4][^7]

Most libraries have no general `select`. Rust `mpsc` has `recv_timeout` and `try_recv` on one receiver.[^14] Java has `poll` with a time limit.[^17] .NET `BlockingCollection<T>` has `TakeFromAny`, which takes an item from any one of several collections.[^19]

### Producers and consumers

The common shape has several producers, one channel and one or more consumers. Fan-out means several consumers take from one channel, so the work spreads over them. Fan-in means several producers send to one channel, so one consumer sees all results. Every example file shows fan-in: two producers each send three values, and one consumer adds them. The order of arrival changes from run to run, but the total does not.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Go | native | `chan T`, `<-`, `close`, `select`, `range` |
| Rust | stdlib | `std::sync::mpsc::channel` and `sync_channel`; multi-producer, single consumer |
| Java | stdlib | `BlockingQueue` (`ArrayBlockingQueue`, `LinkedBlockingQueue`) and `SynchronousQueue` |
| C# | stdlib | `System.Threading.Channels.Channel<T>` (asynchronous); `BlockingCollection<T>` (blocking) |
| PowerShell | stdlib | the .NET `BlockingCollection<T>` with `ForEach-Object -Parallel` or thread jobs |
| Python | stdlib | `queue.Queue` for threads |
| JavaScript | stdlib | `MessageChannel` and `postMessage` between worker threads in Node.js |
| TypeScript | stdlib | as JavaScript |
| Kotlin | library | `Channel` from `kotlinx.coroutines`; the JDK queues with the standard library only |
| C++ | idiom | a queue with `std::mutex` and `std::condition_variable` |
| C | idiom | a queue with `pthread_mutex_t` and `pthread_cond_t` |
| Bash | absent | a pipeline of processes, or a named pipe made by `mkfifo` |
| SQL | n/a | a query has no processes |
| Regex | n/a | a pattern has no processes |
| HTML/CSS | n/a | markup and style rules have no processes |

Go is the only tier 1 language with channels in the language itself. Its documentation traces this design to CSP, and names Newsqueak and Limbo as the languages that its concurrency ideas come from.[^11][^12]

Rust's `mpsc` module gives a `Sender` that can be cloned and one `Receiver`, so several producers can feed one consumer.[^13] `sync_channel(0)` makes a rendezvous channel.[^13] A multi-consumer module, `std::sync::mpmc`, exists only as a nightly-only experimental API.[^15]

Java's `SynchronousQueue` has no internal capacity, not even a capacity of one. Each insert waits for a remove. Its documentation compares it with the rendezvous channels of CSP and Ada.[^16] A `BlockingQueue` has no close or shutdown operation, so the usual end signal is a poison object that the consumer recognizes.[^17] Kotlin on the JVM can use the same queues. The idiomatic Kotlin form is the `Channel` of `kotlinx.coroutines`, a library outside the Kotlin standard library. Its `send` and `receive` suspend a coroutine instead of blocking a thread, and its default capacity is a rendezvous.[^22][^23]

The .NET `Channel<T>` has an asynchronous writer and reader. A bounded channel needs a capacity greater than zero, so .NET has no rendezvous channel. When the buffer is full, the writer waits by default. Other full modes drop the newest item, drop the oldest item, or drop the item being written.[^18] The older `BlockingCollection<T>` blocks the calling thread instead.[^19] PowerShell uses it directly, because a script can call any .NET class.

Python's `queue.Queue` is a multi-producer, multi-consumer queue that does its own locking. A `maxsize` of zero or less makes the queue unbounded. `task_done` and `join` let a producer wait until consumers have processed every item.[^20] Programs that use coroutines take `asyncio.Queue` instead (see [[async-await]]).

In Node.js, each worker thread has its own event loop. A `MessageChannel` is an asynchronous, two-way channel with two ports. `postMessage` copies its value with the HTML structured clone algorithm, so the receiver never shares the object with the sender.[^21] A send does not wait for the receiver. `receiveMessageOnPort` receives one message at once and returns `undefined` when none waits.[^21]

C and C++ have no channel in their standard libraries. The usual form is a queue that a mutex protects, with one condition variable for "not full" and one for "not empty". A wait on a condition variable can return without a signal (a spurious wakeup), so the code tests the condition again in a loop. The C++ standard and POSIX both allow such wakeups.[^24][^25]

A Bash pipeline connects the standard output of one command to the standard input of the next.[^27] It is a channel of bytes between processes. The `mkfifo` utility makes a named pipe, a FIFO special file, that unrelated processes can open.[^28] Neither carries typed values.

## Variations and related ideas

- **Named processes or named channels.** In Hoare's 1978 paper, an input or output command names the other process, not a channel. Hoare discussed ports connected by channels as an alternative and did not adopt it.[^1] Later languages took the other path. The Go FAQ says that Go's primitives come from the part of the CSP family whose main contribution is the channel as a first-class object.[^11]
- **Occam and Erlang.** The Go FAQ names occam and Erlang as two well-known languages that stem from CSP.[^11] Erlang uses asynchronous messages to processes, as the next item shows.
- **Actors and mailboxes.** In Erlang, all communication between processes uses asynchronous signals. The send operator `!` delivers a message to a process, and the receiver takes it from its own message queue with `receive`.[^26] The target of a send is a process, not a channel, and a send does not wait. This mailbox style is the actor style. With channels, the receiver is unknown to the sender, and one channel can have several receivers.
- **Newsqueak and Limbo.** The Go FAQ names both as CSP-inspired languages that shaped Go's concurrency.[^12]
- **Output guards.** Hoare allowed only input commands in guards. He discussed output guards as a more symmetric choice and left them out.[^1] Go's `select` allows both send and receive cases.[^8]
- **Fairness.** Hoare argued that a language should not require an implementation to be fair in its choice between ready guards.[^1] Go's `select` chooses uniformly at random among the ready cases and promises no other order.[^8] The Kotlin documentation states that its channels are fair: they serve coroutines in first-in, first-out order.[^22]
- **Channels of channels.** A channel is a value, so a request can carry its own reply channel. Effective Go shows a server that answers each request on the channel inside it.[^3]
- **Channels and higher-order functions.** A function that takes a channel and returns a fresh channel is a stage of a pipeline. Such stages compose like [[higher-order-functions]] on lists, but each stage runs in its own process.

## Pitfalls

### A send on a closed channel panics

Close ends the stream for receivers, but a sender that still runs can send after the close. In Go, that send panics.[^5] Here one of two producers closes the channel, and the other producer sends after the close.

```go fails expect=panic id=send-closed
package main

import "fmt"

func main() {
	ch := make(chan int)
	done := make(chan bool)
	go func() { // producer 1 closes, as if it were the only producer
		ch <- 1
		close(ch)
		done <- true
	}()
	fmt.Println(<-ch)
	<-done
	ch <- 2 // producer 2 (main) sends after the close
}
```

The program stops with `panic: send on closed channel`. The fix is a single owner of the close. With several producers, a `sync.WaitGroup` counts them, and one goroutine closes the channel after all of them have returned.

```go run id=send-closed-fix
package main

import (
	"fmt"
	"sync"
)

func main() {
	ch := make(chan int)
	var wg sync.WaitGroup
	for id := 1; id <= 3; id++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			ch <- id
		}()
	}
	go func() {
		wg.Wait()
		close(ch) // the only close, after the last send
	}()
	sum := 0
	for v := range ch {
		sum += v
	}
	fmt.Println("sum:", sum)
}
```

```output for=send-closed-fix
sum: 6
```

### A forgotten close leaves range blocked

A `for range` loop over a channel ends only when the channel is closed.[^9] If the producer returns without `close`, the consumer waits forever: a [[deadlock]].

```go fails expect=exit:2 id=no-close
package main

import "fmt"

func main() {
	ch := make(chan int)
	go func() {
		for i := 1; i <= 3; i++ {
			ch <- i
		}
	}()
	sum := 0
	for v := range ch {
		sum += v
	}
	fmt.Println(sum)
}
```

This program ends by itself, because the Go runtime finds that every goroutine is blocked. It prints `fatal error: all goroutines are asleep - deadlock!` and exits with status 2. The runtime finds the deadlock only when no goroutine at all can run. A server with other active goroutines hangs instead. The fix is `close(ch)` after the loop of the producer, or `defer close(ch)` at its start. In Rust, the same bug is a `Sender` that stays alive: a loop over `rx.iter()` blocks while any sender exists.[^14]

### A goroutine leak

A goroutine that blocks forever on a channel never ends, and its memory is never freed. A common cause is a receiver that gives up after a timeout. The worker then has no one to receive its result.

```go wrong id=leak
package main

import (
	"fmt"
	"runtime"
	"time"
)

func fetch() <-chan string {
	ch := make(chan string) // unbuffered: the send waits for a receiver
	go func() {
		time.Sleep(100 * time.Millisecond)
		ch <- "result"
	}()
	return ch
}

func main() {
	select {
	case r := <-fetch():
		fmt.Println(r)
	case <-time.After(10 * time.Millisecond):
		fmt.Println("timeout")
	}
	time.Sleep(200 * time.Millisecond) // the worker has tried to send by now
	fmt.Println("goroutines:", runtime.NumGoroutine())
}
```

```output for=leak
timeout
goroutines: 2
```

The worker still waits at `ch <- "result"`. The fix is a buffer of one, so the send always completes and the worker returns. Another fix is a cancel channel that the worker selects on.

```go run id=leak-fix
package main

import (
	"fmt"
	"runtime"
	"time"
)

func fetch() <-chan string {
	ch := make(chan string, 1) // room for the result, even if nobody receives it
	go func() {
		time.Sleep(100 * time.Millisecond)
		ch <- "result"
	}()
	return ch
}

func main() {
	select {
	case r := <-fetch():
		fmt.Println(r)
	case <-time.After(10 * time.Millisecond):
		fmt.Println("timeout")
	}
	time.Sleep(200 * time.Millisecond)
	fmt.Println("goroutines:", runtime.NumGoroutine())
}
```

```output for=leak-fix
timeout
goroutines: 1
```

### An unbuffered send with no receiver

On an unbuffered channel, a send waits until a receiver is ready.[^4] A send in the same goroutine that would later receive can never complete.

```go fails expect=exit:2 id=no-receiver
package main

import "fmt"

func main() {
	ch := make(chan int)
	ch <- 1 // waits for a receiver that never comes
	fmt.Println(<-ch)
}
```

The runtime reports `all goroutines are asleep - deadlock!`, as before. Two fixes exist. Start the receiver in another goroutine before the send, or give the channel a buffer that holds the value. Other runtimes have no such check. A Python `put` on a full queue or a Java `put` on a `SynchronousQueue` with no taker waits forever. A time limit turns the hang into an error that the program can handle.

```python run id=put-timeout
import queue

q = queue.Queue(maxsize=1)
q.put("first")
try:
    q.put("second", timeout=0.05)  # no consumer takes "first"
except queue.Full:
    print("queue full: no consumer")
```

```output for=put-timeout
queue full: no consumer
```

## Cost

- **A handover per value.** Each send and receive synchronizes the two sides and can wake a waiting process. A value that moves through an unbuffered channel makes the two processes meet each time. For a reference count, Effective Go says that a mutex around an integer variable may be the better choice.[^2]
- **Buffers trade memory for fewer waits.** A buffer lets the producer run ahead of the consumer, so fewer sends block. A bounded buffer of n values needs memory for n values. An unbounded channel can use memory without limit when the consumer is slower than the producer.[^13]
- **Copies.** In Hoare's model, the output command copies the value to the receiver.[^1] A channel of pointers copies only the pointer, and the two processes then share the object, so the sender must stop using it. Node.js copies the whole value with the structured clone algorithm, unless the value is in the transfer list, which moves an `ArrayBuffer` or a port to the receiver.[^21]
- **Threads or coroutines.** A Java `take` or a Python `get` blocks the calling thread while it waits (see [[threads]]). A Kotlin channel suspends the coroutine at `send` and `receive` instead of blocking,[^22] and a .NET channel makes the producer wait asynchronously.[^18]

## History

- **1978.** C. A. R. Hoare published "Communicating Sequential Processes" in Communications of the ACM. Input and output commands synchronize the two processes, and input guards select among ready communications.[^1]
- **Java 1.5.** Java 1.5 added `BlockingQueue` and `SynchronousQueue` to `java.util.concurrent`.[^16][^17]
- **.NET Core 3.0.** .NET Core 3.0 made `System.Threading.Channels` part of the shared framework; before that it was a NuGet package.[^18]
- **Python 3.13.** Python 3.13 added `Queue.shutdown` and the `ShutDown` exception, so a queue can be closed without a sentinel.[^20]

## References
