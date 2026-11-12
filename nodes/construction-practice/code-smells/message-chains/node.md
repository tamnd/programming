---
tag: B7P2X3
kind: pattern
slug: message-chains
name: Message Chains
aliases: [train wreck]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a client asks one object for another, then asks that one for a third, and so on, so the client depends on the whole path between the objects.
family: code-smell
polarity: negative
intent: "Find the clients that walk a chain of objects, and give the first object a method that answers the question, so that the path is known in one place."
also_known_as: []
refactorings: [hide-delegate, extract-function, move-function]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Message Chains is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, feature-envy, encapsulation-and-visibility, middle-man]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Message Chains
  - url: https://en.wikipedia.org/w/index.php?title=Law_of_Demeter&oldid=1306566666
    title: Wikipedia, Law of Demeter, revision 1306566666
    locator: the introduction, In object-oriented programming, and Disadvantages
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Hide Delegate, Extract Function and Move Function
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Message Chains
---

## Intent

Find the code that reaches through one object to get at another, and another after that. Give the first object in the chain a method that answers the question the client really has. Then the path between the objects is written in one place, and a change to the path is one edit.

## Problem

A message chain is a line such as `order.customer.address.country`. The client asks the order for its customer, asks the customer for an address and asks the address for a country. *Refactoring* describes the smell this way: a client asks one object for another object, which the client then asks for yet another object, and so on.[^1] It adds the cost: the client is coupled to the structure of the navigation, and any change to the relationships in between forces the client to change.[^1]

The design rule that the smell breaks is the Law of Demeter. Wikipedia sums it up as "Each unit should only talk to its friends; don't talk to strangers", and gives its object-oriented form: "an object should avoid invoking methods of an object returned by another method."[^2] It also gives the short version for languages with a dot: "use only one dot".[^2]

The forces are these:

- Each step in the chain is a getter that is natural to write. The chain grows one dot at a time.
- The client needs one fact, but to get it the client must know three types and how they connect.
- The same chain appears in many clients. When the path changes, each copy must change, which is a form of [[shotgun-surgery|Shotgun Surgery]].
- Not every chain is a smell. A chain of calls on the same kind of object, as in a builder or a collection pipeline, does not reach through to strangers.

## Solution

*Refactoring* gives two moves.[^1] The catalogue at refactoring.com has a card for each refactoring.[^3]

1. **Hide the path:** [[hide-delegate|Hide Delegate]]. Add a method to the first object that returns the result of the chain, and change the clients to call it. The method can live at any link in the chain.
2. **Move the use:** look at what the client does with the final object. Often it is better to use [[extract-function|Extract Function]] on the code that uses the result, and then [[move-function|Move Function]] to push that code into the chain. Then the client asks for the answer, not for the object.

The first edition of *Refactoring* gives the same two moves.[^4]

*Refactoring* warns that Hide Delegate on every link in a chain can turn the objects in between into middle men.[^1] The aim is to answer the client's question, not to wrap each getter.

## Example

Two functions find the country that an order ships to by walking from the order to the customer to the address. Then the business adds a delivery address on the order, which wins over the customer's address. The second version changes one method; the first version would need each client to learn the rule.

```javascript run id=hide-the-path
import assert from "node:assert/strict";

class Address {
  constructor(country) { this.country = country; }
}
class Customer {
  constructor(address) { this.address = address; }
}
class Order {
  constructor(customer, delivery = null) {
    this.customer = customer;
    this.delivery = delivery;
  }
  // After: the order answers the question, and knows the path.
  shipsTo() {
    return (this.delivery ?? this.customer.address).country;
  }
}

// Before: each client walks the chain.
const taxRegionBefore = (order) => (order.customer.address.country === "DE" ? "EU" : "other");
const carrierBefore = (order) => (order.customer.address.country === "US" ? "ups" : "dhl");

// After: each client asks the order.
const taxRegion = (order) => (order.shipsTo() === "DE" ? "EU" : "other");
const carrier = (order) => (order.shipsTo() === "US" ? "ups" : "dhl");

const home = new Order(new Customer(new Address("DE")));
assert.equal(taxRegionBefore(home), taxRegion(home));
assert.equal(carrierBefore(home), carrier(home));

// A gift order ships to another country than the customer lives in.
const gift = new Order(new Customer(new Address("DE")), new Address("US"));
assert.equal(carrierBefore(gift), "dhl"); // the chain still reads the customer's address
assert.equal(carrier(gift), "ups");
assert.equal(taxRegion(gift), "other");
console.log("shipsTo: tests pass");
```

```output
shipsTo: tests pass
```

In the second version the rule for the delivery address is in `shipsTo`, and every client follows it without an edit. In the first version each client that walks the chain is wrong for a gift order until someone finds and fixes it.

| Language | What the example shows |
|---|---|
| JavaScript | a method on the first object of the chain, as above |
| Python | the same method, as a property |
| Java | `order.getCustomer().getAddress().getCountry()` replaced by `order.shipsTo()` |
| Go | a method on the order struct, with the same rule |

## Consequences

The clients know one object and one method. The path between the objects can change, and only the method that hides it must change too.

The cost is one more method on the first object for each question. Wikipedia notes that the Law of Demeter "may result in having to write many wrapper methods to propagate calls to components".[^2] If the first object gets a wrapper for each field of each object behind it, it becomes a [[middle-man|Middle Man]], which is the opposite smell. The balance is to add methods for the questions that clients ask, not for each getter.

A chain is not always a smell. The rule is about reaching through objects to their parts. A pipeline such as `list.filter(f).map(g)` calls methods on values of the same kind, and a fluent builder returns the same builder from each call; neither one exposes the structure of other objects. Data that is plain data, such as a parsed JSON document, has no behaviour to hide the path behind, and walking it is the normal use.

## How to fix it

- **A client that walks a chain to get one value:** [[hide-delegate|Hide Delegate]] on the first object.[^1]
- **A client that walks a chain and then computes with the result:** [[extract-function|Extract Function]] on the computation, then [[move-function|Move Function]] into the chain.[^1]
- **Wrappers on every link:** see [[middle-man|Middle Man]] and Remove Middle Man.[^1]

## Variants and related patterns

**Middle Man.** The opposite smell, and the risk of too much Hide Delegate. See [[middle-man|Middle Man]].

**Feature Envy.** A client that walks a chain often does so to compute with the data at the end. Moving the computation removes both smells. See [[feature-envy|Feature Envy]].

**Law of Demeter.** The design guideline that limits which objects a method should call. See [[law-of-demeter|Law of Demeter]].

## References
