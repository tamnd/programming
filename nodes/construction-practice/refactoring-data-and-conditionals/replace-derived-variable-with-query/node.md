---
tag: BKXVH8
kind: technique
slug: replace-derived-variable-with-query
name: Replace derived variable with query
aliases: []
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  Replace a stored value that the code keeps in step with other data by a function that computes it from that data when asked, so that the two can never disagree.
family: refactoring
polarity: positive
intent: "Compute a value from its source data in place of storing a copy that must be kept up to date."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Replace Derived Variable with Query is in the chapter on organizing data of the second edition of Refactoring (2018).
see_also: [replace-temp-with-query, split-variable, introduce-assertion, remove-dead-code, mutable-data, encapsulate-collection]
sources:
  - url: https://refactoring.com/catalog/replaceDerivedVariableWithQuery.html
    title: Martin Fowler, Catalog of Refactorings, Replace Derived Variable with Query
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 9, Organizing Data, Replace Derived Variable with Query, motivation and mechanics; Chapter 3, Bad Smells in Code, Mutable Data
---

## Purpose

A derived variable stores a value that other data already determines: a total of the items in a cart, a count of the entries in a list, a discounted price. Each change to the source data must also update the derived variable. When one code path forgets, the two disagree, and the bug shows up far from where it was made.

Replace derived variable with query removes the stored copy and computes the value when asked. The catalogue card replaces a `_discountedTotal` that a setter adjusts with a getter that subtracts the discount from the base total.[^1] The book treats this as a way to reduce the scope of [[mutable-data|mutable data]], which it calls one of the largest sources of problems in software.[^2]

There is an exception. If the source data cannot change, a derived value computed once from it cannot go stale, and keeping it is safe. The refactoring is for derived values that the code keeps in step by hand.[^2]

## Steps

1. Find each place that updates the variable. If it is updated for different reasons, separate them first with [[split-variable|Split Variable]].[^2]
2. Write a function that computes the value from the source data.
3. Where the variable is read, add an assertion that the variable equals the function's result. See [[introduce-assertion|Introduce Assertion]]. Test.[^2]
4. Replace each read of the variable with a call to the function. Test.
5. Delete the variable and the code that updated it, with [[remove-dead-code|Remove Dead Code]]. Test.

Step 3 is the check: if the assertion fails in the tests, the stored value was already wrong somewhere, and you have found a bug.

## Example

A cart stores its total and updates it in `add`. A `remove` added later forgot to update it. The assertion of step 3 finds the bug, and the query fixes it by construction.

```javascript run id=no-stale-copy
import assert from "node:assert/strict";

class CartBefore {
  items = [];
  totalCents = 0;
  add(name, cents) { this.items.push({ name, cents }); this.totalCents += cents; }
  remove(name) { this.items = this.items.filter((i) => i.name !== name); } // forgot the total
}

class Cart {
  #items = [];
  add(name, cents) { this.#items.push({ name, cents }); }
  remove(name) { this.#items = this.#items.filter((i) => i.name !== name); }
  get totalCents() { return this.#items.reduce((s, i) => s + i.cents, 0); }
}

const before = new CartBefore();
before.add("tea", 350); before.add("cake", 400); before.remove("cake");
const query = before.items.reduce((s, i) => s + i.cents, 0);
assert.notEqual(before.totalCents, query); // the step 3 check finds the stale copy

const cart = new Cart();
cart.add("tea", 350); cart.add("cake", 400); cart.remove("cake");
assert.equal(cart.totalCents, 350);
console.log("replace derived variable with query: tests pass");
```

```output
replace derived variable with query: tests pass
```

The cart now has one fact, its items, and the total cannot disagree with them.

| Language | What the example shows |
|---|---|
| JavaScript | a stored total replaced by a getter, after the step 3 check finds a stale copy, as above |
| Python | the same change with a property, and `functools.cached_property` for an object whose items cannot change |
| Go | the same change with a method; the slice of items is the only state |

## When to use

- **A value that other data determines.** If you can compute it, you do not have to store it.
- **Code that keeps two things in step.** Every place that updates the source must also update the copy. That is where bugs come from.
- **A bug where a total or a count is wrong.** Replace the copy with a query, and the class of bug goes away.

## Pitfalls

**Cost.** Computing the value each time can be slow when the data is large and the value is read often. Measure first. If the cost matters, cache the result and clear the cache in the one place that changes the source data.

**Data that cannot change.** When the source data is immutable, a value derived from it once does not go stale, and the refactoring is not needed.[^2]

**Derived data across systems.** A copy in another service or a database table is not one variable. Removing it is a data migration, and the readers may be outside your code.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The assertion of step 3, run by the test suite, is the check that the change is safe.

## References
