---
tag: EA9FF9
kind: pattern
slug: insider-trading
name: Insider Trading
aliases: [inappropriate intimacy]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: two modules read and change each other's internal data, so neither can change without the other, and the coupling between them is hidden from the rest of the program.
family: code-smell
polarity: negative
intent: "Find the modules that reach into each other's internals, and move the shared data and behaviour so that each module talks to the other through a small, visible interface."
also_known_as: [Inappropriate Intimacy]
refactorings: [move-function, move-field, hide-delegate, replace-subclass-with-delegate, replace-superclass-with-delegate]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: The first edition of Refactoring (1999) calls the smell Inappropriate Intimacy and speaks of classes. The second edition (2018) renames it Insider Trading and speaks of modules.
see_also: [code-smell, feature-envy, encapsulation-and-visibility, composition-over-inheritance, class-inheritance, global-data]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Insider Trading
  - url: https://en.wikipedia.org/w/index.php?title=Coupling_(computer_programming)&oldid=1367462368
    title: Wikipedia, Coupling (computer programming), revision 1367462368
    locator: the introduction, Coupling and cohesion, and Content coupling
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Move Function, Move Field, Hide Delegate, Replace Subclass with Delegate and Replace Superclass with Delegate
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Inappropriate Intimacy
---

## Intent

Find the pairs of modules that know too much about each other: one reads the other's private fields, or changes its data directly, or depends on the order in which the other does its work. Move the data and the behaviour so that each piece lives in one module, and let the other module use a small interface that the reader can see.

## Problem

Modules must share some data, or they could not work together. The question is how they share it. A module that calls a function of another module through its public interface depends on that interface. A module that reads and writes the other module's internal fields depends on all of its internals, and the other module cannot change its representation without breaking it.

The second edition of *Refactoring* names the smell after a market crime: modules that trade data behind each other's backs. It asks the programmer to keep such trade to a minimum and to make the rest of it visible.[^1]

The idea is older than the smell. Wikipedia defines coupling as "the degree of interdependence between software modules", and notes that "Low coupling often correlates with high cohesion, and vice versa."[^2] The strongest kind on its list is content coupling, when one module uses the internals of another, which "violates information hiding".[^2]

The forces are these:

- Reaching into another module is the shortest path to a feature. A getter or a public field is already there.
- Two modules that grew up together often share data because their authors shared a mental model. Each change to one is then a change to both.
- The coupling is invisible from outside. A reader of the module's interface does not see that another module reads its fields.
- Inheritance makes it easier. A subclass can see and depend on the fields of its parent, and a parent can come to depend on what its subclasses do.

## Solution

*Refactoring* gives a set of moves.[^1] The catalogue at refactoring.com has a card for each refactoring.[^3]

1. **Put the data with its user:** [[move-function|Move Function]] and [[move-field|Move Field]]. If one module uses the other's data more than its own, move the data or the function so that both are in one module.
2. **Give the shared part a home:** if the two modules have a common interest, extract that interest into a third module, which both use through its interface.
3. **Put a module in the middle:** [[hide-delegate|Hide Delegate]]. Let one module act as the intermediary, so the client does not reach through it into a third.
4. **Break the inheritance:** [[replace-subclass-with-delegate|Replace Subclass with Delegate]] or [[replace-superclass-with-delegate|Replace Superclass with Delegate]], when a subclass and its parent depend on each other's internals.

The first edition gives the same moves under the name Inappropriate Intimacy, and adds two more: change a two-way link between classes into a one-way link, and use Extract Class for the common part.[^4]

## Example

A billing module computes an invoice. It reads the account's private list of payments and writes the account's balance directly. The account cannot change how it stores payments, and its balance can be out of step with its payments if billing forgets a step.

```javascript run id=stop-the-trade
import assert from "node:assert/strict";

// Before: billing reads and writes the account's internals.
class AccountBefore {
  constructor() {
    this._payments = [];
    this._balance = 0;
  }
}
function payBefore(account, cents) {
  account._payments.push({ cents });
  account._balance -= cents;
}
function invoiceBefore(account, chargedCents) {
  account._balance += chargedCents;
  const paid = account._payments.reduce((t, p) => t + p.cents, 0);
  return { charged: chargedCents, paid, due: account._balance };
}

// After: the account owns its data and offers three operations.
class Account {
  #payments = [];
  #charges = [];
  pay(cents) {
    this.#payments.push(cents);
  }
  charge(cents) {
    this.#charges.push(cents);
  }
  get paid() {
    return this.#payments.reduce((t, c) => t + c, 0);
  }
  get due() {
    return this.#charges.reduce((t, c) => t + c, 0) - this.paid;
  }
}
function invoice(account, chargedCents) {
  account.charge(chargedCents);
  return { charged: chargedCents, paid: account.paid, due: account.due };
}

const before = new AccountBefore();
const after = new Account();
payBefore(before, 1500);
after.pay(1500);
assert.deepEqual(invoice(after, 4000), invoiceBefore(before, 4000));
assert.deepEqual(invoice(after, 1000), { charged: 1000, paid: 1500, due: 3500 });
assert.equal(Object.keys(after).length, 0); // no field is visible from outside
console.log("invoice: tests pass");
```

```output
invoice: tests pass
```

In the second version the account keeps its payments and charges in private fields, and the balance is computed from them, so it cannot be out of step. Billing calls `charge`, `paid` and `due`. The account can change how it stores the data, for example to one list of entries, and billing does not change.

| Language | What the example shows |
|---|---|
| JavaScript | private fields and three operations, as above |
| Python | the same account, with the internals behind methods and properties |
| Java | a subclass that reads its parent's protected list, replaced by a delegate |
| Go | an account type with unexported fields and exported methods |

## Consequences

Each module owns its data, and the trade between modules goes through an interface that a reader can see. A change inside one module stays inside it, and a test of one module needs fewer details of the other.

The interface has a cost. Each operation that the other module needs is one more method, and a careless fix can turn every field into a getter and a setter, which keeps the coupling and only hides it. The aim is fewer and larger operations that say what the client wants, as `charge` does here.

Some modules belong together. Two classes that always change together and share most of their data may be one module, and the fix is to merge them, not to build an interface between them. Languages give tools for a close pair: a nested class in Java, unexported names shared inside one package in Go, a module-level function in Python. These keep the closeness local, where a reader can see it.

## How to fix it

- **A function that uses another module's data more than its own:** [[move-function|Move Function]].[^1]
- **A field that another module reads and writes more than its owner:** [[move-field|Move Field]].[^1]
- **Two modules with a common interest:** a third module for it, which both use.[^1]
- **A client that reaches through one module into another:** [[hide-delegate|Hide Delegate]].[^1]
- **A subclass and a parent that depend on each other's internals:** [[replace-subclass-with-delegate|Replace Subclass with Delegate]] or [[replace-superclass-with-delegate|Replace Superclass with Delegate]].[^1]

## Variants and related patterns

**Feature Envy.** One function that uses another module's data more than its own. Insider Trading is the same problem between whole modules, often in both directions. See [[feature-envy|Feature Envy]].

**Global Data.** Data that every module can read and write is a trade with no owner at all. See [[global-data|Global Data]].

**Composition over inheritance.** Replacing a subclass with a delegate turns a close link into one through an interface. See [[composition-over-inheritance|Composition over inheritance]].

## References
