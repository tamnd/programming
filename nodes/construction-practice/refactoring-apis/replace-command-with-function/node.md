---
tag: JZEBFQ
kind: technique
slug: replace-command-with-function
name: Replace command with function
area: construction-practice/refactoring-apis
level: core
summary: >-
  Turn a command object that does a small job, with a constructor and one execute method, back into a plain function.
family: refactoring
polarity: positive
intent: "Remove a class that only wraps one function."
history:
  year: 2018
  people: [Martin Fowler]
  note: The refactoring is new in the second edition of Refactoring (2018), in the chapter on refactoring APIs. It is the inverse of Replace Function with Command.
see_also: [replace-function-with-command, command-pattern, inline-function, lazy-element, change-function-declaration]
sources:
  - url: https://refactoring.com/catalog/replaceCommandWithFunction.html
    title: Martin Fowler, Catalog of Refactorings, Replace Command with Function
    locator: the entry, the ChargeCalculator sketch; inverse of Replace Function with Command
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceCommandWithFunction, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Command with Function
---

## Purpose

A command object is useful when a function is complex or when the request must be stored, queued or undone. When the work is small, the class only adds code. Each caller must make an object and then call `execute`, and a reader must read a constructor, fields and a method to find one line of logic.

Replace command with function turns the command back into a function. The catalogue card has `class ChargeCalculator`, with a constructor that stores `customer` and `usage`, and an `execute` method that returns `this._customer.rate * this._usage`. After the refactoring, it is `function charge(customer, usage)`, which returns `customer.rate * usage`.[^1]

The card names the inverse, Replace Function with Command.[^1]

## Steps

1. Use Extract Function on the creation of the command and the call of `execute`. The new function is the function that callers will use. See [[extract-function|Extract Function]].
2. Change each caller to call the new function. Test.
3. Inline each helper method of the command into `execute`. See [[inline-function|Inline Function]].
4. For each constructor parameter, make `execute` take it as a parameter, and change the call in the new function.
5. Inline `execute` into the new function. Test.
6. Remove the command class.

## Example

The command computes a charge. Each call makes an object for one multiplication.

```javascript run id=command-to-function
import assert from "node:assert/strict";

class ChargeCalculator {
  constructor(customer, usage) {
    this._customer = customer;
    this._usage = usage;
  }
  execute() {
    return this._customer.rate * this._usage;
  }
}

function charge(customer, usage) {
  return customer.rate * usage;
}

const customer = { rate: 0.25 };
assert.equal(charge(customer, 40), new ChargeCalculator(customer, 40).execute());
console.log(charge(customer, 40));
```

```output
10
```

| Language | What the example shows |
|---|---|
| JavaScript | `new ChargeCalculator(customer, usage).execute()` replaced with `charge(customer, usage)`, as above |
| Python | the same change; a class with only `__init__` and one method is often a function |
| Go | the same change; a struct with one method becomes a function, and a caller that needs a value of a function type can still pass `charge` |

## When to use

- **The command has one short method.** The class is a [[lazy-element|Lazy element]]: it does not do enough to be a class.
- **No caller stores, queues or undoes the command.** These are the reasons to have a command object. Without them, a function is enough.
- **After other refactorings.** A command that was complex can become simple when parts of it move to other places. Then it no longer needs to be an object.

## Pitfalls

**A command that is still needed.** If any code keeps the object, passes it to a queue, or calls methods other than `execute`, the class does more than one call. Keep it.

**An API needs an object.** Some APIs take only a value that implements an interface. In a language where a function cannot implement that interface, the class must stay. Where a function value can be passed, as in the Go example, pass the function.

## Tools that do it

No IDE has one action for this refactoring. The steps use Extract Function, Inline Function and Change Function Declaration.

The catalogue lists the refactoring in the second edition only, with the tag refactoring-apis.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
