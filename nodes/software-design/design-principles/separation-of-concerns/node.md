---
tag: HM95SQ
kind: practice
slug: separation-of-concerns
name: Separation of concerns
area: software-design/design-principles
level: core
summary: >-
  Divide a program so that each part deals with one concern, and study or change each concern on its own.
history:
  year: 1974
  people: [Edsger W. Dijkstra]
  note: Dijkstra used the term in the essay On the role of scientific thought (EWD 447), dated 1974.
see_also: [dependency-injection]
sources:
  - url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD04xx/EWD447.html
    title: Edsger W. Dijkstra, On the role of scientific thought, EWD 447, 1974
    locator: the paragraphs that begin "Let me try to explain to you"
  - url: https://en.wikipedia.org/wiki/Separation_of_concerns
    title: Wikipedia, Separation of concerns
    locator: the lead and the section Origin
---

## Statement

A program has many concerns: what it computes, how it stores data, how it shows results, how fast it runs, whether it is correct. Separation of concerns divides the program, and the work on it, so that each concern can be handled on its own.

Edsger W. Dijkstra gave the idea its name in 1974. He wrote: "We know that a program must be correct and we can study it from that viewpoint only; we also know that it should be efficient and we can study its efficiency on another day, so to speak." He called this "the separation of concerns", "which, even if not perfectly possible, is yet the only available technique for effective ordering of one's thoughts, that I know of."[^1]

## Meaning

Dijkstra wrote about a way of thinking: study one aspect at a time. In program design, the rule has a direct form: put each concern in its own part of the code, so that the code for one concern can change without a change to the code for another.

- **One concern per module.** A function, a class or a module does one kind of work. A function that reads a file, computes a total and prints a report has three concerns.
- **Layers.** A network protocol stack is divided into layers. A designer of one layer can ignore the other layers.[^2]
- **Separate languages.** On a web page, HTML holds the content, CSS holds the presentation and JavaScript holds the behaviour.[^2]
- **Concerns that cut across.** Some concerns, such as security and logging, touch many modules. They are hard to put in one place. Aspect-oriented programming lets a programmer address such a concern as a primary unit of code.[^2]

Dijkstra also said what the rule does not mean: "it does not mean ignoring the other aspects, it is just doing justice to the fact that from this aspect's point of view, the other is irrelevant."[^1]

## Examples

A function that mixes three concerns, parsing, calculation and formatting, becomes three functions. Each one can be tested and changed alone.

```javascript run id=three-concerns
import assert from "node:assert/strict";

const input = "apples,3,0.5\npears,2,0.75";

// Before: one function with three concerns.
function reportBefore(text) {
  let total = 0;
  let out = "";
  for (const line of text.split("\n")) {
    const [name, qty, price] = line.split(",");
    const cost = Number(qty) * Number(price);
    total += cost;
    out += `${name}: ${cost.toFixed(2)}\n`;
  }
  return out + `total: ${total.toFixed(2)}`;
}

// After: one function for each concern.
const parse = (text) => text.split("\n").map((line) => {
  const [name, qty, price] = line.split(",");
  return { name, qty: Number(qty), price: Number(price) };
});
const priced = (items) => items.map((i) => ({ name: i.name, cost: i.qty * i.price }));
const format = (rows) => [...rows.map((r) => `${r.name}: ${r.cost.toFixed(2)}`),
  `total: ${rows.reduce((s, r) => s + r.cost, 0).toFixed(2)}`].join("\n");

const report = (text) => format(priced(parse(text)));
assert.equal(report(input), reportBefore(input));
// The calculation can now be tested without text in or text out.
assert.deepEqual(priced([{ name: "x", qty: 2, price: 1.5 }]), [{ name: "x", cost: 3 }]);
console.log(report(input));
```

```output
apples: 1.50
pears: 1.50
total: 3.00
```

| Language | What the example shows |
|---|---|
| JavaScript | a report function split into parse, price and format, as above |
| Python | the same split, with a dataclass for a parsed line |
| Go | the same split, with a struct for a line and an error from the parser |

## Criticism and limits

- **No split is perfect.** Dijkstra himself wrote that the separation is "not perfectly possible".[^1] A change to a data format can still touch the parser and the formatter.
- **Which concerns?** The rule does not say how to divide a program. Two designers can choose different concerns and both follow the rule.
- **Too many parts.** A program cut into very small parts can be harder to follow than one with a few larger parts.

## History

Dijkstra wrote the essay On the role of scientific thought, EWD 447, in 1974. It is the first known use of the term.[^1][^2] Later, Carlo Ghezzi, in the book Fundamentals of Software Engineering, gave separation of concerns as the primary way to deal with the complexity of software production.[^2] [[dependency-injection|Dependency injection]] is one application: an object receives its collaborators from outside, so the choice of collaborators is kept apart from the code that uses them.

## References
