---
tag: YSP8Q9
kind: technique
slug: introduce-special-case
name: Introduce special case
aliases: [introduce null object, special case, special case object, null object]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When many callers check for the same special value, such as a missing customer, and then do the same thing, make an object for the special case that gives that behavior, and remove the checks.
family: refactoring
polarity: positive
intent: "Put the repeated handling of a special value into one special-case object."
also_known_as: [Introduce Null Object]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has Introduce Null Object in the chapter on simplifying conditional expressions. The second edition (2018) names it Introduce Special Case, a name that also covers special cases other than a missing value, and keeps the old name as an alias.
see_also: [null-object-pattern, null-references, replace-conditional-with-polymorphism, duplicated-code, repeated-switches]
sources:
  - url: https://refactoring.com/catalog/introduceSpecialCase.html
    title: Martin Fowler, Catalog of Refactorings, Introduce Special Case
    locator: the entry; alias Introduce Null Object
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Introduce Null Object
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Introduce Special Case, motivation, mechanics and the examples with a class, an object literal and a transform
---

## Purpose

Some code gets a value that can be a special case: a customer that is not known, an account that is closed, a reading that is missing. Each caller checks for the special case, and many of them do the same thing when they find it. The check and the response are duplicated in each caller.

Introduce special case makes one object that stands for the special case, with the same interface as the normal object. Its methods give the response that the callers gave. The callers then call the methods and do not check. The catalogue card replaces a check for the string `"unknown"` with a class `UnknownCustomer` whose `name` is `"occupant"`.[^1] The first edition named the refactoring Introduce Null Object, after the special case of a missing value.[^2] The object-oriented design pattern is described in [[null-object-pattern|Null object pattern]].

## Steps

1. Add to the normal type a property that says whether a value is the special case, and make it false.[^3]
2. Make a special-case object or class, with the same property set to true.
3. Extract the check for the special value into a function, and change each caller to use it.
4. Change the code that makes the special value so that it makes the special-case object.
5. Change the check function to use the property.
6. For each response that the callers share, move it into the special-case object, and remove it from the callers. Test after each change.
7. When no caller needs the check, consider removing it.

## Example

A billing system gets a customer by id. A missing customer comes back as `null`, and each caller checks. After the refactoring, a missing id gives a `GuestCustomer` with the answers that the callers used.

```javascript run id=one-special-object
import assert from "node:assert/strict";

const customers = new Map([["c1", { name: "Ines Moreau", plan: "pro", discountPercent: 10 }]]);

function labelBefore(id) {
  const c = customers.get(id) ?? null;
  return c === null ? "guest" : c.name;
}
function discountBefore(id) {
  const c = customers.get(id) ?? null;
  return c === null ? 0 : c.discountPercent;
}

class Customer {
  constructor({ name, plan, discountPercent }) {
    Object.assign(this, { name, plan, discountPercent });
  }
  get isGuest() { return false; }
}
class GuestCustomer {
  get name() { return "guest"; }
  get plan() { return "free"; }
  get discountPercent() { return 0; }
  get isGuest() { return true; }
}
const GUEST = Object.freeze(new GuestCustomer());

function customer(id) {
  const data = customers.get(id);
  return data === undefined ? GUEST : new Customer(data);
}
const label = (id) => customer(id).name;
const discount = (id) => customer(id).discountPercent;

for (const id of ["c1", "c9"]) {
  assert.equal(label(id), labelBefore(id));
  assert.equal(discount(id), discountBefore(id));
}
assert.equal(customer("c9").isGuest, true);
console.log("introduce special case: tests pass");
```

```output
introduce special case: tests pass
```

The callers `label` and `discount` no longer check. A caller that must treat a guest in its own way can still ask `isGuest`.

| Language | What the example shows |
|---|---|
| JavaScript | a special-case class with the same getters as the customer, as above |
| Python | a special-case class and a protocol that both classes meet |
| Go | an interface with two implementations, so that no caller tests for `nil` |

## When to use

- **Many callers with the same check.** The duplicated `if (customer === null)` with the same response in each place is what the refactoring removes.
- **A special value that is a string or a number.** A check for `"unknown"`, `-1` or an empty string becomes a check of a named object.[^1]
- **Data that is read, not changed.** When the special case only supplies values, the second edition shows a special case as an object literal or a record, with no class.[^3]

## Pitfalls

**Callers that respond in different ways.** If each caller does something different with the special case, a special-case object has no single answer to give. Leave the checks, or give the object only the answers that callers share.

**Hidden errors.** A special-case object makes a missing value act like a real one. If a missing value is a bug, the object hides the bug. Raise an error, or return a type that the caller must check, such as an option type. See [[null-references|Null references]].

**Mutation.** A shared special-case object must not change. If a caller can set a field on it, every other caller sees the change. Freeze it, or make a fresh object each time.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
