---
tag: CR987H
kind: pattern
slug: long-function
name: Long Function
aliases: [long method, large function]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a function does so many steps that a reader must study its body to know what it does, and the steps cannot be tested or reused one at a time.
family: code-smell
polarity: negative
intent: "Find the functions whose body hides their purpose, and split them into smaller functions whose names say what each step does."
also_known_as: [Long Method]
refactorings: [extract-function, replace-temp-with-query, introduce-parameter-object, preserve-whole-object, replace-function-with-command, decompose-conditional, replace-conditional-with-polymorphism, split-loop]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: The first edition of Refactoring (1999) calls the smell Long Method. The second edition (2018) uses JavaScript and calls it Long Function.
see_also: [code-smell, extract-function, comment, duplicated-code, unit-testing]
sources:
  - url: https://martinfowler.com/bliki/FunctionLength.html
    title: Martin Fowler, Function Length, bliki, 30 November 2016
    locator: the whole article and notes 3 and 4
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Long Method
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Long Function
  - url: https://martinfowler.com/articles/refactoring-2nd-changes.html
    title: Martin Fowler, Changes for the 2nd Edition of Refactoring
    locator: Fate of the refactorings in the first edition, Extract Method and Replace Method with Method Object
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Extract Function, Replace Temp with Query, Decompose Conditional, Split Loop and Replace Function with Command
  - url: https://eslint.org/docs/latest/rules/max-lines-per-function
    title: ESLint documentation, Rules, max-lines-per-function
    locator: Options, max (default 50)
  - url: https://eslint.org/docs/latest/rules/max-statements
    title: ESLint documentation, Rules, max-statements
    locator: Options, max (default 10)
  - url: https://checkstyle.org/checks/sizes/methodlength.html
    title: Checkstyle documentation, MethodLength
    locator: Properties, max (default 150) and countEmpty (default true)
  - url: https://pylint.readthedocs.io/en/stable/user_guide/configuration/all-options.html
    title: Pylint documentation, All configuration options
    locator: Design checker, --max-statements (default 50) and --max-branches (default 12)
  - url: https://golangci-lint.run/docs/linters/configuration/#funlen
    title: golangci-lint documentation, Linters configuration
    locator: funlen, lines (default 60) and statements (default 40)
---

## Intent

Find the functions that a reader must read line by line to understand, and split them into steps with names. Then the reader of the top function sees what it does, and goes into a step only to see how.

## Problem

A long function does many things in sequence: it reads input, checks it, computes, formats and writes. Each part may be correct, but the reader must hold all of them in mind to know what the function does as a whole, and to know which local variable is used where.

The forces are these:

- Length is a sign, not the problem. Fowler calls the question of function length "a proxy for the more important question - when should we enclose code in its own function?" His answer is "the separation between intention and implementation": "If you have to spend effort into looking at a fragment of code to figure out what it's doing, then you should extract it into a function and name the function after that “what”."[^1]
- So a short function can be worth a name. Fowler tells of a Smalltalk method `highlight` whose body was one call to `reverse`. The name was longer than the body, and it was still right, because it said the intent.[^1]
- A long function is hard to test in parts. A test can call the whole function only, so each test must set up all the inputs, even to check one step.
- People resist small functions because of the cost of a call. Fowler writes that this was "occasionally a factor" when he was young, but that such cases are very rare, and that optimizing compilers often work better with shorter functions.[^1]

*Refactoring* calls the smell Long Method in the first edition and Long Function in the second, which moves from Java to JavaScript and is less centred on objects.[^2][^3][^4]

## Solution

Most of the time the fix is [[extract-function|Extract Function]]: find a part of the function that goes together, move it to a function of its own, and give it a name that says what it does.[^3] In the second edition, Extract Method of the first edition became Extract Function.[^4]

*Refactoring* gives cues that show where to cut.[^3]

1. **A comment.** A comment that explains the next block of code is a name waiting for a function. Extract the block, and use the comment as the name.
2. **A conditional.** Use [[decompose-conditional|Decompose Conditional]] to give the condition and each branch a function.
3. **A loop.** Extract the loop and its body. If one loop does two jobs, use [[split-loop|Split Loop]] first, so that each loop does one job.
4. **A switch on a type.** When the same switch appears in other functions too, use [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]].

The extracted parts often need many of the local variables of the long function, so the calls get long parameter lists. *Refactoring* gives three ways to reduce them: [[replace-temp-with-query|Replace Temp with Query]] turns a temporary variable into a function call; [[introduce-parameter-object|Introduce Parameter Object]] and [[preserve-whole-object|Preserve Whole Object]] pass one value in place of many. When a function still has too many shared variables, [[replace-function-with-command|Replace Function with Command]] turns it into an object, and the variables become fields that each step can read.[^3] Each of these refactorings has a card in the online catalogue.[^5]

## Example

The function computes the text of an invoice. It adds up the lines, gives a 10% discount at 100.00 or more, adds 20% tax on the discounted amount and formats the result. Comments mark the steps. All amounts are whole cents, so the arithmetic is exact.

