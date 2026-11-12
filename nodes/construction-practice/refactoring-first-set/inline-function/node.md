---
tag: F5JJJY
kind: technique
slug: inline-function
name: Inline function
aliases: [Inline Method, inline call]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Replace each call of a function with the body of the function, then delete the function, when the body says what the function does as clearly as its name.
family: refactoring
polarity: positive
intent: "Remove a function whose name adds nothing to its body, so that the reader has one less step to follow."
also_known_as: [Inline Method]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) named it Inline Method. The second edition (2018) renamed it Inline Function. Its inverse is Extract Function.
see_also: [extract-function, lazy-element, middle-man, speculative-generality, code-smell]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Inline Function, motivation and mechanics
  - url: https://refactoring.com/catalog/inlineFunction.html
    title: Martin Fowler, Catalog of Refactorings, Inline Function
    locator: the entry; inverse of Extract Function; alias Inline Method
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Inline Method
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: Inline call to function, refactor.inline.call
  - url: https://www.jetbrains.com/help/idea/inline.html
    title: IntelliJ IDEA documentation, Inline
    locator: Inline Method and Inline Variable
  - url: https://rust-analyzer.github.io/book/assists.html
    title: rust-analyzer manual, Assists
    locator: inline_call
---

## Purpose

A function earns its place by its name. When the name says more than the body, the function helps the reader: `isOverdue(invoice)` is quicker to read than a comparison of dates. When the body is as clear as the name, the function is only a step that the reader must follow to learn that nothing is there.

Inline function puts the body back at each call and deletes the function. The second edition of *Refactoring* gives three reasons to do it: the body is as clear as the name, a group of functions is badly divided and is easier to divide again once it is one piece, and a chain of functions that only call each other hides the code that does the work.[^1]

The refactoring changes the structure and keeps the behaviour. Its inverse is [[extract-function|Extract Function]], and the first edition called it Inline Method.[^2][^3] The two are often used together: inline a set of badly cut functions into one, then extract the functions that the code needs.

## Steps

1. Run the tests and see them pass.
2. Check that the function is not polymorphic. If a subclass overrides it, or an interface declares it, inlining one call changes what the other types do. Inline only a function with one implementation.
3. Find all the callers. A function that is exported, called by name from configuration, or called by reflection has callers that a search of the source does not show.
4. Replace one call with the body. Bind each parameter to its argument, and keep the order of evaluation: an argument that has a side effect, or that the body uses twice, becomes a local variable first.
5. Run the tests.
6. Repeat for each call, testing after each one.
7. Delete the function.

When the body has more than one `return`, or uses `this` or early exits in a way that does not fit the caller, the inline is not a copy and paste. Change the body first, so that it has one exit, or leave the function.

## Example

A rating function calls a helper that only compares a number. The name `moreThanFiveLateDeliveries` says the same thing as `driver.lateDeliveries > 5`, so the helper is a step with nothing in it.

```javascript run id=put-the-body-back
import assert from "node:assert/strict";

// Before: a helper whose name says the same thing as its body.
function moreThanFiveLateDeliveries(driver) {
  return driver.lateDeliveries > 5;
}
function ratingBefore(driver) {
  return moreThanFiveLateDeliveries(driver) ? 2 : 1;
}

// A second helper reads its argument twice. Inlining it must not
// evaluate the argument twice, so the argument becomes a local first.
let reads = 0;
const nextReading = () => (reads += 1, 7);
function squareBefore(x) {
  return x * x;
}
function scoreBefore() {
  return squareBefore(nextReading());
}

// After: the body at the call, and the helpers deleted.
function rating(driver) {
  return driver.lateDeliveries > 5 ? 2 : 1;
}
function score() {
  const x = nextReading(); // one evaluation, as in the call
  return x * x;
}

for (const lateDeliveries of [0, 5, 6, 12]) {
  assert.equal(rating({ lateDeliveries }), ratingBefore({ lateDeliveries }));
}
reads = 0;
assert.equal(scoreBefore(), 49);
assert.equal(reads, 1);
reads = 0;
assert.equal(score(), 49);
assert.equal(reads, 1);
console.log("inline: tests pass");
```

```output
inline: tests pass
```

The first inline is a copy of the expression. The second shows the step that tools take care of: the argument `nextReading()` has a side effect and the body uses its parameter twice, so a textual copy, `nextReading() * nextReading()`, would read the sensor twice. Binding the argument to a local keeps one read.

| Language | What the example shows |
|---|---|
| JavaScript | a helper inlined, and an argument with a side effect bound to a local, as above |
| Python | the same two inlines, and a method that a subclass overrides, which must not be inlined |
| Go | the same two inlines, in the form that `gopls` produces |

## When to use

- **The body is as clear as the name.** The smell is [[lazy-element|Lazy Element]].[^1]
- **A class or a function only forwards.** The smell is [[middle-man|Middle Man]]; inline the forwarding functions, or remove the middle man.
- **The code was cut in the wrong places.** Inline the pieces into one function, then extract along the lines that the code needs.[^1]
- **A layer was added for a need that did not come.** The smell is [[speculative-generality|Speculative Generality]].
- **A deprecated function has a replacement.** The gopls documentation gives the example of a call to `ioutil.ReadFile`, which inlining replaces with a call to `os.ReadFile`.[^4]

Do not inline a function whose name explains a rule, a function with several callers that would each get a copy of a non-trivial body, or a function that a subclass overrides.

## Pitfalls

**Polymorphism.** Inlining a method replaces a dynamic call with a fixed body. If a subclass overrides the method, its version no longer runs from the inlined call. Check for overrides first.

**Evaluation order and side effects.** A textual copy can evaluate an argument twice, or not at all, or in a different order. Bind each argument to a local when it has a side effect or the body uses it more than once.

**Name clashes.** A local variable of the body can have the same name as a variable at the call. Rename one of them before the copy.

**Lost callers.** A search of the source does not find calls by reflection, by name from configuration, or from other repositories. A public function needs a deprecation period, not an inline.

**Copies.** Inlining a function with ten callers makes ten copies of its body. If the body is more than an expression, the copies are [[duplicated-code|Duplicated Code]].

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java, Kotlin | Inline Method, which results "in placing the method's body into the body of its caller(s)"[^5] |
| gopls | Go | `refactor.inline.call`; inlining "replaces the call expression by a copy of the function body, with parameters replaced by arguments"[^4] |
| rust-analyzer | Rust | `inline_call`, which "Inlines a function or method body creating a let statement per parameter unless the parameter can be inlined"[^6] |

The gopls documentation lists what an inliner cannot do. It cannot inline "a dynamic call through a function value or interface method", and it cannot inline a call "if the callee is declared in another package and refers to non-exported parts of that package".[^4] It also states the bar for such a tool: "it's critical that the tool preserve the original behavior of the program."[^4]

## References
