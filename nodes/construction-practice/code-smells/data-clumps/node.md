---
tag: ED0EC0
kind: pattern
slug: data-clumps
name: Data Clumps
aliases: [data clump]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: the same group of values travels together through many fields and parameter lists, but the program has no type that names the group or holds the rules about it.
family: code-smell
polarity: negative
intent: "Find the values that always appear together, give the group a type of its own, and then move the behaviour that uses the group into that type."
also_known_as: []
refactorings: [extract-class, introduce-parameter-object, preserve-whole-object]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Data Clumps is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, long-parameter-list, primitive-obsession, feature-envy]
sources:
  - url: https://martinfowler.com/bliki/DataClump.html
    title: Martin Fowler, Data Clump, bliki, 5 January 2006
    locator: the whole entry
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Data Clumps
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Extract Class, Introduce Parameter Object and Preserve Whole Object
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Data Clumps
---

## Intent

Find the values that the code always passes and stores together, and give the group a type with a name. Then move the rules about the group, such as checks and comparisons, into that type, so that each rule is written once.

## Problem

Some values only make sense as a group. A start date and an end date are a period. A street, a city and a postcode are an address. A host and a port are an endpoint. When the program has no type for the group, the values travel one by one: two fields in each class that needs them, and two parameters in each function that uses them.

Fowler states the fix as a joke in the bliki entry: "Whenever two or three values are gathered together - turn them into a $%#$%^ object."[^1] He describes how to see the smell: "You spot it when you constantly see the same few data items passed around together", and gives the example that "start and end are a good example of a data clump wanting to be a range."[^1]

The forces are these:

- Each value has a type already, such as a number, a string or a date, so the code compiles and runs without a type for the group.
- The rules about the group have no home. The rule that the start comes before the end, or the test that two periods overlap, is written again in each function that needs it.
- Parameter lists grow. Two periods are four parameters, and a call with two of them in the wrong order is still valid code.
- Fowler notes that "Often data clumps are primitive values that nobody thinks to turn into an object."[^1] This links the smell to [[primitive-obsession|Primitive Obsession]].

*Refactoring* gives a test for a clump: delete one of the values, and ask if the others still make sense. If they do not, the values are one thing, and the program needs an object for it.[^2]

## Solution

*Refactoring* gives the order of the steps.[^2] The catalogue at refactoring.com has a card for each refactoring.[^3]

1. **The values are fields of a class:** [[extract-class|Extract Class]] to move them into a class of their own.
2. **The values are parameters of functions:** [[introduce-parameter-object|Introduce Parameter Object]], to pass one object in place of the group.
3. **The caller already has an object that holds the values:** [[preserve-whole-object|Preserve Whole Object]], to pass that object in place of the values taken out of it.

The first edition of *Refactoring* gives the same three refactorings for this smell.[^4]

Fowler says that the first benefit is shorter parameter lists, and that "The interesting stuff happens as you begin to look for behavior to move into the new objects."[^1] *Refactoring* makes the same point: once the class exists, look for [[feature-envy|Feature Envy]], the functions that work on the values, and move them into it.[^2]

The type does not need to use all of its fields in each place. *Refactoring* says not to worry about a function that uses only some of the fields of the object, as long as two or more fields are replaced.[^2]

## Example

Two functions take a period as two day numbers. Each one checks the order again, and a call with the two pairs mixed up gives a wrong answer and no error. After the change, the period is a `DateRange` that checks its own order once, and the two rules are its methods.

```javascript run id=date-range
import assert from "node:assert/strict";

// Before: each period is two loose numbers.
function overlapsBefore(aStart, aEnd, bStart, bEnd) {
  return aStart <= bEnd && bStart <= aEnd;
}
function containsBefore(start, end, day) {
  return start <= day && day <= end;
}

assert.equal(overlapsBefore(1, 5, 4, 9), true);
assert.equal(overlapsBefore(1, 4, 5, 9), false);
assert.equal(overlapsBefore(1, 5, 9, 4), false); // the second period is reversed, and no one is told
console.log("before: a reversed period gives a wrong answer");

// After: the clump is a type, and the rules live in it.
class DateRange {
  constructor(start, end) {
    if (end < start) throw new RangeError(`range ${start}..${end} ends before it starts`);
    this.start = start;
    this.end = end;
  }
  contains(day) {
    return this.start <= day && day <= this.end;
  }
  overlaps(other) {
    return this.start <= other.end && other.start <= this.end;
  }
}

const week = new DateRange(1, 5);
assert.equal(week.overlaps(new DateRange(4, 9)), true);
assert.equal(week.overlaps(new DateRange(6, 9)), false);
assert.equal(week.contains(5), true);
assert.equal(week.contains(6), false);
assert.equal(containsBefore(1, 5, 5), week.contains(5));
assert.throws(() => new DateRange(9, 4), RangeError);
console.log("after: tests pass");
```

```output
before: a reversed period gives a wrong answer
after: tests pass
```

The second version has one place for the rule that a period does not end before it starts. A reader of `week.overlaps(other)` sees two periods, not four numbers. When a third rule is needed, such as the length of a period, it has an obvious home.

| Language | What the example shows |
|---|---|
| JavaScript | a class with a checking constructor, as above |
| Python | a frozen dataclass with a `__post_init__` check |
| Java | a record with a compact constructor that checks the order |
| Go | a struct with a constructor function that returns an error |

## Consequences

The group has a name, and the name is in the signatures. Functions that take the group have shorter parameter lists. The rules about the group are written once and tested once. A value of the type is valid by construction, so the functions that receive it do not need to check it again.

The cost is one more type, and a conversion at the edges of the program. Code that reads two columns from a database or two fields from a request must build the object, and code that writes them must take it apart again.

Not every pair of values that appears together is a clump. The test from *Refactoring* helps: if one value still makes sense without the other, the two are separate values that happen to be used in the same place.[^2] The x and y of a point fail the test, so they are a clump. A user name and a page size in the same request pass it, so they are not.

## How to fix it

- **The same fields in several classes:** [[extract-class|Extract Class]].[^2]
- **The same parameters in several functions:** [[introduce-parameter-object|Introduce Parameter Object]].[^2]
- **Values taken out of an object and passed one by one:** [[preserve-whole-object|Preserve Whole Object]].[^2]
- **Functions that work on the group:** move them into the type, which removes the [[feature-envy|Feature Envy]] that the clump caused.[^2]

## Variants and related patterns

**Long Parameter List.** A data clump in the parameters is one of the common causes of a long list. See [[long-parameter-list|Long Parameter List]].

**Primitive Obsession.** A group of primitive values with no type is a primitive obsession for a compound value. See [[primitive-obsession|Primitive Obsession]].

**Value objects.** The type that replaces a clump is often a small immutable value, such as a range or an amount of money, compared by its fields. See [[value-object|Value object]].

## References
