---
tag: RDM2X3
kind: technique
slug: inline-variable
name: Inline variable
aliases: [Inline Temp]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Replace the uses of a local variable with the expression that it holds, then delete the variable, when the name says no more than the expression.
family: refactoring
polarity: positive
intent: "Remove a local variable whose name adds nothing to its expression, or that stands in the way of another refactoring."
also_known_as: [Inline Temp]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) named it Inline Temp. The second edition (2018) renamed it Inline Variable. Its inverse is Extract Variable.
see_also: [extract-variable, inline-function, extract-function, code-smell]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Inline Variable, motivation and mechanics
  - url: https://refactoring.com/catalog/inlineVariable.html
    title: Martin Fowler, Catalog of Refactorings, Inline Variable
    locator: the entry; inverse of Extract Variable; alias Inline Temp
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Inline Temp
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: Inline local variable, refactor.inline.variable
  - url: https://www.jetbrains.com/help/idea/inline.html
    title: IntelliJ IDEA documentation, Inline
    locator: Inline Variable
  - url: https://rust-analyzer.github.io/book/assists.html
    title: rust-analyzer manual, Assists
    locator: inline_local_variable
---

## Purpose

A local variable is a name for a value. Often the name helps: `basePrice` says more than `order.quantity * order.itemPrice`. Sometimes it does not: `const price = order.price; return price > 1000;` gives the reader a second name for the same thing.

Inline variable replaces each use of the variable with its expression and deletes the declaration. The second edition of *Refactoring* gives two reasons: the name communicates no more than the expression, and the variable gets in the way of refactoring the code around it.[^1] A common case of the second is [[extract-function|Extract Function]]: a fragment that uses a local variable needs it as a parameter, and inlining the variable first can make the extraction cleaner.

The refactoring changes the structure and keeps the behaviour. Its inverse is [[extract-variable|Extract Variable]], and the first edition called it Inline Temp.[^2][^3]

## Steps

1. Run the tests and see them pass.
2. Check that the expression on the right has no side effect, and that its value is the same at each use as at the declaration. If something between the declaration and a use changes a value that the expression reads, the inline changes the result.
3. If the variable is not already immutable, make it so (`const` or `final`) and compile or test. This finds any later assignment.
4. Replace the first use with the expression, and test.
5. Replace each other use in the same way, testing after each.
6. Delete the declaration and the assignment, and test.

## Example

A function names a value that it reads once, and another function caches a value that it then changes.

```javascript run id=use-the-expression
import assert from "node:assert/strict";

// Before: a variable that adds no meaning.
function isLargeBefore(order) {
  const basePrice = order.basePrice;
  return basePrice > 1000;
}

// After: the expression in place.
function isLarge(order) {
  return order.basePrice > 1000;
}

for (const basePrice of [0, 1000, 1001]) {
  assert.equal(isLarge({ basePrice }), isLargeBefore({ basePrice }));
}

// Not every variable can be inlined. Here the expression reads a value
// that changes between the declaration and the use.
function totalWithFee(cart) {
  const subtotal = cart.items.reduce((s, i) => s + i, 0);
  cart.items.push(5); // the fee is an item too
  return subtotal + 5;
}
function totalWithFeeWrong(cart) {
  cart.items.push(5);
  return cart.items.reduce((s, i) => s + i, 0) + 5; // inlined after the push
}
assert.equal(totalWithFee({ items: [10, 20] }), 35);
assert.equal(totalWithFeeWrong({ items: [10, 20] }), 40);
console.log("inline variable: tests pass");
```

```output
inline variable: tests pass
```

The first inline is safe: the expression is a property read, and nothing changes it between the declaration and the use. The second shows the check of step 2. `subtotal` holds the sum before the fee is pushed, and the inlined expression would sum after it, so the variable must stay.

| Language | What the example shows |
|---|---|
| JavaScript | a variable inlined, and a variable that cannot be, as above |
| Python | the same two cases |
| Go | the same two cases, and the shadowing case that `gopls` refuses |

## When to use

- **The name says no more than the expression.**[^1] This is often a variable that holds the result of a call and is used once on the next line.
- **The variable is in the way.** It blocks an [[extract-function|Extract Function]], or a [[replace-temp-with-query|Replace Temp with Query]], or it ties two parts of a function that should be split.[^1]
- **After Extract Variable went too far.** A chain of variables each used once can read worse than the expression it replaced.

Keep the variable when the name explains, when the expression is expensive and used more than once, or when its value must be taken at a particular moment.

## Pitfalls

**A value that changes.** The variable holds the value at the moment of the declaration. If the code between the declaration and a use changes what the expression reads, the inlined expression computes something else. The example shows this.

**Side effects and cost.** An expression that calls a function with an effect, or that is slow, runs once in the variable and once per use after the inline.

**Shadowing.** A name in the expression can mean something else at the point of use, if an inner block declares a variable with the same name. The gopls documentation shows this case and says that the code action "reports an error if it is not possible to make the transformation because one of the identifiers within the initializer expression" is "shadowed by an intervening declaration".[^4]

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java, Kotlin | Inline Variable, which "replaces redundant variable usage with its initializer"; "The variable must be initialized at declaration."[^5] |
| gopls | Go | `refactor.inline.variable`, which replaces "the reference by the variable’s initializer expression"[^4] |
| rust-analyzer | Rust | `inline_local_variable`, which "Inlines a local variable."[^6] |

The tools differ in how much they check. IntelliJ IDEA notes that if the initial value is modified, only the uses before the change are inlined.[^5] gopls "always replaces the reference by the initializer expression, even if there are later assignments to the variable", and it leaves the declaration for the programmer to delete.[^4]

## References
