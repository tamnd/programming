---
tag: ZY7MSK
kind: technique
slug: replace-primitive-with-object
name: Replace primitive with object
aliases: [Replace Data Value with Object, Replace Type Code with Class, value object]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Wrap a string or a number that stands for a concept of the domain, such as an email address or an amount of money, in a small class that holds the rules of that concept.
family: refactoring
polarity: positive
intent: "Give a value of the domain its own type, so that its rules have one home."
also_known_as: [Replace Data Value with Object, Replace Type Code with Class]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) had Replace Data Value with Object and Replace Type Code with Class. The second edition (2018) joins them as Replace Primitive with Object.
see_also: [primitive-obsession, encapsulate-record, introduce-parameter-object, encapsulate-variable, data-clumps]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Replace Primitive with Object, motivation and mechanics; Chapter 3, Bad Smells in Code, Primitive Obsession
  - url: https://refactoring.com/catalog/replacePrimitiveWithObject.html
    title: Martin Fowler, Catalog of Refactorings, Replace Primitive with Object
    locator: the entry; aliases Replace Data Value with Object, Replace Type Code with Class
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 8, Organizing Data, Replace Data Value with Object and Replace Type Code with Class
  - url: https://martinfowler.com/bliki/ValueObject.html
    title: Martin Fowler, Value Object, bliki
    locator: the definition and the note on immutability
  - url: https://openjdk.org/jeps/395
    title: JEP 395, Records
    locator: Summary, and the standard members that a record class acquires automatically
  - url: https://docs.python.org/3/library/dataclasses.html
    title: Python documentation, dataclasses, Data Classes
    locator: frozen instances
---

## Purpose

Early in a program, a fact is often a plain value: a string for an email address, a number for a price, a string such as `high` for a priority. That is the right first choice. Later the value gathers rules. Emails compare without regard to case; a price has a currency; priorities have an order. With a plain value, each rule is written again at each place that needs it.[^1]

Replace primitive with object makes a small class for the value. At first the class only wraps the value, but it is a place to put the rules, and the rules then move in from the callers.[^1] The catalogue card shows a filter that compares `o.priority` with the strings `high` and `rush` becoming a filter that calls `o.priority.higherThan(...)` with a `Priority` object for `normal`.[^2] The card lists two first-edition names, Replace Data Value with Object and Replace Type Code with Class, which the first edition had as separate refactorings.[^2][^3]

The class is usually a value object. Fowler defines value objects as "Objects that are equal due to the value of their properties", and says that "value objects should be immutable".[^4]

## Steps

1. Run the tests and see them pass.
2. If the variable is not yet behind accessors, apply [[encapsulate-variable|Encapsulate Variable]].[^1]
3. Write a value class. Its constructor takes the primitive value, and it has a getter for it.
4. Run the static checks.
5. Change the setter to make an instance of the class and store it in the field. Change the getter to return the primitive from the instance. Test.
6. Consider renaming the accessors, with [[change-function-declaration|Change Function Declaration]], so that they say what they return.
7. Move each rule about the value from its callers into the class, and test after each.
8. Decide whether the object is a value, compared by its contents, or a reference, with one shared instance per thing.[^1]

## Example

User accounts store email addresses as strings, and the code lowercases them in each place that compares them. After the refactoring, an `Email` class checks the value once, compares by value and answers the question about the domain.

```javascript run id=give-it-a-type
import assert from "node:assert/strict";

// Before: the rules are repeated wherever an email is used.
const sameUserBefore = (a, b) => a.trim().toLowerCase() === b.trim().toLowerCase();
const isStaffBefore = (e) => e.trim().toLowerCase().endsWith("@example.org");

// After: a value object. It is checked once, frozen, and compared by value.
class Email {
  #value;
  constructor(text) {
    const v = String(text).trim().toLowerCase();
    if (!/^[^@\s]+@[^@\s]+$/.test(v)) throw new TypeError(`not an email: ${text}`);
    this.#value = v;
    Object.freeze(this);
  }
  get domain() { return this.#value.slice(this.#value.indexOf("@") + 1); }
  equals(other) { return other instanceof Email && other.#value === this.#value; }
  toString() { return this.#value; }
}
const sameUser = (a, b) => a.equals(b);
const isStaff = (e) => e.domain === "example.org";

const pairs = [[" Ana@Example.org", "ana@example.org"], ["bo@x.io", "Bo@Y.io"]];
for (const [a, b] of pairs) {
  assert.equal(sameUser(new Email(a), new Email(b)), sameUserBefore(a, b));
}
assert.equal(isStaff(new Email("Cy@EXAMPLE.org")), isStaffBefore("Cy@EXAMPLE.org"));

// A bad value fails where it enters, not later where it is used.
assert.throws(() => new Email("not-an-email"), TypeError);
// JavaScript compares objects by reference, so equals is needed.
assert.notEqual(new Email("a@b.c"), new Email("a@b.c"));
console.log("replace primitive with object: tests pass");
```

```output
replace primitive with object: tests pass
```

The rule for comparing emails now lives in one place. The last line shows a trap that the bliki describes: JavaScript "tests equality for js objects by looking at their references, ignoring the values they contain", so a value class needs its own `equals`.[^4]

| Language | What the example shows |
|---|---|
| JavaScript | an `Email` class with a check, `equals` and a derived `domain`, as above |
| Python | a frozen dataclass, which compares by value with no extra code |
| Java | a `record`, which compares by value and checks in its compact constructor |
| Go | a named type `type Email string` with methods, the lightest form of the refactoring |

## When to use

- **A primitive has rules.** Formatting, parsing, comparison, validation or arithmetic that is repeated at each use. This is the smell [[primitive-obsession|Primitive Obsession]], and the book names this refactoring as a fix for it.[^1]
- **A string stands for one of a fixed set of values.** A priority, a status, a type code. The class can give the order and the allowed values. If the values select behaviour, the next step is to replace the conditionals with subclasses or polymorphism.
- **Two values travel together.** An amount and a currency is one concept. That is also [[data-clumps|Data Clumps]], and [[introduce-parameter-object|Introduce Parameter Object]] is the related fix.

## Pitfalls

**Equality by reference.** In JavaScript and in Java classes, `==` or `===` on two objects compares references. A value class needs an `equals` method, or a language form that compares by value. Java records and Python dataclasses generate the comparison.[^5][^6]

**A mutable value.** If two accounts share one `Email` object and one changes it, both change. Make value classes immutable, as the bliki advises.[^4] A Python dataclass with `frozen=True` emulates immutability.[^6]

**A wrapper with no rules.** A class that only wraps a string adds a type and costs a little code at each boundary. The gain is in the check and the rules; if there are none yet, the wrapper is still useful when it stops two kinds of string, such as a user ID and an order ID, from being mixed up.

**The boundary.** Databases, JSON and HTTP carry primitives. Convert at the edge of the program, and use the value class inside it.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. A tool can generate the class, a record type or a dataclass; the move of each rule from its callers into the class is the programmer's step, with a test after each.

## References
