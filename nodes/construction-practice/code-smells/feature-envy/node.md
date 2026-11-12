---
tag: 4X6W70
kind: pattern
slug: feature-envy
name: Feature Envy
aliases: []
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a function uses the data and the functions of another module more than those of its own, so the rule it computes lives far from the data it depends on.
family: code-smell
polarity: negative
intent: "Find the functions that work mostly on the data of another module, and move them, or the envious part of them, to that module."
also_known_as: []
refactorings: [move-function, extract-function]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Feature Envy is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, encapsulation-and-visibility, strategy-pattern, visitor-pattern, shotgun-surgery]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Feature Envy
  - url: https://martinfowler.com/bliki/TellDontAsk.html
    title: Martin Fowler, Tell Don't Ask, bliki
    locator: the definition, the monitor example and note 1
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Move Function (alias Move Method) and Extract Function
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Feature Envy
---

## Intent

Find the functions that are more interested in another module than in their own, and move them to the module whose data they use. Then the rule and the data that it reads change together, in one place.

## Problem

A design splits a program into modules so that the code inside a module works together closely, and the modules talk to each other as little as possible. *Refactoring* describes Feature Envy as a function in one module that spends more of its time with the functions or the data of another module than with its own.[^1] A common form is a function that calls several getters of one object to compute a value from them.[^1]

The forces are these:

- The function was written where it was first needed. The code that withdraws cash needed to know if an account allows the withdrawal, so the check was written in the cash module, from the fields of the account.
- A rule that lives outside its data must be found when the data changes. If the account gets one more field, such as a daily limit, the programmer must know to look in the cash module.
- Other modules need the same rule. Each one either calls into the cash module, which is a strange dependency, or writes its own copy.
- Getters make the smell cheap to write. Each field that an object exposes is one more thing that code in other modules can reach for.

Fowler's article on Tell Don't Ask describes the same pull. It says that "object-orientation is about bundling data with the functions that operate on that data", and that "rather than asking an object for data and acting on that data, we should instead tell an object what to do."[^2]

## Solution

*Refactoring* gives the fix in three cases.[^1] The catalogue at refactoring.com has the cards for the two refactorings, and gives Move Method as another name for Move Function.[^3] Move Method is the name that the first edition uses.[^4]

1. **The whole function envies one module:** [[move-function|Move Function]] to that module.
2. **Only part of the function envies another module:** [[extract-function|Extract Function]] on that part, then Move Function on the result.
3. **The function uses data from several modules:** move it to the module that has most of the data it uses, or extract the parts and move each one to its own module.

*Refactoring* also gives the principle behind the rule: put together the things that change together.[^1] Data and the behaviour that reads it usually change together.

## Example

The function `canWithdrawBefore` lives in the cash machine module. It reads three fields of the account and none of its own module. After the move, it is a method of the account.

```javascript run id=move-to-the-data
import assert from "node:assert/strict";

class Account {
  constructor(balance, overdraftLimit, frozen = false) {
    this.balance = balance;
    this.overdraftLimit = overdraftLimit;
    this.frozen = frozen;
  }

  // After: the rule lives with the data it reads.
  canWithdraw(amount) {
    return !this.frozen && this.balance + this.overdraftLimit >= amount;
  }
}

// Before: the cash machine module asks the account for its data.
function canWithdrawBefore(account, amount) {
  if (account.frozen) return false;
  return account.balance + account.overdraftLimit >= amount;
}

const cases = [
  [new Account(100, 50), 150, true],
  [new Account(100, 50), 151, false],
  [new Account(100, 0, true), 10, false],
];
for (const [account, amount, expected] of cases) {
  assert.equal(canWithdrawBefore(account, amount), expected);
  assert.equal(account.canWithdraw(amount), expected);
}
console.log("canWithdraw: tests pass");
```

```output
canWithdraw: tests pass
```

The test runs the same cases on both versions. After the move, a change to the rule, such as a daily limit, is a change to `Account` and its tests. The cash machine calls `account.canWithdraw(amount)` and no longer needs to know which fields the rule reads.

| Language | What the example shows |
|---|---|
| JavaScript | a function moved into a class as a method |
| Python | the same move, into a dataclass |
| Java | a method that called three getters, moved into the class; the getters can then become private |
| Go | a function moved to a method on the struct |

## Consequences

The rule and its data are in one place. The fields that only the rule used can become private, so the module exposes less. Other callers get the rule without a dependency on the module where it used to live.

A move can go too far. Fowler warns, in the same article, of people who become "GetterEradicators", and says that "there are times when objects collaborate effectively by providing information."[^2] A report that reads the fields of ten kinds of objects is not ten cases of Feature Envy. It is a report, and its job is to read.

Some designs separate behaviour from data on purpose. *Refactoring* names the Strategy and Visitor patterns of the Gang of Four book as exceptions: they put behaviour in a separate object, so that the behaviour can vary without a change to the data.[^1] Fowler adds that co-locating data and behaviour "should sometimes be dropped in favor of other concerns - such as layering."[^2] In those designs the envy is the cost of a choice, not a mistake.

## How to fix it

- **The whole function reads another module:** [[move-function|Move Function]].[^1]
- **One part of the function reads another module:** [[extract-function|Extract Function]], then Move Function.[^1]
- **The function reads several modules:** move it to the one with the most data it uses, or split it first.[^1]
- **A chain of getters to reach the data:** see [[message-chains|Message Chains]] and Hide Delegate.

## Variants and related patterns

**Data Class.** A class with fields and getters and no behaviour is often the other side of Feature Envy: its behaviour lives in the classes that envy it. See [[data-class|Data Class]].

**Insider Trading.** Two modules that exchange too much data with each other. See [[insider-trading|Insider Trading]].

**Strategy and Visitor.** Patterns that keep behaviour apart from data on purpose. See [[strategy-pattern]] and [[visitor-pattern]].

**Law of Demeter.** A rule that a method should talk only to its close collaborators, which limits the reach that envy needs. See [[law-of-demeter|Law of Demeter]].

## References
