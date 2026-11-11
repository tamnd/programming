This area covers how a program decides what to do next, and what it does when something goes wrong. It holds the control structures, the main ways to report an error, and the ways to clean up and to try again.

## What it holds

- Control structures: conditionals, loops, switch and match, and jumps.
- Exceptions: throwing and catching exceptions, and checked and unchecked exceptions.
- Error values and result types: errors as return values, result types and error codes.
- Panics and aborts: errors that stop the program or the thread, and when to use them.
- Assertions and contracts: assertions, preconditions and postconditions in code.
- Non-local exits: leaving several levels of a call or a loop at once.
- Cleanup: finally, defer, using, with and other ways to release a resource on every path.
- Retries and timeouts: trying an operation again, backoff and limits on time.

## What it does not hold

- Design by contract as a verification method. It is in [Program analysis and verification](/program-analysis-and-verification/).
- Retries between services, such as idempotency keys in a distributed system. They are in [Distributed and cloud programming](/distributed-and-cloud/). The general retry page here links to them.
- Logging of errors. It is in [Debugging and observability](/debugging-and-observability/).
- Generators and continuations as control forms. They are in [Functions and abstraction](/functions-and-abstraction/).
