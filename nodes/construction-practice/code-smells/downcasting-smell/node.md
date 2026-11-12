---
tag: Z3B3PQ
kind: pattern
slug: downcasting-smell
name: Downcasting
aliases: [downcast smell, instanceof chain]
area: construction-practice/code-smells
level: advanced
summary: >-
  A code smell: code tests the run-time class of an object and casts it to a subclass to reach what it needs, because the type it was given does not offer that operation.
family: code-smell
polarity: negative
intent: "Find the casts from a general type to a specific one, and give the general type the operation, or keep the specific type, so that the cast is not needed."
also_known_as: [type refinement]
refactorings: [replace-conditional-with-polymorphism, move-function]
history:
  year: 2020
  people: [Gavin Bierman]
  note: Wikipedia's list of code smells has Downcasting, and its article on downcasting names the containers of top types in Java before generics as a badly considered design. JEP 394, created in 2020 and owned by Gavin Bierman, made pattern matching for instanceof final in JDK 16, after previews in JDK 14 and JDK 15.
see_also: [code-smell, subtyping, dynamic-dispatch, type-conversion, parametric-polymorphism, sum-types, pattern-matching, repeated-switches]
sources:
  - url: https://en.wikipedia.org/w/index.php?title=Downcasting&oldid=1383382579
    title: Wikipedia, Downcasting, revision 1383382579
    locator: the introduction, Uses and Considerations
  - url: https://en.wikipedia.org/w/index.php?title=Code_smell&oldid=1379558939
    title: Wikipedia, Code smell, revision 1379558939
    locator: Common code smells, Downcasting
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Type assertions
  - url: https://openjdk.org/jeps/394
    title: JEP 394, Pattern Matching for instanceof
    locator: Summary, History and Motivation
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Replace Conditional with Polymorphism and Move Function
---

## Intent

Find the places where code takes an object of a general type, checks which subclass it really is, and casts it to that subclass to read a field or call a method. Each such place says that the general type lacks something the code needs. Give the general type that operation, or keep the specific type in the variable from the start, so that the cast is not needed.

## Problem

Wikipedia defines downcasting as "the act of casting a base or parent class reference, to a more restricted derived class reference", and notes that it "is only allowable if the object is already an instance of the derived class, and so this conversion is inherently fallible."[^1] The opposite move, an upcast, "is always possible."[^1]

Its list of code smells describes the smell in one line: "a type cast which breaks the abstraction model; the abstraction may have to be refactored or eliminated."[^2] The code was given a `Notice`, but it needs an `EmailNotice`. Either `Notice` should offer what the code needs, or the code should never have lost the more specific type.

The forces are these:

- **The check moves from compile time to run time.** Wikipedia's example shows a Java method that casts an `Object` to a `String`, and says that "downcasting prevents the compiler from detecting a possible error and instead causes a run-time error."[^1]
- **Each cast is a list of the subclasses.** Code that tests for each subclass in turn must be changed when a subclass is added, and the code that forgets one fails at run time. This is the same cost as [[repeated-switches|Repeated Switches]], with the class as the switch value.
- **Containers of the top type force it.** Wikipedia gives as its example of a badly considered design the "containers of top types", such as the Java containers before generics, which require "downcasting of the contained objects so that they can be utilised again."[^1]
- **The language can make the cast safe but not remove the need.** The Go specification says of a type assertion `x.(T)`: "If the type assertion is false, a run-time panic occurs." The form `v, ok := x.(T)` gives a boolean in place of the panic, and "No run-time panic occurs in this case."[^3] JEP 394 adds a type pattern to Java's `instanceof`, because the instanceof-and-cast idiom is "tedious" and "the repetition provides opportunities for errors to creep unnoticed into programs."[^4] These forms make each cast safer. The design question, why the code needs a cast at all, stays.

## Solution

Choose the fix by why the code lost the type.

1. **The operation belongs on the general type.** Each subclass knows how to do it in its own way. Add the operation to the base class or interface, and let each subclass implement it. This is [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]], with the type test as the conditional; the catalogue at refactoring.com has a card for it.[^5] The call is then a [[dynamic-dispatch|dynamic dispatch]].
2. **The code that needs the specific type is in the wrong place.** Move it into the subclass with [[move-function|Move Function]], or call it at the point where the specific type is still known.[^5]
3. **A container or a callback lost the type.** Use a generic type, so that a list of `Email` stays a list of `Email`. See [[parametric-polymorphism|Parametric polymorphism]].
4. **The set of cases is closed and known.** Some data is a fixed set of shapes, such as the nodes of a syntax tree or the results of a parser. Make it a [[sum-types|sum type]] (a sealed interface in Java, a union of tagged objects in TypeScript), and use [[pattern-matching|pattern matching]] that the compiler checks for missing cases. This is a type test by design, not a smell.

