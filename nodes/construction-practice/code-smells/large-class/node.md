---
tag: V7JA1C
kind: pattern
slug: large-class
name: Large Class
aliases: [god class, blob]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a class has too many fields, too many methods or too much code, because it does several jobs, and each change to one job must be made among the parts of the others.
family: code-smell
polarity: negative
intent: "Find the groups of fields and methods inside a large class that belong together, and give each group a class of its own."
also_known_as: []
refactorings: [extract-class, extract-superclass, replace-type-code-with-subclasses, extract-function]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Large Class is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, long-function, duplicated-code, divergent-change, data-clumps, classes-and-instances]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Large Class
  - url: https://en.wikipedia.org/w/index.php?title=God_object&oldid=1352397558
    title: Wikipedia, God object, revision 1352397558
    locator: the introduction and Characteristics
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Extract Class, Extract Superclass, Replace Type Code with Subclasses (alias Extract Subclass) and Extract Function
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Large Class
---

## Intent

Find the class that does several jobs at once. Look inside it for groups of fields and methods that are used together and apart from the rest, and move each group into a class of its own. The large class then becomes a small one that uses the others, or it goes away.

## Problem

A class is a unit of thought. A reader opens it to learn one idea: what an order is, how a cart works. When one class holds the order, the cart, the shipping address and the tax rules, the reader must learn all of them to change any of them.

*Refactoring* describes the smell as a class that tries to do too much, which often shows itself as too many fields. It notes that duplicated code is close behind, because a class with many fields tends to repeat the code that uses them.[^1] The extreme form has its own name. Wikipedia describes a god object as "an object that references a large number of distinct types, has too many unrelated or uncategorized methods, or some combination of both", and adds that "Changes made to the object for the benefit of one routine can have a ripple effect on other unrelated functions."[^2]

The forces are these:

- A class grows one feature at a time. Each feature is small, and the class is the place where the data already is.
- Fields with a common prefix or suffix, such as `shipStreet`, `shipCity` and `shipCountry`, are a sign of a class that is waiting to be extracted. *Refactoring* names this sign.[^1]
- A large class is a source of [[divergent-change|Divergent Change]]: it changes for many reasons, and the changes for one reason touch code that is there for another.
- Not every field is used by every method. A class whose methods use disjoint sets of fields is several classes in one file.

## Solution

*Refactoring* gives the moves.[^1] The catalogue at refactoring.com has a card for each refactoring, and gives Extract Subclass as another name for Replace Type Code with Subclasses.[^3]

1. **Group the fields:** [[extract-class|Extract Class]]. Fields that go together, and the methods that use them, move into a class of their own.
2. **Group by kind:** [[extract-superclass|Extract Superclass]] or [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]], when some fields and methods belong only to some kinds of object.
3. **Remove the repeats:** a class with too much code often has the same steps in several methods. [[extract-function|Extract Function]] on the shared steps makes the class smaller before any split.

*Refactoring* also suggests a way to find the groups: look at how the clients use the class. Different clients often use different subsets of the methods, and each subset is a candidate for a class.[^1] The first edition gives the same moves, and adds Extract Interface for the case where the split is only in what the clients see.[^4]

## Example

A checkout class holds the items of a cart, a coupon, and the fields of a shipping address, all with prefixes. The methods for the cart use only the cart fields, and the methods for the address use only the address fields.

