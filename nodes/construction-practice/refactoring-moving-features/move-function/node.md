---
tag: ET0CNN
kind: technique
slug: move-function
name: Move function
aliases: [move method]
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Move a function to the class or module whose data it uses most, and leave a delegating call or nothing behind; called Move Method in the first edition of Refactoring.
family: refactoring
polarity: positive
intent: "Put a function next to the data and the other functions it works with."
also_known_as: [Move Method]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has Move Method in the chapter on moving features between objects. The second edition (2018) calls it Move Function, so that it also covers functions that are not methods, and puts it in the chapter on moving features.
see_also: [move-field, feature-envy, shotgun-surgery, extract-function, inline-function, combine-functions-into-class]
sources:
  - url: https://refactoring.com/catalog/moveFunction.html
    title: Martin Fowler, Catalog of Refactorings, Move Function
    locator: the entry; aliases Move Method
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 7, Moving Features Between Objects, Move Method
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Move Function, motivation and mechanics; Chapter 3, Bad Smells in Code, Feature Envy
  - url: https://www.jetbrains.com/help/idea/move-refactorings.html
    title: IntelliJ IDEA documentation, Move refactorings
    locator: the introduction; Move static method to another class; Move an instance method to another class
---

## Purpose

Good modules keep things that change together in one place. A function that uses the data of another class more than the data of its own is in the wrong place: each change to that data sends you to two places, and each reader must hold both in mind.

Move function puts the function in the class or module where its data lives. The catalogue card moves an `overdraftCharge` getter from an account to its account type.[^1] The first edition calls the refactoring Move Method; the catalogue keeps that name as an alias.[^1][^2] The second edition calls it Move Function, and its first worked example moves a nested function, not a method, to the top level of its module.[^3]

It is the main fix the book gives for [[feature-envy|Feature Envy]].[^3]

## Steps

1. Look at everything the function uses in its current place: fields, other functions, parameters. Decide whether some of them should move with it.[^3]
2. Check whether the function is polymorphic: an override in a subclass or an implementation of an interface makes the move harder.
3. Copy the function to the target. Change it to fit: data that was on `this` comes in through a parameter or is now the target's own.
4. Decide how the source will reach the target, and turn the source function into a call that delegates to the copy.[^3]
5. Test.
6. If the delegating function adds nothing, inline it into its callers with [[inline-function|Inline Function]], and test.

## Example

The cost to ship an order uses only the parcel's weight and size. It moves to the parcel. The order keeps a delegating getter for a time; then the callers ask the parcel.

```javascript run id=near-its-data
import assert from "node:assert/strict";

class Parcel {
  constructor(weightKg, cm) { this.weightKg = weightKg; this.cm = cm; }
  // Moved here: it uses only the parcel's data.
  get shippingCents() {
    const volumetricKg = (this.cm[0] * this.cm[1] * this.cm[2]) / 5000;
    return 400 + Math.ceil(Math.max(this.weightKg, volumetricKg)) * 150;
  }
}

class Order {
  constructor(id, parcel) { this.id = id; this.parcel = parcel; }
  // Step 4: the old function delegates while callers change.
  get shippingCents() { return this.parcel.shippingCents; }
}

const light = new Order(1, new Parcel(0.8, [10, 10, 5]));
const bulky = new Order(2, new Parcel(1.2, [60, 40, 40]));
assert.equal(light.shippingCents, 550);   // 1 kg billed
assert.equal(bulky.shippingCents, 3400);  // 19.2 kg by volume, 20 billed
assert.equal(bulky.parcel.shippingCents, bulky.shippingCents);
console.log("move function: tests pass");
```

```output
move function: tests pass
```

The parcel rule can now change without a change to the order, and a return label, which has a parcel but no order, can use it.

| Language | What the example shows |
|---|---|
| JavaScript | a getter moved to the class whose data it uses, with a delegating getter left for a time |
| Python | the same move, with a property |
| Java | the same move between two classes, as the IntelliJ Move action does for a method |
| Go | the same move from a method on one type to a method on another |

## When to use

- **The function uses another class's data more than its own.** That is [[feature-envy|Feature Envy]].[^3]
- **One change touches several classes.** Moving functions together is one fix for [[shotgun-surgery|Shotgun Surgery]].
- **A helper is nested where others need it.** A function nested in another can move out to module level when a second caller needs it.[^3]
- **After other refactorings.** Once you know the shape of the data, functions move to where it is. See [[combine-functions-into-class|Combine Functions into Class]].

## Pitfalls

**Data that does not move.** If the function also uses some of the source's data, pass it in as a parameter, or move that data too with [[move-field|Move Field]]. Passing the whole source object brings back the coupling.

**Polymorphism.** A method that subclasses override cannot move as one function. Move each override, or keep a polymorphic call that delegates.

**Unclear target.** If the function uses the data of several classes, the book's rule is to put it with the class that has most of the data, and to use [[extract-function|Extract Function]] first to split it into parts that can go to different places.[^3]

## Tools that do it

IntelliJ IDEA's Move action lets you move "class members to other classes", and its documentation gives the case of a method "used more in another class than in its own class".[^4] In other languages, and for module-level functions, the steps above are done by hand or with the move action of the tool.

## References
