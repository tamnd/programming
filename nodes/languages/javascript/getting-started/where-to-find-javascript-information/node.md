---
tag: Q2G37R
kind: feature
owner: javascript
slug: where-to-find-javascript-information
name: Where to find JavaScript information
aliases: [JavaScript documentation, MDN JavaScript, JavaScript reference, Baseline, browser-compat-data]
area: languages/javascript/getting-started
level: intro
summary: >-
  The main references for JavaScript: the MDN guide and reference, the ECMA-262 specification, the Node.js API documentation, and the Baseline and browser-compat-data records of browser support.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/overview.html", locator: "4 Overview"}
see_also: [javascript, javascript/javascript-and-the-ecmascript-specification, javascript/notational-conventions-of-ecma-262, javascript/tc39-and-the-proposal-process, javascript/language-and-host, javascript/javascript-language-overview, javascript/running-javascript-with-node-js]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: What you should already know; Where to find JavaScript information; JavaScript documentation versus the ECMAScript specification
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide
    title: JavaScript Guide, MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference
    title: JavaScript reference, MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/API/console
    title: console, MDN
    locator: Baseline badge
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/eval
    title: eval(), MDN
    locator: page sections; Specifications
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/overview.html
    title: ECMAScript 2026 Language Specification
    locator: 4 Overview; 4.2 Hosts and Implementations
    accessed: 2026-11-18
  - url: https://nodejs.org/api/documentation.html
    title: About this documentation, Node.js documentation
    locator: Stability index; JSON output; Other versions
    accessed: 2026-11-18
  - url: https://nodejs.org/api/console.html
    title: Console, Node.js documentation
    locator: console.log()
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Baseline/Compatibility
    title: Baseline (compatibility), MDN Glossary
    locator: introduction; Baseline badges
    accessed: 2026-11-18
  - url: https://web.dev/baseline
    title: Baseline, web.dev
    locator: Who defines Baseline?; How do things become Baseline?; The core browser set
    accessed: 2026-11-18
  - url: https://raw.githubusercontent.com/mdn/browser-compat-data/main/README.md
    title: "@mdn/browser-compat-data, README"
    locator: introduction; Installation and Import
    accessed: 2026-11-18
---

## Synopsis

| Question | Where to look |
|---|---|
| How do I start? | MDN's beginner course, Dynamic scripting with JavaScript |
| How does the language work? | the MDN JavaScript Guide |
| What does this function or statement do? | the MDN JavaScript Reference |
| What exactly must an engine do? | ECMA-262, the ECMAScript specification |
| What does this Node.js module offer? | the Node.js API documentation |
| Which browsers support this feature? | the Baseline badge on MDN, and browser-compat-data |
| What is coming to the language? | the TC39 proposals ([[javascript/tc39-and-the-proposal-process]]) |

The MDN guide names its own three parts: "Dynamic scripting with JavaScript provides structured JavaScript guides for beginners", the guide "provides an overview about the JavaScript language and its objects", and the reference "provides detailed reference material for JavaScript".[^1]

## Semantics

**r1. What the guide expects.** The MDN guide assumes "Some programming experience", "Good working knowledge of HyperText Markup Language" and a general understanding of the web. A reader without that experience is sent to the tutorials first.[^1]

**r2. Guide or reference.** "The JavaScript Guide shows you how to use JavaScript and gives an overview of the language." For "exhaustive information about a language feature", it points to the reference.[^2] The reference "serves as a repository of facts about the JavaScript language", and it advises: "If you are new to JavaScript, start with the guide."[^3]

**r3. Language or host.** The reference "attempts to be environment-agnostic and does not target a web browser environment".[^3] What a browser adds, such as `document` and `console`, has its pages under MDN's Web APIs instead: the `console` page sits at `/docs/Web/API/console`.[^4] What Node.js adds is in the Node.js documentation (r6). The line between the two is the subject of [[javascript/language-and-host]].

**r4. The shape of a reference page.** A page of the MDN reference, such as the one for `eval`, has the sections Syntax, Description, Examples, Specifications and Browser compatibility. The Specifications table links to the clause of ECMA-262 that defines the feature.[^5] A page about a web feature opens with a Baseline badge, such as "Baseline Widely available" for `console`.[^4]

**r5. The specification.** MDN describes ECMA-262 as "a set of requirements for implementing ECMAScript". It says: "The ECMAScript document is not intended to help script programmers", and "Although the description of the language may differ in ECMAScript, the language itself remains the same."[^1] Clause 4 of the specification is "a non-normative overview of the ECMAScript language".[^6] The notation of the rest is explained in [[javascript/notational-conventions-of-ecma-262]], and the relation of the name JavaScript to the standard in [[javascript/javascript-and-the-ecmascript-specification]].