```javascript run id=split-the-checkout
import assert from "node:assert/strict";

// Before: one class, two jobs, and fields with prefixes.
class CheckoutBefore {
  constructor() {
    this.cartItems = [];
    this.cartCoupon = null;
    this.shipName = "";
    this.shipStreet = "";
    this.shipCity = "";
    this.shipCountry = "";
  }
  addItem(name, cents) { this.cartItems.push({ name, cents }); }
  applyCoupon(percent) { this.cartCoupon = percent; }
  cartTotal() {
    const sum = this.cartItems.reduce((t, i) => t + i.cents, 0);
    return this.cartCoupon === null ? sum : sum - Math.round((sum * this.cartCoupon) / 100);
  }
  setAddress(name, street, city, country) {
    Object.assign(this, { shipName: name, shipStreet: street, shipCity: city, shipCountry: country });
  }
  shippingLabel() { return [this.shipName, this.shipStreet, `${this.shipCity}, ${this.shipCountry}`].join("\n"); }
  isDomestic() { return this.shipCountry === "NL"; }
}

// After: each group of fields has a class, and the checkout uses both.
class Cart {
  #items = [];
  #coupon = null;
  add(name, cents) { this.#items.push({ name, cents }); }
  applyCoupon(percent) { this.#coupon = percent; }
  get total() {
    const sum = this.#items.reduce((t, i) => t + i.cents, 0);
    return this.#coupon === null ? sum : sum - Math.round((sum * this.#coupon) / 100);
  }
}
class Address {
  constructor(name, street, city, country) { Object.assign(this, { name, street, city, country }); }
  get label() { return [this.name, this.street, `${this.city}, ${this.country}`].join("\n"); }
  isIn(country) { return this.country === country; }
}
class Checkout {
  constructor(cart, address) { this.cart = cart; this.address = address; }
  isDomestic() { return this.address.isIn("NL"); }
}

const before = new CheckoutBefore();
before.addItem("lamp", 3999);
before.addItem("bulb", 499);
before.applyCoupon(10);
before.setAddress("Ada", "Keizersgracht 1", "Amsterdam", "NL");

const cart = new Cart();
cart.add("lamp", 3999);
cart.add("bulb", 499);
cart.applyCoupon(10);
const after = new Checkout(cart, new Address("Ada", "Keizersgracht 1", "Amsterdam", "NL"));

assert.equal(after.cart.total, before.cartTotal());
assert.equal(after.cart.total, 4048);
assert.equal(after.address.label, before.shippingLabel());
assert.equal(after.isDomestic(), before.isDomestic());
console.log("checkout: tests pass");
```

```output
checkout: tests pass
```

In the second version the prefixes are gone, because each prefix became a class name. `Cart` can be tested with no address, and `Address` can be used by an invoice or a user profile that has no cart. The checkout keeps only the rule that needs both.

| Language | What the example shows |
|---|---|
| JavaScript | a cart class and an address class extracted, as above |
| Python | the same split, with a dataclass for the address |
| Java | the same split, with a record for the address |
| Go | the same split, with a struct for each part |

## Consequences

Each class is small enough to read at once, and each one changes for one kind of reason. The parts can be reused and tested alone.

The cost is more classes and more names. A reader who wants to see all of checkout must open three files, not one. The split pays when the groups really are separate; a split along a line that the code crosses often only spreads one job across two classes, and the next change will be [[shotgun-surgery|Shotgun Surgery]].

Size alone is not the smell. A class with many fields that all describe one thing, such as a record for a row with forty columns, may be large and still be one idea. The test is whether the fields and methods fall into groups that do not use each other.

## How to fix it

- **Fields that share a prefix or are used together:** [[extract-class|Extract Class]].[^1]
- **Fields and methods used only by some kinds of object:** [[replace-type-code-with-subclasses|Replace Type Code with Subclasses]] or [[extract-superclass|Extract Superclass]].[^1]
- **Methods that repeat the same steps:** [[extract-function|Extract Function]] first, then split.[^1]
- **Clients that use different parts of the class:** split along the line between those parts.[^1]

## Variants and related patterns

**Long Function.** The same problem at the scale of one function. See [[long-function|Long Function]].

**Divergent Change.** A large class is often the place where one class changes for many reasons. See [[divergent-change|Divergent Change]].

**Data Clumps.** A group of fields that always go together is the first candidate for Extract Class. See [[data-clumps|Data Clumps]].

**God object.** The extreme form, where one object knows and does most of the program. See [[god-object|God object]].

## References
