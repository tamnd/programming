---
tag: WPBMPA
kind: technique
slug: replace-conditional-with-polymorphism
name: Replace conditional with polymorphism
aliases: [replace switch with polymorphism, replace type code with polymorphism]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When a switch on a type code is repeated in several functions, make a class for each case and move each branch into a method of its class, so that the call dispatches to the right code with no switch.
family: refactoring
polarity: positive
intent: "Let each case carry its own behavior, in place of a switch on its type."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Replace Conditional with Polymorphism is in the first edition of Refactoring (1999), in the chapter on simplifying conditional expressions, and has the same name in the second edition (2018).
see_also: [dynamic-dispatch, expression-problem, sum-types, pattern-matching, repeated-switches, introduce-special-case]
sources:
  - url: https://refactoring.com/catalog/replaceConditionalWithPolymorphism.html
    title: Martin Fowler, Catalog of Refactorings, Replace Conditional with Polymorphism
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Replace Conditional with Polymorphism
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Replace Conditional with Polymorphism, motivation, mechanics and the examples with a type code and with a variation on a base case
---

## Purpose

A switch on a type code says, in one place, what each kind of thing does. When the same switch occurs in several functions, a change to one kind, or an added kind, must find each switch. This is the [[repeated-switches|repeated switches]] smell.

Replace conditional with polymorphism makes a class for each kind and moves each branch of the switch into a method of that class. A call to the method then runs the code for the kind of object, by [[dynamic-dispatch|dynamic dispatch]], and the switch is gone. The catalogue card turns a switch on the type of a bird, which computes its plumage, into a class for each type of bird, each with a `plumage` getter.[^1] The refactoring is in both editions of the book.[^2][^3] The book also uses polymorphism for a variation: a base class has the usual behavior, and a subclass overrides only the case that is different.[^3]

The refactoring has a functional form. In a language with [[sum-types|sum types]] and [[pattern-matching|pattern matching]], the cases are the variants of one type, and each operation is one function that matches on the variant. The compiler can then report a match that misses a variant. The two forms make different changes small. With classes, an added kind is one added class, and an added operation is a method in each class. With a sum type, an added operation is one added function, and an added kind is a branch in each match. That trade-off is the [[expression-problem|expression problem]].

## Steps

1. If the classes do not exist yet, make a class for each case, and a factory function that returns the right class for a type code.[^3]
2. Change the callers to use the factory.
3. Move the function with the conditional into the base class, if it is not there.
4. In one subclass, write a method that overrides the function, and copy into it the branch for that case. Test.
5. Repeat for each case.
6. Leave the default case in the base class. If there is no default, make the base method abstract or make it throw.

## Example

A messaging service computes the cost of a message, and the delay before it is sent, with a switch on the channel in each function. After the refactoring, each channel is a class with both methods. The functional form, with a sum type and a match, is in the Rust example.

```javascript run id=each-case-its-code
import assert from "node:assert/strict";

function costBefore(m) {
  switch (m.channel) {
    case "email": return 0;
    case "sms": return Math.ceil(m.text.length / 160) * 7;
    case "push": return m.urgent ? 1 : 0;
    default: throw new Error(`unknown channel ${m.channel}`);
  }
}
function delayBefore(m) {
  switch (m.channel) {
    case "email": return 60;
    case "sms": return m.urgent ? 0 : 30;
    case "push": return 0;
    default: throw new Error(`unknown channel ${m.channel}`);
  }
}

class Message {
  constructor(text, urgent) { Object.assign(this, { text, urgent }); }
  get cost() { throw new Error("subclass must define cost"); }
  get delaySeconds() { return 0; }
}
class Email extends Message {
  get cost() { return 0; }
  get delaySeconds() { return 60; }
}
class Sms extends Message {
  get cost() { return Math.ceil(this.text.length / 160) * 7; }
  get delaySeconds() { return this.urgent ? 0 : 30; }
}
class Push extends Message {
  get cost() { return this.urgent ? 1 : 0; }
}
const CLASSES = { email: Email, sms: Sms, push: Push };
function message({ channel, text, urgent }) {
  const C = CLASSES[channel];
  if (!C) throw new Error(`unknown channel ${channel}`);
  return new C(text, urgent);
}

for (const channel of ["email", "sms", "push"]) for (const urgent of [false, true])
  for (const text of ["hi", "x".repeat(161)]) {
    const data = { channel, text, urgent };
    assert.equal(message(data).cost, costBefore(data));
    assert.equal(message(data).delaySeconds, delayBefore(data));
  }
assert.throws(() => message({ channel: "fax", text: "", urgent: false }));
console.log("replace conditional with polymorphism: tests pass");
```

```output
replace conditional with polymorphism: tests pass
```

The factory still has one switch, as a map from the channel name to a class. That switch is the only one: each operation that a channel needs is now a method in each class. `Push` uses the default delay from the base class.

| Language | What the example shows |
|---|---|
| JavaScript | a class for each channel and a factory, as above |
| Python | the class form, and the match form on frozen dataclasses with the `match` statement |
| Java | a sealed interface with records, and a `switch` on the type that the compiler checks for all cases |
| Go | an interface with a type for each channel |
| Rust | the functional form: an enum and a `match`, which does not compile if a variant is missing |

## When to use

- **The same switch in several places.** Without the refactoring, each added case changes every switch.
- **A base case with variations.** A subclass that overrides one method shows the difference from the usual behavior.[^3]
- **Behavior that depends on a type code.** A field such as `type`, `kind` or `channel` that the code tests in many places.

## Pitfalls

**One switch.** A single switch in one function is a clear and complete list of the cases. Classes for it spread the list over many places. Keep it.

**The wrong axis of change.** If added operations come more often than added kinds, the class form makes each operation a change to every class. The sum-type form with pattern matching fits that case better. See [[expression-problem|The expression problem]].

**A missed case.** With classes, a subclass that does not override an abstract method fails only at run time in a dynamic language. With a sum type in a language that checks matches, a missed variant is a compile error. In JavaScript or Python, a base method that throws makes the miss visible at once.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