```javascript run id=split-the-steps
import assert from "node:assert/strict";

// Before: one function, with comments for the steps.
function invoiceBefore(order) {
  // add up the lines
  let subtotal = 0;
  for (const line of order.lines) subtotal += line.price * line.qty;
  // discount for large orders
  let discount = 0;
  if (subtotal >= 10000) discount = Math.round(subtotal / 10);
  // tax on the discounted amount
  const tax = Math.round((subtotal - discount) / 5);
  // format
  const total = subtotal - discount + tax;
  return `${order.customer}: ${(total / 100).toFixed(2)}`;
}

// After: each comment became the name of a function.
const subtotal = (lines) => lines.reduce((sum, l) => sum + l.price * l.qty, 0);
const discount = (cents) => (cents >= 10000 ? Math.round(cents / 10) : 0);
const tax = (cents) => Math.round(cents / 5);
const formatCents = (cents) => (cents / 100).toFixed(2);

function invoice(order) {
  const gross = subtotal(order.lines);
  const net = gross - discount(gross);
  return `${order.customer}: ${formatCents(net + tax(net))}`;
}

const large = { customer: "Ada", lines: [{ price: 4000, qty: 2 }, { price: 2500, qty: 1 }] };
const small = { customer: "Bob", lines: [{ price: 1500, qty: 2 }] };
for (const f of [invoiceBefore, invoice]) {
  assert.equal(f(large), "Ada: 113.40");
  assert.equal(f(small), "Bob: 36.00");
  assert.equal(discount(9999), 0);
  console.log(`${f.name}: tests pass`);
}
```

```output
invoiceBefore: tests pass
invoice: tests pass
```

For the large order, the subtotal is 105.00, the discount is 10.50, the tax is 20% of 94.50, which is 18.90, and the total is 113.40. The "after" version reads as the rule itself: the net amount is the gross amount less the discount, and the total is the net amount plus tax. Each step can now be tested alone, as the test of `discount(9999)` shows.

| Language | What the example shows |
|---|---|
| JavaScript | the split above, with arrow functions for the steps |
| Python | the same split, with `sum` over a generator for the subtotal |
| Java | the split into private static methods, with a record for each line |
| Go | the split into unexported functions, with `fmt.Sprintf` for the format |

## Consequences

The top function becomes short, and reads like a list of steps. The steps get names that a test, a stack trace and a search can use. A step that two functions need can be shared, which is one of the fixes for [[duplicated-code|Duplicated Code]].

The cost is more names and more jumps. A reader who wants the whole detail must follow the calls. Fowler's answer is that the names must be good: "Small functions like this only work if the names are good".[^1] A step with a bad name is worse than the same lines inline.

There is no agreed number of lines. Fowler writes that "Any function more than half-a-dozen lines of code starts to smell to me", and that half of the methods in the Ruby code of his website, about 15 KLOC, are two lines or less.[^1] The default limits of linters are much higher:

| Tool | Rule | Default limit |
|---|---|---|
| ESLint | `max-lines-per-function` | 50 lines[^6] |
| ESLint | `max-statements` | 10 statements[^7] |
| Checkstyle | `MethodLength` | 150 lines, with blank lines and comments counted[^8] |
| Pylint | `max-statements` | 50 statements[^9] |
| golangci-lint | `funlen` | 60 lines and 40 statements[^10] |

A linter limit finds the worst cases. It cannot find a ten-line function that does two things, and it can push a programmer to split a function at a point that has no meaning, only to get under the limit.

## How to fix it

- **A block with a comment above it:** [[extract-function|Extract Function]], with the comment as the name.[^3]
- **A temporary variable that blocks an extract:** [[replace-temp-with-query|Replace Temp with Query]].[^3]
- **Many values passed to each step:** [[introduce-parameter-object|Introduce Parameter Object]] or [[preserve-whole-object|Preserve Whole Object]].[^3]
- **Too many shared local variables:** [[replace-function-with-command|Replace Function with Command]].[^3]
- **A complex `if`:** [[decompose-conditional|Decompose Conditional]].[^3]
- **A loop that does two things:** [[split-loop|Split Loop]], then extract each loop.[^3]
- **A switch on a type code:** [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]].[^3]

Run the tests after each step. Each extract is small, so a failing test points to the last change.

## Variants and related patterns

**Large Class.** The same smell one level up: a class with too many fields and methods. See [[large-class|Large Class]].

**Long Parameter List.** A long function is often called with many arguments, and splitting it can make the parameter lists of the parts long. See [[long-parameter-list|Long Parameter List]].

**Comments.** A comment that explains a block is one of the cues for Extract Function. See [[comments|Comments]].

**Too many branches.** A function can be short and still hard to follow when it has many nested branches. Pylint checks this apart from length, with `max-branches` at 12 by default.[^9] See [[cyclomatic-complexity|Cyclomatic complexity]].

## References
