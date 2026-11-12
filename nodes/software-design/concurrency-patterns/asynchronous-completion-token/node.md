---
tag: 3FNX6W
kind: pattern
slug: asynchronous-completion-token
name: Asynchronous Completion Token
aliases: [ACT, completion token, active demultiplexing]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  An asynchronous completion token is a value that a client sends with each asynchronous request and that the service returns unchanged, so the client can match each reply to its request.
family: concurrency
polarity: positive
intent: Send a token with each asynchronous operation, and use the token that comes back with the completion to find the state and the action for that operation.
also_known_as: [Active Demultiplexing]
history:
  year: 2000
  people: [Douglas C. Schmidt]
  note: The pattern is one of the event handling patterns of POSA2, which Wiley published in 2000.
see_also: [proactor, reactor, memento-pattern, futures, callback-pattern, async-await, command-pattern, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/ACT.pdf
    title: Douglas C. Schmidt, Asynchronous Completion Token (POSA2 chapter draft)
    locator: Intent; Also Known As; Solution; Structure; Known Uses; Consequences; See Also
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: introduction; Table of Contents, 3 Event Handling Patterns
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: concurrent.futures, Launching parallel tasks, Python documentation
    locator: ThreadPoolExecutor Example; as_completed
  - url: https://man7.org/linux/man-pages/man7/io_uring.7.html
    title: io_uring(7), Linux manual page
    locator: DESCRIPTION, user_data
---

## Intent

The Asynchronous Completion Token design pattern efficiently dispatches processing actions within a client in response to the completion of asynchronous operations invoked by the client.[^1] It is also known as Active Demultiplexing.[^1]

## Problem

When a client invokes an operation request asynchronously on one or more services, each service indicates its completion by sending a response back to the client.[^1] The replies can come in any order. The client must know, for each reply, which request it answers and what to do next. If the client parses the reply or searches a list to find that out, each completion costs time, and the client code grows.

## Solution

For every asynchronous operation that a client invokes on a service, create an asynchronous completion token (ACT) that uniquely identifies the actions and state necessary to process the operation's completion, and pass the ACT along with the operation to the service.[^1] When the service replies to the client, its response must include the ACT that was sent originally.[^1]

The participants are a service, a client, the token and the completion handler.[^1] For a client, an ACT can be an index into a table or a direct pointer to memory. To the service, however, the ACT is simply an opaque value that it returns unchanged.[^1]

```text
client                          service
  token = new id; table[token] = (state, action)
  request(data, token) ------->
                                 ... work, in any order ...
  <------------------------------ reply(result, token)
  (state, action) = table.pop(token)
  action(state, result)
```

## Example

The example files show one program in each language. The client sends two load requests, each with a numeric token, and keeps a table from token to image name. The service answers in the reverse order. The client finds each image by its token, not by the order of the replies. The program prints:

```text
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

## Consequences

Benefits:[^1]

- **Simpler client data structures.** The ACT returned by the service contains all the information needed to demultiplex to the appropriate client completion action.[^1]
- **Efficient state acquisition.** ACTs can be used as indices or pointers to operation state for highly efficient access, thereby eliminating costly table searches.[^1]
- **Space efficiency.** In C and C++, ACTs that are four byte void pointers can reference arbitrarily large objects.[^1]
- **No fixed concurrency policy.** Clients can be single-threaded or multithreaded, depending on application requirements.[^1]

Liabilities:[^1]

- **Memory leaks.** They can result if clients use ACTs as pointers to dynamically allocated memory and services fail to return the ACTs.[^1]
- **Re-mapping.** If ACTs are used as direct pointers to memory, errors can occur if part of the application is re-mapped in virtual memory.[^1]
- **Authentication.** The client may need to authenticate the ACT before using it, if the server cannot be trusted to have treated the ACT opaquely.[^1]

Without a token, a client often matches replies to requests by their order. That works only while the service keeps the order:

```python wrong id=by-order
pending = ["image-7", "image-9"]  # names in the order of the requests
replies = ["data of image-9", "data of image-7"]  # the service answers out of order

for reply in replies:
    name = pending.pop(0)  # assumes the first reply answers the first request
    print(name, "<-", reply)
```

```output for=by-order
image-7 <- data of image-9
image-9 <- data of image-7
```

## When not to use it

- The calls are synchronous. The caller then waits for each reply, and the call stack already holds the state.
- The service keeps a strict order, and the protocol says so. A counter of replies is then enough.
- The language gives a future or a promise for each call. The future is then the token, and the program does not need its own table.

## Variants and related patterns

- **Index or pointer.** The token can be an index into a table of pending operations, or a pointer to the state.[^1] An index can be checked; a pointer is faster but can dangle.
- **Memento.** The Asynchronous Completion Token and Memento patterns are similar with respect to the participants: in both, one object holds a value that it treats as opaque.[^1]
- **Proactor.** The completion handlers of a Proactor often use tokens. POSA2 puts both in its event handling patterns.[^2]
- **Futures as tokens.** In Python, `as_completed` yields futures as they complete.[^3] The documentation example marks each future with its URL in a dictionary, and the future is the key that finds the state again:

```python run id=futures
import concurrent.futures
import time


def load(name, delay):
    time.sleep(delay)
    return f"{name} loaded"


jobs = {"image-7": 0.2, "image-9": 0.0}  # image-9 finishes first
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
    future_to_name = {pool.submit(load, n, d): n for n, d in jobs.items()}
    for future in concurrent.futures.as_completed(future_to_name):
        name = future_to_name[future]  # the future is the token
        print(name, "->", future.result())
```

```output for=futures
image-9 -> image-9 loaded
image-7 -> image-7 loaded
```

## Known uses

- **Operating system asynchronous I/O.** The pattern is used by most operating systems that support asynchronous I/O, such as Windows NT I/O completion ports and the POSIX Asynchronous I/O API.[^1]
- **Linux io_uring.** The `user_data` field of a request is custom data that is passed unchanged from submission to completion.[^4]
- **CORBA.** The TAO CORBA Object Request Broker uses the pattern to demultiplex various types of requests and responses.[^1]
- **FedEx.** The sender of a package uses the internal billing reference field of the Airbill as an ACT, and FedEx returns it with the invoice.[^1]

## References
