---
tag: GM1PA3
kind: technique
slug: replace-exception-with-precheck
name: Replace exception with precheck
aliases: [replace exception with test]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Replace a try and catch around a call that fails in a known, expected case with a check of that case before the call.
family: refactoring
polarity: positive
intent: "Do not use an exception for a case that the caller can test."
also_known_as: [Replace Exception with Test]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has it as Replace Exception with Test. The second edition (2018) calls it Replace Exception with Precheck, in the chapter on refactoring APIs.
see_also: [replace-error-code-with-exception, exception-handling, introduce-assertion, errors-as-values, replace-nested-conditional-with-guard-clauses]
sources:
  - url: https://refactoring.com/catalog/replaceExceptionWithPrecheck.html
    title: Martin Fowler, Catalog of Refactorings, Replace Exception with Precheck
    locator: the entry, the getValueForPeriod sketch; aliases Replace Exception with Test
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceExceptionWithPrecheck, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Exception with Precheck
  - url: https://docs.python.org/3/glossary.html
    title: Python documentation, Glossary
    locator: the entries EAFP and LBYL
---

## Purpose

An exception is for a problem that the code does not expect. When a call fails in a case that the caller knows about and can test, a `try` and `catch` hides a normal branch of the logic inside the error handling. A reader sees a handler and thinks of a failure, but the case is an ordinary input.

Replace exception with precheck tests the case first. The catalogue card has a Java method `getValueForPeriod` that reads `values[periodNumber]` in a `try` and returns `0` when it catches `ArrayIndexOutOfBoundsException`. After the refactoring, the method returns `(periodNumber >= values.length) ? 0 : values[periodNumber]`.[^1]

## Steps

1. Write a test for the case before the call, and use the value from the `catch` block in that branch. Keep the `try` for now.
2. Add an assertion in the `catch` block, so that a test fails if the handler still runs. See [[introduce-assertion|Introduce Assertion]].
3. Test.
4. Remove the `try` and the `catch`. Test again.

## Example

A list of values has one entry for each period. A period after the last one has the value 0. In JavaScript, a read past the end gives `undefined` and does not throw, so the example uses a function that throws, as a Java array does.

```javascript run id=precheck
import assert from "node:assert/strict";

const values = [12.5, 8, 3.25];

function at(array, i) {
  if (!Number.isInteger(i) || i < 0 || i >= array.length) throw new RangeError(`index ${i}`);
  return array[i];
}

function getValueForPeriodBefore(periodNumber) {
  try {
    return at(values, periodNumber);
  } catch (e) {
    if (e instanceof RangeError) return 0;
    throw e;
  }
}

function getValueForPeriod(periodNumber) {
  return periodNumber >= values.length ? 0 : at(values, periodNumber);
}

for (const p of [0, 2, 3, 10]) assert.equal(getValueForPeriod(p), getValueForPeriodBefore(p));
// The precheck covers only the case it names. A negative period is still an error.
assert.throws(() => getValueForPeriod(-1), RangeError);
console.log([0, 1, 2, 3].map(getValueForPeriod));
```

```output
[ 12.5, 8, 3.25, 0 ]
```

The old handler turned each `RangeError` into 0, also for a negative period, which is a bug in the caller. The precheck names the expected case, and other wrong input still fails.

| Language | What the example shows |
|---|---|
| JavaScript | a `try` around a checked read replaced with a test of the length, as above |
| Python | the same precheck, next to the EAFP form with `try` that is common in Python |
| Go | the same precheck; Go has no exceptions, and a read past the end of a slice panics, so the check is the normal form |

## When to use

- **The failing case is a normal input**, such as a period after the last one, an empty list, or a key that is not present.
- **The test is cheap and exact.** A check of a length or of a key costs little and says what the case is.
- **The handler catches a broad class.** A precheck makes the expected case explicit, and other failures stay visible, as in the example.

## Pitfalls

**Python style.** Python often prefers the opposite. The Python glossary defines EAFP: "Easier to ask for forgiveness than permission. This common Python coding style assumes the existence of valid keys or attributes and catches exceptions if the assumption proves false." It contrasts this with LBYL, "Look before you leap", which "explicitly tests for pre-conditions before making calls or lookups."[^4] In Python, use the precheck when it makes the expected case clearer, not as a rule.

**A race between the check and the call.** The glossary warns that "In a multi-threaded environment, the LBYL approach can risk introducing a race condition between “the looking” and “the leaping”."[^4] The same is true of a file that another process can delete between the check and the open. In that case, keep the exception, or hold a lock.

**The check repeats the work.** If the check costs as much as the call, for example a parse of the input to see if it is valid, the precheck does the work twice. Keep the exception, or use a function that returns an optional value or an error value.

**The caller cannot test the case.** If only the called function can know about the failure, there is no precheck. See [[replace-error-code-with-exception|Replace Error Code with Exception]].

## Tools that do it

No IDE has one action for this refactoring. The steps are done by hand.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tags refactoring-apis and errors.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