Some casts stay. Code at the edge of a program, which reads JSON, a message from a queue or an object from a plugin, gets a value whose type it cannot know ahead. A check of the type there is a validation, and it should happen once, at the edge, so that the code inside works with the checked type.

## Example

A program sends notices. The function that writes the log line takes a `Notice`, tests for each subclass, and reads the field that each subclass has. When the team adds push notices, the function throws at run time, and nothing warned them.

```javascript run id=give-the-base-the-method
import assert from "node:assert/strict";

// Before: the base class has no operation, so the client tests and reaches into subclasses.
class NoticeBefore {}
class EmailNoticeBefore extends NoticeBefore {
  constructor(address) { super(); this.address = address; }
}
class SmsNoticeBefore extends NoticeBefore {
  constructor(phone) { super(); this.phone = phone; }
}
class PushNoticeBefore extends NoticeBefore {
  constructor(device) { super(); this.device = device; }
}
function logLineBefore(notice) {
  if (notice instanceof EmailNoticeBefore) return `email to ${notice.address}`;
  if (notice instanceof SmsNoticeBefore) return `sms to ${notice.phone}`;
  throw new TypeError(`unknown notice ${notice.constructor.name}`);
}

// After: each class says how it is described, and the client calls one method.
class Notice {
  describe() { throw new Error("subclass must implement describe"); }
}
class EmailNotice extends Notice {
  constructor(address) { super(); this.address = address; }
  describe() { return `email to ${this.address}`; }
}
class SmsNotice extends Notice {
  constructor(phone) { super(); this.phone = phone; }
  describe() { return `sms to ${this.phone}`; }
}
class PushNotice extends Notice {
  constructor(device) { super(); this.device = device; }
  describe() { return `push to ${this.device}`; }
}
const logLine = (notice) => notice.describe();

assert.equal(logLineBefore(new EmailNoticeBefore("ada@example.com")), "email to ada@example.com");
assert.equal(logLine(new EmailNotice("ada@example.com")), "email to ada@example.com");
assert.equal(logLine(new SmsNotice("+15550100")), logLineBefore(new SmsNoticeBefore("+15550100")));
assert.throws(() => logLineBefore(new PushNoticeBefore("tablet-7")), TypeError); // the subclass that the chain forgot
assert.equal(logLine(new PushNotice("tablet-7")), "push to tablet-7");
console.log("notices: tests pass");
```

```output
notices: tests pass
```

In the second version, a class that is added must say how it is described, and the log function does not change. In JavaScript the reminder is a method that throws; in Java and Go, as the examples show, the compiler gives the reminder.

| Language | What the example shows |
|---|---|
| JavaScript | an `instanceof` chain, replaced by a method on each class, as above |
| Python | an `isinstance` chain, replaced by an abstract method with `abc` |
| Java | a raw `List` with casts and a `ClassCastException`, replaced by a generic list, and a cast chain replaced by an interface method |
| Go | type assertions on an interface, replaced by a method in the interface |

## Consequences

The operation has one name, and each type keeps its own version of it. A reader of the client sees one call, and a type that is added cannot be forgotten by a chain of tests that it does not know about.

The base type grows. Each operation that a client needs becomes a method on the base type, and a base type with many methods that only one client uses is a [[large-class|Large Class]] in the making. When many unrelated operations must work over a fixed set of subclasses, the operations may belong outside the classes. The [[visitor-pattern|Visitor pattern]] and a [[sum-types|sum type]] with pattern matching both keep the operations together and keep the compiler's check for missing cases.

The two fixes trade in opposite directions. With methods on the classes, a class costs little to add and an operation costs a change to each class. With a sum type and pattern matching, an operation costs little to add and a case costs a change to each match. Choose by which of the two changes more often.

## How to fix it

- **The cast reaches a behaviour that each subclass has in its own way:** move the behaviour onto the base type with [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]].[^5]
- **The cast reaches a behaviour that only one subclass has:** move the code that needs it into that subclass with [[move-function|Move Function]], or call it where the subclass is still known.[^5]
- **The cast undoes a container of the top type:** use a generic container.[^1]
- **The cases are closed:** use a sum type and a checked pattern match.
- **The cast is at the edge of the program:** keep it, check once, and pass the checked type inward.

## Variants and related patterns

**Repeated Switches.** A chain of type tests is a switch on the class. See [[repeated-switches|Repeated Switches]].

**Refused Bequest.** A subclass that does not support what its parent offers forces the clients to test for it. See [[refused-bequest|Refused Bequest]].

**Type conversion.** A downcast is one kind of conversion between types. See [[type-conversion|Type conversion]].

## References
