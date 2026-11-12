---
tag: 0P5NED
kind: technique
slug: pull-up-method
name: Pull up method
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Move a method that two or more subclasses define in the same way into their common superclass, and remove the copies.
family: refactoring
polarity: positive
intent: "Keep one copy of behavior that all subclasses share."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [push-down-method, pull-up-field, duplicated-code, class-inheritance, template-method-pattern]
sources:
  - url: https://refactoring.com/catalog/pullUpMethod.html
    title: Martin Fowler, Catalog of Refactorings, Pull Up Method
    locator: the entry, the Employee, Salesman and Engineer sketch; inverse of Push Down Method
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry pullUpMethod, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Pull Up Method
  - url: https://www.jetbrains.com/help/idea/pull-members-up.html
    title: JetBrains, IntelliJ IDEA documentation, Pull members up, push members down
    locator: the first paragraphs
  - url: https://go.dev/doc/effective_go
    title: The Go Programming Language, Effective Go
    locator: Embedding
  - url: https://doc.rust-lang.org/book/ch18-01-what-is-oo.html
    title: The Rust Programming Language, Chapter 18.1, Characteristics of Object-Oriented Languages
    locator: Inheritance as a Type System and as Code Sharing
---

## Purpose

When two subclasses have the same method, a change to the behavior must be made twice. Sooner or later one copy changes and the other does not. This is [[duplicated-code|Duplicated code]] inside a class hierarchy.

Pull up method moves the method into the superclass, and the subclasses inherit it. The catalogue card has a class `Employee` with two subclasses, `Salesman` and `Engineer`, and each subclass has its own `get name()`. After the refactoring, `get name()` is in `Employee`, and the subclasses have no copy.[^1] The card names the inverse, Push Down Method.[^1]

## Steps

1. Look at the methods. Make sure that they do the same thing. If they only look alike, first change them until their bodies are the same.
2. Check that each call and each field that the body uses is in the superclass too. If a field is only in the subclasses, pull it up first with [[pull-up-field|Pull Up Field]].
3. If the signatures differ, make them the same with Change Function Declaration.
4. Copy the method into the superclass.
5. Remove the method from one subclass, and test. Then do the same for each other subclass.

## Example

`Salesman` and `Engineer` both compute the annual cost from the monthly cost. After the refactoring, `Employee` has the one copy.

```javascript run id=pull-up
import assert from "node:assert/strict";

class Employee {
  constructor(name, monthlyCost) {
    this._name = name;
    this._monthlyCost = monthlyCost;
  }
  get name() { return this._name; }
  get monthlyCost() { return this._monthlyCost; }
  get annualCost() { return this.monthlyCost * 12; }
}

class Salesman extends Employee {}
class Engineer extends Employee {}

// Before, each subclass had its own copy of annualCost.
class SalesmanBefore extends Employee {
  get annualCost() { return this.monthlyCost * 12; }
}

const ana = new Salesman("Ana", 1000);
const bo = new Engineer("Bo", 1500);
assert.equal(ana.annualCost, new SalesmanBefore("Ana", 1000).annualCost);
assert.ok(!Object.hasOwn(Salesman.prototype, "annualCost"));
console.log(ana.annualCost, bo.annualCost);
```

```output
12000 18000
```

| Language | What the example shows |
|---|---|
| JavaScript | `annualCost` moved from the subclasses to `Employee`, as above |
| Python | the same move; the subclasses inherit the property from the base class |
| Go | not inheritance: Go has no subclasses, so the shared method goes on a struct that the other structs embed |

### In languages without class inheritance

Go and Rust do not have class inheritance, so the refactoring does not apply as written.

- **Go.** Effective Go says: "Go does not provide the typical, type-driven notion of subclassing, but it does have the ability to “borrow” pieces of an implementation by embedding types within a struct or interface."[^5] The Go form of Pull up method is to move the method to a struct that both types embed. The method is then promoted to each type.
- **Rust.** The Rust book says: "There is no way to define a struct that inherits the parent struct’s fields and method implementations without using a macro."[^6] The Rust form is a default method in a trait that both types implement, or a free function that both call.

## When to use

- **The subclasses have the same method.** The body, or the result, is the same in each one.
- **The methods are almost the same.** First make them the same, then pull the method up. If they differ in a small step, make that step a method of its own, keep the subclass versions of the step, and pull up the rest. The result is the [[template-method-pattern|Template method]] pattern.
- **After Extract Superclass.** The new superclass then takes the shared methods. See [[extract-superclass|Extract Superclass]].

## Pitfalls

**Not all subclasses need it.** A method in the superclass is a part of the interface of each subclass. If one subclass must not have the method, pulling it up gives that subclass behavior that it does not want. See [[push-down-method|Push Down Method]].

**Hidden differences.** Two methods with the same body can still give different results if they call a method that each subclass overrides. This can be correct, but test each subclass after the move.

## Tools that do it

IntelliJ IDEA has Pull Members Up. Its documentation says: "The Pull Members Up refactoring lets you move fields if two classes have the same field, or move methods if your subclasses have methods performing similar work."[^4] Most IDEs for Java, C# and TypeScript have a similar action.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
