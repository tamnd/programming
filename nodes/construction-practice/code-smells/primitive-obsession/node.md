---
tag: 0AEHW0
kind: pattern
slug: primitive-obsession
name: Primitive Obsession
aliases: [stringly typed code]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a domain value, such as an amount of money or a phone number, is kept in a plain number or string, so its rules are spread through the code and the compiler cannot tell it apart.
family: code-smell
polarity: negative
intent: "Find the domain values that live in primitive types, and give each one a small type that holds its rules and that the compiler can tell apart from other values."
also_known_as: []
refactorings: [replace-primitive-with-object, replace-type-code-with-subclasses, replace-conditional-with-polymorphism, extract-class, introduce-parameter-object]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Primitive Obsession is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, data-clumps, value, value-equality-and-identity, encapsulation-and-visibility]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Primitive Obsession, and Chapter 7, Replace Primitive with Object
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Replace Primitive with Object, Replace Type Code with Subclasses, Replace Conditional with Polymorphism, Extract Class and Introduce Parameter Object
  - url: https://martinfowler.com/bliki/DataClump.html
    title: Martin Fowler, Data Clump, bliki, 5 January 2006
    locator: the second paragraph
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Type definitions
  - url: https://docs.python.org/3.14/library/typing.html#newtype
    title: Python documentation, typing, Support for type hints, Python 3.14
    locator: NewType
  - url: https://doc.rust-lang.org/book/ch20-02-advanced-traits.html
    title: The Rust Programming Language, Advanced Traits
    locator: Using Default Generic Type Parameters and Operator Overloading, and Implementing External Traits with the Newtype Pattern
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
    title: The Java Language Specification, Java SE 25 Edition, Chapter 8, Classes
    locator: 8.10 Record Classes
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Primitive Obsession
---

## Intent

Find the values of the problem domain that the program keeps in primitive types, such as numbers and strings, and give each one a small type of its own. The type holds the rules about the value, and the compiler or the type checker can then tell an amount of money from a count, or a user ID from an order ID.

## Problem

Every language gives a set of primitive types: integers, floating point numbers, strings and booleans. A program also deals with values that are more than that: an amount of money, a phone number, a temperature, a postcode. *Refactoring* says that programmers are often slow to make small types for such values, and use a number for money and a string for a phone number.[^1] It also names the extreme form, code where many values are strings, which it calls *stringly typed*.[^1]

The forces are these:

- A primitive is always there and costs nothing to write. A class for money seems like too much work for a value that fits in one integer.
- The rules for the value have no home. The check that a postcode has the right form, the rounding of money and the format for display are written again wherever they are needed, and the copies drift apart.
- Two values of the same primitive type are interchangeable to the compiler. A function that takes a price in cents and a quantity, both integers, accepts them in either order.
- A value with a unit, such as money with a currency or a length in metres, loses the unit when it is a plain number. Code that adds two such numbers cannot know if the units match.
- A type code, such as a string `"admin"` or `"guest"`, is a primitive that stands for a kind of thing. The code that depends on the kind becomes a `switch` on the string in each place.

## Solution

*Refactoring* names a refactoring for each form of the smell.[^1] The catalogue at refactoring.com has a card for each one, and gives Replace Data Value with Object and Replace Type Code with Class as other names for Replace Primitive with Object.[^2]

1. **A single value with rules:** [[replace-primitive-with-object|Replace Primitive with Object]]. Make a small class for the value, and move the checks, the formatting and the arithmetic into it.
2. **A type code that controls behaviour:** [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]], then [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]] to move each branch of the `switch` into a subclass.
3. **A group of primitives that always appear together:** [[extract-class|Extract Class]] and [[introduce-parameter-object|Introduce Parameter Object]]. Fowler notes that "Often data clumps are primitive values that nobody thinks to turn into an object."[^3] See [[data-clumps|Data Clumps]].

Most languages give a cheap way to make a distinct type from a primitive:

- In Go, a type definition creates "a defined type. It is different from any other type, including the type it is created from."[^4] A `type Cents int64` can have methods, and a plain `int64` variable cannot be passed where `Cents` is expected without a conversion.
- In Python, `NewType` creates "distinct types" for a static type checker. The documentation adds that "these checks are enforced only by the static type checker", and that at runtime the call "returns its argument unchanged."[^5]
- In Rust, a thin wrapper of an existing type in another struct "is known as the newtype pattern", and the wrapper is a tuple struct with one field. The Rust book says that "There is no runtime performance penalty for using this pattern, and the wrapper type is elided at compile time."[^6]
- In Java, a record class is "a restricted kind of class" for an aggregate of values, and gets its constructor, accessors, `equals` and `hashCode` from its components.[^7]

