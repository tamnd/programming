---
tag: 0Q82CG
kind: pattern
slug: repeated-switches
name: Repeated Switches
aliases: [switch statements]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: the same switch or if chain on the same kind of value appears in many functions, so each added case needs an edit in each of them.
family: code-smell
polarity: negative
intent: "Find the switches that test the same condition in many places, and replace them with one place that knows the cases, often a class for each case."
also_known_as: [Switch Statements]
refactorings: [replace-conditional-with-polymorphism, replace-type-code-with-subclasses]
history:
  year: 2018
  people: [Kent Beck, Martin Fowler]
  note: The first edition of Refactoring (1999) calls the smell Switch Statements. The second edition (2018) renames it Repeated Switches, because a single switch is not a smell.
see_also: [code-smell, shotgun-surgery, primitive-obsession, class-inheritance, strategy-pattern]
sources:
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Switch Statements
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Repeated Switches
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#match-statements
    title: The Python Tutorial, More Control Flow Tools, Python 3.14
    locator: 4.7 match Statements
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Type switches
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Replace Conditional with Polymorphism and Replace Type Code with Subclasses
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
    title: The Java Language Specification, Java SE 25 Edition, Chapter 14, Blocks, Statements, and Patterns
    locator: 14.11.1.1 Exhaustive Switch Blocks and 14.11.2 The Switch Block of a switch Statement
---

## Intent

Find the switches that test the same condition in many functions, and give the cases one home. The usual home is a class for each case, with a method for each function that used to switch. Then a case that is added is one class, and no function can miss it.

## Problem

A switch on its own is a normal control structure. The first edition of *Refactoring* named the smell Switch Statements.[^1] The second edition renames it Repeated Switches. It says that a switch was once treated as bad in itself, and that the smell is the same switch in many places.[^2]

The usual cause is a type code: a field, such as `kind` or `status`, that tells which kind of thing a value is. Each function that behaves differently for each kind switches on the field. *Refactoring* gives the result: when a case is added, the programmer must find all the switches and update each one.[^2]

The forces are these:

- The first switch is the natural way to write the code. The second and third are copies of the first, written where they were needed.
- Nothing links the switches. A search for the type code finds them, if the programmer knows to search.
- A switch with no default case, or with a default that returns a neutral value, does not fail when a case is missing. It gives a wrong answer.
- Languages differ in how much they help. The Python tutorial describes the `match` statement as one that "takes an expression and compares its value to successive patterns", and notes that "If no case matches, none of the branches is executed."[^3] The Go specification has a type switch, which "compares types rather than values."[^4] Neither one warns about a missing case.

## Solution

*Refactoring* names [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]] as the main fix.[^2] The catalogue at refactoring.com has the card, and the card for [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]], which is often the step before it.[^5]

1. If the cases are a type code, replace it with a class for each case.
2. For each function that switches on the code, add a method with the same name to each class. Move the code of each branch into the class of its case.
3. Replace the switch with a call to the method.

When the cases are data, not behaviour, a table is often enough. A map from each case to its values puts the cases in one place without classes.

## Example

Two functions switch on the `kind` of a shape. A triangle is added to the area function, but the perimeter function is not updated, and it returns 0 for a triangle with no error.

```javascript run id=switch-to-classes
import assert from "node:assert/strict";

// Before: each function switches on the kind.
function areaBefore(s) {
  switch (s.kind) {
    case "circle": return Math.PI * s.r ** 2;
    case "square": return s.side ** 2;
    case "triangle": return (s.base * s.height) / 2;
    default: return 0;
  }
}
function perimeterBefore(s) {
  switch (s.kind) {
    case "circle": return 2 * Math.PI * s.r;
    case "square": return 4 * s.side;
    default: return 0; // the triangle case was never added
  }
}

const t = { kind: "triangle", base: 3, height: 4, sides: [3, 4, 5] };
assert.equal(areaBefore(t), 6);
assert.equal(perimeterBefore(t), 0);
console.log("before: the perimeter of a triangle is 0");

// After: each kind is a class, and each class has all the methods.
class Circle {
  constructor(r) { this.r = r; }
  area() { return Math.PI * this.r ** 2; }
  perimeter() { return 2 * Math.PI * this.r; }
}
class Square {
  constructor(side) { this.side = side; }
  area() { return this.side ** 2; }
  perimeter() { return 4 * this.side; }
}
class Triangle {
  constructor(a, b, c) { this.sides = [a, b, c]; }
  area() {
    const [a, b, c] = this.sides;
    const s = (a + b + c) / 2;
    return Math.sqrt(s * (s - a) * (s - b) * (s - c));
  }
  perimeter() { return this.sides.reduce((x, y) => x + y, 0); }
}

const shapes = [new Circle(1), new Square(2), new Triangle(3, 4, 5)];
assert.deepEqual(shapes.map((s) => s.perimeter()), [2 * Math.PI, 8, 12]);
assert.deepEqual(shapes.map((s) => s.area()), [Math.PI, 4, 6]);
console.log("after: tests pass");
```

```output
before: the perimeter of a triangle is 0
after: tests pass
```

The second version has no switch. Each class answers for its own kind, so a class that is added must supply both methods, and a call to a missing method fails with a `TypeError` in place of a wrong number. The cases are in one place per kind, not one place per function.

| Language | What the example shows |
|---|---|
| JavaScript | a class for each shape, as above |
| Python | `match` on a string in the first version, and classes with an abstract base in the second |
| Java | a `switch` on a string, then a sealed interface with records; a `switch` over the sealed type must cover each case |
| Go | a `switch` on a string, then an interface with a method for each operation |

## Consequences

Each case is in one place, and a case that is added cannot be half done: a class that leaves out a method of the interface is a compile error in Java and Go, and an error at the call in JavaScript and Python. The code that uses the shapes no longer names the kinds.

The cost is the other direction of change. With classes, a kind that is added is one class, but an operation that is added is one method in each class. With switches, an operation that is added is one function, and a kind that is added is one case in each function. The right choice depends on which of the two changes more often. When the set of kinds is fixed and the operations grow, as in a compiler that walks a syntax tree, switches or the [[visitor-pattern]] can be the better design.

Some languages check switches for missing cases. In Java, a `switch` with patterns is an enhanced switch, and the language specification says that such a statement "must be exhaustive".[^6] Its example is a sealed interface with three permitted classes, where a switch with one case for each class needs no default.[^6] A switch over a sealed interface that leaves out a case does not compile. This keeps the switch form and removes the risk of a missed case, which is the real harm of the smell.

A single switch is not the smell. A factory that creates the right class from a string read from a file is often the one place where a switch on the kind belongs.

## How to fix it

- **The same switch on a type code in many functions:** [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]], then [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]].[^2]
- **Switches that only choose a value:** a table from each case to its values.
- **A fixed set of cases with growing operations:** keep the switches, and use a language check for missing cases where one exists, such as an exhaustive switch over a sealed type in Java.[^6]
- **The switch that creates the objects:** keep it, in one factory function.

## Variants and related patterns

**Shotgun Surgery.** Repeated switches are a common cause: a case that is added needs an edit in each switch. See [[shotgun-surgery|Shotgun Surgery]].

**Primitive Obsession.** A type code stored as a string or an integer is the usual subject of the switches. See [[primitive-obsession|Primitive Obsession]].

**Strategy.** A pattern that puts the variable behaviour in a separate object, chosen once. See [[strategy-pattern]].

**Visitor.** A pattern for the opposite trade-off: a fixed set of kinds, and operations that grow. See [[visitor-pattern]].

## References
