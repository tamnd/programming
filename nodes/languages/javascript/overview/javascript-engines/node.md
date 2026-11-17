---
tag: YZEE38
kind: feature
owner: javascript
slug: javascript-engines
name: JavaScript engines
aliases: [JavaScript engine, JS engine, V8, SpiderMonkey, JavaScriptCore, JavaScript runtimes]
area: languages/javascript/overview
level: core
summary: >-
  An engine parses and runs JavaScript: V8, SpiderMonkey and JavaScriptCore in the main browsers, GraalJS and QuickJS elsewhere. Node.js, Deno and Bun are runtimes built on an engine.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/conformance.html#sec-conformance", locator: "2 Conformance; 4.2 Hosts and Implementations"}
see_also: [javascript, javascript/what-is-javascript, javascript/language-and-host, javascript/tc39-and-the-proposal-process, just-in-time-compilation, bytecode, garbage-collection, reference-counting]
sources:
  - url: https://v8.dev/docs
    title: Documentation, V8
    locator: introduction; About V8
    accessed: 2026-11-18
  - url: https://firefox-source-docs.mozilla.org/js/index.html
    title: SpiderMonkey, Firefox Source Docs
    locator: the whole page
    accessed: 2026-11-18
  - url: https://docs.webkit.org/Deep%20Dive/JSC/JavaScriptCore.html
    title: JavaScriptCore, WebKit Documentation
    locator: Overview; LLInt; Baseline; DFG; FTL
    accessed: 2026-11-18
  - url: https://raw.githubusercontent.com/oracle/graaljs/master/README.md
    title: oracle/graaljs README
    locator: introduction; goals
    accessed: 2026-11-18
  - url: https://bellard.org/quickjs/
    title: QuickJS Javascript Engine
    locator: Introduction; Main Features
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/conformance.html#sec-conformance
    title: ECMAScript 2026 Language Specification
    locator: 2 Conformance
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/overview.html#sec-hosts-and-implementations
    title: ECMAScript 2026 Language Specification
    locator: 4.2 Hosts and Implementations
    accessed: 2026-11-18
  - url: https://v8.dev/docs/ignition
    title: Ignition, V8
    locator: the whole page
    accessed: 2026-11-18
  - url: https://v8.dev/blog/sparkplug
    title: "Sparkplug: a non-optimizing JavaScript compiler, V8 blog"
    locator: introduction
    accessed: 2026-11-18
  - url: https://v8.dev/blog/maglev
    title: "Maglev: V8's fastest optimizing JIT, V8 blog"
    locator: introduction
    accessed: 2026-11-18
  - url: https://v8.dev/docs/turbofan
    title: TurboFan, V8
    locator: the whole page
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/getting-started/the-v8-javascript-engine
    title: The V8 JavaScript Engine, Node.js Learn
    locator: the whole page
    accessed: 2026-11-18
  - url: https://raw.githubusercontent.com/denoland/deno/main/README.md
    title: denoland/deno README
    locator: introduction
    accessed: 2026-11-18
  - url: https://raw.githubusercontent.com/oven-sh/bun/main/README.md
    title: oven-sh/bun README
    locator: What is Bun?
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error/stack
    title: Error.prototype.stack, MDN
    locator: introduction; Value; Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/introduction.html
    title: The Python Language Reference, 1. Introduction
    locator: 1.1 Alternate Implementations
    accessed: 2026-11-18
---

## Synopsis

A JavaScript engine is the program that parses and runs JavaScript code. V8 "compiles and executes JavaScript source code, handles memory allocation for objects, and garbage collects objects it no longer needs".[^1] The engine does not give the page its document: "The DOM is not, however, typically provided by the JavaScript engine but instead by a browser."[^1] A runtime such as Node.js puts an engine together with a host, which adds files, timers and the network.

| Engine | Used by | From the source |
|---|---|---|
| V8 | Chrome, Node.js, Deno | Google's engine, "written in C++"[^1] |
| SpiderMonkey | Firefox | "the JavaScript and WebAssembly implementation library" of Firefox[^2] |
| JavaScriptCore | WebKit, Safari, Bun | "the built-in JavaScript engine for WebKit"[^3] |
| GraalJS | Java applications | "a JavaScript engine implemented in Java on top of GraalVM"[^4] |
| QuickJS | embedded use | "a small and embeddable Javascript engine"[^5] |

## Semantics

**r1.** ECMA-262 says what a conforming implementation must do: "A conforming implementation of ECMAScript must provide and support all the types, values, objects, properties, functions, and program syntax and semantics described in this specification."[^6] An engine is the part of the implementation that does this.

