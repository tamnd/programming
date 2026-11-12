---
tag: QFTPN4
kind: pattern
slug: temporary-field
name: Temporary Field
aliases: [optional field smell]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: an object has fields that hold a value only in some cases or during one computation, so a reader cannot tell when the field is valid and the code must check it each time.
family: code-smell
polarity: negative
intent: "Find the fields that are set only some of the time, give them a class of their own, and give the case where they are not set an object of its own too."
also_known_as: []
refactorings: [extract-class, move-function, introduce-special-case]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Temporary Field is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, null-references, strategy-pattern, program-state, data-clumps]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Temporary Field
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Temporary Field
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Extract Class, Move Function and Introduce Special Case (alias Introduce Null Object)
  - url: https://en.wikipedia.org/w/index.php?title=Null_object_pattern&oldid=1378667334
    title: Wikipedia, Null object pattern, revision 1378667334
    locator: the introduction, Description and Criticism
---

## Intent

Find the fields of a class that hold a value only some of the time: only for some kinds of object, or only while one method runs. Move them into a class of their own, together with the code that uses them. If the code must also handle the case where the values are absent, give that case an object of its own, so that the callers stop checking.

## Problem

A reader of a class expects each field to mean something for each object of the class. A temporary field breaks that rule. It is set for some objects and empty for others, or it is set at the start of one method and left behind when the method returns.

*Refactoring* describes the smell as a class with a field that is set only in certain circumstances, and notes that a reader can spend a long time working out why a field is there when it seems not to be used.[^1] The first edition adds a common cause: an algorithm that needs several values, where the programmer stores them in fields to avoid a long list of parameters, and the fields have a meaning only while the algorithm runs.[^2]

The forces are these:

- The field is cheap to add. One more case needs one more value, and the class is already there.
- Each method that reads the field must know when it is valid. The checks for empty values spread across the class, and one forgotten check is a bug.
- A field set by one method for another method to read makes the order of calls part of the interface. A call in the wrong order reads a stale or an empty value.
- Several temporary fields that are set and cleared together are a group with no name, which is close to [[data-clumps|Data Clumps]].

## Solution

*Refactoring* gives the moves in order.[^1] The catalogue at refactoring.com has a card for each refactoring, and gives Introduce Null Object as another name for Introduce Special Case.[^3]

1. **Give the fields a home:** [[extract-class|Extract Class]]. The fields that are valid together become the fields of one class, and that class always has them.
2. **Move the code with them:** [[move-function|Move Function]]. The methods that read the fields move into the class.
3. **Name the empty case:** [[introduce-special-case|Introduce Special Case]]. If the original class must work when the values are absent, give it an object of a second class with the same methods and the neutral behaviour. The checks for an empty field disappear.

For the fields that an algorithm uses while it runs, the first edition suggests the same Extract Class, so that the values and the algorithm live in an object that exists only for the run.[^2] Local variables and parameters are another fix, when the algorithm is small enough.

Wikipedia describes the special case object in the same terms. A null object "implements the expected interface but performs no action", which "avoids the need for conditional checks scattered throughout calling code".[^4]

## Example

An order has three promotion fields. They have a value only for orders with a promotion, and both methods of the order must check them first.

```javascript run id=extract-the-promotion
import assert from "node:assert/strict";

const sumCents = (items) => items.reduce((t, i) => t + i.cents * i.qty, 0);

// Before: the promotion fields are empty for most orders.
class OrderBefore {
  constructor(items) {
    this.items = items;
    this.promoCode = null;
    this.promoPercent = 0;
    this.promoUntil = null;
  }
  applyPromo(code, percent, until) {
    this.promoCode = code;
    this.promoPercent = percent;
    this.promoUntil = until;
  }
  total(day) {
    const sum = sumCents(this.items);
    if (this.promoCode !== null && day <= this.promoUntil) {
      return sum - Math.round((sum * this.promoPercent) / 100);
    }
    return sum;
  }
  label() {
    return this.promoCode === null ? "no promotion" : `promotion ${this.promoCode}`;
  }
}

// After: the fields have a class, and the empty case has an object.
class Promotion {
  constructor(code, percent, until) {
    Object.assign(this, { code, percent, until });
  }
  apply(sum, day) {
    return day <= this.until ? sum - Math.round((sum * this.percent) / 100) : sum;
  }
  get label() {
    return `promotion ${this.code}`;
  }
}
const noPromotion = { apply: (sum) => sum, label: "no promotion" };

class Order {
  constructor(items, promotion = noPromotion) {
    this.items = items;
    this.promotion = promotion;
  }
  total(day) {
    return this.promotion.apply(sumCents(this.items), day);
  }
  label() {
    return this.promotion.label;
  }
}

const items = [{ cents: 1250, qty: 2 }, { cents: 499, qty: 1 }];
const plainBefore = new OrderBefore(items);
const promoBefore = new OrderBefore(items);
promoBefore.applyPromo("SPRING", 10, "2026-04-30");
const plain = new Order(items);
const promo = new Order(items, new Promotion("SPRING", 10, "2026-04-30"));

for (const day of ["2026-04-01", "2026-05-01"]) {
  assert.equal(plain.total(day), plainBefore.total(day));
  assert.equal(promo.total(day), promoBefore.total(day));
}
assert.equal(promo.total("2026-04-01"), 2699);
assert.equal(promo.label(), promoBefore.label());
assert.equal(plain.label(), "no promotion");
console.log("promotion: tests pass");
```

```output
promotion: tests pass
```

In the second version the order has no field that is empty for most orders. The rules for the date and the percentage are in `Promotion`, and the order calls the same two methods for each kind of order. The case with no promotion is one object, not a check in each method.

| Language | What the example shows |
|---|---|
| JavaScript | a promotion class and a special case object, as above |
| Python | the same split, with a dataclass and a special case object |
| Java | a `Promotion` interface with a record for the real case and a constant for the empty case |
| Go | a `promotion` interface with a struct for each case |

## Consequences

Each object has the fields that it uses. A reader of the order no longer needs to know when the promotion fields are valid, and a method cannot forget the check, because there is no check.

The class count goes up by one or two. For one field read in one place, a check may be clearer than a class. The fix earns its cost when several fields go together or when many methods check the same condition.

A special case object has a risk. Wikipedia warns that the pattern "can make errors/bugs appear as normal program execution".[^4] If an empty value is an error, a special case object hides it, and an exception or an option type is the better choice. Use a special case only for an absence that has a correct neutral behaviour, such as an order with no discount.

## How to fix it

- **Fields that are valid together for some objects:** [[extract-class|Extract Class]], then [[move-function|Move Function]] for the code that reads them.[^1]
- **Checks for the empty case in many places:** [[introduce-special-case|Introduce Special Case]].[^1]
- **Fields used only while one algorithm runs:** [[extract-class|Extract Class]] for an object that holds the run, or local variables and parameters.[^2]

## Variants and related patterns

**Data Clumps.** Temporary fields that are set and cleared together are often a clump that needs a class. See [[data-clumps|Data Clumps]].

**Null references.** A temporary field is usually null when it is not valid. See [[null-references|Null references]].

**Strategy pattern.** The promotion and the empty case are two strategies for the same step. See [[strategy-pattern|Strategy pattern]].

## References
