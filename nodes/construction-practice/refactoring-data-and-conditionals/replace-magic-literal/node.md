---
tag: 24HZJQ
kind: technique
slug: replace-magic-literal
name: Replace magic literal
aliases: [replace magic number with symbolic constant, replace magic number, named constant]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When a number or a string in the code has a meaning that the code does not say, give it a name: declare a constant, and use the name in place of the literal.
family: refactoring
polarity: positive
intent: "Give a literal with a meaning a name that says the meaning."
also_known_as: [Replace Magic Number with Symbolic Constant]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has Replace Magic Number with Symbolic Constant in the chapter on organizing data. The online catalogue names it Replace Magic Literal, because the same problem occurs with strings and other literals, and keeps the old name as an alias.
see_also: [mysterious-name, primitive-obsession, extract-variable, rename-variable, constant-evaluation]
sources:
  - url: https://refactoring.com/catalog/replaceMagicLiteral.html
    title: Martin Fowler, Catalog of Refactorings, Replace Magic Literal
    locator: the entry; alias Replace Magic Number with Symbolic Constant
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 8, Organizing Data, Replace Magic Number with Symbolic Constant
  - url: https://eslint.org/docs/latest/rules/no-magic-numbers
    title: ESLint, no-magic-numbers
    locator: the introduction and Rule Details
---

## Purpose

A literal such as `450` or `"A3"` says what its value is, but not what it means. A reader must guess that `450` is a postage fee in cents and that `"A3"` is the code of a warehouse. When the same value occurs in many places, a change to it must find all of them, and must not change a `450` that means something else.

Replace magic literal declares a constant with a name that says the meaning, and uses the name in place of the literal. The catalogue card gives a physics formula a named constant for the acceleration of gravity, in place of the number in the formula.[^1] The first edition of the book named the refactoring Replace Magic Number with Symbolic Constant.[^2]

## Steps

1. Declare a constant with the value of the literal and a name that says what the value means.
2. Find each place where the literal occurs.
3. For each place, check that the literal there has the same meaning. If it has, replace it with the constant. Test after each change, or after each small group of changes.
4. If a literal has a different meaning in some places, give that meaning its own constant, or leave it.

## Example

A function computes the cost of a parcel. The numbers in it are a base fee, a weight limit, a fee for each extra kilogram and a rate of tax. After the refactoring, each number has a name, and the two `1000`s, which mean different things, are two constants.

```javascript run id=name-the-number
import assert from "node:assert/strict";

function parcelCostBefore(grams) {
  let cents = 450;
  if (grams > 2000) cents += Math.ceil((grams - 2000) / 1000) * 120;
  return Math.round(cents * 1.25);
}

const BASE_FEE_CENTS = 450;
const FREE_WEIGHT_GRAMS = 2000;
const GRAMS_PER_KILOGRAM = 1000;
const FEE_PER_EXTRA_KILOGRAM_CENTS = 120;
const TAX_FACTOR = 1.25; // 25 percent tax

function parcelCost(grams) {
  let cents = BASE_FEE_CENTS;
  if (grams > FREE_WEIGHT_GRAMS) {
    const extraKilograms = Math.ceil((grams - FREE_WEIGHT_GRAMS) / GRAMS_PER_KILOGRAM);
    cents += extraKilograms * FEE_PER_EXTRA_KILOGRAM_CENTS;
  }
  return Math.round(cents * TAX_FACTOR);
}

for (const g of [0, 500, 2000, 2001, 3000, 3001, 10000]) assert.equal(parcelCost(g), parcelCostBefore(g));
assert.equal(parcelCost(3001), Math.round((450 + 2 * 120) * 1.25));
console.log("replace magic literal: tests pass");
```

```output
replace magic literal: tests pass
```

The test compares the two functions for weights at and around each limit. A later change to the free weight is now one edit, and it cannot change the number of grams in a kilogram by mistake.

| Language | What the example shows |
|---|---|
| JavaScript | named constants for each number, as above |
| Python | module constants with a `Final` annotation for a type checker, and a string literal replaced by a constant |
| Go | a `const` block, and a typed constant that the compiler keeps apart from other numbers |

## When to use

- **A literal with a meaning that is not obvious.** The ESLint documentation calls magic numbers "numbers that occur multiple times in code without an explicit meaning", and says that they "should preferably be replaced by named constants".[^3]
- **A value that can change.** A fee, a limit or a code that a business rule sets is a value that can change. One constant makes the change one edit.
- **A string that the program compares.** A status name or a key in a map, written in many places, can be misspelled in one of them. A constant makes the misspelling a compile error or a lookup of a name that does not exist.

## Pitfalls

**The same value with different meanings.** Two `1000`s can mean grams in a kilogram and milliseconds in a second. One constant for both joins two things that change for different reasons. Give each meaning its own name.

**A name that only repeats the value.** A constant named `ONE_THOUSAND` or `FOUR_FIFTY` adds no meaning. The name must say what the value is for.

**Literals that are clear.** A `0` as the start of a sum, or a `1` added to an index, is clear in its context. A constant for it adds a name to look up and nothing to read. The ESLint rule has an `ignore` option for such numbers.[^3]

**Many related constants.** A group of constants that are values of one thing, such as the codes of the warehouses, can be a sign that the data wants its own type. See [[primitive-obsession|Primitive obsession]].

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The ESLint no-magic-numbers rule finds the numbers to start with: it "aims to make code more readable and refactoring easier by ensuring that special numbers are declared as constants to make their meaning explicit".[^3]

## References
