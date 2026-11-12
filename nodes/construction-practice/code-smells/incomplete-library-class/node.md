---
tag: SFDWDC
kind: pattern
slug: incomplete-library-class
name: Incomplete Library Class
aliases: []
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a class from a library lacks a method that the program needs, the library cannot be changed, and the missing code is written again in each place that needs it.
family: code-smell
polarity: negative
intent: "Write the missing operation once, next to the library type, as a foreign function or a local extension, so that the clients call it in place of repeating it."
also_known_as: []
refactorings: [introduce-foreign-method, introduce-local-extension, move-function]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Incomplete Library Class is a smell of the first edition of Refactoring (1999), with the refactorings Introduce Foreign Method and Introduce Local Extension. The second edition (2018) drops the smell and the two refactorings.
see_also: [code-smell, duplicated-code, adapter-pattern, feature-envy, class-inheritance]
sources:
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Incomplete Library Class; Chapter 7, Introduce Foreign Method and Introduce Local Extension
  - url: https://en.wikipedia.org/w/index.php?title=Monkey_patch&oldid=1374249116
    title: Wikipedia, Monkey patch, revision 1374249116
    locator: the introduction and Pitfalls
  - url: https://en.wikipedia.org/w/index.php?title=Extension_method&oldid=1340525475
    title: Wikipedia, Extension method, revision 1340525475
    locator: the introduction
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/extension-methods
    title: Microsoft Learn, Extension members (C# programming guide)
    locator: the introduction
  - url: https://kotlinlang.org/docs/extensions.html
    title: Kotlin documentation, Extensions
    locator: the introduction and the section on static resolution
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method declarations
---

## Intent

When a type from a library lacks an operation that the program needs, and the library cannot be changed, write the operation once. Put it in a function that takes the library object as its first argument, or in a small type of the program's own that wraps or extends the library type. Then the clients call that one operation, and do not each repeat the code.

## Problem

A library is written by people who cannot know every program that will use it. Sooner or later a program needs an operation that the library's class does not have: the next working day of a date, the median of a list, a case-insensitive key in a map. The usual fix for a missing method, moving it into the class, does not work, because the class is not the program's to change.

The first edition of *Refactoring* names this smell. It notes that the builders of libraries cannot foresee everything, that the library class usually cannot be modified, and that this makes the normal move of a method into the class impossible.[^1]

The forces are these:

- The operation is short, so each client writes it inline. The copies drift, which is [[duplicated-code|Duplicated Code]].
- Each inline copy reads the library object's data more than its own, which is [[feature-envy|Feature Envy]] toward a class that cannot take the method.
- Some languages let a program add a method to a class it does not own. Wikipedia calls this a monkey patch when it is done at run time, and lists its pitfalls: a release of the patched software "may break the patch", and the patch "creates a discrepancy between the source code and actual behavior that can confuse developers".[^2]
- A wrapper or a subclass of the library type is a type that other libraries do not know about. Each place where the program meets those libraries needs a conversion.

## Solution

The first edition gives two refactorings, by the number of missing operations.[^1]

1. **One or two operations:** Introduce Foreign Method. Write a function in the client code that takes the library object as its first argument and does the missing work. The first edition suggests a comment that marks it as a method that belongs in the library class.
2. **Many operations:** Introduce Local Extension. Make a class of the program's own, either a subclass of the library class or a wrapper around it, and put all the missing operations there.

Several languages give the foreign method a direct form. Wikipedia describes an extension method as "a method added to an object after the original object was compiled", and notes that "There is no syntactic difference between calling an extension method and calling a method declared in the type definition."[^3] The C# guide says that extension members let a program "add" methods "to existing types without creating a new derived type, recompiling, or otherwise modifying the original type".[^4] The Kotlin documentation adds the limit: extensions are resolved "statically, meaning the compiler determines which function to call based on the receiver type at compile time".[^5] An extension is a foreign method with a better syntax, not a change to the class.

Go does not allow a method on a type from another package. The specification says that the receiver base type "must be declared in the same package as the method".[^6] A Go program uses a function, or a defined type of its own whose underlying type is the library type.

## Example

Three parts of a program each find the next working day after a date. The built-in `Date` has no such method, so each part has its own copy of the loop, and one of them forgets that Saturday is not a working day.

```javascript run id=write-it-once
import assert from "node:assert/strict";

const day = (s) => new Date(`${s}T00:00:00Z`);
const iso = (d) => d.toISOString().slice(0, 10);

// Before: each client writes the missing operation inline.
function dueDateBefore(invoiceDate) {
  const d = new Date(invoiceDate);
  do d.setUTCDate(d.getUTCDate() + 1); while (d.getUTCDay() === 0 || d.getUTCDay() === 6);
  return d;
}
function shipDateBefore(orderDate) {
  const d = new Date(orderDate);
  do d.setUTCDate(d.getUTCDate() + 1); while (d.getUTCDay() === 0); // forgot Saturday
  return d;
}

// After: a foreign function, written once, that takes the library object first.
// It belongs on Date, which the program cannot change.
function nextWorkday(date) {
  const d = new Date(date);
  do d.setUTCDate(d.getUTCDate() + 1); while (d.getUTCDay() === 0 || d.getUTCDay() === 6);
  return d;
}
const dueDate = (invoiceDate) => nextWorkday(invoiceDate);
const shipDate = (orderDate) => nextWorkday(orderDate);

for (const [from, to] of [["2026-11-13", "2026-11-16"], ["2026-11-14", "2026-11-16"], ["2026-11-17", "2026-11-18"]]) {
  assert.equal(iso(dueDate(day(from))), to);
  assert.equal(iso(dueDate(day(from))), iso(dueDateBefore(day(from))));
  assert.equal(iso(shipDate(day(from))), to);
}
assert.equal(iso(shipDateBefore(day("2026-11-13"))), "2026-11-14"); // the inline copy ships on a Saturday
console.log("nextWorkday: tests pass");
```

```output
nextWorkday: tests pass
```

In the second version the rule for working days is in one function, and the clients that need it call it. The function copies the date, so it does not change the caller's object, which the inline copies also had to remember.

| Language | What the example shows |
|---|---|
| JavaScript | a foreign function for `Date`, as above |
| Python | a foreign function for `datetime.date`, and a local extension as a subclass |
| Java | a foreign method in a utility class for `LocalDate`, and the `TemporalAdjuster` hook that the library offers |
| Go | a function for `time.Time`, and a defined type with methods as a local extension |

## Consequences

The missing operation exists once, with a name, and it can be tested on its own. A reader sees `nextWorkday(d)` and knows what it does.

A foreign function is less visible than a method. A programmer who looks for the operation in the library's documentation will not find it, and may write another copy. Keep the foreign functions for a library type in one module, named after the type, so that a programmer can find them.

A local extension changes the type. A subclass or a wrapper must be created from the library object, and converted back when the program passes it to code that expects the library type. This is the cost of the adapter idea; see [[adapter-pattern|Adapter pattern]]. Before writing either one, check the library for a hook that was made for this purpose. Java's `java.time` has one, as the Java example shows.

## How to fix it

- **One or two missing operations:** Introduce Foreign Method, or an extension method where the language has them.[^1]
- **Many missing operations:** Introduce Local Extension, as a subclass or a wrapper.[^1]
- **Operations that the library would accept:** propose them to the library's authors, and keep the foreign function until a release has them.
- **A monkey patch:** replace it with one of the above, unless the language community treats patches as the normal way to extend.[^2]

## Variants and related patterns

**Adapter pattern.** A local extension that wraps the library class is a kind of adapter. See [[adapter-pattern|Adapter pattern]].

**Duplicated Code.** The copies of the missing operation are the cost of this smell. See [[duplicated-code|Duplicated Code]].

**Feature Envy.** A foreign method envies the library class by design, because it cannot live there. See [[feature-envy|Feature Envy]].

## References