**r2.** An implementation may add to the language. It "may provide additional types, values, objects, properties, and functions beyond those described in this specification".[^6] It must not redefine the other facilities, and it must not implement a Forbidden Extension.[^6] So the engines share one core language and differ in their extras. Example: [`stack`](#stack).

**r3.** Some facilities are left to the implementation. An implementation-defined facility has no recommended behaviour, and an implementation-approximated one has an ideal that the implementation should come close to. "Some mathematical operations, such as Math.exp, are implementation-approximated."[^7] Example: [`approx`](#approx).

**r4.** V8 is "Google's open source high-performance JavaScript and WebAssembly engine, written in C++", used "in Chrome and in Node.js, among others".[^1] It "can be embedded into any C++ application", and the application can expose its own objects and functions to JavaScript.[^1]

**r5.** V8 runs code in tiers ([[just-in-time-compilation]]). Ignition is "a fast low-level register-based interpreter" that runs [[bytecode]].[^8] Sparkplug is a non-optimizing compiler that "nestles between the Ignition interpreter and the TurboFan optimising compiler".[^9] Maglev "sits between our existing Sparkplug and TurboFan compilers", as a fast optimizing compiler.[^10] TurboFan "is one of V8's optimizing compilers".[^11]

**r6.** JavaScriptCore has the same kind of tiers. It "consists of the following building blocks: lexer, parser, start-up interpreter (LLInt), baseline JIT, a low-latency optimizing JIT (DFG), and a high-throughput optimizing JIT (FTL)".[^3] A function moves to a higher tier when it runs often: the Baseline JIT "kicks in for functions that are invoked at least 6 times, or take a loop at least 100 times", and the WebKit page calls these numbers approximate.[^3] In Safari, the names Nitro and Nitro Extreme are also used, "However, the name of the project and the library is always JavaScriptCore."[^3]

**r7.** SpiderMonkey is the engine of Firefox. "The implementation behaviour is defined by the ECMAScript and WebAssembly specifications."[^2] Node.js documents why its engine could be reused: "the JavaScript engine is independent of the browser in which it's hosted", and "V8 was chosen to be the engine that powered Node.js back in 2009".[^12]

**r8.** A runtime is an engine plus a host. Deno "is a JavaScript, TypeScript, and WebAssembly runtime" and it is built on V8, Rust and Tokio.[^13] Bun is "a fast JavaScript runtime designed as a drop-in replacement for Node.js", and it is "powered by JavaScriptCore under the hood".[^14] A program can test which runtime runs it by looking at the globals that each host adds. Example: [`which-runtime`](#which-runtime).

**r9.** Engines outside the browsers have other aims. GraalJS is "an ECMAScript-compliant runtime to execute JavaScript and Node.js applications", with interoperability with Java.[^4] QuickJS "supports the ES2025 specification", needs "just a few C files, no external dependency", and does "Garbage collection using reference counting (to reduce memory usage and have deterministic behavior) with cycle removal" ([[reference-counting]]).[^5] Each project states what it supports in its own terms, so a reader compares the engines by the features they pass, not by one edition number.

**r10.** The compat tables of the JavaScript feature pages on this site have one column for the ECMAScript edition and one column each for V8 (Node.js), SpiderMonkey, JavaScriptCore, Bun and Deno. A feature in an edition can be missing from one of these engines, and an engine can ship a feature before its edition (see [[javascript/tc39-and-the-proposal-process]]).

## Examples

Which runtime runs this code (rule r8). Bun aims to be a drop-in replacement for Node.js, so the function checks for Bun and Deno before it checks for Node.js.

```javascript run id=which-runtime
function runtime() {
  if (typeof globalThis.Bun !== "undefined") return "Bun, on JavaScriptCore";
  if (typeof globalThis.Deno !== "undefined") return "Deno, on V8";
  if (typeof process !== "undefined" && process.release?.name === "node") {
    return "Node.js, on V8";
  }
  return "unknown";
}
console.log(runtime());
console.log(typeof process.versions.v8);
```

```output for=which-runtime
Node.js, on V8
string
```

An extension of the engine (rule r2). ECMA-262 does not define the `stack` property of errors. MDN marks it as non-standard, and lists `Error.captureStackTrace()` and `Error.stackTraceLimit` as part of the "non-standard stack trace API" of V8.[^15] In V8 the stack is a data property of each error object.

```javascript run id=stack
function inner() {
  return new Error("boom");
}
const e = inner();
console.log(e.stack.split("\n")[0]);
console.log(e.stack.split("\n")[1].trim().startsWith("at inner"));
console.log(typeof Error.captureStackTrace, Error.stackTraceLimit);
console.log(Object.getOwnPropertyNames(e));
```

```output for=stack
Error: boom
true
function 10
[ 'stack', 'message' ]
```

An implementation-approximated result (rule r3). The output is from V8 in Node.js 24.10.0. The spec does not fix the last bits of `Math.exp` or `Math.sin`, so a test compares with a tolerance, as the last line does.

```javascript run id=approx
console.log(Math.exp(1));
console.log(Math.exp(1) === Math.E);
console.log(Math.sin(Math.PI));
console.log(Math.abs(Math.sin(Math.PI)) < Number.EPSILON);
```

```output for=approx
2.718281828459045
true
1.2246467991473532e-16
true
```

## Pitfalls

### The stack string differs between engines

"You cannot rely on the precise content of the stack string due to implementation inconsistencies, but you can generally assume it exists and use it for debugging purposes."[^15] The place of the property differs too: "In Firefox, it's an accessor property on Error.prototype", while Chrome and Safari put a data property on each error.[^15] Code that parses `stack` works on one engine only.

### "Interpreted" describes an engine, not the language

Node.js writes that "modern JavaScript engines no longer just interpret JavaScript, they compile it".[^12] V8 and JavaScriptCore both start in an interpreter and compile the code that runs often (rules r5 and r6). Whether a function is compiled, and when, is the choice of the engine ([[compilers-and-interpreters]]).

### The engine is not the whole runtime

`setTimeout`, `process` and `fetch` come from the host, not from the engine. V8 "does however provide all the data types, operators, objects and functions specified in the ECMA standard", and the embedder adds the rest.[^1] Code that uses the globals of one runtime does not run unchanged in another. The split is the subject of [[javascript/language-and-host]].

## Comparison

**Python** has one main implementation. The Python reference calls CPython "the original and most-maintained implementation of Python, written in C", and says that "New language features generally appear here first."[^16] JavaScript has three main engines from three browser vendors, and a TC39 proposal needs two compatible implementations before it enters the standard ([[javascript/tc39-and-the-proposal-process]]).

## References
