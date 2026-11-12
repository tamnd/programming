---
tag: WTQ3M5
kind: technique
slug: remove-setting-method
name: Remove setting method
area: construction-practice/refactoring-apis
level: core
summary: >-
  Remove the setter of a field that must not change after the object is made, and set the field once, in the constructor.
family: refactoring
polarity: positive
intent: "Show that a field does not change after construction."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on refactoring APIs.
see_also: [immutability, mutable-data, encapsulate-record, change-function-declaration, replace-constructor-with-factory-function]
sources:
  - url: https://refactoring.com/catalog/removeSettingMethod.html
    title: Martin Fowler, Catalog of Refactorings, Remove Setting Method
    locator: the entry, the Person sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry removeSettingMethod, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Remove Setting Method
---

## Purpose

A setter says that a field can change at any time. If the field must keep its first value, for example the id or the name of a person, the setter gives a wrong message, and it lets code break the rule.

Remove setting method deletes the setter and sets the field in the constructor. The catalogue card has a class `Person` with `get name()` and `set name(aString)`. After the refactoring, only the getter is left.[^1]

The catalogue gives the refactoring the tags parameters, immutability and remove.[^2] See [[immutability|Immutability]].

## Steps

1. If the constructor does not take the value yet, add a parameter for it with Change Function Declaration, and make the constructor call the setter. See [[change-function-declaration|Change Function Declaration]].
2. Find each call of the setter. If a call comes just after the constructor, move its value into the constructor call. Test after each change.
3. When no code outside the constructor calls the setter, put the assignment in the constructor, and remove the setter.
4. If the language has a way to mark the field as fixed, use it, for example `readonly` in TypeScript or `final` in Java.

## Example

The old class makes a person, then sets the id. Any code can set the id again later. The new class takes the id in the constructor and has no setter.

```javascript run id=no-setter
import assert from "node:assert/strict";

class PersonBefore {
  get id() { return this._id; }
  set id(arg) { this._id = arg; }
}

class Person {
  #id;
  constructor(id) { this.#id = id; }
  get id() { return this.#id; }
}

const before = new PersonBefore();
before.id = "1234";
before.id = "9999";
assert.equal(before.id, "9999");

const ana = new Person("1234");
assert.throws(() => { "use strict"; ana.id = "9999"; }, TypeError);
assert.equal(ana.id, "1234");
console.log(ana.id);
```

```output
1234
```

A JavaScript module is in strict mode, so an assignment to a property that has only a getter throws a `TypeError`.

| Language | What the example shows |
|---|---|
| JavaScript | a private field with a getter and no setter, as above |
| Python | a frozen dataclass; an assignment to a field raises `FrozenInstanceError` |
| Go | an unexported field with an exported getter; code in other packages cannot set the field |

## When to use

- **A field that must not change after construction**, such as an id or a key.
- **A setter that only callers just after construction use.** The value belongs in the constructor.
- **A step toward an immutable object.** When no field has a setter, the object can be shared without copies. See [[mutable-data|Mutable data]].

## Pitfalls

**Frameworks that need setters.** Some serialization and mapping libraries make an empty object and then call a setter for each field. Check the libraries that make the object before you remove a setter.

**Long constructors.** If many fields move into the constructor, its parameter list gets long. A factory function with a clear name can help. See [[replace-constructor-with-factory-function|Replace Constructor with Factory Function]].

**A fixed reference is not a fixed value.** With no setter, the field always refers to the same object. If that object is mutable, its contents can still change.

## Tools that do it

No IDE has one action for this refactoring. An IDE can find each use of the setter, and it can add the constructor parameter with Change Signature.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
