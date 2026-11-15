---
tag: S5M7JH
kind: pattern
slug: asynchronous-request-reply
name: Asynchronous Request-Reply
aliases: [Asynchronous Request-Reply pattern, async request-reply, HTTP polling, 202 Accepted pattern, long-running operation]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  For work that takes too long for one HTTP call, the API answers at once with 202 Accepted and the URL of a status endpoint; the client polls that URL until it points to the result.
family: integration
polarity: positive
intent: Separate the back end that does long work from the client that asks for it, by an answer of 202 Accepted with a status URL, a status endpoint that the client polls, and a redirect to the result when the work is done.
also_known_as: [async request-response]
see_also: [request-reply, polling-consumer, correlation-identifier, idempotent-receiver, claim-check, competing-consumers, retry-with-backoff, enterprise-integration-patterns]
sources:
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/asynchronous-request-reply
    title: Microsoft, Azure Architecture Center, Asynchronous Request-Reply pattern
    locator: context and problem; solution; problems and considerations; when to use this pattern; example
  - url: https://www.rfc-editor.org/rfc/rfc9110
    title: R. Fielding, M. Nottingham and J. Reschke (eds.), RFC 9110, HTTP Semantics, 2022
    locator: 15.3.3 202 Accepted; 15.4.3 302 Found; 15.4.4 303 See Other; 10.2.3 Retry-After
---

## Intent

Let a client start work that takes a long time over HTTP, and get the result later, without an open connection. The Azure page states the aim so: separate the back-end processing from the front-end host when the processing must run asynchronously, but the front end still needs a clear response.[^1]

## Problem

Most API calls over HTTP answer in about 100 ms or less, and the answer comes back on the same connection.[^1] Some work takes a few seconds, and some runs in the background for minutes or longer. The client cannot wait for the end of such work in one call.[^1]

A queue between the API and the worker lets each side scale alone, but then the client must also learn about the result in an asynchronous way.[^1] Callbacks are one answer. But it is often difficult to give a browser, or a client behind a firewall, a callback endpoint, and a long open connection adds complexity.[^1]

The forces are a quick first answer, a clear state for the client at each time, and a result that the client can get when it is ready.

## Solution

The client sends the request. The API checks it, puts the work on a queue and answers at once with 202 Accepted. The answer has a `Location` header with the URL of a status endpoint. The client polls that URL. While the work runs, the status endpoint answers 200 with the state of the work. When the work is done, it answers 303 See Other with the URL of the result, and the client gets the result there.[^1]

```text
  client                     API / status endpoint               worker
    | POST /reports -------------> |                                |
    | <---- 202, Location: /status/r1, Retry-After: 2               |
    |                              | --- message on a queue ------> |
    | GET /status/r1 ------------> |                                |
    | <---- 200, status=Running    |                                |
    |            ...               | <--- result in a store ------- |
    | GET /status/r1 ------------> |                                |
    | <---- 303, Location: /reports/r1                              |
    | GET /reports/r1 -----------> |                                |
    | <---- 200, the result        |                                |
```

The participants are the client, the API that accepts the work, the queue, the worker, the store for the result, and the status endpoint.[^1]

**The status codes.** RFC 9110 says that 202 means that the request "has been accepted for processing, but the processing has not been completed".[^2] HTTP has no way to send a status code later from an asynchronous operation. Thus the 202 answer ought to describe the current status and point to a status monitor.[^2] A 303 sends the client to another resource, which a client gets with GET or HEAD.[^2] The Azure page prefers 303 to 302. With a 302, the change of the method is not guaranteed, and some clients send the original method again, which can give a second POST.[^1] RFC 9110 says that a user agent may change a POST to a GET after a 302 for historical reasons.[^2]

**How often to poll.** The 202 answer can have a `Retry-After` header with an estimate of the time to wait.[^1] RFC 9110 gives the header a meaning for a 503 and for a redirect; its value is a date or a number of seconds.[^2] The program below compares three ways to poll for jobs of 3, 12 and 40 seconds. The client does not know the time of the job.

