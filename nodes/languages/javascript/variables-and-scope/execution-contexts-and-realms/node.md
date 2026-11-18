---
tag: NEW-javascript-execution-contexts-and-realms
kind: feature
owner: javascript
slug: execution-contexts-and-realms
name: Execution contexts and realms
aliases: [execution context, running execution context, execution context stack, realm, Realm Record, intrinsics, LexicalEnvironment, VariableEnvironment]
area: languages/javascript/variables-and-scope
level: research
summary: >-
  An execution context tracks the run of one piece of code, with its realm and its environments. The contexts form a stack. A realm holds a global object, a global environment and the intrinsics.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-execution-contexts", locator: "9.3 Realms; 9.4 Execution Contexts"}
see_also: [javascript/environment-records, javascript/the-global-scope-and-globalthis, javascript/var-declarations, javascript/let-and-const, javascript/specification-types, javascript/function-scopes-and-closures, python/python-scopes-and-namespaces]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-execution-contexts
    title: ECMAScript 2026 Language Specification
    locator: 9.3 Realms, Table 19; 9.4 Execution Contexts, Tables 20 and 21
    accessed: 2026-11-18
  - url: https://nodejs.org/api/vm.html
    title: VM (executing JavaScript), Node.js documentation
    locator: introduction
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-block
    title: ECMAScript 2026 Language Specification
    locator: 14.2.2 Runtime Semantics, Evaluation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.1 Structure of a program; 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

| Part of an execution context | Holds |
|---|---|
| code evaluation state | the state to run, suspend and resume the code |
| Function | the function object, or null for a script or module |
| Realm | the Realm Record of the code |
| ScriptOrModule | the Script Record or Module Record of the code |
| LexicalEnvironment | the record that resolves names |
| VariableEnvironment | the record for `var` bindings |
| PrivateEnvironment | the record for private names of the nearest class |

"An execution context is a specification device that is used to track the runtime evaluation of code by an ECMAScript implementation."[^1]

```javascript run id=synopsis
function inner() {
  console.log("enter inner");
  console.log("leave inner");
}
function outer() {
  console.log("enter outer");
  inner();
  console.log("leave outer");
}
outer();
```

```output for=synopsis
enter outer
enter inner
leave inner
leave outer
```

## Semantics

**r1. The running context.** "At any point in time, there is at most one execution context per agent that is actually executing code. This is known as the agent's running execution context."[^1]

**r2. The stack.** "The execution context stack is used to track execution contexts. The running execution context is always the top element of this stack." "The newly created execution context is pushed onto the stack and becomes the running execution context."[^1] Example: [`synopsis`](#synopsis).

**r3. Not always LIFO.** "Transition of the running execution context status among execution contexts usually occurs in stack-like last-in/first-out manner. However, some ECMAScript features require non-LIFO transitions of the running execution context."[^1] A generator is one. Example: [`generator`](#generator).

**r4. Two environments.** The LexicalEnvironment "Identifies the Environment Record used to resolve identifier references made by code within this execution context." The VariableEnvironment "Identifies the Environment Record that holds bindings created by VariableStatements within this execution context."[^1] See [[javascript/environment-records]].

**r5. Realms.** "Before it is evaluated, all ECMAScript code must be associated with a realm. Conceptually, a realm consists of a set of intrinsic objects, an ECMAScript global environment, all of the ECMAScript code that is loaded within the scope of that global environment, and other associated state and resources."[^1]

**r6. The Realm Record.** Its fields include `[[Intrinsics]]`, "The intrinsic values used by code associated with this realm", `[[GlobalObject]]`, "The global object for this realm", and `[[GlobalEnv]]`, "The global environment for this realm".[^1]

## Examples

The examples run with Node.js 24.

A generator suspends its context and resumes it later (rule r3):

```javascript run id=generator
function* steps() {
  console.log("start");
  yield 1;
  console.log("resume");
  yield 2;
}
const it = steps();
console.log(it.next().value);
console.log("between");
console.log(it.next().value);
```

```output for=generator
start
1
between
resume
2
```

Two realms have two sets of intrinsics (rules r5 and r6). The Node.js `vm` module can "run the code in a different V8 Context. This means invoked code has a different global object than the invoking code."[^2]

```javascript run id=realm
const vm = require("node:vm");
const other = vm.runInNewContext("[1, 2]");
console.log(other instanceof Array, Array.isArray(other));
console.log(vm.runInNewContext("Array") === Array);
```

```output for=realm
false true
false
```

## Pitfalls

### instanceof across realms

An array from another realm has that realm's `Array.prototype`, so `instanceof Array` is false (example [`realm`](#realm)). `Array.isArray` still gives true.

### A block changes the environment, not the context

A block does not push an execution context. It swaps the LexicalEnvironment of the running one: "Set the running execution context's LexicalEnvironment to blockEnv." After the block, the steps set it back "to oldEnv".[^3]

### A realm is not a security boundary

"The node:vm module is not a security mechanism. Do not use it to run untrusted code."[^2]

## Comparison

| Topic | JavaScript spec | Python |
|---|---|---|
| unit that runs code | execution context[^1] | execution frame[^4] |
| holds | realm, environments, function[^1] | "some administrative information (used for debugging)"[^4] |
| separate globals | a realm[^1] | each module has a global namespace[^4] |

In Python, "A code block is executed in an execution frame. A frame contains some administrative information (used for debugging) and determines where and how execution continues after the code block’s execution has completed."[^4] See [[python/python-scopes-and-namespaces]].

## References
