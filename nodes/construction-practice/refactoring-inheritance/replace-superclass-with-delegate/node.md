---
tag: XAQPR8
kind: technique
slug: replace-superclass-with-delegate
name: Replace superclass with delegate
aliases: [replace inheritance with delegation]
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Replace inheritance from a class whose interface does not fit the subclass with a field that holds an instance of that class, and forward only the calls that are needed.
family: refactoring
polarity: positive
intent: "Use a class without taking on all of its interface."
also_known_as: [Replace Inheritance with Delegation]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has it as Replace Inheritance with Delegation. The second edition (2018) calls it Replace Superclass with Delegate, in the chapter on dealing with inheritance.
see_also: [replace-subclass-with-delegate, composition-over-inheritance, refused-bequest, extract-superclass, middle-man, class-inheritance]
sources:
  - url: https://refactoring.com/catalog/replaceSuperclassWithDelegate.html
    title: Martin Fowler, Catalog of Refactorings, Replace Superclass with Delegate
    locator: the entry, the List and Stack sketch; aliases Replace Inheritance with Delegation
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceSuperclassWithDelegate, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Replace Superclass with Delegate
  - url: https://www.jetbrains.com/help/idea/replace-inheritance-with-delegation.html
    title: JetBrains, IntelliJ IDEA documentation, Replace inheritance with delegation
    locator: the first paragraph
---

## Purpose

Inheritance gives a subclass the whole interface of its superclass. If some of the operations of the superclass make no sense for the subclass, callers can still call them, and they can break the rules of the subclass.

The catalogue card shows the classic case: `class Stack extends List`. A stack inherits each operation of a list, so a caller can insert an element in the middle of a stack. After the refactoring, `Stack` does not extend `List`. Its constructor sets `this._storage = new List()`, and the stack uses the list only inside.[^1]

The card gives the first edition name, Replace Inheritance with Delegation.[^1]

## Steps

1. Add a field to the subclass that refers to an instance of the superclass. Set it to a new instance.
2. For each method of the superclass that the callers of the subclass use, add a forwarding method in the subclass that calls the field. Test after each group.
3. When each needed method forwards, remove the inheritance. Test.

## Example

```javascript run id=superclass-to-delegate
import assert from "node:assert/strict";

class StackBefore extends Array {
  peek() { return this[this.length - 1]; }
}

class Stack {
  constructor() { this._storage = []; }
  push(x) { this._storage.push(x); }
  pop() { return this._storage.pop(); }
  peek() { return this._storage[this._storage.length - 1]; }
  get size() { return this._storage.length; }
}

const before = new StackBefore();
before.push(1, 2, 3);
before.splice(1, 1); // The array interface lets a caller remove an element from the middle.
assert.deepEqual([...before], [1, 3]);

const stack = new Stack();
[1, 2, 3].forEach((x) => stack.push(x));
assert.equal(typeof stack.splice, "undefined");
assert.equal(stack.pop(), 3);
console.log(stack.peek(), stack.size);
```

```output
2 2
```

The new `Stack` has only the stack operations. The array is a detail of how it stores the elements.

| Language | What the example shows |
|---|---|
| JavaScript | a stack that extended `Array` changed to hold an array in a field, as above |
| Python | the same change; the stack holds a list, and `insert` is no longer available |
| Go | the Go form: a struct with an unexported slice field; Go has no class inheritance, so this is the normal style |

## When to use

- **The subclass does not want parts of the superclass interface.** This is [[refused-bequest|Refused bequest]].
- **A subclass is not a kind of the superclass.** A stack is not a list that callers can edit at any place.
- **The superclass is in another module.** A change in it can break the subclass. A delegate depends only on the methods that it calls.

See [[composition-over-inheritance|Composition over inheritance]].

## Pitfalls

**Many forwarding methods.** If the subclass needs most of the methods of the superclass, each one needs a forwarding method. The class can then become a [[middle-man|Middle man]]. In that case, inheritance can be the right choice.

**Callers that need the superclass type.** Code that passes the subclass where the superclass type is expected no longer compiles or works. Give the subclass an interface of its own, or keep the inheritance.

**Identity.** With inheritance, there is one object. With a delegate, there are two. Code that compares objects, or that stores the delegate elsewhere, must take this into account.

## Tools that do it

IntelliJ IDEA has Replace Inheritance With Delegation. Its documentation says that it "lets you remove a class from inheritance hierarchy, while preserving the functionality of the parent."[^4]

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
