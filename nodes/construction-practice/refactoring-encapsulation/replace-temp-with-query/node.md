---
tag: SYBXKD
kind: technique
slug: replace-temp-with-query
name: Replace temp with query
aliases: [replace variable with query]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Replace a local variable that holds a computed value with a function that computes it, so that other functions can ask for the value and the long function gets easier to split.
family: refactoring
polarity: positive
intent: "Turn a computed local variable into a query function that any method of the class can call."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Replace Temp with Query is in the first edition of Refactoring (1999), with the composing refactorings, and in the chapter on encapsulation of the second edition (2018).
see_also: [extract-function, inline-variable, extract-variable, long-function, code-smell]
sources:
  - url: https://refactoring.com/catalog/replaceTempWithQuery.html
    title: Martin Fowler, Catalog of Refactorings, Replace Temp with Query
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Replace Temp with Query, motivation and mechanics
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Replace Temp with Query
  - url: https://www.jetbrains.com/help/idea/inline.html
    title: IntelliJ IDEA documentation, Inline
    locator: Inline Variable
---

## Purpose

A local variable gives a computed value a name, which helps the reader. It also has a cost: the value exists only inside one function. Another method that needs the same value must compute it again, and a long function full of such variables is hard to split, because each part that is extracted needs the variables as parameters.

Replace temp with query moves the computation into a function, often a getter of the class, and replaces the variable with calls of that function. The catalogue card shows a temp `basePrice`, computed as `this._quantity * this._itemPrice`, becoming a getter `basePrice` that the code then calls.[^1] Now the value has a name that every method of the class can use, and an [[extract-function|Extract Function]] on the code that used the temp needs no parameter for it.[^2]

The book notes that the refactoring fits a class well, where the query has the fields of the object as its context, and that it is for variables that are computed once and then only read.[^2] The first edition has it among the refactorings for composing methods.[^3]

## Steps

1. Run the tests and see them pass.
2. Check that the variable is fully computed before it is used, and that the code that computes it gives the same value each time it runs.[^2]
3. If the variable is not read-only, make it so, and test.
4. Extract the right side of the assignment into a function. If the variable and the function cannot have the same name, give the function a temporary name.
5. Check that the function has no side effects. If it has, separate the query from the modifier first.[^2]
6. Test.
7. Use [[inline-variable|Inline Variable]] to replace the variable with calls of the function, and test.

## Example

A rental computes a subtotal and a fee in local variables. A second method, the receipt, needs the same values and computes them again. After the refactoring, both use the queries.

```javascript run id=ask-for-it
import assert from "node:assert/strict";

// Before: temps inside one method; the receipt repeats the rules.
class RentalBefore {
  constructor(days, dailyRate) { this.days = days; this.dailyRate = dailyRate; }
  total() {
    const subtotal = this.days * this.dailyRate;
    const fee = subtotal > 500 ? 0 : 25;
    return subtotal + fee;
  }
  receipt() {
    const subtotal = this.days * this.dailyRate; // the rule again
    const fee = subtotal > 500 ? 0 : 25;
    return `${subtotal} + ${fee}`;
  }
}

// After: queries that every method can ask.
class Rental {
  constructor(days, dailyRate) { this.days = days; this.dailyRate = dailyRate; }
  get subtotal() { return this.days * this.dailyRate; }
  get fee() { return this.subtotal > 500 ? 0 : 25; }
  total() { return this.subtotal + this.fee; }
  receipt() { return `${this.subtotal} + ${this.fee}`; }
}

for (const [d, r] of [[2, 100], [6, 100], [5, 100]]) {
  const a = new RentalBefore(d, r);
  const b = new Rental(d, r);
  assert.equal(b.total(), a.total());
  assert.equal(b.receipt(), a.receipt());
}
assert.equal(new Rental(6, 100).receipt(), "600 + 0");
console.log("replace temp with query: tests pass");
```

```output
replace temp with query: tests pass
```

The rule for the fee now has one home. A change to it, such as a different threshold, changes `total` and `receipt` together.

| Language | What the example shows |
|---|---|
| JavaScript | getters that replace the temps, as above |
| Python | `@property` queries, and `functools.cached_property` when the value is costly and the object does not change |
| Go | methods as queries on a struct |

## When to use

- **Another function needs the same value.** A query removes the repeated computation.
- **Before an Extract Function.** A temp that the extracted code uses would become a parameter. A query does not.[^2]
- **In a [[long-function|Long Function]].** Temps are one of the things that hold a long function together. Replacing them is often the first step of splitting it.

## Pitfalls

**A snapshot.** Some variables exist to keep a value from a moment in time, before a change. A query computes the value when it is called, so it gives the value after the change. Keep the variable in that case.[^2]

**A query with side effects.** If the computation changes state, calling it several times changes the state several times. Separate the query from the modifier before you replace the temp.

**Cost.** A query that is called several times computes the value several times. For a cheap expression, this does not matter. For a costly one, cache the result in the object if the data does not change, or keep the variable.

**A variable that is assigned more than once.** A temp that is set, then changed in a loop, is an accumulator, not a computed value. Split the variable first, or leave it.

## Tools that do it

This refactoring is two tool steps: an extract of the expression into a function or a method, and an inline of the variable. The tools of the other pages of this area do the extraction. In IntelliJ IDEA, Inline Variable "replaces redundant variable usage with its initializer", so it is applied after the extraction, when the initializer is a call of the query.[^4]

## References
