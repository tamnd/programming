---
tag: YR8CYJ
kind: pattern
slug: dead-code-smell
name: Dead code
aliases: [dead code, unreachable code, unused code]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: the source has functions that nobody calls, branches that cannot run and values that nobody reads, and each reader must still read them and keep them working.
family: code-smell
polarity: negative
intent: "Find the code that the program never runs or whose result it never uses, make sure of it with tools and tests, and delete it, so that the source says only what the program does."
also_known_as: [Unreachable code, Unused code]
refactorings: [remove-dead-code]
history:
  year: 2018
  people: [Martin Fowler]
  note: The second edition of Refactoring (2018) has the refactoring Remove Dead Code, and the catalogue at refactoring.com lists it.
see_also: [code-smell, speculative-generality, lazy-element, unit-testing]
sources:
  - url: https://en.wikipedia.org/w/index.php?title=Dead_code&oldid=1325383945
    title: Wikipedia, Dead code, revision 1325383945
    locator: the introduction
  - url: https://en.wikipedia.org/w/index.php?title=Unreachable_code&oldid=1383170496
    title: Wikipedia, Unreachable code, revision 1383170496
    locator: the introduction and Analysis
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Remove Dead Code
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the card Remove Dead Code
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
    title: The Java Language Specification, Java SE 25 Edition, Chapter 14
    locator: 14.22, Unreachable Statements
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Variable declarations and Import declarations
  - url: https://eslint.org/docs/latest/rules/no-unreachable
    title: ESLint, no-unreachable
    locator: the rule summary
  - url: https://staticcheck.dev/docs/checks/
    title: Staticcheck, Checks
    locator: SA4006
  - url: https://coverage.readthedocs.io/
    title: Coverage.py documentation
    locator: the introduction
---

## Intent

Find the code that is in the source but does no work: functions and classes that nothing calls, branches whose condition is never true, statements after a `return`, and values that are computed and never read. Check with the compiler, a linter and coverage that the code is in fact dead, then delete it. The version control system keeps it, if it is ever needed again.

## Problem

The term has two meanings. Wikipedia notes that some use dead code for code "which can never be executed at run-time", and that in other areas it is code "which is executed but whose result is never used in any other computation."[^1] Code of the first kind is also called unreachable code: code that "can never be executed because there exists no control flow path to the code from the rest of the program."[^2] For a reader, both kinds cost the same: lines to read, understand and keep working, that change nothing in what the program does.

The forces are these:

- **Dead code looks live.** A function with a good name and tests looks like part of the program. Wikipedia notes that "Test scaffolding can make it appear that the code is still live".[^1] A reader who changes a rule must decide whether the dead copy needs the change too.
- **Code dies quietly.** A feature is removed but its helper stays. A branch handles a case that a check further up now rejects. A flag is always set the same way. No one decides to keep the code; no one notices that it died.
- **Fear keeps it.** Programmers keep code in case it is needed again, or comment it out. The second edition of *Refactoring* answers this with version control: once the code is deleted, the history still has it.[^3]
- **Removal can change behaviour.** Code whose result is not used can still have an effect. Wikipedia warns that a dead computation "may raise exceptions or affect some global state, thus removal of such code may change the output of the program and introduce unintended bugs."[^1]

## Solution

The refactoring is [[remove-dead-code|Remove Dead Code]], which the catalogue at refactoring.com lists.[^4]

1. **Find candidates.** Use the tools that the language gives. Java's compiler refuses some of it: "It is a compile-time error if a statement cannot be executed because it is unreachable".[^5] Go's compiler refuses an import without a use, because "It is illegal for a package to import itself, directly or indirectly, or to directly import a package without referring to any of its exported identifiers."[^6] Linters find more: ESLint's rule `no-unreachable` reports code after `return`, `throw`, `continue` and `break`,[^7] and Staticcheck's SA4006 reports "A value assigned to a variable is never read before being overwritten."[^8]
2. **Confirm.** Static tools see the source; they cannot see calls by reflection, by name from configuration, or from other programs that use a public API. Coverage helps here. Coverage.py "monitors your program, noting which parts of the code have been executed".[^9] Code that a full run of the tests and a period in production never executes is a strong candidate.
3. **Delete.** Remove the code, its tests and its comments in one commit with a message that says what was removed and why. Do not comment it out.
4. **Run the tests.** If they pass and the code was in fact dead, nothing else changes.

