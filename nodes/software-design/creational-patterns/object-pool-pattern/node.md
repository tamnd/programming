---
tag: CAQSKE
kind: pattern
slug: object-pool-pattern
name: Object pool
aliases: [Object Pool, object pool pattern, resource pool, free list, pooling]
area: software-design/creational-patterns
level: core
summary: >-
  A pool keeps a set of ready objects. A client acquires one, uses it and releases it, and the pool resets it for the next client, so a costly object is not created again.
family: creational-design
polarity: positive
intent: "Keep a set of initialized objects ready for use, so that a client acquires an object from the set and returns it after use, in place of creating and destroying a costly object each time."
also_known_as: [Resource Pool]
history:
  year: 2004
  people: [Michael Kircher, Prashant Jain]
  note: Kircher and Jain described the general form as the Pooling pattern in Pattern-Oriented Software Architecture, Volume 3 (2004), a book of patterns for resource management.
see_also: [flyweight-pattern, singleton-pattern, lazy-initialization, resource-acquisition-is-initialization]
sources:
  - url: https://en.wikipedia.org/wiki/Object_pool_pattern
    title: Object pool pattern, Wikipedia
    locator: Introduction; Description; Implementation; Handling of empty pools; Pitfalls
  - url: https://learn.microsoft.com/en-us/dotnet/framework/data/adonet/sql-server-connection-pooling
    title: SQL Server Connection Pooling (ADO.NET), Microsoft Learn
    locator: Introduction
  - url: https://learn.microsoft.com/en-us/aspnet/core/performance/objectpool?view=aspnetcore-9.0
    title: Object reuse with ObjectPool in ASP.NET Core, Microsoft Learn
    locator: Introduction; the note on limits; ObjectPool concepts
  - url: https://go.dev/doc/effective_go#leaky_buffer
    title: Effective Go
    locator: A leaky buffer
  - url: https://github.com/golang/go/blob/go1.25.0/src/fmt/print.go#L146-L182
    title: Go go1.25.0 source, src/fmt/print.go
    locator: ppFree, newPrinter and free, lines 146 to 182
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.buffers.arraypool-1.rent?view=net-9.0
    title: ArrayPool<T>.Rent(Int32) Method, .NET API reference
    locator: Remarks
  - url: https://pkg.go.dev/sync#Pool
    title: Package sync, the Go standard library
    locator: type Pool
  - book: isbn:9780470845257
    title: Michael Kircher and Prashant Jain, Pattern-Oriented Software Architecture, Volume 3, Patterns for Resource Management, Wiley, 2004
    locator: Chapter 3, Resource Lifecycle, Pooling
  - url: https://commons.apache.org/proper/commons-pool/
    title: Apache Commons Pool
    locator: The Commons Pool
---

## Intent

Keep a set of initialized objects ready for use. A client acquires an object from the set, uses it, and returns it. The object is not destroyed, and the next client gets it in place of a new one.[^1]

## Problem

A server opens a connection to a database for each request. To open a connection, the program must set up a socket, do a handshake, parse the connection string and authenticate. Each step takes time.[^2] A server that handles many short requests spends more time to open and close connections than to use them.

The same problem comes with other objects that are costly to create: threads, large buffers, and objects that hold a native resource. The forces are these:

- Creation is costly, and the program needs the object often but for a short time.
- The program uses only a few objects at the same time, so a small set can serve all requests.
- After use, the object can be put back into a known state, so that the next user can use it as if it were new.

## Solution

Put a **pool** between the clients and the creation of the objects. The pool keeps a set of idle objects. **Acquire** takes an idle object, or creates one when the set is empty. **Release** resets the object and puts it back.[^1]

```text
 Client                    Pool                          Reusable
 ------                    ---------------------------   --------
 b = pool.acquire() -----> idle empty? create() ------->  new Buffer
                           else pop an idle object
 use b
 pool.release(b)  -------> reset(b)
                           idle below max_idle? keep b
                           else drop b
```

The participants are the **pool**, the **reusable** object and the **client**. The pool also holds two policies:

- **Sizing.** A pool can limit the objects it keeps idle, the objects it creates in total, or both. The ASP.NET Core `ObjectPool` limits only the objects that it retains, not the objects that it allocates.[^3]
- **Empty pool.** When no object is idle, a pool can fail and return an error, create a new object, or, in a program with threads, block the client until another thread returns an object.[^1]

Each pool must make release reliable. In a language with destructors, a smart pointer can return the object when it goes out of scope. In a language with garbage collection, the client must release the object in a `finally` block, a `with` block, or a `using` statement. A finalizer is a poor choice, because there is usually no guarantee when it runs, or whether it runs at all.[^1]

## Example

The example files show one pool in each language. The pool keeps at most two idle buffers. Three requests run one after another and share one buffer. Then three requests run at the same time, so the pool creates two more buffers. After the release, the pool keeps two buffers and drops the third. Each example releases the buffer with the scoped form of its language:

