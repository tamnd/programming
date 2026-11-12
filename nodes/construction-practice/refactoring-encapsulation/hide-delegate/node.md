---
tag: 48SR1T
kind: technique
slug: hide-delegate
name: Hide delegate
aliases: []
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Give a class a method that forwards to an object it holds, so that its clients ask the class and do not reach through it to the object behind; the inverse of Remove Middle Man.
family: refactoring
polarity: positive
intent: "Stop clients from depending on the objects behind a class by letting the class answer for them."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Hide Delegate is in the first edition of Refactoring (1999), with the refactorings that move features between objects, and in the chapter on encapsulation of the second edition (2018). Its inverse is Remove Middle Man.
see_also: [message-chains, middle-man, encapsulation-and-visibility, feature-envy, extract-class, inline-class]
sources:
  - url: https://refactoring.com/catalog/hideDelegate.html
    title: Martin Fowler, Catalog of Refactorings, Hide Delegate
    locator: the entry; inverse of Remove Middle Man
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Hide Delegate, motivation and mechanics, and Remove Middle Man, motivation; Chapter 3, Bad Smells in Code, Message Chains and Middle Man
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 7, Moving Features Between Objects, Hide Delegate
---

## Purpose

A client that writes `shipment.warehouse.timeZone` knows two things: that a shipment has a warehouse, and that a warehouse has a time zone. If the shipment later gets its time zone from somewhere else, every client that reached through the warehouse must change.

Hide delegate gives the server class, here the shipment, a method that asks the delegate for the client. The client writes `shipment.timeZone` and knows only the shipment. The catalogue card shows the same move for a person, a department and a manager.[^1] The inverse is Remove Middle Man.[^1]

This is the fix the book gives for [[message-chains|Message Chains]], where a client follows a chain of objects to get what it needs.[^2] The first edition has the refactoring under the same name.[^3]

## Steps

1. Run the tests and see them pass.
2. For each method of the delegate that clients use, write a method on the server that forwards to it.[^2]
3. Change each client to call the server's method. Test after each change.
4. When no client needs the delegate any more, remove the server's accessor for it, and test.[^2]

## Example

Clients read the time zone of a shipment through its warehouse. After the refactoring, the shipment answers, and clients do not know about the warehouse.

```javascript run id=ask-the-server
import assert from "node:assert/strict";

class Warehouse {
  constructor(code, timeZone) { this.code = code; this.timeZone = timeZone; }
}

class Shipment {
  #warehouse;
  constructor(id, warehouse) { this.id = id; this.#warehouse = warehouse; }
  // The forwarding method: clients ask the shipment.
  get timeZone() { return this.#warehouse.timeZone; }
}

const hub = new Warehouse("AMS1", "Europe/Amsterdam");
const s = new Shipment(7, hub);

// Before, a client wrote: s.warehouse.timeZone
assert.equal(s.timeZone, "Europe/Amsterdam");
assert.equal(s.warehouse, undefined); // step 4: the accessor is gone
console.log("hide delegate: tests pass");
```

```output
hide delegate: tests pass
```

Now suppose shipments move to a time zone of their own, set at dispatch. Only the `timeZone` getter of `Shipment` changes. No client changes.

| Language | What the example shows |
|---|---|
| JavaScript | a getter that forwards, and a private field for the delegate, as above |
| Python | a property that forwards, and the delegate kept in a field with a leading underscore |
| Go | a method that forwards, and an unexported field so that other packages cannot reach the delegate |

## When to use

- **Clients follow a chain.** A client that writes `a.b().c().d()` is coupled to the structure of the whole chain. That is [[message-chains|Message Chains]].[^2]
- **The delegate may change.** If the server may get the value from another object later, hiding the delegate keeps that change inside the server.
- **Many clients do the same walk.** One forwarding method replaces the same chain in each client.

## Pitfalls

**Too many forwarding methods.** Each feature of the delegate that clients use needs its own method on the server. When most of the server's methods only forward, the server has become a [[middle-man|Middle Man]], and the inverse, Remove Middle Man, is the fix.[^2] The book says that the right amount of hiding changes as the system changes, and that both refactorings are tools to move it.[^2]

**Hiding what the client needs.** If clients need many features of the delegate, a forwarding method for each one hides nothing. Give the clients the delegate, or ask if the features belong on the server: see [[feature-envy|Feature Envy]].

**An accessor left in place.** Step 4 is where the coupling goes away. If the server still exposes the delegate, the old chains can come back.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. Do steps 3 and 4 one client at a time, with a test after each.

## References
