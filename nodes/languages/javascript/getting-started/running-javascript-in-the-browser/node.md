---
tag: 7KJ7TF
kind: feature
owner: javascript
slug: running-javascript-in-the-browser
name: Running JavaScript in the browser
aliases: [script element, script tag, defer, async, type module, classic script]
area: languages/javascript/getting-started
level: intro
summary: >-
  A web page runs JavaScript through the script element: inline or from a file, as a classic script or a module, at once or after parsing with defer, async or type="module".
grammar: "<script [src=url] [type=module] [async] [defer] [nomodule]>source</script> (HTML Standard, 4.12.1 The script element)"
spec: {standard: "HTML Living Standard", url: "https://html.spec.whatwg.org/multipage/scripting.html#the-script-element", locator: "4.12.1 The script element"}
see_also: [javascript/language-and-host, javascript/the-browser-console, javascript/a-first-program, javascript/running-javascript-with-node-js, event-loop-pattern, lexical-scope]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/HTML/Reference/Elements/script
    title: "<script>: The Script element, MDN"
    locator: Attributes; Notes; Examples, async and defer
    accessed: 2026-11-18
  - url: https://html.spec.whatwg.org/multipage/scripting.html#the-script-element
    title: HTML Living Standard
    locator: 4.12.1 The script element
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Learn_web_development/Core/Scripting/What_is_JavaScript
    title: What is JavaScript?, MDN Learn web development
    locator: How do you add JavaScript to your page?; Script loading strategies
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Modules
    title: JavaScript modules, MDN
    locator: What's a module?
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Document/DOMContentLoaded_event
    title: "Document: DOMContentLoaded event, MDN"
    locator: introduction; Checking whether loading is already complete
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-global-environment-records
    title: ECMAScript 2026 Language Specification
    locator: 9.1.1.4 Global Environment Records; 9.1.1.5.4 GetThisBinding
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-scripts-and-modules.html#sec-globaldeclarationinstantiation
    title: ECMAScript 2026 Language Specification
    locator: 16.1.7 GlobalDeclarationInstantiation, Note 2
    accessed: 2026-11-18
---

## Synopsis

```html
<script>/* inline classic script: runs now, blocks the parser */</script>
<script src="app.js"></script>              <!-- external classic script: fetch, run, then parse on -->
<script defer src="app.js"></script>        <!-- fetch in parallel, run after parsing, in order -->
<script async src="stats.js"></script>      <!-- fetch in parallel, run when it arrives, any order -->
<script type="module" src="main.js"></script>  <!-- a module: deferred by default -->
<script nomodule src="old.js"></script>     <!-- skipped by browsers that run modules -->
```

A browser runs JavaScript that a page names in `<script>` elements. The code can sit inside the element or come from the file that `src` names.[^1] The `type` attribute decides between a classic script and a module, and `async` and `defer` decide when the code runs.[^2]

## Semantics

**r1. Inline and external.** Without `src`, the element holds the code itself. With `src`, the browser loads the file at that URL.[^1] MDN's beginner course gives external files the extension `.js` and calls them "generally a good thing", because several pages can share one file.[^3]

**r2. Classic script or module.** With no `type` attribute, or a JavaScript MIME type, the element is a classic script. The value `module` "causes the code to be treated as a JavaScript module". Any other value makes a data block that the browser does not run.[^1] A module is in strict mode, may use `import` and `export`, and runs in its own scope.[^4]

**r3. When a classic script runs.** For an external classic script, the HTML Standard gives three modes. With `async`, the script "will be fetched in parallel to parsing and evaluated as soon as it is available". With `defer`, it is fetched in parallel and "evaluated when the page has finished parsing". With neither, it "is fetched and evaluated immediately, blocking parsing until these are both complete".[^2] An inline classic script runs at once, and `async` and `defer` have no effect on it.[^1]

**r4. Order.** Scripts with `defer` "execute in the order in which they appear in the document". Scripts with `async` give "no guarantee that scripts will run in any specific order".[^1]

**r5. When a module runs.** A module and its imports are fetched in parallel to parsing. Without `async` it runs after parsing, and with `async` it runs as soon as it is ready. "The defer attribute has no effect on module scripts."[^2] MDN adds that "they defer by default", so modules without `async` keep their document order.[^1]

**r6. DOMContentLoaded.** The event fires when the document "has been completely parsed, and all deferred scripts" have run, which includes modules. It does not wait for `async` scripts, so an `async` script may run after it.[^5] The first example shows the order.

