---
tag: KX6AKP
kind: technique
slug: push-down-field
name: Push down field
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Move a field from a superclass into the one subclass, or the few subclasses, that use it.
family: refactoring
polarity: positive
intent: "Keep data in the class that uses it."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999) and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [pull-up-field, push-down-method, refused-bequest]
sources:
  - url: https://refactoring.com/catalog/pushDownField.html
    title: Martin Fowler, Catalog of Refactorings, Push Down Field
    locator: the entry, the Java quota sketch; inverse of Pull Up Field
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry pushDownField, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Push Down Field
  - url: https://www.jetbrains.com/help/idea/pull-members-up.html
    title: JetBrains, IntelliJ IDEA documentation, Pull members up, push members down
    locator: Push members down
---

## Purpose

A field in a superclass is in each object of each subclass. If only one subclass uses it, the other objects carry an empty value, and a reader of the superclass must ask what the field is for.

Push down field moves the field to the subclass that uses it. The catalogue card shows it in Java: `Employee` declares `private String quota;`. After the refactoring, `Salesman` declares `protected String quota;`, and `Employee` and `Engineer` have no quota.[^1] The card names the inverse, Pull Up Field.[^1]

The refactoring often goes with [[push-down-method|Push Down Method]]: the methods that use the field move down with it.

## Steps

1. Declare the field in each subclass that uses it.
2. Remove the field from the superclass.
3. Test.
4. Remove the field from each subclass that does not use it. Test.

## Example

Before the refactoring, each employee object had a `_quota` field, with `null` for engineers. After it, only salesmen have the field.

```javascript run id=push-down-field
import assert from "node:assert/strict";

class EmployeeBefore {
  constructor(name, quota = null) { this._name = name; this._quota = quota; }
}

class Employee {
  constructor(name) { this._name = name; }
}
class Engineer extends Employee {}
class Salesman extends Employee {
  constructor(name, quota) { super(name); this._quota = quota; }
}

assert.deepEqual(Object.keys(new EmployeeBefore("Bo")), ["_name", "_quota"]);
assert.deepEqual(Object.keys(new Engineer("Bo")), ["_name"]);
console.log(Object.keys(new Salesman("Ana", 50000)));
```

```output
[ '_name', '_quota' ]
```

| Language | What the example shows |
|---|---|
| JavaScript | the `_quota` field moved from `Employee` to `Salesman`, as above |
| Python | the same move; `vars` shows the attributes of each object |
| Go | not inheritance: the field moves from the embedded struct to the struct of the one type that uses it |

## When to use

- **A field that most subclasses leave empty.** It is a sign that the field belongs to one subclass.
- **A subclass that does not want what it inherits.** See [[refused-bequest|Refused bequest]].

## Pitfalls

**Code in the superclass uses the field.** If a method of the superclass reads the field, push that method down first, or the field cannot move.

**A field that tells the subclasses apart.** If the value of the field is the only difference between subclasses, the subclasses may not be needed. See [[remove-subclass|Remove Subclass]].

## Tools that do it

IntelliJ IDEA has Push Members Down, which moves "class members to a subclass or a subinterface".[^4] Most IDEs for Java and C# have a similar action.

The catalogue lists the refactoring in the first and the second edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
