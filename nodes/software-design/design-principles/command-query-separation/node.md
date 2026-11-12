---
tag: 4FRA6Y
kind: practice
slug: command-query-separation
name: Command-query separation
aliases: [CQS]
area: software-design/design-principles
level: core
summary: >-
  Every method is either a query, which returns a result and does not change the observable state, or a command, which changes the state and returns no value, but not both.
history:
  year: 1988
  people: [Bertrand Meyer]
  note: Meyer gave the principle in the book Object-Oriented Software Construction, first published in 1988, as part of his work on the Eiffel language.
see_also: [separate-query-from-modifier, principle-of-least-astonishment, dont-repeat-yourself]
sources:
  - url: https://martinfowler.com/bliki/CommandQuerySeparation.html
    title: Martin Fowler, CommandQuerySeparation, 5 December 2005
    locator: the whole post
  - url: https://en.wikipedia.org/wiki/Command%E2%80%93query_separation
    title: Wikipedia, Command–query separation
    locator: the lead and the sections Connection with design by contract, Command Query Responsibility Segregation and Limitations
---

## Statement

Command-query separation (CQS) "states that every method should either be a command that performs an action, or a query that returns data to the caller, but not both. In other words, asking a question should not change the answer."[^2]

Martin Fowler describes the two categories:[^1]

- **Queries** "Return a result and do not change the observable state of the system (are free of side effects)."
- **Commands** "Change the state of a system but do not return a value."

## Meaning

Fowler says: "The really valuable idea in this principle is that it's extremely handy if you can clearly separate methods that change state from those that don't." A query can then be used "in many situations with much more confidence, introducing them anywhere, changing their order. You have to be more careful with modifiers."[^1]

- **The return type tells the kind.** In the principle, "the return type is the give-away for the difference."[^1] A method that returns a value is a query. A method that returns nothing is a command.
- **Observable state only.** The rule about not changing state applies only to the observable state of the system.[^1] Fowler gives this as one reason why a language cannot easily detect the methods that change state.
- **Assertions stay safe.** CQS suits design by contract, because "any value-returning method (any query) can be called by any assertion without fear of modifying program state."[^2] Assertion checks can then be turned off without a change to the behaviour of the program.[^2]

To fix a method that breaks the rule, use the refactoring [[separate-query-from-modifier|Separate query from modifier]].

## Examples

The first counter has a method that adds one and returns the new value, so a call to read the value also changes it. The second counter has a command that changes the state and a query that reads it.

```javascript run id=counter
import assert from "node:assert/strict";

// Breaks CQS: one method both changes state and returns it.
class MixedCounter {
  #n = 0;
  next() { this.#n += 1; return this.#n; }
}

// Follows CQS: a command and a query.
class Counter {
  #n = 0;
  increment() { this.#n += 1; }      // command: returns nothing
  get value() { return this.#n; }    // query: changes nothing
}

const m = new MixedCounter();
console.log(m.next(), m.next());     // two reads, two different answers

const c = new Counter();
c.increment();
assert.equal(c.value, c.value);       // asking does not change the answer
console.log(c.value, c.value);
```

```output
1 2
1 1
```

| Language | What the example shows |
|---|---|
| JavaScript | the mixed counter and the separated counter, as above |
| Python | the separated counter, with a property for the query |
| Go | the separated counter, with an Increment method and a Value method |

## Criticism and limits

- **Useful exceptions.** Fowler writes: "Meyer likes to use command-query separation absolutely, but there are exceptions. Popping a stack is a good example of a query that modifies state." He prefers to follow the principle when he can, "but I'm prepared to break it to get my pop."[^1] The Java iterator method next both returns the next item and advances; Fowler would prefer separate advance and current methods.[^1]
- **Threads.** "CQS can introduce complexities for implementing reentrant and multithreaded software correctly."[^2] With a separate increment command and value query, another thread can run between the two calls. A combined method that increments and returns the new value under one lock avoids this race, but it breaks CQS.[^2]
- **Little language support.** Fowler notes that a language could let the programmer mark the methods that change state, but this is rare; the only case he had met was the const modifier in C++.[^1]

## History

Bertrand Meyer devised CQS as part of his work on the Eiffel programming language.[^2] He coined the term in his book Object-Oriented Software Construction, first published in 1988, with a second edition in 1994.[^1][^2] Fowler wrote about it on his bliki in 2005.[^1] Command query responsibility segregation (CQRS) later generalized the idea to services: it uses separate query and command interfaces, and usually separate data models, to read and to change data.[^2]

## References
