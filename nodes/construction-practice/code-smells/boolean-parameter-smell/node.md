---
tag: 7T14G6
kind: pattern
slug: boolean-parameter-smell
name: Boolean parameter
aliases: [flag argument, boolean trap, boolean argument]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a function takes a true or false argument that selects which of two things it does, so a call such as book(customer, false) does not say what it means.
family: code-smell
polarity: negative
intent: "Replace a boolean argument that picks the operation with a function for each operation, or with a named option, so that each call says what it does."
also_known_as: [Flag Argument]
refactorings: [remove-flag-argument]
history:
  year: 2011
  people: [Martin Fowler, Kent Beck]
  note: Martin Fowler wrote the bliki entry Flag Argument in June 2011. The second edition of Refactoring (2018) has the refactoring Remove Flag Argument, which the catalogue also lists as Replace Parameter with Explicit Methods.
see_also: [code-smell, boolean-type, long-parameter-list, parameter-passing, extract-function]
sources:
  - url: https://martinfowler.com/bliki/FlagArgument.html
    title: Martin Fowler, Flag Argument, bliki, 23 June 2011
    locator: the whole entry
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the card Remove Flag Argument (alias Replace Parameter with Explicit Methods)
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Remove Flag Argument
  - url: https://docs.python.org/3/tutorial/controlflow.html
    title: The Python Tutorial, More Control Flow Tools
    locator: 4.9.3.3, Keyword-Only Arguments
---

## Intent

Find the functions that take a boolean whose value chooses between two operations, such as `book(customer, true)` for a premium booking and `book(customer, false)` for a regular one. Give each operation its own function, or make the choice a named option, so that a reader of the call knows what it does without opening the function.

## Problem

Martin Fowler defines the term on his bliki: "A flag argument is a kind of function argument that tells the function to carry out a different operation depending on its value."[^1] He writes that his general reaction to flag arguments is to avoid them, and he prefers separate methods, because "the separate methods communicate more clearly what my intention is when I make the call."[^1] He gives the example of a reader who sees `book(martin, false)` and must remember what `false` means, where `regularBook(martin)` says it.[^1]

The forces are these:

- **The call site hides the meaning.** A literal `true` or `false` has no name at the call. The reader must look up the parameter list, and a second boolean doubles the guessing.
- **The order is a trap.** Two boolean parameters next to each other accept each other's values, and the compiler cannot tell.
- **The function does two things.** The body tests the flag and runs one branch or the other, so it has two reasons to change, and its tests must cover both branches through one name.
- **Not every boolean is a flag.** A boolean that is data, such as the value of a check box that the caller passes on, does not choose an operation. Fowler notes that an API "should be written to make it easier for the caller", and that a setter which takes a boolean from a UI control is better than a test of the value at each call.[^1]

## Solution

The refactoring is Remove Flag Argument, which the catalogue at refactoring.com also lists as Replace Parameter with Explicit Methods.[^2] The second edition of *Refactoring* describes it in the chapter on refactoring APIs.[^3]

1. **Make a function for each value.** Write `premiumBook(customer)` and `regularBook(customer)`, each calling the old function with the literal value.
2. **Change the callers.** Replace each call with a literal flag by the explicit function.
3. **Inline or hide the old function.** If the two branches are separate, move each into its own function and delete the flag. If the branches are tangled, Fowler suggests keeping the flag function but hidden, as a private implementation with an ugly name that only the two explicit functions call.[^1]

When the callers pass a variable, not a literal, the explicit functions do not help, because the caller would need an `if` to choose. Then the fix is a name at the call site: a keyword argument, an options object, or an enumeration in place of the boolean. Python can force the name; its tutorial explains that a `*` in the parameter list marks the parameters after it as keyword-only, so that they "must be passed by keyword argument".[^4]

Fowler also notes the case where the flag can be derived: if the kind of booking depends only on the customer's status, the booking code should read the status itself, and the caller should not pass a flag at all.[^1]

## Example

A booking function takes a customer and a flag for premium seats. The calls read `book(ada, true)`, and a reader must open the function to learn what `true` means.

```javascript run id=name-the-choice
import assert from "node:assert/strict";

// Before: a flag argument chooses the operation.
function bookBefore(customer, isPremium) {
  if (isPremium) return { customer, seat: "front", fee: 40 };
  return { customer, seat: "general", fee: 0 };
}

// After, the callers pass literals: a function for each operation.
const premiumBook = (customer) => ({ customer, seat: "front", fee: 40 });
const regularBook = (customer) => ({ customer, seat: "general", fee: 0 });

// After, the callers pass a value they read: a named option, not a bare boolean.
function book(customer, { tier }) {
  if (tier === "premium") return premiumBook(customer);
  if (tier === "regular") return regularBook(customer);
  throw new RangeError(`unknown tier ${tier}`);
}

assert.deepEqual(premiumBook("ada"), bookBefore("ada", true));
assert.deepEqual(regularBook("ada"), bookBefore("ada", false));
assert.deepEqual(book("ada", { tier: "premium" }), bookBefore("ada", true));
assert.throws(() => book("ada", { tier: "gold" }), RangeError);
// The flag accepts any value, and a truthy string silently books a premium seat.
assert.equal(bookBefore("ada", "no").fee, 40);
console.log("booking: tests pass");
```

```output
booking: tests pass
```

In the second version, each call says what it does: `premiumBook(ada)`, or `book(ada, { tier: "premium" })` when the tier comes from a form. A value that is not a tier fails at once, where the flag version took any truthy value as true.

| Language | What the example shows |
|---|---|
| JavaScript | explicit functions and a named option, as above |
| Python | explicit functions, and a keyword-only enumeration argument |
| Java | explicit methods, and an `enum` in place of the boolean |
| Go | explicit functions, and a named type with constants |

## Consequences

Each call reads as what it does, and each function does one thing. The tests name the operation that they cover.

The number of functions grows. Two flags give four combinations, and four explicit functions for them can be worse than one function with two named options. Remove the flag when the values pick operations that the callers think of as different; use a named option when the value is a setting that the callers pass through.

A hidden implementation that takes the flag keeps the tangled body in one place. That is a step, not an end: the branches still share a body, and a later change can separate them.

## How to fix it

- **The callers pass a literal true or false:** [[remove-flag-argument|Remove Flag Argument]], with one function for each value.[^2]
- **The callers pass a value from data or a UI control:** keep one function, and make the choice a named option or an enumeration.[^1]
- **The flag can be derived from another argument:** remove it, and let the function derive it.[^1]
- **Several flags together:** see [[long-parameter-list|Long Parameter List]], and consider an options object or a parameter object.

## Variants and related patterns

**Long Parameter List.** Flags are often how a parameter list grows. See [[long-parameter-list|Long Parameter List]].

**Boolean type.** The type itself, and why a value with two states says little about what the states mean. See [[boolean-type|Boolean type]].

**Repeated Switches.** A flag that is tested in many functions is a switch on the same value in many places. See [[repeated-switches|Repeated Switches]].

## References