## Example

An order has lines, and each line has a price in cents and a currency code as a string. The total adds up the cents. When a line in another currency gets into the order, the total is a number with no meaning, and no code notices.

```javascript run id=money-type
import assert from "node:assert/strict";

// Before: money is a number of cents and a separate string.
function totalBefore(lines) {
  return lines.reduce((sum, line) => sum + line.cents, 0);
}
const formatBefore = (cents, currency) => `${(cents / 100).toFixed(2)} ${currency}`;

const lines = [
  { cents: 1250, currency: "EUR" },
  { cents: 800, currency: "EUR" },
  { cents: 500, currency: "USD" },
];
assert.equal(formatBefore(totalBefore(lines), "EUR"), "25.50 EUR"); // 5 dollars counted as 5 euros
console.log("before: the total mixes two currencies");

// After: Money holds the amount, the currency and the rules.
class Money {
  constructor(cents, currency) {
    if (!Number.isInteger(cents)) throw new TypeError(`cents must be an integer: ${cents}`);
    if (!/^[A-Z]{3}$/.test(currency)) throw new RangeError(`currency code ${currency}`);
    this.cents = cents;
    this.currency = currency;
    Object.freeze(this);
  }
  add(other) {
    if (other.currency !== this.currency) {
      throw new RangeError(`cannot add ${other.currency} to ${this.currency}`);
    }
    return new Money(this.cents + other.cents, this.currency);
  }
  toString() {
    return `${(this.cents / 100).toFixed(2)} ${this.currency}`;
  }
}

const total = (prices) => prices.reduce((sum, price) => sum.add(price));

const euros = [new Money(1250, "EUR"), new Money(800, "EUR")];
assert.equal(total(euros).toString(), "20.50 EUR");
assert.throws(() => total([...euros, new Money(500, "USD")]), RangeError);
assert.throws(() => new Money(12.5, "EUR"), TypeError);
assert.throws(() => new Money(100, "euro"), RangeError);
console.log("after: tests pass");
```

```output
before: the total mixes two currencies
after: tests pass
```

The second version moves three rules into one class: the amount is a whole number of cents, the currency is a three-letter code, and two amounts add only when their currencies match. The format for display is written once, in `toString`. The total of the mixed order is now an error at the point where the currencies meet.

| Language | What the example shows |
|---|---|
| JavaScript | a frozen `Money` class, as above |
| Python | a frozen dataclass with `__add__`, and `NewType` for an ID |
| Java | a record with a compact constructor and a `plus` method |
| Go | a struct with unexported fields, and a defined type `Cents` |

## Consequences

The rules about a value are in one place and are tested once. A value that passed the constructor is valid, so the functions that receive it need no checks of their own. Signatures say what they take: `charge(Money)` is clearer than `charge(int, String)`, and two arguments of different domain types cannot be swapped by mistake.

The type is also a place for behaviour to grow. *Refactoring*, in the chapter on Replace Primitive with Object, notes that a small class for a value often begins as a wrapper with no behaviour, and becomes useful as functions that need the value move into it.[^1]

The costs are small but real:

- Each type is more code, and the conversions at the edges of the program, such as reading from a database or writing JSON, must build and take apart the objects.
- An object for each value can use more memory than a primitive. The Go defined type and the Rust newtype avoid this cost.[^4][^6] A Python `NewType` avoids it too, but gives no check at runtime.[^5]
- A wrapper for every number is its own kind of noise. A loop counter or an index is a primitive and should stay one. The smell is a domain value with rules, not every integer.

## How to fix it

- **A value with checks or formatting written in many places:** [[replace-primitive-with-object|Replace Primitive with Object]].[^1][^8]
- **A string or integer code that selects behaviour:** [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]], then [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]].[^1]
- **Several primitives that always appear together:** [[extract-class|Extract Class]] or [[introduce-parameter-object|Introduce Parameter Object]].[^1]
- **Two IDs or quantities of the same primitive type that can be mixed up:** a distinct type for each, such as a Go defined type, a Rust newtype or a Python `NewType`.[^4][^6][^5]

## Variants and related patterns

**Data Clumps.** A group of primitives that travels together. The fix for both smells is a small type. See [[data-clumps|Data Clumps]].

**Repeated Switches.** A type code that is a string or an integer leads to a `switch` on it in many functions. See [[repeated-switches|Repeated Switches]].

**Value objects.** The small immutable types that replace primitives, compared by their fields. See [[value-object|Value object]] and [[value-equality-and-identity]].

**Mutable Data.** A value type is often made immutable, so that it can be shared safely. See [[mutable-data|Mutable Data]].

## References
