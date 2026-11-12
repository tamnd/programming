---
tag: 9G8Q6V
kind: technique
slug: split-variable
name: Split variable
aliases: [split temp, split temporary variable, remove assignments to parameters]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When one variable is assigned more than once to hold different things, give each thing its own variable, and make each one a constant where the language allows it.
family: refactoring
polarity: positive
intent: "Give each variable one job and one name."
also_known_as: [Split Temporary Variable, Remove Assignments to Parameters]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has Split Temporary Variable and Remove Assignments to Parameters, both in the chapter on composing methods. The second edition (2018) merges them as Split Variable in the chapter on organizing data, and the catalogue keeps the old names as aliases.
see_also: [rename-variable, extract-variable, replace-temp-with-query, slide-statements, mysterious-name, immutability]
sources:
  - url: https://refactoring.com/catalog/splitVariable.html
    title: Martin Fowler, Catalog of Refactorings, Split Variable
    locator: the entry; aliases Remove Assignments to Parameters, Split Temp
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Split Temporary Variable and Remove Assignments to Parameters
  - url: https://eslint.org/docs/latest/rules/no-param-reassign
    title: ESLint, no-param-reassign
    locator: Rule Details
  - url: https://eslint.org/docs/latest/rules/prefer-const
    title: ESLint, prefer-const
    locator: Rule Details
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 9, Organizing Data, Split Variable, motivation, mechanics and the example of assigning to an input parameter
---

## Purpose

A variable should hold one thing. When the same variable holds a perimeter and then an area, or the raw input and then the cleaned input, its name fits at most one of them, and a reader must track which one it holds at each line.

Split variable gives each of those things its own variable. The catalogue card turns a `temp` that holds first a perimeter and then an area into two constants, `perimeter` and `area`.[^1] The first edition had two refactorings for this, Split Temporary Variable and the case of a parameter that the function assigns to, Remove Assignments to Parameters; the catalogue lists both as aliases.[^1][^2]

The book makes one exception: a collecting variable, such as a sum or a string that a loop builds, is assigned many times on purpose and holds one thing all along. Do not split it.[^3]

## Steps

1. At the declaration and the first assignment, change the name of the variable to one that says what it holds.[^3]
2. If you can, declare it as a constant: `const` in JavaScript, `final` in Java.
3. Change all the references up to the second assignment to use the changed name.
4. At the second assignment, declare a variable with its own name.
5. Test.
6. Repeat for each later assignment.

## Example

A function cleans a user name. It assigns to its parameter twice, and then reuses one `result` for two different values. After the split, each value has a name and is a constant.

```javascript run id=one-job-each
import assert from "node:assert/strict";

function handleBefore(input) {
  input = input.trim();                  // the parameter now holds the trimmed text
  input = input.toLowerCase();           // and now the lowercase text
  let result = input.replace(/\s+/g, "-");
  result = result.length > 12 ? result.slice(0, 12) : result;
  return result;
}

function handle(rawInput) {
  const trimmed = rawInput.trim();
  const lower = trimmed.toLowerCase();
  const slug = lower.replace(/\s+/g, "-");
  const clipped = slug.length > 12 ? slug.slice(0, 12) : slug;
  return clipped;
}

for (const s of ["  Ana Pires ", "Very Long Display Name", "x"]) assert.equal(handle(s), handleBefore(s));
assert.equal(handle("  Ana Pires "), "ana-pires");
console.log("split variable: tests pass");
```

```output
split variable: tests pass
```

With one name for each value, each line can now be read on its own, and any of the steps can be extracted or inlined. A reader no longer has to ask which `input` a line sees.

| Language | What the example shows |
|---|---|
| JavaScript | a reassigned parameter and a reused result split into constants, as above |
| Python | the same split; Python has no `const`, so the split rests on the names, and a `Final` annotation for a type checker |
| Go | the same split with `:=`, where each value gets its own name |

## When to use

- **A variable assigned twice for different things.** That is the case the refactoring is for.[^3]
- **A parameter that the function assigns to.** Callers and readers expect a parameter to hold what was passed in.[^2]
- **Before Extract Function or Replace Temp with Query.** A variable with one job is easier to replace with a query. See [[replace-temp-with-query|Replace Temp with Query]].

## Pitfalls

**Collecting variables.** A running total or a loop counter is assigned many times and holds one thing. Leave it.[^3]

**Pass by reference.** In a language where a parameter can be a reference to the caller's variable, assigning to it changes the caller's value. Then the assignment is part of the function's behavior, and splitting it changes what the function does.

**Long functions.** Many variables with one job each can make a long function longer. That is a sign to extract functions, not a reason to keep the reuse.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. Linter rules can point at the places to start. ESLint's no-param-reassign rule "aims to prevent unintended behavior caused by modification or reassignment of function parameters".[^4] Its prefer-const rule "is aimed at flagging variables that are declared using the let keyword, but never reassigned after the initial assignment", which finds the variables that step 2 can make constants.[^5]

## References