```python run id=poll-cost
# A job ends after D seconds; the client does not know D. Each strategy
# polls the status endpoint and stops at the first poll after the end.


def fixed(step):
    def gen():
        t = 0
        while True:
            t += step
            yield t
    return gen


def backoff(first, cap):
    def gen():
        t, step = 0, first
        while True:
            t += step
            yield t
            step = min(step * 2, cap)
    return gen


strategies = [("every 1 s", fixed(1)), ("Retry-After: 5", fixed(5)),
              ("backoff 1, 2, 4 ... 16 s", backoff(1, 16))]
print("%-25s %-16s %-16s %s" % ("", "job of 3 s", "job of 12 s", "job of 40 s"))
for name, gen in strategies:
    cells = []
    for d in (3, 12, 40):
        polls = 0
        for t in gen():
            polls += 1
            if t >= d:
                break
        cells.append("%2d polls, +%2d s" % (polls, t - d))
    print("%-25s %s" % (name, "  ".join(cells)))
```

```output
                          job of 3 s       job of 12 s      job of 40 s
every 1 s                  3 polls, + 0 s  12 polls, + 0 s  40 polls, + 0 s
Retry-After: 5             1 polls, + 2 s   3 polls, + 3 s   8 polls, + 0 s
backoff 1, 2, 4 ... 16 s   2 polls, + 0 s   4 polls, + 3 s   6 polls, + 7 s
```

A poll each second sees the end quickly, but it sends one request for each second of the job. A fixed wait of 5 seconds sends few requests, but it can see the end up to 5 seconds late. A wait that doubles up to 16 seconds sends the fewest requests for long jobs, but the client can see the end of a long job much later. `Retry-After` lets the server, which can estimate the time of the job, set the interval.

## Example

Each example file runs an API in the same process, with no network. The client sends a POST with an idempotency key and gets 202 with a status URL. It sends the POST again with the same key, as after a lost answer, and gets the same status URL, and no second job starts. The Azure page suggests this key, because the client cannot tell a lost answer from a lost request.[^1] A request with no month gets 400 at once, because the API checks the request before it starts the work.[^1] The client then polls at the interval of `Retry-After` and follows the 303. An unknown request id gets 404.

## Consequences

**Benefits.**

- **A quick answer.** The client gets an answer in a short time, also for long work.[^1]
- **Independent scale.** The API and the worker scale separately, and the server can schedule the work when it has capacity.[^1]
- **Only HTTP on the client.** The client needs no callback endpoint and no open connection.[^1]

**Liabilities.**

- **More requests.** Each poll is a request to the server. `Retry-After` helps a client not to poll too often, but a client written by others can ignore it.[^1]
- **State on the server.** The status resource and the result use storage. The Azure page asks for a retention policy, and an `Expires` header can tell the client how long the result stays.[^1]
- **Errors later.** An error in the work must be kept at the URL of the `Location` header, with a 4xx status code that matches the failure. The Azure page suggests RFC 9457 problem details for this.[^1]
- **No single standard.** Implementations differ. Some have no status endpoint, and the client polls the result URL and gets 404 until the result exists, which is unclear, because an invalid id also gives 404.[^1]

## When not to use it

The Azure page names these cases.[^1]

- A service for asynchronous notifications, such as Azure Event Grid, is available.
- The answers must stream to the client in real time. Server-Sent Events give a push channel from the server.
- The client must collect many results, and their latency is important. A message broker is better.
- The server can keep a connection open, as with WebSockets or SignalR, and notify the caller.
- The network allows a callback or a webhook to the client.

## Variants and related patterns

**Long polling.** The client sends a request, and the server keeps the connection open until new data is available or a timeout occurs. This gives a shorter delay than periodic polling, but the server must manage the open connections and timeouts.[^1] The Azure sample has such a mode: the status endpoint waits with a delay that doubles each time, for up to about one minute.[^1]

**Cancel.** A DELETE on the status resource can cancel the work. The back end then sets the state to canceled.[^1]

**A facade for old clients.** A client that cannot handle 202 can call a facade that hides the asynchronous work. The Azure page names Logic Apps as such a layer.[^1]

**Over messaging.** With queues in place of HTTP, the same idea is a [[request-reply]] with a reply channel and a [[correlation-identifier]]. The request id in the status URL has the role of the correlation identifier.

## Known uses

- Azure: the sample of the Azure page uses Azure Functions for the API, the status endpoint and the worker, a Service Bus queue between them, and Blob Storage for the result. When the result exists, the status endpoint answers 303 with a URL that has a shared access signature.[^1]
- Azure Resource Manager uses a variant of the pattern for its asynchronous operations, and Logic Apps supports the pattern natively.[^1]

## References
