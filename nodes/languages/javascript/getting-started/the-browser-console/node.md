---
tag: 8690A4
kind: feature
owner: javascript
slug: the-browser-console
name: The browser console
aliases: [developer console, web console, DevTools console, console.log, console API]
area: languages/javascript/getting-started
level: intro
summary: >-
  The console of the browser developer tools shows the messages of a page and runs JavaScript typed into it. The console object of the Console Standard writes to it, and Node.js has the same object.
grammar: null
spec: {standard: "Console Living Standard", url: "https://console.spec.whatwg.org/", locator: "1 Namespace console; 2.2 Formatter"}
see_also: [javascript/running-javascript-in-the-browser, javascript/running-javascript-with-node-js, javascript/a-first-program, javascript/language-and-host, interactive-debugging, call-stack, exception-handling]
sources:
  - url: https://developer.chrome.com/docs/devtools/console
    title: Console overview, Chrome DevTools
    locator: Overview
    accessed: 2026-11-18
  - url: https://developer.chrome.com/docs/devtools/console/reference
    title: Console features reference, Chrome DevTools
    locator: Open the Console; Eager Evaluation
    accessed: 2026-11-18
  - url: https://firefox-source-docs.mozilla.org/devtools-user/web_console/index.html
    title: Web Console, Firefox DevTools User Docs
    locator: Opening the Web Console
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: Getting started with JavaScript
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/eval
    title: eval(), MDN
    locator: introduction; Return value
    accessed: 2026-11-18
  - url: https://firefox-source-docs.mozilla.org/devtools-user/web_console/the_command_line_interpreter/index.html
    title: The JavaScript input interpreter, Firefox DevTools User Docs
    locator: Single-line mode; Multi-line mode; Accessing variables
    accessed: 2026-11-18
  - url: https://developer.chrome.com/docs/devtools/console/utilities
    title: Console Utilities API reference, Chrome DevTools
    locator: introduction; $_; $0 - $4; $(selector)
    accessed: 2026-11-18
  - url: https://firefox-source-docs.mozilla.org/devtools-user/web_console/helpers/index.html
    title: Web Console Helpers, Firefox DevTools User Docs
    locator: $(); $0; $_
    accessed: 2026-11-18
  - url: https://console.spec.whatwg.org/
    title: Console Living Standard
    locator: 1.1 Logging functions; 1.2 Counting functions; 1.3 Grouping functions; 2.2 Formatter; 2.3 Printer
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/API/console
    title: console, MDN
    locator: introduction; Instance methods; Snapshotting objects
    accessed: 2026-11-18
  - url: https://nodejs.org/api/console.html
    title: Console, Node.js documentation
    locator: introduction; console.error(); console.info(); console.log(); console.warn()
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error/stack
    title: Error.prototype.stack, MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/command-line/how-to-use-the-nodejs-repl
    title: How to use the Node.js REPL, Node.js Learn
    locator: How to use the Node.js REPL; The _ special variable
    accessed: 2026-11-18
---

## Synopsis

```text
open in Chrome:    Control+Shift+J, or Command+Option+J on a Mac
open in Firefox:   Ctrl+Shift+K, or Cmd+Opt+K on macOS

> 3 + 5                         an expression: the console prints its value
8
> console.log("hi")             a call: the message, then the call's own value
hi
undefined
> $_                            the last result (a console helper)
undefined
```

Every desktop browser has developer tools, and their console is the first place to try JavaScript. The Chrome team names its "2 main uses: viewing logged messages and running JavaScript".[^1] The keys above open it in Chrome and in Firefox.[^2] [^3]

## Semantics

