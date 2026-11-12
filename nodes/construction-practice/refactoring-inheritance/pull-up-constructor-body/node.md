---
tag: CGVKYV
kind: technique
slug: pull-up-constructor-body
name: Pull up constructor body
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Move the statements that the constructors of subclasses share into the constructor of the superclass, and call it with super.
family: refactoring
polarity: positive
intent: "Let the superclass set up the state that it owns."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999) and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [pull-up-field, pull-up-method, replace-constructor-with-factory-function, duplicated-code]
sources:
  - url: https://refactoring.com/catalog/pullUpConstructorBody.html
    title: Martin Fowler, Catalog of Refactorings, Pull Up Constructor Body
    locator: the entry, the Party and Employee sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry pullUpConstructorBody, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Pull Up Constructor Body
---

## Purpose

Constructors are not ordinary methods. A subclass does not inherit the body of a constructor, and in many languages it must call the constructor of the superclass first. So when two subclasses set the same fields in the same way, [[pull-up-method|Pull Up Method]] cannot remove the copies.

Pull up constructor body moves the shared statements into the constructor of the superclass. Each subclass passes the values with `super(...)`. The catalogue card has an `Employee` constructor that calls `super()` and then sets `this._name = name`. After the refactoring, the constructor of `Party` takes `name` and sets the field, and `Employee` calls `super(name)`.[^1]

## Steps

1. If the superclass has no constructor, add one. Make sure that each subclass constructor calls it.
2. Move the statements that all subclasses share into the superclass constructor, one at a time. Pass the values that they need as arguments to `super`.
3. Test after each move.
4. If a shared statement must run after a statement that is only in the subclass, first move the statement that can run first. If that is not possible, use Extract Function on the shared statements, and pull the new method up with Pull Up Method.

## Example

`Employee` and `Department` both set the name. After the refactoring, `Party` sets it.

```javascript run id=pull-up-constructor
import assert from "node:assert/strict";

class Party {
  constructor(name) { this._name = name; }
  get name() { return this._name; }
}

class Employee extends Party {
  constructor(name, id, monthlyCost) {
    super(name);
    this._id = id;
    this._monthlyCost = monthlyCost;
  }
}

class Department extends Party {
  constructor(name, staff) {
    super(name);
    this._staff = staff;
  }
}

const ana = new Employee("Ana", "E1", 1000);
const sales = new Department("Sales", [ana]);
assert.equal(Object.keys(ana)[0], "_name");
console.log(ana.name, sales.name);
```

```output
Ana Sales
```

The field `_name` is the first own key of the employee, because the superclass constructor runs first.

| Language | What the example shows |
|---|---|
| JavaScript | `this._name = name` moved to the constructor of `Party`, as above |
| Python | the same move; each subclass calls `super().__init__(name)` |
| Go | not inheritance: a constructor function of the embedded struct makes the shared part |

## When to use

- **Subclass constructors repeat the same assignments.** This is [[duplicated-code|Duplicated code]] that Pull Up Method cannot fix.
- **After Pull Up Field.** The field is in the superclass, so the code that sets it belongs there too. See [[pull-up-field|Pull Up Field]].

## Pitfalls

**Order of statements.** In JavaScript, Java and C#, the superclass constructor runs before the rest of the subclass constructor. A shared statement that uses a value that the subclass sets later cannot simply move up.

**Calls to overridable methods.** If the superclass constructor calls a method that a subclass overrides, the override runs before the subclass has set its own fields. Keep such calls out of constructors.

**Complex creation.** If the subclasses differ a lot in how they are made, a factory function can be clearer than a long chain of constructors. See [[replace-constructor-with-factory-function|Replace Constructor with Factory Function]].

## Tools that do it

No IDE has one action for this refactoring under this name. The moves of the statements are done by hand, or with Extract Function and Pull Members Up.

The catalogue lists the refactoring in the first and the second edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
