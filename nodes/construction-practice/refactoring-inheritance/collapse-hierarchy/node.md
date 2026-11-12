---
tag: 0VSVGF
kind: technique
slug: collapse-hierarchy
name: Collapse hierarchy
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Merge a subclass into its superclass, or the superclass into the subclass, when the two are no longer different enough to be two classes.
family: refactoring
polarity: positive
intent: "Remove a level of a hierarchy that adds nothing."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on dealing with inheritance.
see_also: [remove-subclass, extract-superclass, lazy-element, speculative-generality, pull-up-method, push-down-method]
sources:
  - url: https://refactoring.com/catalog/collapseHierarchy.html
    title: Martin Fowler, Catalog of Refactorings, Collapse Hierarchy
    locator: the entry, the Employee and Salesman sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry collapseHierarchy, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Collapse Hierarchy
---

## Purpose

A hierarchy changes as the code changes. Features move up and down, and after a time a subclass can be almost the same as its superclass. Then the two classes are one idea in two places, and a reader must look at both to understand either.

Collapse hierarchy merges the two. The catalogue card has `class Employee` and `class Salesman extends Employee`. After the refactoring, only `Employee` is left.[^1]

## Steps

1. Choose which class to remove. Keep the one whose name fits the merged class better. If neither name fits, pick one and rename it afterwards.
2. Move all the features of the class to remove into the one to keep, with [[pull-up-method|Pull Up Method]], [[pull-up-field|Pull Up Field]], [[push-down-method|Push Down Method]] and [[push-down-field|Push Down Field]].
3. Change each reference to the removed class to the class that stays. Test.
4. Remove the empty class. Test.

## Example

After earlier changes, `Salesman` only adds a region with a default value. The region moves into `Employee`, and `Salesman` goes.

```javascript run id=collapse
import assert from "node:assert/strict";

class EmployeeBefore { constructor(name) { this._name = name; } get name() { return this._name; } }
class SalesmanBefore extends EmployeeBefore {
  constructor(name, region = "none") { super(name); this._region = region; }
  get region() { return this._region; }
}

class Employee {
  constructor(name, region = "none") { this._name = name; this._region = region; }
  get name() { return this._name; }
  get region() { return this._region; }
}

const before = new SalesmanBefore("Ana", "north");
const after = new Employee("Ana", "north");
assert.equal(after.region, before.region);
assert.equal(after.name, before.name);
console.log(after.name, after.region);
```

```output
Ana north
```

| Language | What the example shows |
|---|---|
| JavaScript | `Salesman` merged into `Employee`, as above |
| Python | the same merge, with a dataclass that has a default region |
| Go | the same merge; the struct that embedded `Employee` is removed, and the field moves into `Employee` |

## When to use

- **A subclass that adds almost nothing.** See [[lazy-element|Lazy element]].
- **A hierarchy made for a need that did not come.** See [[speculative-generality|Speculative generality]].
- **After many moves.** When most features have moved up or down, check if each level of the hierarchy still has a reason to exist.

## Pitfalls

**Other subclasses.** If the superclass has other subclasses, merging the superclass into one of them is not possible. Merge the subclass up instead.

**Constant differences.** If several subclasses differ only in data, use [[remove-subclass|Remove Subclass]], which replaces them with a field.

**Public types.** If code outside your control uses the class that you remove, keep the name, for example as an alias, until that code changes.

## Tools that do it

No IDE has one action for this refactoring under this name. The moves of the features use Pull Members Up and Push Members Down, and the IDE can then find the remaining references to the empty class.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
