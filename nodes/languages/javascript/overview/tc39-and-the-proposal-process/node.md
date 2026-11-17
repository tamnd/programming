---
tag: 57FPE7
kind: feature
owner: javascript
slug: tc39-and-the-proposal-process
name: TC39 and the proposal process
aliases: [TC39, the TC39 process, proposal stages, Stage 3, Stage 4, test262]
area: languages/javascript/overview
level: core
summary: >-
  Ecma TC39 changes JavaScript through proposals that pass six stages, 0 to 4 with 2.7. A proposal reaches Stage 4 with test262 tests and two compatible implementations.
grammar: null
spec: {standard: "The TC39 Process", url: "https://tc39.es/process-document/", locator: "Stages; Spec revisions and scheduling"}
see_also: [javascript, javascript/javascript-and-the-ecmascript-specification, javascript/history-of-javascript, javascript/what-is-javascript, semantic-versioning]
sources:
  - url: https://tc39.es/process-document/
    title: The TC39 Process
    locator: Stages; ECMAScript Proposal Stages; Spec revisions and scheduling; Scope of responsibility for champions; Test262 tests; Tips for achieving consensus
    accessed: 2026-11-18
  - url: https://tc39.es/
    title: TC39, Specifying JavaScript
    locator: home page; Specs; State of Proposals
    accessed: 2026-11-18
  - url: https://github.com/tc39/test262/blob/main/README.md
    title: tc39/test262 README
    locator: Test262, ECMAScript Test Suite (ECMA TR/104)
    accessed: 2026-11-18
  - url: https://github.com/tc39/test262/blob/main/CONTRIBUTING.md
    title: tc39/test262 CONTRIBUTING
    locator: Test Case Style; Frontmatter; Test Environment (assert, assert.sameValue)
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/
    title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
    locator: Introduction (ECMAScript 2016)
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html#sec-map.prototype.getorinsert
    title: ECMAScript 2026 Language Specification
    locator: 24.1.3.7 Map.prototype.getOrInsert
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2025/multipage/
    title: ECMAScript 2025 Language Specification (ECMA-262, 16th edition)
    locator: Table of contents, 24.1.3 Properties of the Map Prototype Object
    accessed: 2026-11-18
  - url: https://github.com/tc39/proposals/blob/main/README.md
    title: tc39/proposals README
    locator: Active proposals
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/JavaScript_technologies_overview
    title: JavaScript technologies overview, MDN
    locator: Standardization process
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0001/
    title: "PEP 1: PEP Purpose and Guidelines"
    locator: What is a PEP?; PEP Review & Resolution
    accessed: 2026-11-18
  - url: https://openjdk.org/jeps/1
    title: "JEP 1: JDK Enhancement-Proposal & Roadmap Process"
    locator: Summary; Goals
    accessed: 2026-11-18
---

## Synopsis

"Ecma Technical Committee 39 (TC39) is responsible for evolving the ECMAScript programming language and authoring the specification."[^1] A change to the language is a proposal, and a proposal moves through these stages. "The committee must approve proposals for progression to each stage."[^1]

| Stage | Status | Entrance criteria |
|---|---|---|
| 0 | an idea, not yet considered by the committee | none |
| 1 | under consideration | a champion, a description of the problem and the shape of a solution, a public repository |
| 2 | a preferred solution, still a draft | the high-level API and syntax, examples, initial spec text |
| 2.7 | approved in principle, under validation | complete spec text, signed off by the reviewers and the editors |
| 3 | recommended for implementation | enough tests and pre-implementation experience |
| 4 | complete, ready for the standard | two compatible implementations that pass test262, a pull request to the spec, the editors' sign-off |

The table shortens the "ECMAScript Proposal Stages" table of the process document.[^1]

## Semantics

**r1.** TC39 is a committee of Ecma International. It describes itself as "a group of JavaScript developers, implementers, academics, and more". It develops the specification on GitHub and meets "every two months".[^2] To take part in the meetings as a member, an organization joins Ecma.[^2]

**r2.** "The committee operates by consensus".[^1] Consensus decides whether a proposal advances. A delegate who objects should give a constraint with a rationale, and early, so that the champion can work it into the design.[^1]

**r3.** "There are six stages: a strawperson stage and five maturity stages."[^1] Any idea that has not been submitted as a formal proposal counts as Stage 0, and it "has no acceptance criteria".[^1] From Stage 1 on, the proposal is owned by the committee.[^1]

**r4.** Each proposal has a champion, or a champion group. "The champion is responsible for the evolution of the proposal from Stage 0 through Stage 4, at which point maintenance is transferred to the editor group."[^1]

