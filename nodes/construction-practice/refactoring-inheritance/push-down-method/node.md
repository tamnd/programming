---
tag: ZF7RND
kind: technique
slug: push-down-method
name: Push down method
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Move a method from a superclass into the one subclass, or the few subclasses, that use it.
family: refactoring
polarity: positive
intent: "Keep a method in the class whose behavior it is."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [pull-up-method, push-down-field, refused-bequest, class-inheritance]
sources:
  - url: https://refactoring.com/catalog/pushDownMethod.html
    title: Martin Fowler, Catalog of Refactorings, Push Down Method
    locator: the entry, the quota sketch; inverse of Pull Up Method
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry pushDownMethod, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Push Down Method
  - url: https://www.jetbrains.com/help/idea/pull-members-up.html
    title: JetBrains, IntelliJ IDEA documentation, Pull members up, push members down
    locator: Push members down
---

## Purpose

A method in a superclass is a part of the interface of each subclass. If only one subclass has a use for it, the other subclasses carry behavior that does not belong to them. A caller can call the method on an object where it makes no sense.

Push down method moves the method to the subclass that needs it. The catalogue card has `get quota` in `Employee`, with the subclasses `Engineer` and `Salesman`. After the refactoring, `get quota` is in `Salesman` only.[^1] The card names the inverse, Pull Up Method.[^1]

## Steps

1. Copy the method into each subclass that needs it.
2. Remove the method from the superclass. Test.
3. Remove the method from each subclass that does not need it. Test.

A caller must know that it has an object of the subclass. If some callers call the method through a variable of the superclass type, the method cannot move down. In that case, keep it, or change those callers first.

## Example

Only salesmen have a sales quota. Before the refactoring, an engineer also had a `quota` property.

```javascript run id=push-down
import assert from "node:assert/strict";

class Employee {
  constructor(name) { this._name = name; }
  get name() { return this._name; }
}

class Engineer extends Employee {}

class Salesman extends Employee {
  constructor(name, quota) { super(name); this._quota = quota; }
  get quota() { return this._quota; }
}

const ana = new Salesman("Ana", 50000);
const bo = new Engineer("Bo");
assert.equal(ana.quota, 50000);
assert.equal("quota" in bo, false);
console.log(ana.name, ana.quota);
```

```output
Ana 50000
```

| Language | What the example shows |
|---|---|
| JavaScript | `quota` moved from `Employee` to `Salesman`, as above |
| Python | the same move; `hasattr` shows that an engineer no longer has a quota |
| Go | not inheritance: the method moves from the embedded struct to the one type that needs it |

## When to use

- **Only one subclass uses the method.** The other subclasses ignore it, or they override it to throw an error.
- **A subclass does not want what it inherits.** This is [[refused-bequest|Refused bequest]]. Push down method is the first fix for it.
- **Before you replace the subclass.** A method that is in the right subclass is easier to move into a delegate. See [[replace-subclass-with-delegate|Replace Subclass with Delegate]].

## Pitfalls

**Polymorphic callers.** Code that holds a list of employees and asks each one for its quota needs the method on the superclass. Then the method belongs there, perhaps with a default value for the other subclasses.

**Many subclasses need it.** If most subclasses need the method, pushing it down makes copies. Keep it in the superclass, or add an intermediate class.

## Tools that do it

IntelliJ IDEA has Push Members Down. Its documentation says: "The Push Members Down refactoring lets you clean up the class hierarchy by moving class members to a subclass or a subinterface."[^4] Most IDEs for Java and C# have a similar action.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
