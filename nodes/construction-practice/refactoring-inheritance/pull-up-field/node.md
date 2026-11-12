---
tag: G2P8YE
kind: technique
slug: pull-up-field
name: Pull up field
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Move a field that two or more subclasses declare and use in the same way into their common superclass.
family: refactoring
polarity: positive
intent: "Declare shared data once, in the class that all users share."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999) and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [push-down-field, pull-up-method, pull-up-constructor-body, duplicated-code]
sources:
  - url: https://refactoring.com/catalog/pullUpField.html
    title: Martin Fowler, Catalog of Refactorings, Pull Up Field
    locator: the entry, the Java sketch with Salesman and Engineer; inverse of Push Down Field
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry pullUpField, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Pull Up Field
  - url: https://www.jetbrains.com/help/idea/pull-members-up.html
    title: JetBrains, IntelliJ IDEA documentation, Pull members up, push members down
    locator: the first paragraphs
  - url: https://go.dev/doc/effective_go
    title: The Go Programming Language, Effective Go
    locator: Embedding
---

## Purpose

Subclasses are often written at different times, by different people. Two of them can end up with the same field under the same or a different name. Each subclass then has its own code to read and write the field, and the methods that use it cannot move to the superclass.

Pull up field declares the field once, in the superclass. The catalogue card shows it in Java: `Salesman` and `Engineer` each declare `private String name;`. After the refactoring, `Employee` declares `protected String name;`, and the subclasses have no declaration.[^1] The card names the inverse, Push Down Field.[^1]

The refactoring often comes before [[pull-up-method|Pull Up Method]], because a method can only move up when the fields that it uses are there.

## Steps

1. Look at each use of the field in each subclass. Make sure that it means the same thing in each one.
2. If the names differ, rename the fields to one name.
3. Declare the field in the superclass. In a language with access control, make it visible to the subclasses, for example `protected` in Java and C#.
4. Remove the field from the subclasses.
5. Test.

## Example

In JavaScript, a field is made when a constructor assigns to it. Each subclass set `this._name` in its own constructor. After the refactoring, the superclass sets it.

```javascript run id=pull-up-field
import assert from "node:assert/strict";

class EmployeeBefore {}
class SalesmanBefore extends EmployeeBefore {
  constructor(name) { super(); this._name = name; }
}
class EngineerBefore extends EmployeeBefore {
  constructor(name) { super(); this._name = name; }
}

class Employee {
  constructor(name) { this._name = name; }
  get name() { return this._name; }
}
class Salesman extends Employee {}
class Engineer extends Employee {}

assert.equal(new Salesman("Ana").name, new SalesmanBefore("Ana")._name);
assert.equal(new Engineer("Bo").name, new EngineerBefore("Bo")._name);
console.log([new Salesman("Ana"), new Engineer("Bo")].map((e) => e.name));
```

```output
[ 'Ana', 'Bo' ]
```

| Language | What the example shows |
|---|---|
| JavaScript | the assignment of `_name` moved to the constructor of `Employee`, as above |
| Python | the same move; the base class `__init__` sets the attribute |
| Go | not inheritance: the field moves to a struct that both types embed, and it is promoted to each one |

## When to use

- **Subclasses declare the same data.** Look for fields with the same type and a similar name, and check how each class uses them.
- **Before pulling up methods.** The methods that use the field can then move too.
- **After Extract Superclass.** See [[extract-superclass|Extract Superclass]].

## Pitfalls

**Same name, other meaning.** Two fields called `rate` can hold an hourly rate and a tax rate. Pulling them up joins two different ideas. Check the uses, not only the names.

**Wider visibility.** A field that was private in each subclass becomes protected in the superclass, so more code can change it. If the superclass can own all the code that changes the field, keep it private there and give the subclasses a getter.

**Go and Rust.** These languages have no class inheritance. In Go, embedding gives the same effect for fields. Effective Go says that Go can "“borrow” pieces of an implementation by embedding types within a struct or interface."[^5] In Rust, put the shared field in a struct that each type holds as a field.

## Tools that do it

IntelliJ IDEA has Pull Members Up, which "lets you move fields if two classes have the same field".[^4] Most IDEs for Java and C# have a similar action.

The catalogue lists the refactoring in the first and the second edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
