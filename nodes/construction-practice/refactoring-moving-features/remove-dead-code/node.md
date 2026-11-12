---
tag: H8PMMW
kind: technique
slug: remove-dead-code
name: Remove dead code
aliases: [delete dead code]
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Delete code that no longer runs or whose results nobody uses, after you check that nothing reaches it; version control keeps the old version if you ever need it.
family: refactoring
polarity: positive
intent: "Delete code that does nothing, so that readers do not have to work out that it does nothing."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Remove Dead Code is in the chapter on moving features of the second edition of Refactoring (2018), and Dead Code is one of the smells in the book's third chapter.
see_also: [dead-code-smell, smell-detection-tools, speculative-generality, git, code-smell]
sources:
  - url: https://refactoring.com/catalog/removeDeadCode.html
    title: Martin Fowler, Catalog of Refactorings, Remove Dead Code
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Remove Dead Code, motivation and mechanics
  - url: https://github.com/jendrikseipp/vulture
    title: Vulture, README
    locator: the introduction; confidence values
  - url: https://go.dev/blog/deadcode
    title: Alan Donovan, Finding unreachable functions with deadcode, The Go Blog, 12 December 2023
    locator: the introduction; Rapid Type Analysis; Tests
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Import declarations; Variable declarations, implementation restriction
  - url: https://eslint.org/docs/latest/rules/no-unused-vars
    title: ESLint, no-unused-vars
    locator: Rule Details
---

## Purpose

Dead code is code that never runs, or that runs but whose result nobody uses. It costs the reader: they must read it, work out that it does nothing, and remember that it does nothing. It also costs the next change, because a search finds it and someone must decide whether to change it too.

Remove dead code deletes it. The catalogue card shows the plainest case, a block guarded by `if (false)`.[^1] The book's argument against keeping dead code for a possible future need is version control: if the code is needed again, the old revision still has it.[^2] Commenting code out, in place of deleting it, made sense before version control was common, and the book does not recommend it now.[^2] See [[dead-code-smell|Dead Code]] for the smell and [[git|version control]] for the history it keeps.

## Steps

1. If the dead code can be reached from outside, for example a whole function or a public method, search for callers first.[^2]
2. Delete the code.
3. Test.

Step 1 is where the risk is. A search finds direct calls. It does not find calls through reflection, a name built from a string, a plugin that loads the code at run time, or a client outside the repository.

## Example

A pricing module has a branch for a currency that the shop stopped selling in, a helper that nothing calls, and a flag that is always off. After the refactoring, they are gone, and the tests still pass.

```javascript run id=gone
import assert from "node:assert/strict";

// Before: three kinds of dead code.
const LEGACY_ROUNDING = false;                  // a flag that is always off
function legacyRound(c) { return Math.floor(c / 5) * 5; } // only the dead branch calls it
function oldRate(cur) { return cur === "DEM" ? 1.95583 : 1; } // nothing calls it

function priceBefore(cents, cur) {
  if (LEGACY_ROUNDING) return legacyRound(cents);
  if (cur === "EUR") return cents;
  throw new Error(`unsupported currency ${cur}`);
}

// After: only the live code.
function price(cents, cur) {
  if (cur === "EUR") return cents;
  throw new Error(`unsupported currency ${cur}`);
}

assert.equal(price(1999, "EUR"), priceBefore(1999, "EUR"));
assert.throws(() => price(100, "USD"), /unsupported/);
assert.throws(() => priceBefore(100, "USD"), /unsupported/);
console.log("remove dead code: tests pass");
```

```output
remove dead code: tests pass
```

The old helpers are still in the history of the repository. If someone asks why the module has no Deutsche Mark rate, `git log -S oldRate` finds the commit that removed it.

| Language | What the example shows |
|---|---|
| JavaScript | an always-false flag, a helper only the dead branch calls, and an unused function, removed, as above |
| Python | the same removal, with a note on code that is reached only by name, which a search can miss |
| Go | the same removal; the compiler reports unused imports and local variables, which helps step 1 |

## When to use

- **A branch that cannot run.** A condition that is always false, or a case for data that no longer exists.
- **A function with no callers.** After a change, the old version of a function often stays behind.
- **Code kept for a future that did not come.** That is [[speculative-generality|Speculative Generality]].
- **Commented-out code.** Delete it; version control has it.[^2]

## Pitfalls

**Callers you cannot see.** Public APIs, reflection, dependency injection by name, serialization frameworks and scripts outside the repository all reach code that a search misses. For a public API, deprecate first, and remove in a later release.

**Code that only the tests call.** A function that only tests use is dead in the product. Delete the tests with it.

**Feature flags.** Code behind a flag that is off is not dead if the flag can be turned on. Find out whether the flag is still in use before you remove the code it guards.

## Tools that do it

Tools find candidates; step 1 and the tests decide.

- **Python.** "Vulture finds unused code in Python programs."[^3] It gives each finding a confidence value, and its README warns that, because of Python's dynamic nature, it is likely to miss some dead code, and that code called only implicitly may be reported as unused.[^3]
- **Go.** The `deadcode` command "reports unreachable functions in Go programs".[^4] It uses Rapid Type Analysis, starting from the entry points of the main packages, and its `-test` flag adds the test programs as entry points.[^4] The language itself helps: the specification makes an import that the package does not use illegal, and lets a compiler reject a local variable that is never used.[^5]
- **JavaScript.** ESLint's no-unused-vars rule "is aimed at eliminating unused variables, functions, and function parameters".[^6]

For more tools, see [[smell-detection-tools|smell detection tools]].

## References
