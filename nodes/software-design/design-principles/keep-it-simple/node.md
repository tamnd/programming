---
tag: V647QD
kind: practice
slug: keep-it-simple
name: Keep it simple
aliases: [KISS, Keep it simple, stupid]
area: software-design/design-principles
level: core
summary: >-
  Make simplicity a design goal: choose the simplest design that does the job, and add complexity only when the job needs it.
history:
  year: 1960
  people: [Kelly Johnson]
  note: The KISS acronym was noted by the U.S. Navy in 1960 and is associated with the aircraft engineer Kelly Johnson of the Lockheed Skunk Works.
see_also: [you-arent-gonna-need-it, dont-repeat-yourself, separation-of-concerns]
sources:
  - url: https://en.wikipedia.org/wiki/KISS_principle
    title: Wikipedia, KISS principle
    locator: the lead and the sections Origin and Variants
  - url: https://en.wikiquote.org/wiki/C._A._R._Hoare
    title: Wikiquote, C. A. R. Hoare
    locator: the section The Emperor's Old Clothes, 1980 ACM Turing Award Lecture, p. 81
  - url: https://en.wikipedia.org/wiki/Unix_philosophy
    title: Wikipedia, Unix philosophy
    locator: the sections on Doug McIlroy, Eric Raymond's 17 Unix Rules and Worse is better
---

## Statement

KISS, "Keep it simple, stupid", says that simplicity should be a design goal.[^1] In software, it means: of the designs that do the job, choose the simplest one.

C. A. R. Hoare gave the reason in his 1980 Turing Award lecture: "There are two ways of constructing a software design: One way is to make it so simple that there are obviously no deficiencies and the other way is to make it so complicated that there are no obvious deficiencies. The first method is far more difficult."[^2]

## Meaning

The rule does not mean a poor design or a design that does less than the job needs. It means that each part of the design must earn its place.

- **Prefer the plain construct.** Use a loop, a function or a table before a framework, a plugin system or a new class hierarchy.
- **Write small programs that do one thing.** Doug McIlroy summarized the Unix philosophy as: "Write programs that do one thing and do it well. Write programs to work together."[^3]
- **Remove what is not needed.** McIlroy described how the early Unix team worked: "We used to sit around in the Unix Room saying, 'What can we throw out? Why is there this option?'"[^3]
- **Put complexity in data, not in code.** One of the design rules of Eric S. Raymond is "Make data complicated when required, not the program".[^3]

The word stupid in the acronym is not about the reader. In the story of the origin, Kelly Johnson gave his engineers a few tools and said that the jet they designed must be repairable by an average mechanic in the field with only those tools. The word "refers to the relationship between the way things break and the sophistication available to repair them."[^1]

## Examples

A program reads settings in the form key=value. The first design has a registry of parser objects for each line type. The second design is a short function. Both give the same result, and the second has fewer parts to read, test and break.

```javascript run id=simple-config
import assert from "node:assert/strict";

const text = "host=example.org\nport=8080\n# a comment\n";

// A complex design: a registry of line handlers.
class LineHandlerRegistry {
  constructor() { this.handlers = []; }
  register(test, handle) { this.handlers.push({ test, handle }); }
  parse(src) {
    const out = {};
    for (const line of src.split("\n")) {
      const h = this.handlers.find((x) => x.test(line));
      if (h) h.handle(line, out);
    }
    return out;
  }
}
const registry = new LineHandlerRegistry();
registry.register((l) => l.startsWith("#") || l === "", () => {});
registry.register((l) => l.includes("="), (l, out) => {
  const [k, v] = l.split("=");
  out[k] = v;
});

// A simple design: one function.
function parseConfig(src) {
  const out = {};
  for (const line of src.split("\n")) {
    if (line === "" || line.startsWith("#")) continue;
    const [k, v] = line.split("=");
    out[k] = v;
  }
  return out;
}

assert.deepEqual(parseConfig(text), registry.parse(text));
console.log(parseConfig(text));
```

```output
{ host: 'example.org', port: '8080' }
```

If the file format later gets a second kind of line, the function can grow. The registry is worth its cost only when there are many kinds of lines.

| Language | What the example shows |
|---|---|
| JavaScript | the registry and the simple function, as above |
| Python | the simple function, with a dict |
| Go | the simple function, with a map and strings.Cut |

## Criticism and limits

- **Simple for whom.** A design that is simple to use can be hard to build, and the reverse. Richard P. Gabriel described the Unix style as "worse is better": "simplicity of both the interface and the implementation are more important than any other attributes of the system", including correctness, consistency and completeness.[^3] Other designers put correctness first and accept a more complex implementation.
- **Not simpler than the problem.** A design that is too simple moves the complexity to the users or to the callers. A well-known paraphrase, often attributed to Albert Einstein, says: "Make everything as simple as possible, but not simpler".[^1]
- **The rule gives no measure.** KISS does not say how to measure simplicity. Other rules, such as [[you-arent-gonna-need-it|You aren't gonna need it]] and [[dont-repeat-yourself|Don't repeat yourself]], give more specific tests.

## History

The variant "Keep it Short and Simple" appeared in 1938. The U.S. Navy noted the KISS principle in 1960, and the phrase is associated with Kelly Johnson, lead engineer at the Lockheed Skunk Works. The term "KISS principle" was in popular use by 1970.[^1] In software, Hoare argued for simple designs in his 1980 Turing Award lecture.[^2] In The Art of Unix Programming, first published in 2003, Eric S. Raymond summarizes the Unix philosophy as "Keep it Simple, Stupid."[^3]

## References
