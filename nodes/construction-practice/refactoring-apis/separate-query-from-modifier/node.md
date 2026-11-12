---
tag: P2TV52
kind: technique
slug: separate-query-from-modifier
name: Separate query from modifier
aliases: [split query and command, separate query from command]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Split a function that both returns a value and changes state into a query that only returns the value and a modifier that only makes the change.
family: refactoring
polarity: positive
intent: "Let a caller ask a question without changing anything."
history:
  year: 1999
  people: [Martin Fowler, Bertrand Meyer]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on refactoring APIs. It applies the command-query separation principle of Bertrand Meyer.
see_also: [side-effect, mutable-data, replace-query-with-parameter, return-modified-value, extract-function, command-pattern]
sources:
  - url: https://refactoring.com/catalog/separateQueryFromModifier.html
    title: Martin Fowler, Catalog of Refactorings, Separate Query from Modifier
    locator: the entry, the getTotalOutstandingAndSendBill sketch
  - url: https://martinfowler.com/bliki/CommandQuerySeparation.html
    title: Martin Fowler, CommandQuerySeparation, 5 December 2005
    locator: the whole note
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry separateQueryFromModifier, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Separate Query from Modifier
---

## Purpose

A function that returns a value and also changes state is hard to use safely. A caller who only wants the value must accept the change too. A test that reads the value twice changes the state twice.

Separate query from modifier splits the function in two. The query returns the value and changes nothing. The modifier makes the change and returns nothing. The catalogue card splits `getTotalOutstandingAndSendBill` into `totalOutstanding` and `sendBill`.[^1]

The rule behind the refactoring is command-query separation. Martin Fowler writes that "The term 'command query separation' was coined by Bertrand Meyer in his book “Object Oriented Software Construction“".[^2] The principle divides methods into two groups. Queries "Return a result and do not change the observable state of the system (are free of side effects)." Commands "Change the state of a system but do not return a value."[^2]

## Steps

1. Copy the function. Give the copy a name that says what it returns.
2. Remove each side effect from the copy, so that it only computes and returns the value.
3. Find each caller of the old function. If the caller uses the return value, replace the call with a call to the query, followed by a call to the old function.
4. Remove the return value from the old function, and give it a name that says what it changes.
5. Test after each step.
6. Look for duplication: the modifier can often call the query, or both can call a shared helper.

## Example

`chargeAndTotal` adds the open invoices of a customer, sends a bill and returns the total. A report that only wants the total must send a bill to get it. After the split, `totalOutstanding` is a query and `sendBill` is a modifier.

```javascript run id=query-and-modifier
import assert from "node:assert/strict";

const sent = [];
const customer = { name: "Ana", invoices: [{ amount: 40 }, { amount: 2 }] };

function chargeAndTotalBefore(c) {
  const total = c.invoices.reduce((sum, each) => sum + each.amount, 0);
  sent.push(`bill to ${c.name}: ${total}`);
  return total;
}

function totalOutstanding(c) {
  return c.invoices.reduce((sum, each) => sum + each.amount, 0);
}

function sendBill(c) {
  sent.push(`bill to ${c.name}: ${totalOutstanding(c)}`);
}

assert.equal(chargeAndTotalBefore(customer), 42);
assert.equal(sent.length, 1);

// The query can run any number of times and sends nothing.
assert.equal(totalOutstanding(customer), 42);
assert.equal(totalOutstanding(customer), 42);
assert.equal(sent.length, 1);

// The caller that wants both now makes both calls.
const total = totalOutstanding(customer);
sendBill(customer);
assert.equal(total, 42);
assert.deepEqual(sent, ["bill to Ana: 42", "bill to Ana: 42"]);
console.log("separate query from modifier: tests pass");
```

```output
separate query from modifier: tests pass
```

The modifier calls the query, so the sum is written once.

| Language | What the example shows |
|---|---|
| JavaScript | the split into `totalOutstanding` and `sendBill`, as above |
| Python | the same split, with the query as a function that does not touch the list of sent bills |
| Go | the same split; the query takes the customer by value, and the modifier takes a pointer to the outbox that it changes |

## When to use

- **A function with a return value and a side effect.** A caller cannot get the value without the effect.[^2]
- **A value that callers read many times.** Fowler writes that "you can use queries in many situations with much more confidence, introducing them anywhere, changing their order. You have to be more careful with modifiers."[^2]
- **Before caching or moving the query.** A query with no side effects can be cached, inlined or moved without a change in behavior.

## Pitfalls

**Useful exceptions.** Command-query separation is a guideline. Fowler gives the exception himself: "Popping a stack is a good example of a query that modifies state." He adds that he is "prepared to break it to get my pop."[^2] The same is true of `next` on an iterator, which he also names.[^2]

**Concurrency.** When two threads share the state, a query followed by a modifier is two steps, and another thread can change the state between them. An operation such as pop or compare-and-swap is one call because it must be atomic. Do not split it.

**Observable state only.** A query can still change state that no caller can see, such as a cache. Fowler notes that "the rule about not changing state really only applies to the ObservableState of the system."[^2]

## Tools that do it

No IDE has one action for this refactoring. It is a sequence of Extract Function, Change Function Declaration and a removal of the return value. See [[extract-function|Extract Function]] and [[change-function-declaration|Change Function Declaration]].

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition of Refactoring, with the tags refactoring-apis and immutability.[^3] In the second edition it is in Chapter 11, Refactoring APIs.[^4]

## References
