---
tag: YJR11W
kind: technique
slug: replace-subclass-with-delegate
name: Replace subclass with delegate
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Replace a subclass that holds one variation of behavior with a separate delegate object that the main class holds and calls.
family: refactoring
polarity: positive
intent: "Let an object vary in more than one way, and change its variation at run time."
history:
  year: 2018
  people: [Martin Fowler]
  note: The refactoring is new in the second edition of Refactoring (2018), in the chapter on dealing with inheritance.
see_also: [replace-superclass-with-delegate, composition-over-inheritance, replace-type-code-with-subclasses, strategy-pattern, state-pattern, class-inheritance]
sources:
  - url: https://refactoring.com/catalog/replaceSubclassWithDelegate.html
    title: Martin Fowler, Catalog of Refactorings, Replace Subclass with Delegate
    locator: the entry, the Order and PriorityOrder sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceSubclassWithDelegate, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Replace Subclass with Delegate
---

## Purpose

Subclasses are a simple way to vary behavior, but they have two limits:

- An object can vary in only one way with subclasses. If orders vary by priority and by region, one set of subclasses cannot express both.
- The class of an object is fixed. An order cannot become a priority order after it is made.

Replace subclass with delegate moves the variation into a separate object. The main class holds a reference to the delegate and calls it. The catalogue card has `Order`, whose `daysToShip` reads `this._warehouse.daysToShip`, and `PriorityOrder extends Order`, which overrides it to read `this._priorityPlan.daysToShip`. After the refactoring, `Order` asks a `_priorityDelegate` if it has one, and the warehouse if it does not. The class `PriorityOrderDelegate` holds the priority logic.[^1]

## Steps

1. If callers call the subclass constructor, use Replace Constructor with Factory Function, so that the creation is in one place. See [[replace-constructor-with-factory-function|Replace Constructor with Factory Function]].
2. Make an empty delegate class. Its constructor takes the data of the subclass, and often a back-reference to the main object.
3. Add a field for the delegate to the main class. Change the factory function to set the delegate in place of making the subclass.
4. Move each method of the subclass to the delegate. In the main class, make the method call the delegate when it has one, and use its own logic when it does not. Test after each method.
5. When the subclass is empty, remove it. Test.

## Example

A priority order ships faster. After the refactoring, the priority logic is in a delegate, and an order can become a priority order at run time.

```javascript run id=subclass-to-delegate
import assert from "node:assert/strict";

const warehouse = { daysToShip: 5 };
const priorityPlan = { daysToShip: 2 };

class OrderBefore { constructor(warehouse) { this._warehouse = warehouse; } get daysToShip() { return this._warehouse.daysToShip; } }
class PriorityOrderBefore extends OrderBefore {
  constructor(warehouse, plan) { super(warehouse); this._priorityPlan = plan; }
  get daysToShip() { return this._priorityPlan.daysToShip; }
}

class PriorityOrderDelegate {
  constructor(host, plan) { this._host = host; this._priorityPlan = plan; }
  get daysToShip() { return this._priorityPlan.daysToShip; }
}

class Order {
  constructor(warehouse) { this._warehouse = warehouse; }
  get daysToShip() {
    return this._priorityDelegate ? this._priorityDelegate.daysToShip : this._warehouse.daysToShip;
  }
  makePriority(plan) { this._priorityDelegate = new PriorityOrderDelegate(this, plan); }
}

function createOrder(warehouse, plan) {
  const order = new Order(warehouse);
  if (plan) order.makePriority(plan);
  return order;
}

assert.equal(createOrder(warehouse).daysToShip, new OrderBefore(warehouse).daysToShip);
assert.equal(createOrder(warehouse, priorityPlan).daysToShip, new PriorityOrderBefore(warehouse, priorityPlan).daysToShip);

// With a delegate, an order can become a priority order after it is made.
const order = createOrder(warehouse);
const days = [order.daysToShip];
order.makePriority(priorityPlan);
days.push(order.daysToShip);
console.log(days);
```

```output
[ 5, 2 ]
```

| Language | What the example shows |
|---|---|
| JavaScript | `PriorityOrder` replaced with `PriorityOrderDelegate`, as above |
| Python | the same change; the delegate is an optional attribute |
| Go | the Go form: Go has no subclasses, so a delegate held in a field of interface type is the normal way to vary behavior |

## When to use

- **More than one axis of variation.** Use subclasses for one axis at most, and delegates for the others.
- **The variation changes at run time.** A field can point to a new delegate. A class cannot change.
- **The superclass and subclass are in different modules.** A change to a superclass can break a subclass in another module. A delegate has a smaller interface with the main class.

See [[composition-over-inheritance|Composition over inheritance]].

## Pitfalls

**More code.** A delegate needs a class, a field and a call in each method that varies. For one stable axis of variation, a subclass is simpler.

**The back-reference.** A delegate that needs data of the main object holds a reference to it. The two classes then depend on each other. Keep the delegate's use of the host small.

**Many delegated methods.** If each method of the main class starts with a test for the delegate, use a default delegate for the normal case, so that the main class always calls a delegate. This is the [[strategy-pattern|Strategy]] pattern.

## Tools that do it

No IDE has one action for this refactoring. The steps are done by hand.

The catalogue lists the refactoring in the second edition only, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
