---
tag: Q1310M
kind: pattern
slug: module-pattern
name: Module pattern
aliases: [JavaScript module pattern, IIFE module, module object]
area: software-design/structural-patterns
level: core
summary: >-
  The module pattern groups private state and a public API in one closure or object, in a language without full support for modules. It was the usual form of a JavaScript module before ES modules.
family: structural-design
polarity: positive
intent: "Give a language with incomplete support for modules the features of a module: one named unit with a public API, and private state and helpers that code outside the unit cannot reach."
also_known_as: [Module]
history:
  year: 2015
  people: []
  note: In JavaScript, the pattern used a function that runs at once (an IIFE) to make a private scope. ECMAScript 2015, the 6th edition of ECMA-262 (June 2015), added modules to the language, and each module runs in its own scope.
see_also: [closures, encapsulation-and-visibility, singleton-pattern, facade-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Module_pattern
    title: Module pattern, Wikipedia
    locator: Introduction; Features; Comparisons to other concepts
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Modules
    title: JavaScript modules, MDN Web Docs
    locator: A background on modules; Other differences between modules and classic scripts; Exporting module features
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
    title: Closures, MDN Web Docs
    locator: Emulating private methods with closures
  - url: https://docs.python.org/3.14/tutorial/modules.html
    title: The Python Tutorial, Modules, Python 3.14
    locator: 6.1 More on Modules
  - url: https://go.dev/ref/spec#Exported_identifiers
    title: The Go Programming Language Specification
    locator: Exported identifiers
  - url: https://doc.rust-lang.org/book/ch07-02-defining-modules-to-control-scope-and-privacy.html
    title: The Rust Programming Language, Control Scope and Privacy with Modules
    locator: Modules Cheat Sheet; introduction
  - url: https://262.ecma-international.org/6.0/
    title: ECMAScript 2015 Language Specification, ECMA-262 6th Edition
    locator: title page (6th Edition, June 2015); 15.2 Modules
  - url: https://nodejs.org/docs/latest-v24.x/api/modules.html#the-module-wrapper
    title: Modules, CommonJS modules, Node.js v24 documentation
    locator: The module wrapper
  - url: https://github.com/nodejs/node/blob/v24.0.0/lib/internal/modules/cjs/loader.js#L341-L352
    title: Node.js v24.0.0 source, lib/internal/modules/cjs/loader.js
    locator: wrap and wrapper, lines 341 to 352
---

## Intent

Give a language with incomplete support for modules the features of a module.[^1] The unit has one name and a public API. Its state and its helpers are private, and code outside the unit cannot reach them.

## Problem

Old browser JavaScript ran every classic script in one global scope. A variable that one script declared at the top level was visible to every later script on the page.[^2] Two libraries that used the same name changed each other's state, and nothing in the language stopped a caller from changing the internals of a library.

The forces are these:

- A group of functions shares state, and that state must be private.
- The group needs one public name, so that it does not fill the global scope.
- The language has functions or classes, but no module construct with private members.

Here two scripts share the global scope, and the second one changes the state of the first:

```javascript wrong id=shared-global
// script 1: a counter
globalThis.count = 0;
function increment() {
  count += 1;
}

// script 2: a list widget that also uses the name count
globalThis.count = 10; // the number of items to show

increment();
increment();
console.log("counter value:", count);
```

```output for=shared-global
counter value: 12
```

## Solution

Put the state and the helpers inside a function, call the function at once, and return an object that holds only the public functions. The public functions are closures, so they keep access to the private names after the function returns.[^3]

```text
 const counter = (function () {          one scope, created once
     let privateCounter = 0;               private
     function changeBy(n) { ... }          private
     return { increment, decrement, value };   public API
 })();

 caller:  counter.increment()   ok
          counter.privateCounter   undefined
```

MDN calls this function that is executed as soon as it is defined an IIFE, and says that its closures follow the Module Design Pattern.[^3] The participants are the **scope**, which holds the private members, and the **API object**, which the scope returns.

In other languages, the same pattern uses what the language has. A class with only static members and a private constructor, or a singleton, can stand for a module where namespaces are incomplete.[^1]

## Example

The example files show one counter module in each language. It has a private count and a private helper `changeBy`, and it shows `increment`, `decrement` and `value`. The program prints:

```text
value: 0
value: 2
value: 1
```

| Language | Private scope | Public API |
|---|---|---|
| Python | a closure | a `SimpleNamespace` of the closures |
| JavaScript | an IIFE | the returned object |
| TypeScript | an IIFE | the returned object, typed by an interface |
| Java | a final class with a private constructor | `static` methods |
| C# | a `static class` | `public static` methods |
| C++ | an unnamed namespace inside a named namespace | the functions of the named namespace |
| Go | a closure | a struct of functions |
| Rust | a `mod` block | its `pub` functions |

The private name is not on the API object:

```javascript run id=iife-counter
const counter = (function () {
  let privateCounter = 0;
  function changeBy(val) {
    privateCounter += val;
  }
  return {
    increment() {
      changeBy(1);
    },
    value() {
      return privateCounter;
    },
  };
})();

counter.increment();
console.log("value:", counter.value());
console.log("privateCounter:", counter.privateCounter);
```

```output for=iife-counter
value: 1
privateCounter: undefined
```

## Consequences

The benefits are privacy and a clean global scope. The pattern adds one global name, the module, and the private names cannot clash with the names of other scripts. A caller can use only the API, so the module can change its internals.

The costs:

- **One instance.** The IIFE runs once, so the module is a singleton. A module that needs many instances must be a factory function, called once for each instance.
- **Hard to test and extend.** A test cannot reach the private state, and code outside the module cannot add a method that uses it.
- **No dependency system.** The module reads its dependencies from global names, so the order of the scripts on the page matters. The pattern does not load code, and it does not show what a module imports.
- **Memory for each closure.** Each public function is a closure. For a singleton this cost is small, but a factory that makes many objects pays it for each object.

## When not to use it

Do not use the pattern in a language that has modules. A JavaScript module runs in its own scope, not in the global scope, and its declarations are not visible outside it unless it exports them.[^2] An ES module gives the privacy of the pattern, with static imports in place of global names. Python has the same feature: each module has its own private namespace, which the functions of the module use as their global namespace.[^4]

Do not use the pattern when the program needs many instances with private state. A class with private fields is clearer.

## Variants and related patterns

**Built-in modules.** Most languages now have the pattern in the language. Wikipedia notes that in Python each `.py` file is automatically a module.[^1] An identifier in Go is exported only if it starts with an upper-case letter and is declared in the package block, or is a field or method name.[^5] In Rust, code within a module is private by default, and `pub` makes an item public.[^6] ECMAScript 2015 added modules to JavaScript.[^7]

**CommonJS.** Node.js puts the pattern in the loader. Before it runs a module, it wraps the code in a function, so that top-level variables are scoped to the module and not to the global object.[^8]

**Singleton and facade.** The module pattern is often a [[singleton-pattern]], because it makes one instance. Its API can act as a [[facade-pattern]] over its private helpers.

## Known uses

- Node.js: [lib/internal/modules/cjs/loader.js at v24.0.0](https://github.com/nodejs/node/blob/v24.0.0/lib/internal/modules/cjs/loader.js#L341-L352) defines `wrap`, which puts the text `(function (exports, require, module, __filename, __dirname) {` before the source of each CommonJS module and `});` after it. Each module then runs in a function scope of its own.[^9][^8]

## References
