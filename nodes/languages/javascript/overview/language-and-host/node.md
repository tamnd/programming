---
tag: 6SB5CG
kind: feature
owner: javascript
slug: language-and-host
name: The language and the host
aliases: [host environment, host-defined, host hooks, ECMAScript host, embedder]
area: languages/javascript/overview
level: core
summary: >-
  ECMA-262 defines the language but no input or output. The host, a browser or Node.js, adds timers, I/O and module loading, and runs the jobs through the host hooks of Annex D.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/overview.html#sec-hosts-and-implementations", locator: "4 Overview; 4.2 Hosts and Implementations; 9.5 Jobs; Annex D"}
see_also: [javascript, javascript/javascript-engines, javascript/what-is-javascript, javascript/javascript-and-the-ecmascript-specification, event-loop-pattern]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/overview.html#sec-overview
    title: ECMAScript 2026 Language Specification
    locator: 4 Overview; 4.1 Web Scripting
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/overview.html#sec-hosts-and-implementations
    title: ECMAScript 2026 Language Specification
    locator: 4.2 Hosts and Implementations
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/host-layering-points.html#sec-host-layering-points
    title: ECMAScript 2026 Language Specification
    locator: Annex D Host Layering Points (D.1 to D.6)
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-jobs
    title: ECMAScript 2026 Language Specification
    locator: 9.5 Jobs and Host Operations to Enqueue Jobs; 9.5.5 HostEnqueuePromiseJob
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Execution_model
    title: JavaScript execution model, MDN
    locator: The engine and the host; Job queue and event loop
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/asynchronous-work/event-loop-timers-and-nexttick
    title: The Node.js Event Loop, Node.js Learn
    locator: timers; process.nextTick()
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-code-realms
    title: ECMAScript 2026 Language Specification
    locator: 9.3 Realms
    accessed: 2026-11-18
  - url: https://nodejs.org/docs/latest-v24.x/api/vm.html
    title: "VM (executing JavaScript), Node.js v24 documentation"
    locator: introduction; vm.runInNewContext()
    accessed: 2026-11-18
  - url: https://v8.dev/docs
    title: Documentation, V8
    locator: About V8
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/introduction.html
    title: The Python Language Reference, 1. Introduction
    locator: introduction
    accessed: 2026-11-18
---

## Synopsis

ECMA-262 defines a language with no input and no output: "ECMAScript as defined here is not intended to be computationally self-sufficient; indeed, there are no provisions in this specification for input of external data or output of computed results."[^1] The host fills the gap. A browser or Node.js gives the program its globals, such as `console`, `setTimeout` and `process`, loads its modules, and decides when its jobs run.

| ECMA-262 defines | The host defines |
|---|---|
| syntax, types, operators, the built-in objects | the extra properties of the global object |
| promises, and when a promise job is queued | when a queued job runs, and the event loop |
| `import` syntax and module records | how a module is found and loaded |
| realms and their intrinsic objects | when a realm is made |

The table sums up 4.2, 9.5 and Annex D of ECMA-262.[^2] [^3] [^4]

## Semantics

**r1.** ECMA-262 expects the environment to give the program "certain environment-specific objects, whose description and behaviour are beyond the scope of this specification".[^1] For a web browser, the spec lists "objects that represent windows, menus, pop-ups, dialog boxes, text areas, anchors, frames, history, cookies, and input/output". A web server gives "objects representing requests, clients, and files".[^1]

**r2.** A host is "an external source that further defines facilities listed in Annex D". A host is often an external specification, such as WHATWG HTML.[^2] "A host environment is a particular choice of definition for all host-defined facilities."[^2] It "typically includes objects or functions which allow obtaining input and providing output as host-defined properties of the global object".[^2]

**r3.** A host hook is an abstract operation defined in whole or in part outside the spec, and "All host hooks must be listed in Annex D".[^2] Annex D lists the hooks, among them HostEnqueuePromiseJob, HostEnqueueTimeoutJob, HostLoadImportedModule, HostPromiseRejectionTracker and InitializeHostDefinedRealm.[^3] It also gives the host the global object, and "Any built-in objects and methods not defined within this specification".[^3]

**r4.** The code runs in jobs. "A Job is an Abstract Closure with no parameters that initiates an ECMAScript computation when no other ECMAScript computation is currently in progress."[^4] The host schedules the jobs. "Once evaluation of a Job starts, it must run to completion before evaluation of any other Job starts in an agent."[^4]

