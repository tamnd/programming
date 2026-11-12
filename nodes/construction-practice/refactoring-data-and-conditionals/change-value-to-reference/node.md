---
tag: R1CFJN
kind: technique
slug: change-value-to-reference
name: Change value to reference
aliases: [share one object, use a repository for shared objects]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When many copies of one logical object exist, and an update to it must be seen by all holders, keep one shared object for it in a repository, and give each holder a reference to that object.
family: refactoring
polarity: positive
intent: "Keep one object for one thing, and let every holder refer to it."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Change Value to Reference is in the first edition of Refactoring (1999), in the chapter on organizing data, and has the same name in the second edition (2018). Its inverse is Change Reference to Value.
see_also: [change-reference-to-value, repository-pattern, value-equality-and-identity, pointers-and-references, global-data]
sources:
  - url: https://refactoring.com/catalog/changeValueToReference.html
    title: Martin Fowler, Catalog of Refactorings, Change Value to Reference
    locator: the entry; inverse of Change Reference to Value
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 8, Organizing Data, Change Value to Reference
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 9, Organizing Data, Change Value to Reference, motivation, mechanics and the example of orders and customers
---

## Purpose

When each order makes its own customer object from the data it was given, ten orders for one customer hold ten copies. As long as nobody changes a customer, that is fine. When the customer's address changes, each copy must be found and changed, or the copies disagree.

Change value to reference keeps one object for each customer, and gives each order a reference to it. The catalogue card replaces a constructor call, `new Customer(customerData)`, with a lookup in a repository by the customer's id.[^1] The refactoring is in both editions of the book.[^2][^3] The repository is the one place that makes and holds the shared objects. See [[repository-pattern|Repository pattern]].

## Steps

1. Make a repository for the objects, if there is none.[^3]
2. Make sure that each object has an id that the repository can look it up by, and that the constructor can get the right object for an id.
3. Change each place that makes an object of the type to get it from the repository. Test after each change.

## Example

Two orders for one customer each make their own `Customer`. An update to the email of one copy does not reach the other. After the refactoring, both orders get the customer from a repository, and see one update.

```javascript run id=one-object-for-one-thing
import assert from "node:assert/strict";

class Customer {
  constructor(id, email) { Object.assign(this, { id, email }); }
}

// Before: each order makes its own copy.
class OrderBefore {
  constructor(data) { this.customer = new Customer(data.customerId, data.email); }
}
const o1 = new OrderBefore({ customerId: "c1", email: "old@example.org" });
const o2 = new OrderBefore({ customerId: "c1", email: "old@example.org" });
o1.customer.email = "changed@example.org";
assert.equal(o2.customer.email, "old@example.org"); // the copies disagree

// After: one object for each id, in a repository.
class CustomerRepository {
  #byId = new Map();
  register(id, email) {
    if (!this.#byId.has(id)) this.#byId.set(id, new Customer(id, email));
    return this.#byId.get(id);
  }
  get(id) {
    const c = this.#byId.get(id);
    if (!c) throw new Error(`no customer ${id}`);
    return c;
  }
}
class Order {
  constructor(data, repo) { this.customer = repo.get(data.customerId); }
}

const repo = new CustomerRepository();
repo.register("c1", "old@example.org");
const o3 = new Order({ customerId: "c1" }, repo);
const o4 = new Order({ customerId: "c1" }, repo);
o3.customer.email = "changed@example.org";
assert.equal(o4.customer.email, "changed@example.org");
assert.equal(o3.customer, o4.customer); // one object
console.log("change value to reference: tests pass");
```

```output
change value to reference: tests pass
```

The repository here is a map in memory. In a larger program, it can load the objects from a database the first time they are asked for.

| Language | What the example shows |
|---|---|
| JavaScript | a repository class that keeps one object for each id, as above |
| Python | a repository with a dictionary, and a check with `is` that two orders hold the same object |
| Java | a repository with `computeIfAbsent`, and `==` to show one object |
| Go | a repository that hands out pointers, so that each order refers to one struct |

## When to use

- **An update that every holder must see.** A customer's address, a product's price, an account's status.
- **Many copies of large data.** One shared object can also save memory, but the reason for the refactoring is the update.
- **Identity matters.** When two objects with the same data are still two different things, they are entities, not values. See [[value-equality-and-identity|Value equality and identity]].

## Pitfalls

**Shared mutable state.** A reference that many holders share can be changed by any of them. That is the point of the refactoring, and also its risk: a change in one part of the program has effects in others. If nothing must be updated, a value is safer. See [[change-reference-to-value|Change Reference to Value]].[^1]

**A repository that is global data.** A repository that any code can reach is a kind of [[global-data|global data]]. Pass it to the code that needs it, as the example does.

**Concurrency.** If several threads share the object, its updates need synchronization.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
