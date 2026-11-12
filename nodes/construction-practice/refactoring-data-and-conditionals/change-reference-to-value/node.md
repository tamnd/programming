---
tag: PJWFP1
kind: technique
slug: change-reference-to-value
name: Change reference to value
aliases: [make immutable value, replace mutation with a fresh value]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When a small object that is held inside another is changed in place, make it immutable: give it value equality, and replace it with a fresh object each time its contents change.
family: refactoring
polarity: positive
intent: "Treat a small object as a value: never change it, replace it."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Change Reference to Value is in the first edition of Refactoring (1999), in the chapter on organizing data, and has the same name in the second edition (2018). Its inverse is Change Value to Reference.
see_also: [change-value-to-reference, immutability, value-equality-and-identity, pointers-and-references, replace-primitive-with-object, mutable-data]
sources:
  - url: https://refactoring.com/catalog/changeReferenceToValue.html
    title: Martin Fowler, Catalog of Refactorings, Change Reference to Value
    locator: the entry; inverse of Change Value to Reference
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 8, Organizing Data, Change Reference to Value
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 9, Organizing Data, Change Reference to Value, motivation, mechanics and the example of a telephone number
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
    title: MDN Web Docs, Object.freeze()
    locator: Description, and the example of a shallow freeze
  - url: https://docs.python.org/3/library/dataclasses.html
    title: Python documentation, dataclasses, Data Classes
    locator: the frozen parameter, and the hash method
---

## Purpose

An object that is held inside another object can be a reference or a value. If it is a reference, code changes it in place, and every holder of the reference sees the change. If it is a value, nobody changes it: a change makes a fresh object, which replaces the old one in the holder.

Change reference to value makes a small object a value. The catalogue card changes a `Product` that subtracts a discount from the amount of its price object into a `Product` that sets its price to a fresh `Money` object.[^1] The refactoring is in both editions of the book.[^2][^3] A value is safe to share and to pass around, since no holder can change it under another one. See [[immutability|Immutability]] and [[value-equality-and-identity|Value equality and identity]].

## Steps

1. Check that the object can be immutable, or that it can become so.[^3]
2. For each setter of the object, remove it. Where callers used it, make them build a fresh object, and assign it to the field that holds the object. Test after each change.
3. Give the object equality based on its fields: two values with the same contents are equal.
4. If the language uses a hash code with equality, make the hash code agree with equality.

## Example

An order holds a delivery window. Code that moves the delivery by a day changes the window in place, and the change also moves the window of each other order that shares the object. After the refactoring, the window is a frozen value, and moving it gives a fresh window.

```javascript run id=replace-not-change
import assert from "node:assert/strict";

// Before: the window is a reference, changed in place.
const sharedWindow = { startHour: 9, endHour: 12, day: 3 };
const a = { id: "a", window: sharedWindow };
const b = { id: "b", window: sharedWindow };
a.window.day += 1; // also moves order b
assert.equal(b.window.day, 4);

// After: the window is a value.
class DeliveryWindow {
  constructor(day, startHour, endHour) {
    Object.assign(this, { day, startHour, endHour });
    Object.freeze(this);
  }
  movedBy(days) { return new DeliveryWindow(this.day + days, this.startHour, this.endHour); }
  equals(other) {
    return other instanceof DeliveryWindow && this.day === other.day &&
      this.startHour === other.startHour && this.endHour === other.endHour;
  }
}

const morning = new DeliveryWindow(3, 9, 12);
const c = { id: "c", window: morning };
const d = { id: "d", window: morning };
c.window = c.window.movedBy(1);
assert.equal(d.window.day, 3);                       // d is not moved
assert.ok(c.window.equals(new DeliveryWindow(4, 9, 12)));
assert.throws(() => { "use strict"; morning.day = 5; }, TypeError);
console.log("change reference to value: tests pass");
```

```output
change reference to value: tests pass
```

An assignment to a field of a frozen object fails, and MDN notes that in strict mode such attempts "will throw TypeErrors".[^4] JavaScript modules run in strict mode, and the test checks the error. In Python, `frozen=True` on a dataclass lets you "emulate immutability", and when equality and `frozen` are both on, the decorator generates a hash method.[^5]

| Language | What the example shows |
|---|---|
| JavaScript | a frozen class with a method that returns a fresh value, and an `equals` method, as above |
| Python | a frozen dataclass, which gives equality and a hash code from the fields, and `dataclasses.replace` |
| Java | a record, which gives `equals` and `hashCode` from its components |
| Go | a struct passed and assigned by value, and compared with `==` |

## When to use

- **A small object that holds data, not identity.** An amount of money, a date range or a coordinate: two with the same contents are the same thing.
- **A shared object that one holder changes by mistake.** A value cannot be changed under another holder.
- **An object used as a key in a map or an element of a set.** A key that changes after it is put into a hash map can no longer be found.

## Pitfalls

**Objects that must be shared and updated.** If several holders must see one update, such as a customer whose address changes, the object needs identity. That is the inverse refactoring, [[change-value-to-reference|Change Value to Reference]].[^1]

**Cost of copies.** A large object that changes often makes a fresh copy at each change. For a small object this cost is small. For a large one, measure.

**Shallow freezing.** `Object.freeze` in JavaScript does not freeze the objects that a field holds: "those objects are not frozen".[^4] A frozen dataclass in Python also does not freeze the objects in its fields. A value must hold only values.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
