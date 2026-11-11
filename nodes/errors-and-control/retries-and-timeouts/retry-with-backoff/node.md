---
tag: NEW-retry-with-backoff
kind: concept
slug: retry-with-backoff
name: Retry with backoff
aliases: [exponential backoff, retry, backoff, jitter, retry policy, retry budget]
area: errors-and-control/retries-and-timeouts
level: core
summary: >-
  A retry calls a failed operation again, in case the failure was temporary. Backoff makes each wait longer, and jitter makes it random, so that retries do not overload the service that failed.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1976
  people: [Robert Metcalfe, David Boggs]
  note: The Ethernet paper described controllers that, after each collision, wait for a random interval with a mean twice that of the previous interval. The authors called the algorithm Binary Exponential Backoff.
see_also: [errors-as-values, exception-handling, panics, async-await, uncontrolled-resource-consumption, generate-random-numbers, structured-logging, test-double]
same_as:
  wikidata: Q1417920
  wikipedia: Exponential_backoff
sources:
  - url: https://web.stanford.edu/class/cs240/old/sp2014/readings/ethernet.pdf
    title: Robert M. Metcalfe and David R. Boggs, Ethernet, Distributed Packet Switching for Local Computer Networks, Communications of the ACM 19(7), July 1976, pages 395 to 404
    locator: page 400, the section on the controller (Binary Exponential Backoff)
  - url: https://aws.amazon.com/blogs/architecture/exponential-backoff-and-jitter/
    title: Marc Brooker, Exponential Backoff And Jitter, AWS Architecture Blog, 4 March 2015
    locator: the sections on Full Jitter, Equal Jitter and Decorrelated Jitter, and the closing paragraphs
  - url: https://builder.aws.com/content/3EumjoZascWd1oZiEgL8ORlv3qE/timeouts-retries-and-backoff-with-jitter
    title: Marc Brooker, Timeouts, retries, and backoff with jitter, Amazon Builders' Library, 2019
    locator: the sections Timeouts, Retries and backoff, and Jitter
  - url: https://sre.google/sre-book/addressing-cascading-failures/
    title: Mike Ulrich, Addressing Cascading Failures, chapter 22 of Site Reliability Engineering, O'Reilly, 2016
    locator: the sections Retries, and Latency and Deadlines (Picking a deadline, Deadline propagation)
  - url: https://www.rfc-editor.org/rfc/rfc9110
    title: RFC 9110, HTTP Semantics, June 2022
    locator: section 9.2.2 Idempotent Methods; section 10.2.3 Retry-After
  - url: https://www.rfc-editor.org/rfc/rfc6298
    title: RFC 6298, Computing TCP's Retransmission Timer, June 2011
    locator: section 2.5 (the maximum value of RTO); section 5, step 5.5 (back off the timer)
  - url: https://docs.stripe.com/api/idempotent_requests
    title: Stripe API Reference, Idempotent requests
    locator: the whole page
  - url: https://pkg.go.dev/context#WithDeadline
    title: Go standard library, package context
    locator: func WithDeadline
  - url: https://grpc.io/docs/guides/retry/
    title: gRPC documentation, Retry
    locator: the sections on the retry policy and on throttling
  - url: https://martinfowler.com/bliki/CircuitBreaker.html
    title: Martin Fowler, CircuitBreaker, 6 March 2014
    locator: the first paragraphs
  - url: https://man.freebsd.org/cgi/man.cgi?query=sysexits&sektion=3
    title: FreeBSD Library Functions Manual, sysexits(3)
    locator: EX_USAGE and EX_TEMPFAIL
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/errno.h.html
    title: The Open Group Base Specifications Issue 8, errno.h
    locator: the list of symbolic names (EAGAIN, EINTR, EINVAL)
  - url: https://timsong-cpp.github.io/cppwp/n4950/rand.dist.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[rand.dist.general], paragraph 3"
  - url: https://www.rfc-editor.org/rfc/rfc6585#section-4
    title: RFC 6585, Additional HTTP Status Codes, April 2012
    locator: section 4, 429 Too Many Requests
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/invoke-restmethod
    title: PowerShell documentation, Invoke-RestMethod
    locator: the parameters MaximumRetryCount and RetryIntervalSec
  - url: https://www.postgresql.org/docs/current/transaction-iso.html
    title: PostgreSQL documentation, Transaction Isolation
    locator: 13.2.2 Repeatable Read Isolation Level; 13.2.3 Serializable Isolation Level
---

## Idea

Some failures go away by themselves. A server restarts, a network link drops a packet, a database lock is released, a rate limit resets. If the program calls the same operation again a little later, the call can succeed. A retry is this second call.

