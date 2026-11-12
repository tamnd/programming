---
tag: FE8ZPG
kind: technique
slug: inline-class
name: Inline class
aliases: [merge class]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Move all the fields and methods of a class that does too little into the class that uses it, and delete the class; the inverse of Extract Class.
family: refactoring
polarity: positive
intent: "Remove a class that no longer pulls its weight by folding it into the class that uses it."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Inline Class is in the first edition of Refactoring (1999), with the refactorings that move features between objects, and in the chapter on encapsulation of the second edition (2018). Its inverse is Extract Class.
see_also: [extract-class, lazy-element, speculative-generality, middle-man, inline-function, code-smell]
sources:
  - url: https://refactoring.com/catalog/inlineClass.html
    title: Martin Fowler, Catalog of Refactorings, Inline Class
    locator: the entry; inverse of Extract Class
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Inline Class, motivation and mechanics; Chapter 3, Bad Smells in Code, Lazy Element
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 7, Moving Features Between Objects, Inline Class
  - url: https://www.jetbrains.com/help/idea/inline.html
    title: IntelliJ IDEA documentation, Inline
    locator: the introduction; Inline Superclass
---

## Purpose

A class costs something: a name to learn, a file to open, a step to follow from caller to code. It pays for itself when it holds a job. Sometimes it stops doing so. Other refactorings moved its work elsewhere, or it was made for a need that never came, and what is left is a field or two and a method that forwards a call.

Inline class moves what is left into the class that uses it, and deletes the class. The catalogue card is the reverse of Extract Class: a `TelephoneNumber` class whose area code and number fold back into `Person`.[^1] The inverse is [[extract-class|Extract Class]].

The book gives a second reason. When the split between two classes is in the wrong place, it can be easier to inline one into the other and then extract again along a better line, than to move features one by one between them.[^2] The first edition has the refactoring under the same name.[^3]

## Steps

1. Run the tests and see them pass.
2. In the target class, the one that will remain, write a method for each public method of the source class. Each one delegates to the source.[^2]
3. Change each caller of the source class to call the target's methods instead. Test after each.
4. Move the fields and methods of the source into the target, one at a time, and test after each.
5. When the source class has no callers and no content, delete it, and test.

## Example

An order uses a `Delivery` class that holds only a tracking ID and builds one URL. After the refactoring, the order holds the ID and the method, and the class is gone.

```javascript run id=fold-it-back
import assert from "node:assert/strict";

// Before: a class that holds one field and one method.
class Delivery {
  constructor(trackingId) { this.trackingId = trackingId; }
  trackingUrl() { return `https://track.example/${encodeURIComponent(this.trackingId)}`; }
}
class OrderBefore {
  constructor(id, trackingId) { this.id = id; this.delivery = new Delivery(trackingId); }
}

// After: the field and the method live in the order.
class Order {
  constructor(id, trackingId) { this.id = id; this.trackingId = trackingId; }
  trackingUrl() { return `https://track.example/${encodeURIComponent(this.trackingId)}`; }
}

for (const t of ["AB12", "x y/z"]) {
  assert.equal(new Order(1, t).trackingUrl(), new OrderBefore(1, t).delivery.trackingUrl());
}
assert.equal(new Order(1, "x y/z").trackingUrl(), "https://track.example/x%20y%2Fz");
console.log("inline class: tests pass");
```

```output
inline class: tests pass
```

Callers now write `order.trackingUrl()` and not `order.delivery.trackingUrl()`. If delivery later grows a carrier, a date and a status, the job is back, and Extract Class brings the class back.

| Language | What the example shows |
|---|---|
| JavaScript | a one-field class folded into its user, as above |
| Python | the same fold, with a delegating method kept for a time for the old callers |
| Go | the same fold, where an embedded struct is removed and its field moves up |

## When to use

- **The class does too little.** After other refactorings, a class with one field and a forwarding method is a [[lazy-element|Lazy Element]], and the book names this refactoring as a fix for it.[^2]
- **The class was made for a future that did not come.** That is [[speculative-generality|Speculative Generality]].
- **Two classes are split in the wrong place.** Inline one into the other, then extract along the right line.[^2]
- **A class only forwards.** If most methods of the class pass calls to another object, see also [[middle-man|Middle Man]] and Remove Middle Man.

## Pitfalls

**Other users of the class.** If more than one class uses the source, inlining it into one of them leaves the others without it. Inline only a class that has one user, or move it to the user that owns it and leave the others to call that.

**Polymorphism.** A small class can be one of several implementations of an interface. Then its size is not the point: the interface is the job. Do not inline it.

**A class at a boundary.** A class that a framework, a serializer or another team depends on by name is part of an interface. Inlining it changes the interface.

**Name clashes.** Fields and methods from the source can have the same names as the target's. Rename before you move.

## Tools that do it

IntelliJ IDEA's Inline action can "reverse the Extract refactoring for a method, constructor, parameter, superclass, anonymous class, and closure", and its Inline Superclass folds a superclass into its subclasses.[^4] For a class that is used as a field, as in the example, the steps above are done by hand or with the move actions of the tool, with a test after each.

## References