**r5.** At Stage 2 the spec text may still have placeholders, and the champions may write "loosely-correct (not for production use) polyfills" to test the design.[^1] Example: [`polyfill`](#polyfill).

**r6.** "During Stage 2.7, Test262 tests should be authored and submitted via pull request."[^1] Test262 is "the implementation conformance test suite" for ECMA-262, ECMA-402 and ECMA-404.[^3] Each test file has YAML frontmatter between `/*---` and `---*/`; `description` is required, and `esid` (the id of the spec section) is required for new tests. A test checks results with functions such as `assert.sameValue(actual, expected, message)`.[^4] Example: [`test262-style`](#test262-style).

**r7.** At Stage 3, design work is complete, and the committee calls for implementation experience. The criteria for Stage 4 include "Two compatible implementations which pass the Test262 acceptance tests", "Significant in-the-field experience with shipping implementations", a pull request to tc39/ecma262 or tc39/ecma402, and the sign-off of the editors.[^1]

**r8.** A Stage 4 proposal goes into the next edition. The process gives a yearly timeline: a candidate draft on February 1, the Stage 4 proposals merged at the March meeting, a review by Ecma from April to June, and the approval of the General Assembly in July.[^1] ECMAScript 2016 was the first edition made this way.[^5]

**r9.** For example, `Map.prototype.getOrInsert` is in the 2026 edition, as section 24.1.3.7,[^6] and is not in the 2025 edition.[^7] The active proposals at Stage 2 and higher are listed in the tc39/proposals repository,[^8] and tc39.es lists the Stage 3 ones as "Current Candidates for the Specification".[^2]

**r10.** Engines do not wait for the edition. MDN says a proposal "is typically implemented by JavaScript engines at stage 3 or stage 4".[^9] So a feature can ship before its edition, and a feature of an edition can be missing from an engine.

## Examples

A test in the style of test262 (rule r6). The frontmatter names the spec section, and the stand-in for the harness file throws when a check fails. The real harness is in the test262 repository; this small copy lets the file run alone in Node.js.

```javascript run id=test262-style
/*---
esid: sec-array.prototype.includes
description: includes finds NaN, because it compares with SameValueZero
features: [Array.prototype.includes]
---*/

// A small stand-in for the test262 harness file assert.js.
class Test262Error extends Error {}
const assert = (value, message) => {
  if (value !== true) throw new Test262Error(message);
};
assert.sameValue = (actual, expected, message) => {
  if (!Object.is(actual, expected)) {
    throw new Test262Error(`${message}: expected ${expected}, got ${actual}`);
  }
};

assert.sameValue([NaN].includes(NaN), true, "[NaN].includes(NaN)");
assert.sameValue([NaN].indexOf(NaN), -1, "[NaN].indexOf(NaN)");
assert.sameValue([-0].includes(0), true, "[-0].includes(0)");
console.log("pass");
```

```output for=test262-style
pass
```

A polyfill for `Map.prototype.getOrInsert` (rules r5 and r9). The steps of 24.1.3.7 are: if the map has the key, return its value; else append the key and the value, and return the value.[^6] The polyfill installs itself only when the engine has no such method, and it defines the property as not enumerable, like the built-in methods.

```javascript run id=polyfill
if (typeof Map.prototype.getOrInsert !== "function") {
  Object.defineProperty(Map.prototype, "getOrInsert", {
    value: function getOrInsert(key, value) {
      if (!(this instanceof Map)) throw new TypeError("not a Map");
      if (this.has(key)) return this.get(key);
      this.set(key, value);
      return value;
    },
    writable: true,
    configurable: true,
  });
}

const counts = new Map();
for (const word of "to be or not to be".split(" ")) {
  counts.set(word, counts.getOrInsert(word, 0) + 1);
}
console.log(counts);
console.log(Object.keys(Map.prototype));
```

```output for=polyfill
Map(4) { 'to' => 2, 'be' => 2, 'or' => 1, 'not' => 1 }
[]
```

The output is the same with the built-in method and with the polyfill. On Node.js 24.10.0 the polyfill runs, because that release does not have the method.

## Pitfalls

### A Stage 3 feature is not yet in the standard

A Stage 3 feature can ship in an engine (rule r10), but "some necessary changes may still occur due to web incompatibilities or feedback from production-grade implementations".[^1] Code that depends on it may need changes. Before Stage 2.7, the design "may still change significantly".[^2] [^1]

### A polyfill is not the specification

A polyfill is written in JavaScript, and it can differ from the spec in small ways. The `polyfill` above tests `instanceof Map`, but 24.1.3.7 checks for the internal slot `[[MapData]]`,[^6] so an object made with `Object.create(Map.prototype)` passes the polyfill's test and then fails inside `has`. Test262 tests are the way to find such differences (rule r6).

### Stage numbers are not version numbers

Stage 2.7 lies between 2 and 3; it is not a version.[^1] A proposal can also move back: "a proposal champion may request regressing a proposal to an earlier stage, or withdrawing it altogether", with the consensus of the committee.[^1]

## Comparison

**Python** changes through PEPs. "PEP stands for Python Enhancement Proposal."[^10] There are no stages of maturity like TC39's, and no rule of two implementations: the Steering Council has the final say. "The final authority for PEP approval is the Steering Council."[^10]

**Java** has JEPs for the JDK. JEP 1 defines "a process for collecting, reviewing, sorting, and recording the results of proposals for enhancements to the JDK". Its main goal is "a regularly-updated list of proposals to serve as the long-term Roadmap for JDK Release Projects".[^11] The Java SE specification itself goes through the Java Community Process, and a JEP is about one implementation, the JDK, while a TC39 proposal must reach two engines.

## References