A retry is easy to write and easy to get wrong. If every client retries at once, the retries arrive as one more burst of load on a service that is already in trouble. Marc Brooker calls retries "selfish": a client that retries takes more of the capacity of the server to make its own success more likely.[^3] Two techniques keep retries safe for the service. Backoff makes each wait longer than the one before. Jitter makes each wait random, so that clients that failed at the same moment do not retry at the same moment.

This Python program prints the limits of a capped exponential backoff, and one random wait below each limit. The wait before attempt n+1 is a random number between 0 and `min(cap, base * 2**n)`.

```python run id=idea
import random

base, cap = 100, 1000  # milliseconds
rng = random.Random(7)
for attempt in range(6):
    limit = min(cap, base * 2**attempt)
    print(f"retry {attempt + 1}: limit {limit:4} ms, wait {rng.randint(0, limit):4} ms")
```

```output
retry 1: limit  100 ms, wait   41 ms
retry 2: limit  200 ms, wait   38 ms
retry 3: limit  400 ms, wait  202 ms
retry 4: limit  800 ms, wait  666 ms
retry 5: limit 1000 ms, wait   49 ms
retry 6: limit 1000 ms, wait   74 ms
```

A retry policy answers five questions. Which failures are worth a retry? How many attempts are allowed? How long is the wait before each attempt? What is the total time limit? Is the operation safe to do twice? The sections below take these in turn.

## How it works

### Which failures to retry

A retry helps only when the next attempt can give a different result. The error must say which kind of failure happened, so the caller can decide ([[errors-as-values]], [[exception-handling]]).

| Worth a retry | Not worth a retry |
|---|---|
| a timeout, or a connection reset before the response | a request that is not valid, such as HTTP 400 |
| HTTP 503 Service Unavailable, HTTP 429 Too Many Requests | a missing resource, HTTP 404 |
| `EAGAIN` and `EINTR` from a system call | `EINVAL`, wrong arguments |
| exit status 75, `EX_TEMPFAIL` | exit status 64, `EX_USAGE` |
| a serialization failure of a database transaction, SQLSTATE 40001 | a constraint violation |

The C example retries on `EAGAIN` ("Resource unavailable, try again") and `EINTR` ("Interrupted function"), and stops on `EINVAL` ("Invalid argument").[^12] The Bash example uses the exit codes of `sysexits.h`, where `EX_TEMPFAIL` marks a temporary failure and `EX_USAGE` marks a command that was called wrongly.[^11] The TypeScript example puts the kinds of outcome in a union type, so the compiler checks that the policy handles each one ([[sum-types]]).

A database also gives errors that are worth a retry. PostgreSQL says that applications at the Repeatable Read and Serializable isolation levels "must be prepared to retry transactions due to serialization failures", and that such failures always return the SQLSTATE `40001`.[^16]

### How long to wait

Let `base` be the first wait, `cap` the longest wait, and `n` the number of attempts that have failed. Brooker's article compares four schedules.[^2]

- **Exponential backoff.** Wait `min(cap, base * 2**n)`. The waits double until they reach the cap. All clients that failed at the same moment wait the same time, so they collide again.
- **Full jitter.** Wait a random time between 0 and `min(cap, base * 2**n)`. The Python, Go, Rust and Bash examples use this schedule.
- **Equal jitter.** Let `v = min(cap, base * 2**n)`. Wait `v/2` plus a random time between 0 and `v/2`. No wait is very short. The C example uses this schedule.
- **Decorrelated jitter.** Wait `min(cap, random(base, 3 * previous_wait))`. Each wait depends on the one before it, not on the attempt number. The C++ example uses this schedule.

In Brooker's simulation of 100 clients that contend for one resource, jitter "reduced our call count by more than half". Of the jittered schedules, Brooker calls equal jitter "the loser": it does slightly more work than full jitter and takes much longer. Decorrelated jitter makes more calls than full jitter, and full jitter takes a little more time.[^2] Brooker recommends jittered backoff as "a standard approach for remote clients".[^2]

The cap keeps the waits finite. Without a cap, the waits grow without limit. The Rust example shows that after 20 failed attempts, a base of 100 ms gives a wait of more than 29 hours.

A server can say how long to wait. The HTTP header `Retry-After` carries either a date or a number of seconds. With a 503 response, it says how long the service is expected to be unavailable.[^5] A 429 response can include it too.[^14] The JavaScript example waits for the longer of the hint and its own backoff.

### How long to try

A retry loop needs two limits: a number of attempts and a total time. The number of attempts stops a loop that fails fast. The total time stops a loop that fails slowly, where each attempt waits for a long timeout.

