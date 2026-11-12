---
tag: 8Q5R21
kind: practice
slug: program-to-an-interface
name: Program to an interface, not an implementation
aliases: [program to an interface, code to an interface]
area: software-design/design-principles
level: core
summary: >-
  Write client code against the interface of a collaborator, the operations it promises, and not against one concrete class, so that the implementation can change or be replaced.
history:
  year: 1995
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: The four authors of Design Patterns gave the principle in the introduction of the book, Addison-Wesley, 1995, as one of two principles of reusable object-oriented design.
see_also: [dependency-injection, composition-over-inheritance, orthogonality, adapter-pattern, extension-object]
sources:
  - url: https://www.artima.com/articles/design-principles-from-design-patterns
    title: Bill Venners, Design Principles from Design Patterns, a conversation with Erich Gamma, part III, Artima, 6 June 2005
    locator: the sections Program to an interface, not an implementation and The value of interfaces
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: section Interface types, Implementing an interface
  - url: https://peps.python.org/pep-0544/
    title: PEP 544, Protocols, Structural subtyping (static duck typing)
    locator: the sections Rationale and Goals and Terminology
---

## Statement

"Program to an interface, not an implementation." This is the first of the "two principles of reusable object-oriented design" in the introduction of the book Design Patterns, by Erich Gamma, Richard Helm, Ralph Johnson and John Vlissides.[^1]

## Meaning

Gamma explained: "This principle is really about dependency relationships which have to be carefully managed in a large app. It's easy to add a dependency on a class. It's almost too easy". The reverse is hard: "getting rid of an unwanted dependency can be real refactoring work".[^1]

"Once you depend on interfaces only, you're decoupled from the implementation. That means the implementation can vary, and that's a healthy dependency relationship."[^1]

- **Interface means the promised operations.** The principle is not only about the Java interface construct. Bill Venners described it as writing client code "against the interface of the type that's as far up that hierarchy as I can go, without going too far", and Gamma agreed.[^1] An abstract class can also be the interface.
- **Use only the published contract.** If a class implements an interface with five methods and adds ten more, a client that calls one of the ten uses the implementation. Gamma: "You call a method that is out of contract, which I might break anytime."[^1]
- **The interface is the vocabulary.** "An interface distills the collaboration between objects. An interface is free from implementation details, and it defines the vocabulary of the collaboration."[^1]
- **No need to design it first.** Gamma said that you can distill an interface from a concrete class later, when you understand the problem: "The intended interface is just one extract interface refactoring away."[^1]

[[dependency-injection|Dependency injection]] is the usual way to give a client an object that it knows only by its interface.

## Examples

A report function needs to load the sales of a day. It uses only one operation, salesOn. A test gives it a small object in memory in place of the database, as Gamma suggests: "for testing purposes you can replace a heavy database implementation with a lighter-weight mock implementation."[^1]

```javascript run id=sales-store
import assert from "node:assert/strict";

// The client knows only the operation salesOn(day).
function dailyReport(store, day) {
  const sales = store.salesOn(day);
  const total = sales.reduce((s, x) => s + x, 0);
  return `${day}: ${sales.length} sales, total ${total}`;
}

// One implementation, for example over a database.
class SqlStore {
  constructor(rows) { this.rows = rows; }
  salesOn(day) { return this.rows.filter((r) => r.day === day).map((r) => r.amount); }
}

// Another implementation, for a test.
const fakeStore = { salesOn: () => [5, 7] };

const sql = new SqlStore([{ day: "mon", amount: 5 }, { day: "mon", amount: 7 }, { day: "tue", amount: 1 }]);
assert.equal(dailyReport(fakeStore, "mon"), dailyReport(sql, "mon"));
console.log(dailyReport(sql, "mon"));
```

```output
mon: 2 sales, total 12
```

| Language | What the example shows |
|---|---|
| JavaScript | any object with a salesOn method works, as above |
| Python | a Protocol class names the interface, and the classes match it without a base class |
| Go | an interface type names the interface; a type implements it with no declaration |

In Go, a type implements an interface when it has the methods of the interface; the program needs no implements clause.[^2] In Python, PEP 544 lets a class be "implicitly considered a subtype" of a protocol by static type checkers, "without explicit base classes in the class definition".[^3]

## Criticism and limits

- **A published interface is hard to change.** "In Java when you add a new method to an interface, you break all your clients." Gamma's advice is to "consider them as immutable once you've published them" and to add new methods in a separate extension interface. This puts "some burden on the caller", who must check at run time whether an object implements the extension.[^1]
- **Interface or abstract class.** An abstract class "gives you more flexibility when it comes to evolution", because a new method can come with a default implementation. "As always there is a trade-off".[^1]
- **One implementation.** An interface with only one implementation adds a type and a level of indirection. Gamma's remark that an interface can be extracted later, when it is needed, suggests that it does not have to be written first.[^1]

## History

The principle is from the introduction of Design Patterns: Elements of Reusable Object-Oriented Software (Addison-Wesley, 1995), together with the second principle, "favor object composition over class inheritance".[^1] See [[composition-over-inheritance|Composition over inheritance]] for the second one. Gamma discussed both principles with Bill Venners in 2005, with examples from the design of Eclipse.[^1]

## References