**r6. The Node.js documentation.** Each section of the Node.js API documentation carries a stability index: 0 Deprecated, 1 Experimental, 2 Stable or 3 Legacy. Of Experimental features it says: "Use of the feature is not recommended in production environments."[^7] Each function records the release that added it, such as `console.log`, "Added in: v0.1.100".[^8] A menu lists the documentation of other release lines, and "Every .html document has a corresponding .json document."[^7]

**r7. Baseline.** "Baseline identifies the availability of web platform features across popular browsers, including APIs, CSS properties, and JavaScript syntax." It considers seven browsers: Safari on iOS and macOS, Chrome on Android and desktop, Edge on desktop, and Firefox on Android and desktop.[^9] A feature has one of three statuses:[^9]

- "widely available": "a consistent history of support in each of the Baseline browsers for at least 2.5 years";
- "newly available": it works "in at least the latest stable version of each of the Baseline browsers";
- "limited availability": "not yet available in all browsers".

"Baseline was originated by the Chrome team and is now defined by the WebDX Community Group".[^10]

**r8. browser-compat-data.** The data under the badges and the compatibility tables is a project of its own. It "contains machine-readable browser (and JavaScript runtime) compatibility data for Web technologies", and MDN, CanIUse and Visual Studio Code use it. It installs as the npm package `@mdn/browser-compat-data`.[^11]

**r9. Asking the engine.** The tables describe browsers in general. The engine that runs a program can be asked directly, by testing whether a name exists before using it. Example: [`detect`](#detect).

## Examples

Which of five features the engine provides (rule r9). The output is that of the Node.js 24 that runs the examples of this guide; another engine or release line can answer differently.

```javascript run id=detect
const checks = {
  "Array.prototype.findLast": typeof Array.prototype.findLast === "function",
  "Object.groupBy": typeof Object.groupBy === "function",
  "Promise.withResolvers": typeof Promise.withResolvers === "function",
  "Iterator helpers": typeof Iterator === "function" && typeof Iterator.prototype.map === "function",
  "Temporal": typeof Temporal === "object",
};
for (const [feature, present] of Object.entries(checks)) {
  console.log(feature.padEnd(25), present ? "yes" : "no");
}
```

```output for=detect
Array.prototype.findLast  yes
Object.groupBy            yes
Promise.withResolvers     yes
Iterator helpers          yes
Temporal                  no
```

A test like this lets a program fall back to its own code when a feature is missing. Here `groupBy` uses `Object.groupBy` when it exists, and a loop otherwise; both give the same groups:

```javascript run id=fallback
function groupBy(items, keyOf) {
  if (typeof Object.groupBy === "function") {
    return Object.groupBy(items, keyOf);
  }
  const groups = Object.create(null);
  for (const item of items) {
    (groups[keyOf(item)] ??= []).push(item);
  }
  return groups;
}

const parity = (n) => (n % 2 === 0 ? "even" : "odd");
console.log(JSON.stringify(groupBy([1, 2, 3, 4, 5], parity)));
```

```output for=fallback
{"odd":[1,3,5],"even":[2,4]}
```

## Pitfalls

### Learning from the specification

ECMA-262 states what an engine must do, in its own notation. MDN's advice is plain: "Use the JavaScript documentation for information when writing scripts."[^1]

### Looking for browser features in the language reference

`document`, `fetch` and `console` are not part of the language, so the JavaScript reference does not list them (rule r3). Search MDN's Web APIs, or the Node.js documentation, instead.[^3]

### Reading Baseline as a guarantee

MDN: "Baseline is a summary of browser support. It is not a substitute for accessibility, usability, performance, security, or other testing." It may not cover older devices, web views, or screen readers.[^9]

### Experimental Node.js features

A feature at stability 1 may change or go away in any release; the documentation says that "Non-backward compatible changes or removal may occur in any future release".[^7] Check the index before depending on one.

### The wrong release line

Each function records the release that added it (rule r6), so a function documented for one release line can be missing from an older one. Pick the line that runs the program from the menu of other versions.[^8] [^7]

## Comparison

| Source | Kind | What it gives |
|---|---|---|
| MDN JavaScript Guide | tutorial | how to use the language, chapter by chapter[^2] |
| MDN JavaScript Reference | description | the facts of each feature, with examples[^3] |
| ECMA-262 | normative standard | the requirements for an implementation[^1] |
| Node.js API documentation | description | each module of Node.js, with stability and history[^7] |
| Baseline | summary | one status per feature over a core set of browsers[^9] |
| browser-compat-data | data | support per browser and runtime, machine-readable[^11] |

The first three cover the language, the Node.js documentation covers one host, and the last two cover support across browsers.

## References