**r1. The console prints a completion value.** MDN: "The console works the exact same way as eval: the last expression entered is returned."[^4] `eval` "returns its completion value", and it returns `undefined` when that value is empty.[^5] An expression gives its value. A declaration gives nothing, so the console shows `undefined`. An `if` gives the value of the branch that ran, and a loop gives the value of its last pass. Example: [`completion`](#completion).

**r2. Multi-line input.** Enter runs the line. When the line is unfinished, such as `function foo() {`, or when Shift is held down, Enter starts another line instead.[^4] Firefox also has a multi-line editor, opened with Ctrl+B (Cmd+B on macOS).[^6]

**r3. The console runs in the page.** Code typed into the console sees the globals of the page: "both built-in variables like window and variables added by JavaScript libraries".[^6] Chrome evaluates the expression as you type and shows a preview of the result, a feature it calls Eager Evaluation.[^2]

**r4. Console helpers.** The consoles add names that only they define. In Chrome and in Firefox, `$_` is the result of the last expression, `$0` is the element selected in the element inspector, and `$()` and `$$()` find the first element, or all of the elements, that match a CSS selector.[^7] [^8] Chrome warns that these functions "won't work if you try to call them in your scripts".[^7]

**r5. The console object.** A script writes to the console with the `console` object of the Console Standard.[^9] Its logging functions are `log`, `info`, `warn`, `error`, `debug`, `assert`, `table`, `trace`, `dir` and `dirxml`. Other functions count calls (`count`), indent output (`group`, `groupEnd`) and time code (`time`, `timeEnd`).[^9] [^10] `console.count(label)` keeps a map from labels to numbers and prints `label: n`. A group has "one further level of indentation than its parent".[^9] Example: [`formats`](#formats).

**r6. Format specifiers.** When the first argument is a string, the specifiers in it take the next arguments: `%s` a string, `%d` or `%i` an integer, `%f` a float, `%o` and `%O` an object, and `%c` CSS for the rest of the text.[^9] The arguments left over are printed after the message. How a value looks is up to the console, since the printer "is implementation-defined".[^9] Example: [`formats`](#formats).

**r7. Message levels.** `log`, `info`, `warn`, `error` and `debug` each style their output differently, and a reader can use "the filtering controls provided by your browser to view only the kinds of output that interest you".[^10] Node.js has the same global `console`, "configured to write to process.stdout and process.stderr". There, `console.log` prints to standard output and `console.info` is an alias for it, while `console.error` prints to standard error and `console.warn` is an alias for that.[^11] Example: [`streams`](#streams).

**r8. Errors and stack traces.** The `stack` property of an error holds its stack trace, the chain of calls that led to it ([[call-stack]]). "Every implementation uses a separate line in the stack to represent each function call." The call that caused the error is at the top, and the call that started the chain is at the bottom. The property is not standardized, and each engine has its own format. MDN shows the V8 form `at baz (filename.js:10:15)`: a function name, then the file, the line and the column.[^12] Example: [`stack`](#stack).

## Examples

The examples run with Node.js 24, whose `console` follows the same standard.[^11] The outputs show Node.js formatting; a browser console shows the same values in its own style (rule r6).

What the console prints for each kind of input (rule r1). The example calls `eval` on each line, as the console does:

```javascript run id=completion
const inputs = [
  "3 + 5",
  "let a = 1; a + 1",
  "var b = 3",
  "function f() {}",
  "if (true) { 'yes' } else { 'no' }",
  "for (let i = 0; i < 3; i++) i * 10",
  "console.log('logged')",
];
for (const input of inputs) {
  console.log(JSON.stringify(input).padEnd(38), "->", eval(input));
}
```

```output for=completion
"3 + 5"                                -> 8
"let a = 1; a + 1"                     -> 2
"var b = 3"                            -> undefined
"function f() {}"                      -> undefined
"if (true) { 'yes' } else { 'no' }"    -> yes
"for (let i = 0; i < 3; i++) i * 10"   -> 20
logged
"console.log('logged')"                -> undefined
```

The last input prints `logged` first, because the argument is evaluated before the line is printed, and then gives `undefined`, the return value of `console.log`.

Format specifiers, groups, counters and a table (rules r5 and r6):

```javascript run id=formats
console.log("%s has %d items", "cart", 3);
console.log("%s costs %f", "tea", 2.5, "(extra arguments are appended)");
console.log("an object:", { id: 5, tags: ["a", "b"] });

console.group("outer");
console.log("indented once");
console.group("inner");
console.log("indented twice");
console.groupEnd();
console.groupEnd();

console.count("clicks");
console.count("clicks");
console.table([{ name: "Ada", year: 1815 }, { name: "Grace", year: 1906 }]);
```

```output for=formats
cart has 3 items
tea costs 2.5 (extra arguments are appended)
an object: { id: 5, tags: [ 'a', 'b' ] }
outer
  indented once
  inner
    indented twice
clicks: 1
clicks: 2
┌─────────┬─────────┬──────┐
│ (index) │ name    │ year │
├─────────┼─────────┼──────┤
│ 0       │ 'Ada'   │ 1815 │
│ 1       │ 'Grace' │ 1906 │
└─────────┴─────────┴──────┘
```

Where each level goes in Node.js (rule r7). The example runs a child process and shows its two streams. A failed `console.assert` writes to standard error too.

```javascript run id=streams
const { spawnSync } = require("node:child_process");

const code = "console.log('log'); console.info('info'); console.warn('warn'); console.error('error'); console.assert(false, 'assert')";
const result = spawnSync(process.execPath, ["-e", code], { encoding: "utf8" });
console.log("stdout:", JSON.stringify(result.stdout));
console.log("stderr:", JSON.stringify(result.stderr));
```

```output for=streams
stdout: "log\ninfo\n"
stderr: "warn\nerror\nAssertion failed: assert\n"
```

Reading a stack trace (rule r8). `loadUser` calls `parseRecord`, which calls `JSON.parse` with broken text. The example prints the name, the message and the function of the top three frames, innermost first:

```javascript run id=stack
function loadUser(id) {
  return parseRecord(`${id}`);
}

function parseRecord(text) {
  return JSON.parse(text + "}");
}

try {
  loadUser(7);
} catch (error) {
  console.log(error.name);
  console.log(error.message);
  const frames = error.stack.split("\n").filter((line) => line.trim().startsWith("at "));
  for (const frame of frames.slice(0, 3)) {
    console.log(frame.trim().split(" ")[1]);
  }
}
```

```output for=stack
SyntaxError
Unexpected non-whitespace character after JSON at position 1 (line 1 column 2)
JSON.parse
parseRecord
loadUser
```

An uncaught error ends the script, and the console shows it. The message names the property that was read and the value it was read from:

```javascript fails id=null-read expect=exception:TypeError
const users = { ada: { name: "Ada" } };
const user = users.grace ?? null;
console.log(user.name);
```

Node.js reports `TypeError: Cannot read properties of null (reading 'name')`.

## Pitfalls

### A logged object can change later

MDN: "Information about an object is lazily retrieved." A browser console shows an object as it is when the reader first expands it, not as it was when it was logged. To keep the state at logging time, log a copy: `JSON.parse(JSON.stringify(obj))` or `structuredClone(obj)`.[^10]

### Console helpers in a script

`$_`, `$0` and `$()` exist only in the console (rule r4). A script that calls them fails, or finds a different `$` defined by a library of the page.[^7]

### The `undefined` after a log

The `undefined` line after `console.log` is not an error. It is the return value of the call, which the console prints by rule r1 (see [`completion`](#completion)).

### Code that parses `stack`

The format of `stack` differs between engines (rule r8). MDN labels the property non-standard and says: "We do not recommend using non-standard features in production".[^12] Use it for reading, not for program logic.

## Comparison

| Task | Chrome console | Firefox console | Node.js REPL |
|---|---|---|---|
| open | Control+Shift+J | Ctrl+Shift+K | `node` |
| last result | `$_` | `$_` | `_` |
| selected element | `$0` | `$0` | — |
| another line | Shift+Enter | Shift+Enter | an unfinished line continues |
| where `console.log` goes | the console panel | the console panel | standard output |

The rows come from MDN, the Chrome and Firefox documentation and the Node.js pages.[^4] [^2] [^3] [^6] [^7] [^8] [^13] [^11] A dash means that the sources above give no entry for that tool. The REPL of Node.js is covered in [[javascript/running-javascript-with-node-js]], and the debugger that pauses code is covered in [[interactive-debugging]].

## References