**r7. One global scope for classic scripts.** MDN: "all classic scripts attached to the webpage are executed under the same scope", the global scope.[^4] In ECMA-262 that scope is one global environment record with two parts. Function and `var` declarations become properties of the global object, and `let`, `const` and `class` declarations go in a separate declarative part.[^6] A later script sees both kinds of names. Example: [`shared-global`](#shared-global).

**r8. A clash stops the whole script.** A `let`, `const` or `class` name that an earlier script declared is an error. ECMA-262 says that such conflicts "that span more than one Script are detected as runtime errors" before the script runs, and then "no bindings are instantiated for the script".[^7] Not one statement of that script runs. Example: [`shared-global`](#shared-global).

**r9. `this` at the top level.** In a classic script the top-level `this` is the global object, which is `window` in a page. In a module it is `undefined`, because the module environment record returns `undefined` as its `this` binding.[^6] Example: [`module-this`](#module-this).

**r10. Modules need a server.** A browser loads a module only from the same origin as the page or through CORS, and a `file://` URL does not count. The beginner course says the page then shows "Cross-origin request blocked", and recommends a local testing server.[^3] [^1]

## Examples

The page below has a deferred script, a module and an inline script. It needs a browser. Served from a local server, its console shows the lines in this order: `inline`, `deferred`, `module`, `DOMContentLoaded`. The inline script runs while the parser reaches it (r3). The deferred script and the module run after parsing, in document order (r4, r5), and the event comes after both (r6).

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Script order</title>
    <script defer src="deferred.js"></script>        <!-- console.log("deferred") -->
    <script type="module" src="module.js"></script>  <!-- console.log("module") -->
  </head>
  <body>
    <script>
      document.addEventListener("DOMContentLoaded", () => console.log("DOMContentLoaded"));
      console.log("inline");
    </script>
  </body>
</html>
```

Without a browser at hand, the module `node:vm` of Node.js 24 can play one page. Each `runInContext` call on one context is one classic script (r7, r8). The third script names a `let` that the first one declared. It fails before its first statement, so `third starts` never appears.

```javascript run id=shared-global
const vm = require("node:vm");

const page = vm.createContext({ log: console.log });

vm.runInContext(`
  var greeting = "hello";
  let count = 1;
  function shout(text) { return text.toUpperCase(); }
`, page, { filename: "first.js" });

vm.runInContext(`
  log(shout(greeting), count);
  count++;
  log("greeting" in globalThis, "shout" in globalThis, "count" in globalThis);
`, page, { filename: "second.js" });

try {
  vm.runInContext(`log("third starts"); let count = 5;`, page, { filename: "third.js" });
} catch (error) {
  console.log(`${error.name}: ${error.message}`);
}
vm.runInContext(`log(count)`, page);
```

```output for=shared-global
HELLO 1
true true false
SyntaxError: Identifier 'count' has already been declared
2
```

The second line shows the two parts of the global scope: `greeting` and `shout` are properties of the global object, and `count` is not. The last line shows that `count` kept the value 2.

The top-level `this` of rule r9, first in a classic script (through `node:vm`), then in a module (through a child process with `--input-type=module`):

```javascript run id=module-this
const { execFileSync } = require("node:child_process");
const vm = require("node:vm");

const probe = "typeof this + ' ' + typeof (function () { return this; })()";

console.log("classic:", vm.runInContext(probe, vm.createContext({})));

const out = execFileSync(process.execPath, ["--input-type=module", "-e", `console.log(${probe})`]);
console.log("module: ", out.toString().trim());
```

```output for=module-this
classic: object object
module:  undefined undefined
```

The plain function call gives `this` as the global object in sloppy mode, and `undefined` in the strict code of a module.

## Pitfalls

### A script in the head that reads the page

A classic script without `defer` runs before the parser has built the rest of the page, so `document.querySelector('button')` finds nothing. Use `defer` or `type="module"`, or put the script at the end of the body.[^3]

### `async` for scripts that depend on each other

The order of `async` scripts is not fixed. MDN's example: a library and two scripts that use it, all three `async`, may run in any order, and the scripts then fail because the library "will not be defined at the time the script runs".[^1]

### A listener added too late

An `async` script or a module that waits for a fetch may run after `DOMContentLoaded`, and a listener added then never fires. MDN advises checking `document.readyState` before adding the listener.[^5]

### Opening a module page from disk

A page opened with a `file://` URL cannot load its modules (rule r10). Serve the folder with a local server instead.[^3]

## Comparison

| Form | Fetch | Runs | Order kept | Scope |
|---|---|---|---|---|
| inline classic | none | at once, parser waits | yes | global |
| external classic | parser waits | at once after fetch | yes | global |
| `defer` | in parallel | after parsing | yes | global |
| `async` | in parallel | when fetched | no | global |
| `type="module"` | in parallel, with imports | after parsing | yes | its own module scope |
| `type="module" async` | in parallel, with imports | when ready | no | its own module scope |

The rows follow the HTML Standard and MDN.[^2] [^1] [^4] Node.js runs a file without any of this: [[javascript/running-javascript-with-node-js]].

## References
