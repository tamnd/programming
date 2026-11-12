---
tag: EZ8M6H
kind: pattern
slug: refused-bequest
name: Refused Bequest
aliases: []
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a subclass inherits methods and data that it does not use or does not want, and in the strong form it inherits an interface that it cannot honour.
family: code-smell
polarity: negative
intent: "Find the subclasses that do not want what they inherit, and move the unwanted parts down to the subclasses that use them, or replace the inheritance with delegation."
also_known_as: []
refactorings: [push-down-method, push-down-field, replace-subclass-with-delegate, replace-superclass-with-delegate]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Refused Bequest is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, class-inheritance, composition-over-inheritance, subtyping, interfaces]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Refused Bequest
  - url: https://en.wikipedia.org/w/index.php?title=Liskov_substitution_principle&oldid=1370463505
    title: Wikipedia, Liskov substitution principle, revision 1370463505
    locator: the introduction and Principle
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Stack.html
    title: Java SE 25 API, java.util.Stack
    locator: the class description
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Push Down Method, Push Down Field, Replace Subclass with Delegate and Replace Superclass with Delegate (alias Replace Inheritance with Delegation)
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Refused Bequest
---

## Intent

Find the subclasses that do not want what their parent gives them. In the mild form, a subclass ignores some inherited methods or fields; move those down to the subclasses that use them. In the strong form, a subclass inherits a public interface that it must not support; replace the inheritance with a field that holds the parent, so that the subclass offers only the methods it can honour.

## Problem

Inheritance gives a subclass two things at once: the parent's code and the parent's interface. A subclass that wants the code but not the interface still gets both. Each inherited public method is a promise to the clients, and a subclass that cannot keep it either breaks the promise or throws.

*Refactoring* describes the smell as a subclass that is given methods and data by its parent but does not want them. It treats the mild form calmly: the traditional fix is a sibling class and pushing the unused parts down to it, but the smell is often too faint to be worth the work. The strong form is the one to fix: a subclass that reuses the parent's behaviour but does not want to support the parent's interface.[^1]

The strong form breaks the Liskov substitution principle. Wikipedia states it as the rule "that an object of a superclass may be replaced by an object of a subclass without breaking the program."[^2] A subclass that refuses part of the interface cannot be used everywhere its parent can.

The Java library has a known case. `java.util.Stack` extends `Vector`, so a stack also has the methods of a list, including insertion and removal at any index. Its documentation says that it "extends class Vector with five operations that allow a vector to be treated as a stack", and recommends the `Deque` interface instead: "A more complete and consistent set of LIFO stack operations is provided by the Deque interface and its implementations, which should be used in preference to this class."[^3]

The forces are these:

- Inheritance is the quickest way to reuse code. The parent already has the storage and half of the methods.
- The unwanted methods do not show in the subclass's file. A reader of the subclass sees five methods; a client sees forty.
- A subclass that overrides an inherited method to throw an error keeps the interface and breaks the promise. The error appears at run time, in a client that trusted the parent type.
- Some unused inheritance is harmless. A subclass that does not call one helper of its parent is not refusing an interface.

## Solution

*Refactoring* gives two levels of fix.[^1] The catalogue at refactoring.com has a card for each refactoring, and gives Replace Inheritance with Delegation as another name for Replace Superclass with Delegate.[^4]

1. **The mild form:** make a sibling class, and use [[push-down-method|Push Down Method]] and [[push-down-field|Push Down Field]] to move the unused parts from the parent to the sibling. The parent then holds only what all its subclasses share.
2. **The strong form, where the parent is the problem:** [[replace-superclass-with-delegate|Replace Superclass with Delegate]]. The class stops extending the parent and holds an instance of it in a field. It forwards only the methods that it wants to offer.
3. **The strong form, where the subclass is one of several variants:** [[replace-subclass-with-delegate|Replace Subclass with Delegate]]. The variant becomes an object that the parent holds.

The first edition gives the same advice, with Replace Inheritance with Delegation for the strong form.[^5]

## Example

A stack extends the built-in array to reuse its storage. It adds `peek`, and it gets `push` and `pop` for free. It also gets `shift`, `unshift`, `splice`, `sort` and index assignment, and each one lets a client break the rule that only the top can change.

```javascript run id=stop-inheriting-the-array
import assert from "node:assert/strict";

// Before: a stack that is an array, with all of the array's methods.
class StackBefore extends Array {
  peek() { return this[this.length - 1]; }
}

// After: a stack that has an array, and offers four methods.
class Stack {
  #items = [];
  push(x) { this.#items.push(x); return this; }
  pop() { return this.#items.pop(); }
  peek() { return this.#items[this.#items.length - 1]; }
  get size() { return this.#items.length; }
}

const before = new StackBefore();
const after = new Stack();
for (const x of [1, 2, 3]) {
  before.push(x);
  after.push(x);
}
assert.equal(after.peek(), before.peek());
assert.equal(after.pop(), before.pop());
assert.equal(after.size, before.length);

// The bequest: a client can change the bottom of the first stack.
before.unshift(0);
assert.deepEqual([...before], [0, 1, 2]);
assert.equal(typeof after.unshift, "undefined");
assert.equal(typeof after.splice, "undefined");
console.log("stack: tests pass");
```

```output
stack: tests pass
```

In the second version the array is a private field, and the stack offers only the operations of a stack. A client cannot reach the bottom of the stack, because the class does not inherit a method that would let it.

| Language | What the example shows |
|---|---|
| JavaScript | a stack that extends `Array`, replaced by a stack that holds one, as above |
| Python | a stack that extends `list`, replaced by a class that holds a list |
| Java | `java.util.Stack`, which extends `Vector`, compared with a class that holds an `ArrayDeque` |
| Go | a struct that embeds a slice type and gets its methods, replaced by a named field |

## Consequences

The class's interface is the one it can keep. Clients cannot call a method that breaks its rules, and the class can change its storage without breaking a client that used an inherited method.

The cost is the forwarding methods, one for each operation that the class does want to offer. For a parent with many useful methods, the forwards add up, and the class can become a [[middle-man|Middle Man]] if it forwards almost everything. If it does want almost everything, it may be a real subtype after all.

The mild form is often fine to leave. *Refactoring* says as much.[^1] A subclass that does not call one protected helper of its parent costs a reader little. Fix it when the unused part confuses readers, or when the parent changes for the sake of the subclasses that use it and breaks the ones that do not.

## How to fix it

- **Inherited methods or fields that only some subclasses use:** [[push-down-method|Push Down Method]] and [[push-down-field|Push Down Field]].[^1]
- **A subclass that must not offer the parent's interface:** [[replace-superclass-with-delegate|Replace Superclass with Delegate]].[^1]
- **A variant subclass that refuses the parent's interface:** [[replace-subclass-with-delegate|Replace Subclass with Delegate]].[^1]
- **An override that throws to refuse an inherited method:** one of the two delegate refactorings. The throw is the sign of the strong form.

## Variants and related patterns

**Composition over inheritance.** The delegate refactorings turn an is-a link into a has-a link. See [[composition-over-inheritance|Composition over inheritance]].

**Subtyping.** The Liskov substitution principle is the rule that a refused bequest breaks. See [[subtyping|Subtyping]].

**Middle Man.** The risk of a delegate that forwards almost every method. See [[middle-man|Middle Man]].

## References
