---
tag: JFG512
kind: feature
owner: javascript
slug: eval-and-the-function-constructor
name: eval and the Function constructor
aliases: [eval, direct eval, indirect eval, "(0, eval)", new Function, Function constructor, dynamic code, code from strings, unsafe-eval, HostEnsureCanCompileStrings]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  eval(code) runs a string as a script, and new Function(params, body) makes a function from strings. A direct eval sees the local scope. An indirect eval and new Function see only the global scope.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/global-object.html#sec-eval-x", locator: "19.2.1 eval ( x ); 19.2.1.1 PerformEval; 19.2.1.2 HostEnsureCanCompileStrings"}
see_also: [javascript/meta-programming, javascript/strict-mode, javascript/language-and-host, python/eval-and-exec, python/eval-of-untrusted-input]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/eval
  title: MDN Web Docs, JavaScript Reference, eval()
  locator: the introduction; Description (Direct and indirect eval); Never use direct eval!
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/global-object.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 19
  locator: 19.2.1 eval ( x ); 19.2.1.1 PerformEval; 19.2.1.2 HostEnsureCanCompileStrings
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.2.1.1 Function ( ...parameterArgs, bodyArg ); 20.2.1.1.1 CreateDynamicFunction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/Function
  title: MDN Web Docs, JavaScript Reference, Function() constructor
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Content-Security-Policy/script-src
  title: MDN Web Docs, HTTP Reference, CSP script-src
  locator: Unsafe eval expressions
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/cli.html
  title: Node.js v24 documentation, Command-line API
  locator: --disallow-code-generation-from-strings
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/vm.html
  title: Node.js v24 documentation, VM (executing JavaScript)
  locator: vm.createContext (codeGeneration); the introduction
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 18.2.1 eval (x); 19.2.1.1 Function ( p1, p2, … , pn, body )
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=scopes
globalThis.x = "global";
function test() {
  const x = "local";
  const direct = eval("x");
  const indirect = (0, eval)("x");
  const fromFunction = new Function("return x")();
  return [direct, indirect, fromFunction];
}
console.log(test());
```

```output for=scopes
[ 'local', 'global', 'global' ]
```

The direct call `eval("x")` reads the local `x`. The indirect call `(0, eval)("x")` and the function from `new Function` read the global `x`.

```text
eval(code)
new Function(param1, ..., paramN, body)
Function(param1, ..., paramN, body)
```

| Form | Scope of the code | Strict mode |
|---|---|---|
| `eval(code)`, a direct call | the scope of the call | the same as the code that calls it |
| `(0, eval)(code)`, `eval?.(code)`, `const e = eval; e(code)` | the global scope | only with `"use strict"` in the string |
| `new Function(...)` | the global scope | only with `"use strict"` in the body |

## Semantics

**r1.** "The eval() function evaluates JavaScript code represented as a string and returns its completion value. The source is parsed as a script."[^1] The value of an expression is its completion value, and many statements have one too. "If the argument of eval() is not a TrustedScript or string, eval() returns the argument unchanged."[^1] The specification step is "If x is not a String, return x."[^2] Example: [`values`](#values).

**r2.** There are two kinds of call. "Direct eval, as the name implies, refers to directly calling the global eval function with eval(...). Everything else, including invoking it via an aliased variable, via a member access or other expression, or through the optional chaining ?. operator, is indirect."[^1] The function `eval` is the same object in both cases. The form of the call decides the kind.

**r3.** A direct eval runs in the scope of the call. An indirect eval "works in the global scope rather than the local scope, and the code being evaluated doesn't have access to local variables within the scope where it's being called."[^1] An indirect eval "does not inherit the strictness of the surrounding context", but a direct eval "inherits the strictness of the invoking context."[^1] Example: [`scopes`](#scopes).

**r4.** In code that is not strict, a `var` in the string makes a variable in the scope around the call. In strict code, the `var` and function declarations of the string stay in the eval code.[^1] MDN: "let and const declarations within the evaluated string are always scoped to that script."[^1] Example: [`var-leak`](#var-leak).

**r5.** "The last argument (if any) specifies the body (executable code) of a function; any preceding arguments specify formal parameters."[^3] "Function() can be called with or without new. Both create a new Function instance."[^4] "However, unlike eval (which may have access to the local scope), the Function constructor creates functions which execute in the global scope only."[^4] The new function has the name `anonymous`. Examples: [`values`](#values), [`params`](#params).

**r6.** Both forms parse the string when they run. A string that is not valid code throws a `SyntaxError` at that time. MDN: "Function objects created with the Function constructor are parsed when the function is created."[^4] Example: [`params`](#params).

**r7.** The host can block both forms. HostEnsureCanCompileStrings "allows host environments to block certain ECMAScript functions which allow developers to interpret and evaluate strings as ECMAScript code."[^2] In a browser, a Content Security Policy does this: "If a page has a CSP header and 'unsafe-eval' isn't specified with the script-src directive, the following methods are blocked and won't have any effect".[^5] The list includes `eval()` and `Function()`.[^5] In Node, the flag `--disallow-code-generation-from-strings` makes "built-in language features like eval and new Function that generate code from strings throw an exception instead."[^6] A `vm` context can turn them off too: "If set to false any calls to eval or function constructors (Function, GeneratorFunction, etc) will throw an EvalError."[^7] Example: [`blocked`](#blocked).

**r8.** "eval() executes the code it's passed with the privileges of the caller."[^1] A string that comes from a user can run any code. MDN warns that these APIs "are known as injection sinks, and are potentially a vector for cross-site-scripting (XSS) attacks."[^1] Example: [`untrusted`](#untrusted).

## Examples

```javascript run id=values
console.log(eval("1 + 2"), eval("let a = 5; a * 2"), eval("if (true) 'yes'; else 'no'"));
console.log(eval(42), eval(new String("1 + 1")));
const add = new Function("a", "b", "return a + b");
console.log(add(2, 3), add.name, add.length);
console.log(add.toString());
```

```output for=values
3 10 yes
42 [String: '1 + 1']
5 anonymous 2
function anonymous(a,b
) {
return a + b
}
```

The `if` statement has a completion value too (rule r1). A `String` object is not a string, so `eval` returns it with no change. `new Function` puts the parameters and the body into the source of a function named `anonymous` (rule r5).

```javascript run id=var-leak
function run() {
  eval("var leaked = 1");
  console.log(typeof leaked);
}
run();
function strictRun() {
  "use strict";
  eval("var kept = 1");
  console.log(typeof kept);
}
strictRun();
```

```output for=var-leak
number
undefined
```

In code that is not strict, the `var` in the string adds `leaked` to the function scope. In strict code, the `var` stays in the scope of the eval code (rule r4).

```javascript run id=blocked
const vm = require("node:vm");
const context = vm.createContext({}, { codeGeneration: { strings: false } });
console.log(vm.runInContext("1 + 1", context));
try {
  vm.runInContext("eval('1 + 1')", context);
} catch (e) {
  console.log(e.name, e.message);
}
```

```output for=blocked
2
EvalError Code generation from strings disallowed for this context
```

The context can run a script, but it cannot make code from a string (rule r7). The Node documentation warns: "The node:vm module is not a security mechanism. Do not use it to run untrusted code."[^7]

## Versions

ECMAScript 2015, the 6th edition, already had "18.2.1 eval (x)" and "19.2.1.1 Function ( p1, p2, … , pn, body )".[^8] That edition had no host hook for code from strings. The 17th edition (ECMAScript 2026) has the hook HostEnsureCanCompileStrings in 19.2.1.2.[^2] It has `Function` in 20.2.1.1.[^3] The flag `--disallow-code-generation-from-strings` is "Added in: v9.8.0" of Node.[^6]

## Pitfalls

### eval of input

```javascript wrong id=untrusted
const input = '{"name": "Ana", "admin": false}';
console.log(JSON.parse(input).name);
const evil = '({ name: (globalThis.pwned = true, "x") })';
console.log(eval(evil).name, globalThis.pwned);
```

```output for=untrusted
Ana
x true
```

`eval` runs all the code in the string, so the string sets a global variable (rule r8). Use `JSON.parse` for data. It reads only JSON and runs no code.

### A body that is not valid code

```javascript fails id=params expect=exception:SyntaxError
const f = new Function("a", "b", "c", "return a + b + c");
const g = new Function("a, b, c", "return a + b + c");
console.log(f(1, 2, 3), g(1, 2, 3));
new Function("return {");
```

The parameters can be in separate strings or in one string. The last `new Function` throws a `SyntaxError` when it runs, not when the file loads (rule r6).

### Direct eval and the local scope

A direct eval can read any variable in the scope around it. MDN: "Direct eval can read and mutate bindings in the surrounding scope, which may lead to external input corrupting local data."[^1] So the engine and the build tools cannot inline or rename those variables.[^1] When code must come from a string, use an indirect eval or `new Function`, which see only the global scope (rules r3 and r5). Better, use a function or a table of functions.

## Comparison

Python has `eval` for an expression and `exec` for statements, and the caller can give the dictionaries of names. See [[python/eval-and-exec]]. JavaScript has one `eval` for all code, and the form of the call decides the scope (rule r2). To give names to the code, `new Function` takes them as parameters: MDN suggests "If you need to specify additional variables that the evaluated source should have access to, consider using the Function() constructor."[^1]

## References