| Language | Release | Thread safety |
|---|---|---|
| Python | a `contextmanager` and a `with` block | `threading.Lock` |
| JavaScript | a callback in `try` and `finally` | one thread, so no lock |
| TypeScript | a generic `Pool<T>` with a callback | one thread, so no lock |
| Java | `try` and `finally` | `synchronized` methods |
| C# | a lease that implements `IDisposable`, in a `using` statement | `lock` |
| C++ | `std::unique_ptr` with a custom deleter (RAII) | `std::mutex` |
| Go | `defer`; idle items in a buffered channel[^4] | the channel |
| Rust | a guard type that implements `Drop` | `Mutex` |

## Consequences

The main benefit is speed when creation is costly. The pool pays the cost once for each object, not once for each use. A pool also makes the time to get an object predictable, because an idle object is ready at once.[^1] A pool with a maximum size limits the resources that a program holds, for example the number of connections that a database must accept.

The costs are in the lifetime of the objects. An object from a pool is not new, and an object that a client releases is not gone.

**Stale state.** The pool must reset each object before the next client gets it. If it does not, one client sees the data of another.[^1] Here the pool has no reset, and the second request gets the token of the first:

```python wrong id=no-reset
class Buffer:
    def __init__(self):
        self.parts = []


idle = []


def acquire():
    return idle.pop() if idle else Buffer()


def release(buffer):
    idle.append(buffer)  # no reset


b = acquire()
b.parts.append("token=alice-secret")
release(b)

b = acquire()
b.parts.append("page=2")
print(" ".join(b.parts))
```

```output for=no-reset
token=alice-secret page=2
```

The fix is one line in `release`: clear the parts before the buffer goes back. A reset must cover every field. The Go `fmt` package clears its argument fields and its buffer before each printer goes back to its pool.[^5]

**Leaks.** A client that does not release an object takes it from the pool for ever. A pool that creates on demand then grows, and a bounded pool runs out and blocks or fails. The .NET documentation of `ArrayPool<T>.Rent` says that a buffer that is not returned is not a fatal error, but it can reduce performance, because the pool may create a new buffer to replace it.[^6] The same page warns that a rented array may not be zero-initialized, which is the stale-state problem again.[^6]

**Use after release.** A client that keeps a reference after it releases the object can change the object while another client uses it. Most languages cannot stop this. A Rust guard type can, because the guard owns the object.

**Memory.** Idle objects stay in memory. The ASP.NET Core documentation says that objects managed by a pool are not de-allocated until the pool is de-allocated.[^3]

**Variable size.** A pool of buffers can keep a very large buffer that one request needed once. The Go `fmt` package drops a buffer larger than 64 KiB in place of putting it back, so that the entries of its pool keep about the same memory cost.[^5]

## When not to use it

Do not pool objects that are cheap to create. The ASP.NET Core documentation says that object pooling does not always improve performance: unless the initialization cost of an object is high, it is usually slower to get the object from the pool. It advises to use object pooling only after collecting performance data from realistic scenarios.[^3] A modern garbage collector allocates a small object fast, and a pool of such objects only adds a lock and the risk of stale state.

Do not pool an object that a client cannot reset. If the state of the object depends on its past use in ways the pool cannot see, a new object is safer.

Do not write a pool when the platform has one. Database drivers pool connections, and ADO.NET does it behind `Open` and `Close`.[^2] Most standard libraries have a thread pool.

## Variants and related patterns

**Connection pool.** The most common pool. ADO.NET keeps connections in a pool, and a call to `Close` returns the connection to the pool in place of closing the link to the server.[^2] A connection pool usually has a maximum size and blocks or fails when it is full.

**Thread pool.** A pool of worker threads that take tasks from a queue. The thread is the costly object, and the task is the use.

**Temporary-object pool.** Go's `sync.Pool` caches allocated but unused items to reduce the pressure on the garbage collector. Any item in it may be removed at any time without notification.[^7] It suits temporary buffers, and not objects that the program must keep. The Go documentation names the `fmt` package as a good use: a store of output buffers that grows under load and shrinks when the program is quiet.[^7]

**Leaky buffer.** Effective Go builds a free list from a buffered channel. A client takes a buffer from the channel if one is there, or allocates a new one. After use, it puts the buffer back if there is room, or drops it.[^4] The Go example on this page uses this form.

**Array pool.** .NET's `ArrayPool<T>` lends arrays of at least a requested length, and the caller should return each array to the same pool.[^6]

**Pooling (POSA3).** Kircher and Jain describe pooling as a general resource-management pattern, with a family of related patterns for the life of a resource: lazy acquisition, eager acquisition, caching and evictor.[^8]

**Flyweight.** A [[flyweight-pattern]] also shares objects, but its objects are immutable and many clients use one object at the same time. A pooled object is mutable and has one client at a time.

**Lazy initialization.** A pool that creates objects only on demand applies [[lazy-initialization]] to each object. A pool that creates all its objects at the start trades a slower start for a fast first request.

## Known uses

- Go: [fmt/print.go at go1.25.0](https://github.com/golang/go/blob/go1.25.0/src/fmt/print.go#L146-L182) keeps its printer state in the `sync.Pool` named `ppFree`. `newPrinter` gets a printer from the pool, and `free` clears its fields, drops a buffer larger than 64 KiB, and puts the printer back.[^5]
- Apache Commons Pool is a Java library that gives an object-pooling API and several pool implementations.[^9]

## References
