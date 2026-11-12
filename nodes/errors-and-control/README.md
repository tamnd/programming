This area covers how a program decides what to do next, and what it does when something goes wrong. It holds the control structures, the main ways to report an error, and the ways to clean up and to try again.

## What it holds

- **Control structures.** Conditionals, multiway branches and pattern matching: the ways a program chooses between paths.
- **Loops.** Iteration with counted loops, condition loops and loops over collections. The iterator protocols are in functions-and-abstraction.
- **Jumps and labels.** Statements that leave the normal order inside one function: break, continue, labels, goto and early returns.
- **Error concepts.** What an error is, the strategies to report it, and the practices that decide where errors go.
- **Exceptions.** Throwing and catching exceptions, exception types, checked exceptions and exception safety.
- **Error values and result types.** Errors as return values: result types, multiple return values and the operators that propagate them.
- **Panics and aborts.** Errors that stop the program or the thread, ways to exit with a status, and the information left behind.
- **Assertions and contracts.** Assertions, invariants, preconditions and postconditions in code. Contracts as a verification method are in program-analysis-and-verification.
- **Non-local exits.** Leaving several levels of a call or a loop at once, and the condition systems that go beyond exceptions.
- **Cleanup.** Releasing a resource on every path out of a block: finally, defer, using, with and their peers.
- **Retries and timeouts.** Trying an operation again, waiting between tries, and limits on time inside one program. Retries between services are in distributed-and-cloud.

## What it does not hold

- Design by contract as a verification method. It is in [Program analysis and verification](/program-analysis-and-verification/).
- Retries between services, such as idempotency keys in a distributed system. They are in [Distributed and cloud programming](/distributed-and-cloud/). The general retry page here links to them.
- Logging of errors. It is in [Debugging and observability](/debugging-and-observability/).
- Generators and continuations as control forms. They are in [Functions and abstraction](/functions-and-abstraction/).
