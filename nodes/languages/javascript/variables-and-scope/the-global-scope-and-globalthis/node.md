---
tag: NEW-javascript-the-global-scope-and-globalthis
kind: feature
owner: javascript
slug: the-global-scope-and-globalthis
name: The global scope and globalThis
aliases: [global scope, globalThis, global object, global variables, window, global, Global Environment Record]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  The global scope is the outermost scope of a script. Its object part is the global object, which globalThis names. A script-level var or function makes a property there, and let and const do not.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/global-object.html#sec-global-object", locator: "19 The Global Object; 9.1.1.4 Global Environment Records"}
see_also: [javascript/declarations, javascript/var-declarations, javascript/lexical-scoping, javascript/environment-records, javascript/execution-contexts-and-realms, javascript/strict-mode, python/the-builtins-namespace, python/global-statement]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Global_object
    title: Global object, MDN Glossary
    locator: definition
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/globalThis
    title: globalThis, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/global-object.html#sec-global-object
    title: ECMAScript 2026 Language Specification
    locator: 19 The Global Object; 19.1.1 globalThis
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-global-environment-records
    title: ECMAScript 2026 Language Specification
    locator: 9.1.1.4 Global Environment Records, Table 20
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var
    title: var, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.2 Resolution of names; 4.2.4 Builtins and restricted execution
    accessed: 2026-11-18
---

## Synopsis

```text
globalThis.name = value;   // a global variable
globalThis.name            // read it from any scope
```

"The global object in JavaScript is an object which represents the global scope."[^1] "The globalThis property provides a standard way of accessing the global this value (and hence the global object itself) across environments."[^2]

```javascript run id=synopsis
globalThis.answer = 42;
console.log(answer);
console.log(globalThis === global, globalThis.globalThis === globalThis);
```

```output for=synopsis
42
true true
```

## Semantics

**r1. The global object.** "The global object: is created before control enters any execution context." It "may have host-defined properties in addition to the properties defined in this specification."[^3]

**r2. One per environment.** "In each JavaScript environment, there's always a global object defined." "Scripts running under Node.js have an object called global as their global object."[^1]

**r3. Two records.** The Global Environment Record holds an object record and a declarative record. The object record's "Binding object is the global object." Its declarative record "Contains bindings for all declarations in global code for the associated realm code except for FunctionDeclaration, GeneratorDeclaration, AsyncFunctionDeclaration, AsyncGeneratorDeclaration, and VariableDeclaration bindings."[^4]

**r4. What makes a property.** "var statements and function declarations at the top level of a script create properties of the global object. On the other hand, let and const declarations never create properties of the global object."[^1] Example: [`script`](#script).

**r5. Scripts and modules.** "In both Node.js CommonJS modules and native ECMAScript modules, top-level variable declarations are scoped to the module, and are not added as properties to the global object."[^5]

**r6. The global this.** globalThis is "the same value as the this value in a non-strict function called without an object." It is also the `this` value in the global scope of a script.[^2] Example: [`this`](#this).

## Examples

The examples run with Node.js 24.

Code run as a script by the `vm` module (rules r3 and r4):

```javascript run id=script
const vm = require("node:vm");
vm.runInThisContext("var sv = 1; function sf() {} let sl = 2;");
console.log(Object.hasOwn(globalThis, "sv"), Object.hasOwn(globalThis, "sf"), Object.hasOwn(globalThis, "sl"));
console.log(Object.getOwnPropertyDescriptor(globalThis, "sv").configurable);
console.log(vm.runInThisContext("sl"));
```

```output for=script
true true false
false
2
```

The this of a sloppy function called without an object (rule r6):

```javascript run id=this
function f() {
  return this;
}
console.log(f() === globalThis);
```

```output for=this
true
```

## Pitfalls

### A host can give a different value

MDN says that "hosts are allowed to provide a different value for globalThis" that is not the global object. In browsers, globalThis "references a WindowProxy around the actual global object".[^2]

### A module is not the global scope

A top-level `var` in a module is not a global property (rule r5). Write `globalThis.name = value` when a module must make a global.

### Global names leak into each script

Each script of a realm shares one global scope. "A Global Environment Record is used to represent the outer most scope that is shared by all of the ECMAScript Script elements that are processed in a common realm."[^4]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| outermost scope | the global scope, shared by each script[^4] | the module's global namespace, then the builtins namespace[^6] |
| name for it | `globalThis`[^2] | `builtins` module[^6] |
| scope of a module's top level | the module[^5] | the module's global namespace[^6] |

In Python, top-level names are resolved "by searching the global namespace, i.e. the namespace of the module containing the code block, and the builtins namespace, the namespace of the module builtins."[^6] Each module has its own global namespace, which is close to the JavaScript module scope. See [[python/the-builtins-namespace]] and [[python/global-statement]].

## References
