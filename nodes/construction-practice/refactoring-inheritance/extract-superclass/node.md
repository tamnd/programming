---
tag: 30JVFJ
kind: technique
slug: extract-superclass
name: Extract superclass
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Make a new superclass for two classes that have features in common, and move the shared fields and methods into it.
family: refactoring
polarity: positive
intent: "Put what two classes share in one place."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [pull-up-method, pull-up-field, pull-up-constructor-body, collapse-hierarchy, duplicated-code, composition-over-inheritance]
sources:
  - url: https://refactoring.com/catalog/extractSuperclass.html
    title: Martin Fowler, Catalog of Refactorings, Extract Superclass
    locator: the entry, the Department, Employee and Party sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry extractSuperclass, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Extract Superclass
  - url: https://www.jetbrains.com/help/idea/extract-superclass.html
    title: JetBrains, IntelliJ IDEA documentation, Extract superclass
    locator: the first paragraph
---

## Purpose

Two classes can grow the same features without a common parent: the same field, the same getter, the same calculation. The copies must then be kept the same by hand.

Extract superclass makes a new class above both, and moves the shared features into it. The catalogue card has `Department`, with `totalAnnualCost`, `name` and `headCount`, and `Employee`, with `annualCost`, `name` and `id`. After the refactoring, a new class `Party` has `name` and `annualCost`, and both classes extend it. `Department` keeps `headCount`, and `Employee` keeps `id`.[^1]

## Steps

1. Make an empty superclass. Make both classes extend it.
2. Test.
3. Move the shared parts up one at a time: first the fields with [[pull-up-field|Pull Up Field]], then the constructor code with [[pull-up-constructor-body|Pull Up Constructor Body]], then the methods with [[pull-up-method|Pull Up Method]]. Test after each move.
4. Look at the methods that are left in the subclasses. If parts of them are the same, extract those parts and pull them up too.
5. Look at the callers. Some of them can now use the superclass type.

## Example

Before the refactoring, `Department` called its cost `totalAnnualCost`. The first step renames it to `annualCost`, so that the two classes have the same interface.

```javascript run id=extract-superclass
import assert from "node:assert/strict";

class Party {
  constructor(name) { this._name = name; }
  get name() { return this._name; }
}

class Employee extends Party {
  constructor(name, id, monthlyCost) { super(name); this._id = id; this._monthlyCost = monthlyCost; }
  get id() { return this._id; }
  get annualCost() { return this._monthlyCost * 12; }
}

class Department extends Party {
  constructor(name, staff) { super(name); this._staff = staff; }
  get headCount() { return this._staff.length; }
  get annualCost() { return this._staff.reduce((sum, e) => sum + e.annualCost, 0); }
}

const staff = [new Employee("Ana", "E1", 1000), new Employee("Bo", "E2", 1500)];
const parties = [...staff, new Department("Sales", staff)];
assert.ok(parties.every((p) => p instanceof Party));
console.log(parties.map((p) => `${p.name} ${p.annualCost}`));
```

```output
[ 'Ana 12000', 'Bo 18000', 'Sales 30000' ]
```

A caller can now treat each party in the same way. The two `annualCost` getters stay in the subclasses because their bodies differ.

| Language | What the example shows |
|---|---|
| JavaScript | a new class `Party` above `Employee` and `Department`, as above |
| Python | the same change; `Party` is an abstract base class, so a party without `annual_cost` cannot be made |
| Go | not inheritance: a `Party` struct that both types embed, and an interface for the shared method |

## When to use

- **Two classes with the same features.** This is [[duplicated-code|Duplicated code]] across classes.
- **Callers that must treat both classes in the same way.** A common superclass gives them one type.

## Pitfalls

**Delegation can be better.** In Java, C# and JavaScript, a class can have only one superclass. If the shared part is not a part of what the classes are, move it into a separate class that both use. See [[composition-over-inheritance|Composition over inheritance]]. If the superclass later proves wrong, [[replace-superclass-with-delegate|Replace Superclass with Delegate]] undoes it.

**Shared names, different ideas.** Two `name` fields can mean different things. Pull up only the features that mean the same thing in both classes.

**Go and Rust.** These languages have no class inheritance. In Go, put the shared fields in a struct that both types embed, and the shared method set in an interface. In Rust, use a trait for the shared methods and a struct field for the shared data.

## Tools that do it

IntelliJ IDEA has Extract Superclass. Its documentation says: "The Extract Superclass refactoring lets you create a superclass for an existing class."[^4] Most IDEs for Java and C# have a similar action.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
