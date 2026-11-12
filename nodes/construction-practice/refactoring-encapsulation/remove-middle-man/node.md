---
tag: 2TNNPC
kind: technique
slug: remove-middle-man
name: Remove middle man
aliases: [remove middleman]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  When a class does little but forward calls to an object it holds, let clients call that object directly and delete the forwarding methods; the inverse of Hide Delegate.
family: refactoring
polarity: positive
intent: "Let clients talk to the object that does the work when the class in between only forwards."
also_known_as: [Remove Middleman]
history:
  year: 1999
  people: [Martin Fowler]
  note: Remove Middle Man is in the first edition of Refactoring (1999), with the refactorings that move features between objects, and in the chapter on encapsulation of the second edition (2018). Its inverse is Hide Delegate. IntelliJ IDEA has an action with the name Remove Middleman.
see_also: [hide-delegate, middle-man, message-chains, inline-function, inline-class]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Remove Middle Man, motivation and mechanics; Chapter 3, Bad Smells in Code, Middle Man
  - url: https://refactoring.com/catalog/removeMiddleMan.html
    title: Martin Fowler, Catalog of Refactorings, Remove Middle Man
    locator: the entry; inverse of Hide Delegate
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 7, Moving Features Between Objects, Remove Middle Man
  - url: https://www.jetbrains.com/help/idea/remove-middleman.html
    title: IntelliJ IDEA documentation, Remove middleman
    locator: the introduction; Run the Remove Middleman refactoring
---

## Purpose

[[hide-delegate|Hide Delegate]] puts a forwarding method on a class so that clients do not reach through it. Each feature of the delegate that clients use needs one such method. As clients use more features, the class fills with methods that only forward, and it becomes a [[middle-man|Middle Man]].[^1]

Remove middle man undoes that. The class gives clients the delegate, clients call the delegate directly, and the forwarding methods go. The catalogue card is Hide Delegate run backwards: `aPerson.manager` becomes `aPerson.department.manager` again.[^2] The first edition has the refactoring under the same name.[^3]

The two refactorings move the same line in two directions. The book says that the right amount of hiding is hard to find, and that it changes as the system changes; the pair lets you move the line when it does.[^1]

## Steps

1. Run the tests and see them pass.
2. Give the server class a getter for the delegate.[^1]
3. For each client call to a forwarding method, replace it with a call through the getter to the delegate. Test after each.
4. When no client calls a forwarding method, delete it, and test.

You can keep some forwarding methods. If one of them is used by many clients and hides a real choice, it can stay.

## Example

An account forwards five methods to its ledger. Clients use all of them, and the account adds nothing. After the refactoring, clients get the ledger from the account and call it.

```javascript run id=talk-to-the-worker
import assert from "node:assert/strict";

class Ledger {
  #entries = [];
  add(day, cents) { this.#entries.push({ day, cents }); }
  balance() { return this.#entries.reduce((s, e) => s + e.cents, 0); }
  count() { return this.#entries.length; }
  since(day) { return this.#entries.filter((e) => e.day >= day); }
}

// Before: every method of the account forwards to the ledger.
class AccountBefore {
  #ledger = new Ledger();
  add(day, cents) { this.#ledger.add(day, cents); }
  balance() { return this.#ledger.balance(); }
  count() { return this.#ledger.count(); }
  since(day) { return this.#ledger.since(day); }
}

// After: the account owns the ledger and gives it out.
class Account {
  #ledger = new Ledger();
  constructor(owner) { this.owner = owner; }
  get ledger() { return this.#ledger; }
}

const a = new Account("Ana");
const b = new AccountBefore();
for (const [day, cents] of [[1, 500], [3, -120], [9, 40]]) {
  a.ledger.add(day, cents);
  b.add(day, cents);
}
assert.equal(a.ledger.balance(), b.balance());
assert.equal(a.ledger.count(), 3);
assert.deepEqual(a.ledger.since(3), b.since(3));
console.log("remove middle man: tests pass");
```

```output
remove middle man: tests pass
```

The account keeps its own job, the owner. When the ledger gets a method, clients can use it at once, and the account does not change.

| Language | What the example shows |
|---|---|
| JavaScript | an account that gives out its ledger, as above |
| Python | the same change, with a property for the delegate |
| Java | the same change, with a getter, as the IntelliJ action leaves it |
| Go | the same change, where the forwarding methods go and an exported field takes their place |

## When to use

- **Most methods forward.** When about half of the methods of a class only delegate, the book names this refactoring as the fix for [[middle-man|Middle Man]].[^1]
- **The forwarding grows with each feature.** If each change to the delegate needs a change to the server too, the server is in the way.
- **After Hide Delegate went too far.** The line between the two refactorings moves; this one moves it back.

## Pitfalls

**Undoing a needed hiding.** If the server may later get the value from somewhere else, the forwarding method protected the clients from that change. Removing it brings back the [[message-chains|Message Chains]] that Hide Delegate removed.

**A shared delegate.** Giving out the delegate also gives clients the power to change it. If the server keeps rules about the delegate's state, a client can now break them. Give out a read-only view, or keep the methods that change state.

**Partial removal.** If some clients call the server and some call the delegate, readers see two ways to do the same thing. Finish the change, or keep a clear rule for which methods stay.

## Tools that do it

IntelliJ IDEA has a Remove Middleman action. It "lets you replace all calls to delegating methods in a class with the equivalent calls directly to the field delegated to".[^4] You place the caret on the delegating field and choose the methods to inline, and the action adds a getter for the field.[^4]

## References