The total time usually comes from the caller. A user waits a few seconds for a page, and work after that is wasted. The Google SRE book says that servers "should employ deadline propagation": the deadline of the incoming request becomes the limit of each call that the server makes for it.[^4] In Go, `context.WithDeadline` gives a child context whose deadline is "no later than" the one asked for. If the parent's deadline is earlier, the child keeps the deadline of the parent.[^8] The Go example shows a call that asks for an hour and stops after the 20 ms of its request. The Python example stops when the next wait would pass its deadline.

The timeout of each attempt is a separate choice. Brooker suggests that a team picks a rate of false timeouts that it accepts, such as 0.1%, and sets the timeout from the matching latency percentile of the service that it calls, here p99.9.[^3]

### Is it safe to do twice

A failed call does not prove that nothing happened. When a connection breaks after the server has done the work but before the response arrives, the client sees a failure. A retry then does the work a second time. The Go example `double-charge` shows a payment that a retry charges twice.

An operation is idempotent when two identical calls have the same effect as one. HTTP defines `PUT`, `DELETE` and the safe methods such as `GET` as idempotent.[^5] Brooker writes that "APIs with side effects aren't safe to retry unless they provide idempotency".[^3]

An idempotency key makes an operation safe to retry. The client makes a unique key for each logical operation and sends the same key with every attempt. Stripe saves the status code and the body of the first request with a given key, and it returns the saved response to a retry with that key, even a `500` error. Stripe compares the parameters of the retry with those of the first request and returns an error if they differ. Keys can be removed after they are at least 24 hours old.[^7] The Java example keeps such a table of keys on the server, and the charge happens once although the server receives it three times.

## In languages

No tier 1 language has a general retry function in its standard library. The parts come from the standard library: a way to sleep, a random number generator, a clock, and a way to tell the kinds of error apart. The example files build the loop from those parts. Each one keeps time in a counter in place of a real sleep and seeds its generator, so the output is the same on each run.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | a loop with `random` and `time.sleep`; exception classes say which errors are transient |
| JavaScript | stdlib | a loop around an `async` call; `Retry-After` from the response |
| TypeScript | stdlib | as JavaScript; a union type of outcomes with an exhaustive `switch` |
| Java | stdlib | a loop with `java.util.Random`; an idempotency key in the example |
| C | absent | a loop that reads `errno`; equal jitter |
| C++ | stdlib | `<random>` and `<chrono>`; decorrelated jitter |
| C# | stdlib | a loop with a token bucket as a retry budget |
| Go | stdlib | a loop with `errors.Is`; `context` carries the deadline |
| Rust | stdlib | a loop with `match` on the error kind; no random generator in `std` |
| Kotlin | idiom | the same loop as Java, or `delay` in a coroutine |
| PowerShell | native | `Invoke-RestMethod -MaximumRetryCount -RetryIntervalSec`, with a fixed interval |
| Bash | idiom | a loop on the exit status `$?`, with `sleep` |
| SQL | absent | the client retries a transaction that fails with SQLSTATE 40001 |
| Regex | n/a | a pattern does not call anything that can fail |
| HTML/CSS | n/a | markup and style rules do not call anything that can fail |

Libraries and frameworks supply retry policies on top of these parts. gRPC reads a retry policy from the service config, with the fields `maxAttempts`, `initialBackoff`, `maxBackoff`, `backoffMultiplier` and `retryableStatusCodes`. It applies a jitter of plus or minus 20% to each wait.[^9] PowerShell's `Invoke-RestMethod` has a retry built in. It retries on "a failure code between 400 and 599, inclusive or 304", at the fixed interval of `-RetryIntervalSec`. For a 429 response with a `Retry-After` header, it waits for the time in the header.[^15] Because the interval is fixed and the range includes the 4xx codes, it also retries requests that cannot succeed, and many clients that fail together retry together.

Some notes on the examples:

- **C++.** The engine `std::mt19937` gives the same sequence on every implementation, but "the algorithms for producing each of the specified distributions are implementation-defined".[^13] The output of `uniform_int_distribution` can differ between standard libraries.
- **Rust.** The standard library has no random number generator, so the example uses a xorshift generator of a few lines.
- **JavaScript.** `Math.random` cannot take a seed, so the example uses a small linear congruential generator.

## Variations and related ideas

