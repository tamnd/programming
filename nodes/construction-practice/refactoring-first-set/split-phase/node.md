---
tag: 4WBAB8
kind: technique
slug: split-phase
name: Split phase
aliases: [split into phases]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Split code that does two different things in sequence into two functions, joined by an intermediate data structure, so that each phase can be read, tested and changed alone.
family: refactoring
polarity: positive
intent: "Separate the code that prepares the data from the code that uses it, with a data structure between them."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: The second edition of Refactoring (2018) ends its first set of refactorings with Split Phase.
see_also: [extract-function, combine-functions-into-transform, divergent-change, introduce-parameter-object, code-smell]
sources:
  - url: https://refactoring.com/catalog/splitPhase.html
    title: Martin Fowler, Catalog of Refactorings, Split Phase
    locator: the entry
  - url: https://en.wikipedia.org/wiki/Compiler
    title: Wikipedia, Compiler
    locator: the list of compiler operations, and the example of the scanner and the parser
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Split Phase, motivation and mechanics; Chapter 3, Bad Smells in Code, Divergent Change
---

## Purpose

Some code does two things one after the other. It reads an input and then computes with it, or it works out what to do and then does it. When the two parts are mixed in one function, a reader must keep both in mind at once, a change to the input format touches the computation, and neither part can be tested alone.

Split phase separates the two into sequential functions. The first phase produces an intermediate data structure; the second phase takes only that structure. The catalogue card shows a line of order code that splits a string, looks up a price and multiplies, in one expression. After the refactoring, a function `parseOrder` returns a record with `productID` and `quantity`, and a function `price` takes the record and the price list.[^1]

A compiler is a well-known case of the idea. Wikipedia says that a compiler is likely to perform "some or all of the following operations, often called phases". In its example, the scanner "composes a sequence of tokens", and the tokens are then "transformed by the parser into a syntax tree, which is then treated by the remaining compiler phases".[^2] The book uses this example to explain the refactoring.[^3] The phases are separate, so each can change alone, and the structure between them is a contract that both sides can test.

## Steps

1. Run the tests and see them pass.
2. Extract the code of the second phase into its own function.[^3]
3. Test.
4. Add an intermediate data structure as one more parameter of that function. It starts empty.
5. Test.
6. Look at each other parameter of the second-phase function. If the first phase computes it, move it into the intermediate structure, and test after each.
7. Extract the code of the first phase into a function that returns the intermediate structure.[^3]
8. Test. The caller now reads as two calls: one that builds the structure, one that uses it.

## Example

A payroll function reads lines of text and computes pay with overtime, in one loop. After the refactoring, a parse phase makes records and a pay phase computes from them.

```javascript run id=two-phases
import assert from "node:assert/strict";

const input = "ana,38,2500\nbo,45,2000\ncy,40,3000";

// Before: parsing and paying mixed in one loop. Rates are in cents per hour.
function payrollBefore(text) {
  const out = {};
  for (const line of text.split("\n")) {
    const [name, h, r] = line.split(",");
    const hours = Number(h);
    const rate = Number(r);
    const overtime = Math.max(0, hours - 40);
    out[name] = (hours - overtime) * rate + overtime * rate * 1.5;
  }
  return out;
}

// After, phase 1: text in, records out. It knows the format and nothing about pay.
function parseTimesheets(text) {
  return text.split("\n").map((line) => {
    const [name, hours, rate] = line.split(",");
    return { name, hours: Number(hours), rateCents: Number(rate) };
  });
}

// After, phase 2: records in, pay out. It knows the rules and nothing about text.
function computePay(sheets) {
  const out = {};
  for (const { name, hours, rateCents } of sheets) {
    const overtime = Math.max(0, hours - 40);
    out[name] = (hours - overtime) * rateCents + overtime * rateCents * 1.5;
  }
  return out;
}

const payroll = (text) => computePay(parseTimesheets(text));
assert.deepEqual(payroll(input), payrollBefore(input));
assert.deepEqual(payroll(input), { ana: 95000, bo: 95000, cy: 120000 });

// Each phase can now be tested alone, and the input format can change
// without touching the pay rules.
assert.deepEqual(computePay([{ name: "dee", hours: 42, rateCents: 1000 }]), { dee: 43000 });
const fromJson = JSON.parse('[{"name":"ed","hours":10,"rateCents":1000}]');
assert.deepEqual(computePay(fromJson), { ed: 10000 });
console.log("split phase: tests pass");
```

```output
split phase: tests pass
```

After the split, a second input format, JSON here, needs only a second phase-one function. The pay rules are tested with records, with no text to parse.

| Language | What the example shows |
|---|---|
| JavaScript | a parse phase and a pay phase, and a second input format, as above |
| Python | the same split, with a dataclass as the intermediate structure |
| Go | the same split, where the parse phase returns errors and the pay phase cannot fail |

## When to use

- **One function handles two concerns in sequence**, such as reading and computing, or deciding and acting.
- **Changes come from two directions.** A change to the input format and a change to the rules both touch the same function. This is [[divergent-change|Divergent Change]], and the book names Split Phase as a fix for it.[^3]
- **A part cannot be tested alone.** If you must build text to test a rule, the rule is mixed with the parsing.
- **After a transform.** [[combine-functions-into-transform|Combine Functions into Transform]] often makes the first phase: the enriched record is the intermediate structure.

## Pitfalls

**A leaky structure.** If the second phase still reads the raw input, or the first phase computes results of the second, the split is only in name. Each parameter of the second phase should come from the intermediate structure or from data that the first phase never touches.

**Errors in the wrong phase.** Validation of the input belongs in the first phase. If the second phase finds a malformed value, the first phase let it through.

**Too many phases.** A split for a function of ten lines that never changes adds a type and a function and gains little. The refactoring pays when the phases change for different reasons.

**Cost of the intermediate structure.** The first phase builds the whole structure before the second phase starts. For a large input, this can use much more memory than the mixed loop. A stream or an iterator between the phases keeps the separation without holding all the data.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. Each [[extract-function|Extract Function]] step can use the extract action of the IDE, and [[introduce-parameter-object|Introduce Parameter Object]] can make the intermediate structure. The choice of where one phase ends and the other starts is the programmer's.

## References