**r5.** The spec queues the job of a promise reaction with HostEnqueuePromiseJob, and the host runs it later. The spec allows priorities: "web browsers and Node.js treat Promise-handling Jobs as a higher priority than other work".[^4] Example: [`job-order`](#job-order).

**r6.** The event loop belongs to the host. MDN describes the callback of an asynchronous action as a job "which gets placed into a job queue —or, in HTML terminology, an event loop—once the action is completed".[^5] In HTML, "Microtasks have higher priority and the microtask queue is drained first before the task queue is pulled."[^5]

**r7.** Node.js has its own queue, for `process.nextTick()`: "the nextTickQueue will be processed after the current operation is completed, regardless of the current phase of the event loop".[^6] For timers, "A timer specifies the threshold after which a provided callback may be executed rather than the exact time a person wants it to be executed."[^6]

**r8.** A realm is the world that code runs in. "Conceptually, a realm consists of a set of intrinsic objects, an ECMAScript global environment, all of the ECMAScript code that is loaded within the scope of that global environment, and other associated state and resources."[^7] The host makes realms. In Node.js, the `node:vm` module can run code "in a different V8 Context", so that the "invoked code has a different global object than the invoking code".[^8] Example: [`fresh-realm`](#fresh-realm).

**r9.** The engine is the part of the host that runs the language. MDN: "JavaScript execution requires the cooperation of two pieces of software: the JavaScript engine and the host environment."[^5] V8 "enables any C++ application to expose its own objects and functions to JavaScript code"; that is how a host adds its globals.[^9]

## Examples

The order of jobs in Node.js (rules r5 and r7). The script runs to its end first. Then Node.js runs the `nextTick` queue, then the promise jobs, and the timer comes last.

```javascript run id=job-order
setTimeout(() => console.log("timeout"), 0);
Promise.resolve().then(() => console.log("promise"));
process.nextTick(() => console.log("nextTick"));
console.log("script");
```

```output for=job-order
script
nextTick
promise
timeout
```

A realm with only the language (rule r8). `vm.runInNewContext` runs the code in a fresh context. The built-ins of ECMA-262 are there, but they are separate objects, so the `Array` of the fresh realm is not the `Array` of the script. The Node.js globals are missing.

```javascript run id=fresh-realm
const vm = require("node:vm");

const names = ["Array", "Promise", "JSON", "setTimeout", "process", "require"];
const inContext = vm.runInNewContext(
  "names.map((n) => n + ': ' + typeof globalThis[n])",
  { names },
);
console.log(inContext.join("\n"));
console.log(vm.runInNewContext("Array") === Array);

const other = vm.runInNewContext("[1, 2]");
console.log(other instanceof Array, Array.isArray(other));
```

```output for=fresh-realm
Array: function
Promise: function
JSON: object
setTimeout: undefined
process: undefined
require: undefined
false
false true
```

So a call to `setTimeout` in that realm is a `ReferenceError`.

```javascript fails id=no-timer expect=exception:ReferenceError
const vm = require("node:vm");
vm.runInNewContext("setTimeout(() => {}, 0)");
```

## Pitfalls

### The event loop is not in ECMA-262

The spec has jobs and host hooks, not an event loop (rules r4 and r6). The order of the queues comes from the host: from HTML in browsers, and from Node.js in Node.js (rules r6 and r7). Code that depends on the order of a timer and a promise depends on the host.

### A zero delay does not run at once

`setTimeout(f, 0)` gives a threshold, not a time (rule r7). In `job-order`, the timer callback runs after the promise job, though both were queued before the script ended.

### A separate context is not a sandbox

`node:vm` makes another global object, but "The node:vm module is not a security mechanism. Do not use it to run untrusted code."[^8]

### Values from another realm fail `instanceof`

Each realm has its own intrinsics (rule r8). In `fresh-realm`, the two `Array` constructors differ, so the array made in the fresh realm is not an `instanceof Array` in the script. `Array.isArray` still returns `true` for it, as the last line of `fresh-realm` shows.

## Comparison

**Python** draws the line in another place. Its language reference covers the language, and "Every Python implementation comes with a number of built-in and standard modules", documented in the library reference.[^10] So `print` and file I/O are part of every Python. ECMA-262 has no output at all, so in JavaScript the functions for output come from the host (rule r1).

## References
