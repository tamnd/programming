---
tag: HTTQKT
kind: technique
slug: extract-variable
name: Extract variable
aliases: [Introduce Explaining Variable, Introduce Variable]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Give a part of a complex expression a name, by moving it into a local variable and using the variable in its place, so that the expression reads as the steps of a rule.
family: refactoring
polarity: positive
intent: "Name a part of an expression, so that a reader sees what it means and not only how it is computed."
also_known_as: [Introduce Explaining Variable]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) named it Introduce Explaining Variable. The second edition (2018) renamed it Extract Variable. Its inverse is Inline Variable.
see_also: [extract-function, inline-function, mysterious-name, code-smell]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Extract Variable, motivation and mechanics
  - url: https://refactoring.com/catalog/extractVariable.html
    title: Martin Fowler, Catalog of Refactorings, Extract Variable
    locator: the entry; inverse of Inline Variable; alias Introduce Explaining Variable
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Introduce Explaining Variable
  - url: https://www.jetbrains.com/help/idea/extract-variable.html
    title: IntelliJ IDEA documentation, Extract/Introduce variable
    locator: the introduction
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript
    locator: Extract to constant
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: refactor.extract.variable and refactor.extract.variable-all
  - url: https://rust-analyzer.github.io/book/assists.html
    title: rust-analyzer manual, Assists
    locator: extract_variable
---

## Purpose

An expression can be correct and still hard to read. `order.quantity * order.itemPrice - Math.max(0, order.quantity - 500) * order.itemPrice * 0.05 + Math.min(order.quantity * order.itemPrice * 0.1, 100)` computes a price, but the reader must take it apart to see the base price, the discount for quantity and the shipping.

Extract variable gives each part a name: `basePrice`, `quantityDiscount`, `shipping`. The expression becomes `basePrice - quantityDiscount + shipping`, which says the rule. The second edition of *Refactoring* gives the reason: a local variable names a piece of a complex expression, so that a reader sees its purpose, and a debugger can show its value.[^1]

The refactoring changes the structure and keeps the behaviour. Its inverse is [[inline-variable|Inline Variable]], and the first edition called it Introduce Explaining Variable.[^2][^3]

## Steps

1. Run the tests and see them pass.
2. Check that the part of the expression has no side effect. Moving a call with a side effect to an earlier line can change the order of effects.
3. Declare a local variable, preferably immutable (`const`, `final`, `let` in Rust), and set it to the part of the expression.
4. Replace the part in the expression with the variable.
5. Run the tests.
6. If the same part appears more than once in the function, replace each copy, after a check that each one has the same value at that point.

When the name is useful outside the function, the second edition suggests [[extract-function|Extract Function]] in place of a variable, so that other code can use the name too.[^1] In a class, that often means a method or a getter.

## Example

The price formula is one expression with three ideas in it.

```javascript run id=name-the-parts
import assert from "node:assert/strict";

// Before: one expression, three ideas.
function priceBefore(order) {
  return (
    order.quantity * order.itemPrice -
    Math.max(0, order.quantity - 500) * order.itemPrice * 0.05 +
    Math.min(order.quantity * order.itemPrice * 0.1, 100)
  );
}

// After: each idea has a name.
function price(order) {
  const basePrice = order.quantity * order.itemPrice;
  const quantityDiscount = Math.max(0, order.quantity - 500) * order.itemPrice * 0.05;
  const shipping = Math.min(basePrice * 0.1, 100);
  return basePrice - quantityDiscount + shipping;
}

for (const order of [
  { quantity: 1, itemPrice: 10 },
  { quantity: 500, itemPrice: 2 },
  { quantity: 501, itemPrice: 2 },
  { quantity: 2000, itemPrice: 3 },
]) {
  assert.equal(price(order), priceBefore(order));
}
assert.equal(price({ quantity: 2000, itemPrice: 3 }), 6000 - 225 + 100);
console.log("price: tests pass");
```

```output
price: tests pass
```

The second version also reuses `basePrice` in the shipping rule, where the first version wrote the product out a second time. A reader now sees that shipping is ten percent of the base price, up to 100.

| Language | What the example shows |
|---|---|
| JavaScript | three parts of a price named, as above |
| Python | the same three parts, and a condition named with a boolean variable |
| Go | the same three parts, in the form that `gopls` extracts |

## When to use

- **A complex expression.** Name its parts, so that the expression reads as the rule.[^1]
- **A complex condition.** A variable such as `isEligible` or `isWeekend` names the test, and the `if` reads as the rule. This fixes a small [[mysterious-name|Mysterious Name]].
- **A repeated subexpression.** Compute it once, after a check that each copy has the same value.
- **A value to watch in a debugger.** A named variable can be inspected; a part of an expression usually cannot.

When the name is useful beyond this function, use [[extract-function|Extract Function]] instead.[^1]

## Pitfalls

**Side effects and order.** Moving a part of an expression to an earlier line moves its evaluation. If it calls a function with an effect, or reads a value that the rest of the expression changes, the result can differ.

**Short-circuit operators.** In `a && f(b)`, `f(b)` runs only if `a` is true. A variable `const fb = f(b)` before the `if` runs it always, which can throw or do work that was skipped before.

**Names that repeat the code.** `const quantityTimesPrice = quantity * itemPrice` adds a line and no meaning. Name the role, `basePrice`, not the computation.

**Mutation.** A variable that is assigned again later is not a name for one value. Keep extracted variables immutable, and see [[split-variable|Split Variable]] when one variable holds several things.

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java, Kotlin | Extract Variable, which places "the result of such expression or its part into a separate variable that is less complex and easier to understand"[^4] |
| Visual Studio Code | TypeScript, JavaScript | Extract to constant: "Extract the selected expression to a new constant in the file."[^5] |
| gopls | Go | `refactor.extract.variable`, which "replaces an expression by a reference to a new local variable named newVar initialized by the expression", and `refactor.extract.variable-all` for all copies in the function[^6] |
| rust-analyzer | Rust | `extract_variable`, which "Extracts subexpression into a variable."[^7] |

The tools choose a placeholder name, such as `newVar` in gopls, and then start a rename. The name is the part of the refactoring that a tool cannot do.

## References
