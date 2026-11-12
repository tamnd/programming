---
tag: EGT4G4
kind: technique
slug: remove-subclass
name: Remove subclass
aliases: [replace subclass with fields]
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Replace subclasses that differ only in constant data with a field in the superclass, and remove the subclasses.
family: refactoring
polarity: positive
intent: "Remove a class that does not do enough to be a class."
also_known_as: [Replace Subclass with Fields]
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on dealing with inheritance. The catalogue lists Replace Subclass with Fields as its alias.
see_also: [replace-type-code-with-subclasses, collapse-hierarchy, lazy-element, speculative-generality, replace-constructor-with-factory-function]
sources:
  - url: https://refactoring.com/catalog/removeSubclass.html
    title: Martin Fowler, Catalog of Refactorings, Remove Subclass
    locator: the entry, the Person and genderCode sketch; inverse of Replace Type Code with Subclasses; aliases Replace Subclass with Fields
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry removeSubclass, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Remove Subclass
---

## Purpose

Subclasses are useful when the kinds of an object behave in different ways. When the only difference left is a constant, each subclass is a class that a reader must find and understand for one value.

Remove subclass replaces the subclasses with a field. The catalogue card has a class `Person` whose `genderCode` getter returns the code X, and two subclasses that override it to return M and F. After the refactoring, `Person` has one getter that returns `this._genderCode`, and the subclasses are gone.[^1] The card names the inverse, Replace Type Code with Subclasses, and the alias Replace Subclass with Fields.[^1]

## Steps

1. Use Replace Constructor with Factory Function on each subclass constructor, so that callers do not name the subclasses. See [[replace-constructor-with-factory-function|Replace Constructor with Factory Function]].
2. If code tests the class of an object, for example with `instanceof`, move each test into a method of the superclass.
3. Add a field to the superclass for the data that the subclasses give. Set it in the constructor, from an argument of the factory function.
4. Change the methods of the superclass to use the field. Test.
5. Remove the subclasses, one at a time, and test after each one.

## Example

```javascript run id=subclass-to-field
import assert from "node:assert/strict";

class PersonBefore { constructor(name) { this._name = name; } get genderCode() { return "X"; } }
class Male extends PersonBefore { get genderCode() { return "M"; } }
class Female extends PersonBefore { get genderCode() { return "F"; } }

class Person {
  constructor(name, genderCode = "X") { this._name = name; this._genderCode = genderCode; }
  get name() { return this._name; }
  get genderCode() { return this._genderCode; }
  get isMale() { return this._genderCode === "M"; }
}

function createPerson(name, genderCode) { return new Person(name, genderCode); }

assert.equal(createPerson("Ana", "F").genderCode, new Female("Ana").genderCode);
assert.equal(createPerson("Bo", "M").genderCode, new Male("Bo").genderCode);
assert.equal(createPerson("Cy").genderCode, new PersonBefore("Cy").genderCode);
console.log([createPerson("Ana", "F"), createPerson("Bo", "M")].map((p) => `${p.name} ${p.genderCode} ${p.isMale}`));
```

```output
[ 'Ana F false', 'Bo M true' ]
```

The test `p instanceof Male` becomes the getter `isMale` on the class.

| Language | What the example shows |
|---|---|
| JavaScript | two subclasses replaced with a `genderCode` field, as above |
| Python | the same change, with a dataclass that has a default code |
| Go | one struct with a field; the Go form is the same, because the types that are removed are only names for a value |

## When to use

- **Subclasses that only return constants.** A field holds the same data with less code. See [[lazy-element|Lazy element]].
- **Subclasses that were made for a future that did not come.** See [[speculative-generality|Speculative generality]].
- **The kind of an object must change.** A field can change. A class cannot.

## Pitfalls

**The behavior grows again.** If the kinds start to behave in different ways, the subclasses come back. That is the inverse, [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]].

**Class tests in other code.** Code that tests the class, or that a framework maps by class, breaks when the subclass goes. Find each use of the subclass name first.

**One subclass.** If only one subclass is left and it does not differ enough, merge it into the superclass with [[collapse-hierarchy|Collapse Hierarchy]].

## Tools that do it

No IDE has one action for this refactoring. The steps use Replace Constructor with Factory Method, field refactorings and the removal of a class.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
