---
tag: M2VK0J
kind: practice
slug: orthogonality
name: Orthogonality
aliases: [orthogonal design]
area: software-design/design-principles
level: core
summary: >-
  Keep things that are not related in concept unrelated in the system, so that a change to one does not cause a change to another.
history:
  year: 1999
  people: [Andy Hunt, Dave Thomas]
  note: The Pragmatic Programmer, 1999, made orthogonality a general design rule; the word was used earlier for the design of programming languages and instruction sets, such as Algol 68.
see_also: [separation-of-concerns, dont-repeat-yourself, dependency-injection]
sources:
  - url: https://www.artima.com/articles/orthogonality-and-the-dry-principle
    title: Bill Venners, Orthogonality and the DRY Principle, a conversation with Andy Hunt and Dave Thomas, Artima, 2003
    locator: the sections Be Orthogonal and the helicopter story
  - url: https://en.wikipedia.org/wiki/Orthogonality_(programming)
    title: Wikipedia, Orthogonality (programming)
    locator: the whole article
---

## Statement

Andy Hunt: "The basic idea of orthogonality is that things that are not related conceptually should not be related in the system. Parts of the architecture that really have nothing to do with the other, such as the database and the UI, should not need to be changed together. A change to one should not cause a change to the other."[^1]

## Meaning

In programming, "orthogonality means that operations change just one thing without affecting others."[^2]

- **Low coupling.** Hunt connects the idea to coupling: "There are exceptions and tradeoffs, but in most cases you want to minimize coupling between things that are otherwise unrelated."[^1]
- **A simple test.** "If you want to make a little program to test out one API or interface and you have to link in every library in the system, you have too much coupling. Your system is not orthogonal."[^1]
- **Small combinable parts.** In the design of a language or an API, orthogonality means a small set of primitive parts that combine freely. "The meaning of an orthogonal feature is independent of context".[^2]

Dave Thomas gave an example of a system that was not orthogonal: "when one client changed the number of lines on the screen, they had to change the database schema too."[^1]

He compared such a system to a helicopter. Its controls are related by aerodynamics and gyroscopic effects, so "one small change, such as lowering the collective, causes the helicopter to dip and turn to one side." In code, "if the pieces interact with each other more than necessary, then you'll always get that kind of distributed bug fixing."[^1]

## Examples

The first function computes an invoice total but needs a database object to do it, so a test of the arithmetic must build a database. The orthogonal version takes plain data. The storage and the arithmetic can now change, and be tested, on their own.

```javascript run id=orthogonal-total
import assert from "node:assert/strict";

// A fake database, for the coupled version.
const db = {
  rows: { 7: [{ qty: 2, price: 3 }, { qty: 1, price: 4 }] },
  linesOf(id) { return this.rows[id]; },
};

// Coupled: the arithmetic needs the database.
function invoiceTotalFromDb(database, id) {
  return database.linesOf(id).reduce((s, l) => s + l.qty * l.price, 0);
}

// Orthogonal: the arithmetic needs only the lines.
const invoiceTotal = (lines) => lines.reduce((s, l) => s + l.qty * l.price, 0);

assert.equal(invoiceTotal(db.linesOf(7)), invoiceTotalFromDb(db, 7));
// A test of the arithmetic needs no database at all.
assert.equal(invoiceTotal([{ qty: 3, price: 5 }]), 15);
console.log(invoiceTotal(db.linesOf(7)));
```

```output
10
```

| Language | What the example shows |
|---|---|
| JavaScript | the coupled and the orthogonal total, as above |
| Python | the orthogonal total over a list of dataclasses |
| Go | the orthogonal total over a slice of structs |

## Criticism and limits

- **Some coupling is needed.** Hunt himself says: "There are exceptions and tradeoffs".[^1] Parts that work together must share some knowledge, such as the shape of the data that passes between them.
- **Not always simpler.** Orthogonality "does not necessarily result in simpler systems". The VAX had one general add instruction for any mix of registers and memory, where the IBM mainframe had two. Yet the less orthogonal RISC CPU architectures were more successful than the CISC architectures.[^2]
- **Coupling grows slowly.** Thomas calls the loss of orthogonality "one of those creeping viral problems": each new link between two parts looks harmless, and then "Soon you have four things that know about each other that shouldn't."[^1]

## History

The term was first used for programming languages and instruction sets. The Revised Report on Algol 68 said that its concepts "have been applied orthogonally in order to maximize the expressive power of the language".[^2] The idea later spread to the design of APIs and user interfaces.[^2] Andy Hunt and Dave Thomas made it a general rule of software design in The Pragmatic Programmer, published in 1999, and explained it to Bill Venners in a 2003 interview.[^1]

## References
