---
tag: H3HFC3
kind: technique
slug: remove-flag-argument
name: Remove flag argument
aliases: [replace parameter with explicit methods]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Replace an argument that a caller passes only to select what a function does with one explicit function for each choice.
family: refactoring
polarity: positive
intent: "Say at the call which operation you want."
also_known_as: [Replace Parameter with Explicit Methods]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has it as Replace Parameter with Explicit Methods. The second edition (2018) calls it Remove Flag Argument, in the chapter on refactoring APIs. Fowler wrote the note Flag Argument on his site in 2011.
see_also: [boolean-parameter-smell, parameterize-function, decompose-conditional, change-function-declaration, replace-conditional-with-polymorphism]
sources:
  - url: https://refactoring.com/catalog/removeFlagArgument.html
    title: Martin Fowler, Catalog of Refactorings, Remove Flag Argument
    locator: the entry, the setDimension sketch; aliases Replace Parameter with Explicit Methods
  - url: https://martinfowler.com/bliki/FlagArgument.html
    title: Martin Fowler, FlagArgument, 23 June 2011
    locator: the whole note, with the sections Tangled Implementation, Deriving the flag and Boolean Setting Method
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry removeFlagArgument, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Remove Flag Argument
---

## Purpose

A call such as `book(customer, true)` does not say what `true` means. The reader must open the function to find out. Fowler defines the problem: "A flag argument is a kind of function argument that tells the function to carry out a different operation depending on its value."[^2]

Remove flag argument gives each value of the flag its own function. The catalogue card turns `setDimension(name, value)`, which tests whether `name` is the height or the width, into `setHeight(value)` and `setWidth(value)`.[^1] Fowler's reason is that "the separate methods communicate more clearly what my intention is when I make the call."[^2]

## Steps

1. Make one explicit function for each value of the flag. If the body is a conditional on the flag, use Decompose Conditional to move each branch into its own function. See [[decompose-conditional|Decompose Conditional]].
2. Change each caller that passes a literal for the flag to call the matching explicit function. Test after each change.
3. When no caller passes the flag, remove the old function.

If some callers pass the flag from a variable, for example from a form field, keep the old function for them, or let it call the explicit functions.

## Example

A booking function takes a boolean that selects a premium booking. After the refactoring, callers call `regularBooking` or `premiumBooking`. The shared work stays in one place.

```javascript run id=explicit-functions
import assert from "node:assert/strict";

function bookBefore(customer, isPremium) {
  const booking = { customer, seat: "standard", lounge: false };
  if (isPremium) { booking.seat = "front"; booking.lounge = true; }
  return booking;
}

function baseBooking(customer) {
  return { customer, seat: "standard", lounge: false };
}

function regularBooking(customer) {
  return baseBooking(customer);
}

function premiumBooking(customer) {
  return { ...baseBooking(customer), seat: "front", lounge: true };
}

assert.deepEqual(regularBooking("Ana"), bookBefore("Ana", false));
assert.deepEqual(premiumBooking("Ana"), bookBefore("Ana", true));
console.log(premiumBooking("Ana"));
```

```output
{ customer: 'Ana', seat: 'front', lounge: true }
```

The call `premiumBooking("Ana")` says what `bookBefore("Ana", true)` hid.

| Language | What the example shows |
|---|---|
| JavaScript | a boolean flag replaced with `regularBooking` and `premiumBooking`, as above |
| Python | the same split; a keyword-only flag (`*, premium=False`) is shown as the smaller fix when the flag must stay |
| Go | the same split; Go has no keyword arguments, so a flag call reads `book("Ana", true)` |

## When to use

- **A boolean or enum literal at the call site** that selects an operation.[^2]
- **A flag that most callers pass as a constant.** The constant is the name of the function that the caller wants.
- **A setter that takes a boolean**, such as `setSwitch(on)`. Fowler prefers `setOn()` and `setOff()` in most cases.[^2]

## Pitfalls

**Tangled logic.** When the flag is tested in many places in the body, the explicit functions can duplicate most of it. Fowler's answer is to keep the flagged function private: "one option is to retain the method with the flag argument, but keep it hidden."[^2] The explicit functions call it, and no other code does.

**The value comes from data.** If the caller reads the flag from a UI control or a file, it then needs an `if` to pick the function. For that case Fowler writes that he would "rather have setSwitch(aValue)".[^2]

**The flag is derived.** If the choice depends only on the state of an argument, such as the status of the customer, the caller does not need to choose. Then the function can decide by itself, and no flag and no second function are needed.[^2]

**A value is not a flag.** A parameter that the function uses in a calculation, such as a rate, is not a flag. To merge functions that differ in such a value, use [[parameterize-function|Parameterize Function]].

## Tools that do it

No IDE has one action for this refactoring. In a typed language, the IDE can find each call that passes a literal for the parameter, and Change Function Declaration can remove the parameter at the end. See [[change-function-declaration|Change Function Declaration]].

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tags refactoring-apis and remove.[^3] In the second edition it is in Chapter 11, Refactoring APIs.[^4] The smell that it fixes is [[boolean-parameter-smell|Boolean parameter]].

## References