Wikipedia notes that detection is a form of control flow analysis, and that "the sophistication of the analysis has a significant impact on the amount of unreachable code that is detected."[^2] No tool finds all dead code, so the review of a change that removes a feature should also look for the helpers it leaves behind.

## Example

A pricing module was written when the shop sold in three currencies. It now sells only in euros, a check at the edge refuses other currencies, and the code for the other two is still there, with a helper that nothing calls and a value that is computed and thrown away.

```javascript run id=delete-what-does-not-run
import assert from "node:assert/strict";

const accepted = new Set(["EUR"]);
function checkCurrency(c) {
  if (!accepted.has(c)) throw new RangeError(`currency ${c} is not accepted`);
  return c;
}

// Before: branches that the edge check makes unreachable, a helper that nothing
// calls, and a value that is computed and never read.
function legacyRoundUsd(cents) {
  return Math.round(cents / 5) * 5;
}
function priceBefore(cents, currency) {
  checkCurrency(currency);
  const withTax = Math.round(cents * 1.2);
  const display = (withTax / 100).toFixed(2); // never read
  if (currency === "USD") return { cents: withTax + 30, currency };
  if (currency === "GBP") return { cents: withTax + 20, currency };
  return { cents: withTax, currency };
  console.log("priced"); // unreachable
}

// After: only the code that runs.
function price(cents, currency) {
  checkCurrency(currency);
  return { cents: Math.round(cents * 1.2), currency };
}

for (const cents of [0, 1, 999, 1250]) {
  assert.deepEqual(price(cents, "EUR"), priceBefore(cents, "EUR"));
}
for (const c of ["USD", "GBP"]) {
  assert.throws(() => priceBefore(100, c), RangeError);
  assert.throws(() => price(100, c), RangeError);
}
assert.equal(typeof legacyRoundUsd, "function"); // still in the source, and no caller
console.log("price: tests pass");
```

```output
price: tests pass
```

The second version behaves the same for every input that the edge check lets through, and it fails the same way for the rest. It is shorter, and a reader of it does not wonder whether dollars are still sold.

| Language | What the example shows |
|---|---|
| JavaScript | unreachable branches, an unused helper and an unread value, removed, as above |
| Python | the same module, and a check with `coverage` of which lines a test run executes |
| Java | a statement after `return` that the compiler refuses, and an `if (false)` that it allows |
| Go | an unused local variable and an unused import, which the compiler refuses, and an unused function, which it allows |

## Consequences

The source is shorter and says what the program does. A search for a rule finds only the live copy, and a change does not need to keep dead paths compiling.

Some code looks dead and is not. A public function of a library may have no caller in its own repository, and many outside. A handler may be called by name from configuration, or by reflection. Before deleting, check for these uses, and for a public API, deprecate first.

Removing code that had side effects changes the program, as Wikipedia warns.[^1] Such code is not dead; it is code whose purpose is hidden. Make the side effect explicit, or confirm that no one needs it, before removing it.

## How to fix it

- **A function, class or module that nothing calls:** [[remove-dead-code|Remove Dead Code]], with its tests.[^4]
- **A statement after a `return` or `throw`:** delete it; the compiler or the linter usually reports it.[^5][^7]
- **A branch that a check elsewhere makes impossible:** delete the branch, and keep the check.
- **A value that is computed and never read:** delete the computation, after a check that it has no side effect.[^8]
- **Commented-out code:** delete it. See [[comments|Comments]].

## Variants and related patterns

**Speculative Generality.** Code written for a need that never came, and that only its tests call, is dead code from the start. See [[speculative-generality|Speculative Generality]].

**Lazy Element.** An element that does almost nothing, as opposed to one that is never used. See [[lazy-element|Lazy Element]].

**Detecting smells with tools.** Compilers, linters and coverage find most dead code. See [[smell-detection-tools|Detecting smells with tools]].

## References
