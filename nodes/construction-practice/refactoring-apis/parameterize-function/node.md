---
tag: K5GKAQ
kind: technique
slug: parameterize-function
name: Parameterize function
aliases: [parameterize method]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Merge two or more functions that do the same work with different literal values into one function that takes the value as a parameter.
family: refactoring
polarity: positive
intent: "Turn the values that differ into a parameter."
also_known_as: [Parameterize Method]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) calls it Parameterize Method. The second edition (2018) calls it Parameterize Function and puts it in the chapter on refactoring APIs; the catalogue keeps the old name as an alias.
see_also: [duplicated-code, remove-flag-argument, change-function-declaration, extract-function, long-parameter-list]
sources:
  - url: https://refactoring.com/catalog/parameterizeFunction.html
    title: Martin Fowler, Catalog of Refactorings, Parameterize Function
    locator: the entry, the tenPercentRaise sketch; aliases Parameterize Method
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry parameterizeFunction, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Parameterize Function
  - url: https://www.jetbrains.com/help/idea/extract-parameter.html
    title: JetBrains, IntelliJ IDEA documentation, Extract parameter
    locator: the first paragraph
---

## Purpose

Two functions that differ only in a number or a string are one function written twice. A change to the shared logic must be made in each copy, and a third value needs a third copy.

Parameterize function replaces the copies with one function and makes the value that differed a parameter. The catalogue card merges `tenPercentRaise` and `fivePercentRaise` into `raise(aPerson, factor)`.[^1] The first edition called the refactoring Parameterize Method, and the catalogue keeps that name as an alias.[^1][^2]

## Steps

1. Choose one of the similar functions as the start.
2. Use Change Function Declaration to add a parameter for each literal that differs. See [[change-function-declaration|Change Function Declaration]].
3. In the body, replace the literal with the parameter. Test.
4. Change each caller of that function to pass the literal it needs. Test.
5. For each other similar function, replace its body with a call to the new function, then inline it into its callers. Test after each one.

If the functions differ in more than a value, for example in a range with a low and a high bound, the new function may need more than one parameter. If the logic differs, the functions are not copies, and this refactoring does not apply.

## Example

Three functions charge a base rate for usage inside a band. They differ only in the bounds of the band. After the refactoring, `withinBand(usage, bottom, top)` replaces all three.

```javascript run id=one-band-function
import assert from "node:assert/strict";

function bottomBand(usage) { return Math.min(usage, 100); }
function middleBand(usage) { return usage > 100 ? Math.min(usage, 200) - 100 : 0; }
function topBand(usage) { return usage > 200 ? usage - 200 : 0; }

function withinBand(usage, bottom, top) {
  return usage > bottom ? Math.min(usage, top) - bottom : 0;
}

for (const usage of [0, 50, 100, 150, 200, 350]) {
  assert.equal(withinBand(usage, 0, 100), bottomBand(usage));
  assert.equal(withinBand(usage, 100, 200), middleBand(usage));
  assert.equal(withinBand(usage, 200, Infinity), topBand(usage));
}

const charge = (usage) =>
  withinBand(usage, 0, 100) * 0.03 + withinBand(usage, 100, 200) * 0.05 + withinBand(usage, 200, Infinity) * 0.07;
assert.equal(charge(350).toFixed(2), "18.50");
console.log("parameterize function: tests pass");
```

```output
parameterize function: tests pass
```

The test checks that the new function gives the same result as each old function, at the bounds and between them, before the old functions are removed.

| Language | What the example shows |
|---|---|
| JavaScript | three band functions merged into `withinBand`, as above |
| Python | the same merge; `math.inf` is the top bound of the last band |
| Go | the same merge with integer usage; `math.MaxInt` stands for no top bound |

## When to use

- **Duplicated code that differs in a value.** See [[duplicated-code|Duplicated Code]].
- **A family of functions named after a value**, such as `tenPercentRaise` and `fivePercentRaise`.[^1]
- **Before a new case.** When a new value is needed, a parameter is cheaper than a new copy.

## Pitfalls

**A flag is not a value.** If the new parameter is a boolean that selects between two different behaviors, the result is a flag argument. That is the opposite move. See [[remove-flag-argument|Remove Flag Argument]].

**Too many parameters.** Each merge can add a parameter. When a function needs many of them, look at [[long-parameter-list|Long Parameter List]].

**Callers lose a name.** `tenPercentRaise(p)` says what it does at the call. `raise(p, 0.1)` needs the reader to know what `0.1` means. A named constant at the call, or a thin wrapper for a common value, keeps the name.

## Tools that do it

IDEs do the core step. In IntelliJ IDEA, "The Extract Parameter refactoring lets you extract a new parameter to a method."[^4] The tool replaces a literal in the body with a parameter and updates the callers. Merging the other copies is then a manual step.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag refactoring-apis.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