- **Retry budgets.** A budget limits the retries of a whole process, not of one request. The SRE book suggests a server-wide budget, "for example, only allow 60 retries per minute in a process".[^4] Brooker describes "limiting retries locally using a token bucket", which AWS added to the AWS SDK in 2016.[^3] gRPC throttles retries per server with a token count that starts at `maxTokens`. Each failure takes away 1, each success adds `tokenRatio`, and retries stop while the count is below half of `maxTokens`.[^9] The C# example shows a budget that cuts the calls to a failing dependency from 40 to 18.
- **Circuit breakers.** A circuit breaker stops the calls for a while after many failures. Martin Fowler credits Michael Nygard's book Release It! with making the pattern popular. When the breaker trips, "all further calls to the circuit breaker return with an error, without the protected call being made at all".[^10] A retry policy reacts to one failure, and a circuit breaker reacts to a pattern of failures.
- **Hedged requests.** A client sends a second copy of a slow request before the first one fails, and uses the first response that arrives. This trades extra load for lower tail latency, and it needs an idempotent operation as a retry does.
- **TCP retransmission.** TCP backs off its retransmission timer. When the timer expires, "The host MUST set RTO <- RTO * 2". A maximum may be placed on the timer "provided it is at least 60 seconds".[^6]
- **Jitter for timers.** Brooker suggests adding jitter to "all timers, periodic jobs, and other delayed work", for example a job that many servers run from cron at the same minute.[^3]
- **Polling.** A loop that polls for a result, such as the status of a job, uses the same schedule: start often and slow down while nothing changes.
- **Retries in tests.** A test that retries until it passes hides a flaky test. A [[test-double]] that fails a set number of times, such as the `service` function of the Go and Python examples, tests the retry loop without a network.

## Pitfalls

### No jitter

Clients that fail together retry together. The Python example `no-jitter` shows 100 clients with a plain exponential backoff. Every retry round arrives as one burst of 100 calls, at 0, 100, 300 and 700 ms. Full jitter spreads each round over the whole interval.

### No cap

The waits double without limit. The Rust example `no-cap` gives a wait of 29.1 hours after 20 attempts. A cap, and a total deadline, keep the loop in the range that a user waits for.

### Retries at every layer

When each layer of a system retries, the retries multiply. The SRE book gives an example where three layers each make four attempts, so "a single user action may create 64 attempts (4^3) on the database".[^4] Brooker gives a stack five layers deep and says that "the load on the database will increase 243x".[^3] The C example `layers` counts the 64 calls. Retry at one layer, usually the one nearest to the user, and let the other layers fail fast.

### Retrying an operation that is not idempotent

A timeout does not say whether the server did the work. A retry of a payment, an email or an insert can do it twice. Use an idempotency key, or make the operation idempotent, before it gets a retry policy.

### Retrying errors that cannot succeed

A `400 Bad Request` fails the same way on every attempt. Each retry adds load and delay and gives the same answer. The examples stop at once on a permanent error and return it to the caller.

### Retrying without a deadline

Each attempt can wait for its full timeout. Five attempts with a 10 second timeout and the waits between them take more than 50 seconds, long after the user has gone. Pass the deadline of the request down to the retry loop.

### Hiding the failure

A retry that succeeds hides the failure that came before it. Record each failed attempt, for example with [[structured-logging]], and count the retries. A rise in the retry rate is often the first sign that a dependency is in trouble.

### Retrying in a hot loop

A loop that retries at once, with no wait, sends calls as fast as the CPU allows. When the dependency is down, the loop uses a whole core and floods the dependency ([[uncontrolled-resource-consumption]]).

## Cost

- **Latency.** Each retry adds its wait and the time of the next attempt. With full jitter, the mean wait before retry n+1 is half of `min(cap, base * 2**n)`.
- **Load.** In the worst case, every request makes the maximum number of attempts. A retry policy with four attempts can multiply the load on a failing dependency by four, and nested retries multiply further. Budgets and circuit breakers put a limit on this.
- **State.** An idempotency key needs storage on the server for as long as a retry can arrive. Stripe allows keys to be removed after 24 hours.[^7]
- **Testing.** The retry loop is code that runs only on failure. The examples inject the clock and the random generator, so a test can check the waits without sleeping.

## History

- **1976.** Robert Metcalfe and David Boggs described Ethernet. After each collision, a controller "delays for an interval of random length with a mean twice that of the previous interval". The authors called this Binary Exponential Backoff.[^1]
- **2011.** RFC 6298 specified the TCP retransmission timer, with the timer doubled after each timeout.[^6]
- **2014.** Martin Fowler described the circuit breaker, after Michael Nygard's Release It!.[^10]
- **2015.** Marc Brooker's article "Exponential Backoff And Jitter" compared full, equal and decorrelated jitter in a simulation.[^2]
- **2016.** The Google SRE book described retry budgets and deadline propagation.[^4] AWS added a token bucket for retries to the AWS SDK.[^3]
- **2019.** The Amazon Builders' Library article "Timeouts, retries, and backoff with jitter" described the multiplication of retries across layers, and idempotency as the condition for a safe retry.[^3]

## References
