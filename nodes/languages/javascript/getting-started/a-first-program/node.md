---
tag: XFMCXZ
kind: feature
owner: javascript
slug: a-first-program
name: A first JavaScript program
aliases: [hello world in JavaScript, first JavaScript program, hello.js]
area: languages/javascript/getting-started
level: intro
summary: >-
  A first JavaScript program: hello world with console.log, then names, a function, a loop and command-line input in Node.js, and the same ideas in a web page with an event.
grammar: null
spec: null
see_also: [hello-world-program, python/hello-world-in-python, javascript/running-javascript-with-node-js, javascript/running-javascript-in-the-browser, javascript/the-browser-console, javascript/javascript-language-overview, parse-command-line-arguments]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Learn_web_development/Core/Scripting/What_is_JavaScript
    title: What is JavaScript?, MDN Learn web development
    locator: How do you add JavaScript to your page?; JavaScript running order
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/getting-started/introduction-to-nodejs
    title: Introduction to Node.js, Node.js Learn
    locator: An Example Node.js Application
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Learn_web_development/Core/Scripting/A_first_splash
    title: A first splash into JavaScript, MDN Learn web development
    locator: Adding variables to store our data; Functions; Text strings; Conditionals; Events; Loops
    accessed: 2026-11-18
  - url: https://nodejs.org/api/process.html#processargv
    title: Process, Node.js documentation
    locator: process.argv; Exit codes
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: Getting started with JavaScript
    accessed: 2026-11-18
---

## Synopsis

```text
hello.js:     console.log("Hello, world!");

terminal:     node hello.js
browser:      <script src="hello.js"></script> in a page, then open the console
```

The first program of most languages prints one line of text ([[hello-world-program]]). In JavaScript that line is a call to `console.log`. The same file runs under Node.js from a terminal, or in a web page through a script element, where the line shows up in the developer console ([[javascript/the-browser-console]]).[^1] [^2]

## Semantics

**r1. A program is a list of statements.** A script has no main function. "When the browser encounters a block of JavaScript, it generally runs it in order, from top to bottom."[^1] Node.js runs a file the same way: `node hello.js` runs `hello.js`.[^2] Example: [`hello`](#hello).

**r2. Output.** `console.log` prints its arguments on one line. Under Node.js the line goes to standard output; in a page it goes to the console of the developer tools ([[javascript/the-browser-console]]). A page shows text to its reader by changing the page itself, for example by setting the `textContent` of an element.[^1] [^3]

**r3. Names.** "You create a variable with the keyword let followed by a name for your variable." `const` declares a constant: "unlike variables, you can't change the value once set."[^3] Example: [`table`](#table).

**r4. Functions and text.** "Functions are reusable blocks of code that you can write once and run again and again". A string in backticks is a template literal, in which "you can embed other variables or even expressions".[^3] Example: [`first`](#first).

**r5. Decisions and loops.** `if` and `else if` run a block when a condition holds, and a `for...of` loop runs its body once for each value of an array.[^3] Example: [`table`](#table).

**r6. Input.** A Node.js program reads its command line from `process.argv`, an array of strings. The first two elements are the path of `node` and the path of the program; "The remaining elements are additional command-line arguments."[^4] A page gets its input from events instead. "Events are things that happen in the browser", such as a click, and `addEventListener` connects a function to one of them.[^3] Example: [`first`](#first).

**r7. Errors.** A mistake that the engine finds while it runs, such as a misspelled name, throws an error. The error stops the script, and a browser shows it in the developer console.[^1] In Node.js, an uncaught error also ends the process with exit code 1, "Uncaught Fatal Exception".[^4] Example: [`typo`](#typo).

## Examples

The examples run with Node.js 24. The classic first program:

```javascript run id=hello
console.log("Hello, world!");
```

```output for=hello
Hello, world!
```

A program that greets each name on its command line (rules r4 and r6). Run as `node first.js Ada Grace`, it prints:

```javascript run id=first args="Ada Grace"
const names = process.argv.slice(2);

function greet(name) {
  return `Hello, ${name}!`;
}

if (names.length === 0) {
  console.log(greet("world"));
}
for (const name of names) {
  console.log(greet(name));
}
console.log(`Greeted ${Math.max(names.length, 1)} time(s).`);
```

```output for=first
Hello, Ada!
Hello, Grace!
Greeted 2 time(s).
```

With no arguments, `names` is an empty array, the loop runs zero times, and the program greets the world.

A counting loop and a variable that changes on each pass (rules r3 and r5). `total` is declared with `let` because it changes; `n` belongs to the loop.

```javascript run id=table
let total = 0;
for (let n = 1; n <= 5; n++) {
  total += n * n;
  console.log(`${n} squared is ${n * n}, running total ${total}`);
}
```

```output for=table
1 squared is 1, running total 1
2 squared is 4, running total 5
3 squared is 9, running total 14
4 squared is 16, running total 30
5 squared is 25, running total 55
```

The arguments are strings (rule r6). Run as `node sum.js 2 3`, `+` joins them, and `Number` converts them first:

```javascript run id=sum args="2 3"
const [a, b] = process.argv.slice(2);
console.log(a + b);
console.log(Number(a) + Number(b));
```

```output for=sum
23
5
```

A typo in a name (rule r7). The first line runs, and the error comes when the second line runs:

```javascript fails id=typo expect=exception:ReferenceError
const message = "Hello";
consol.log(message);
```

Node.js reports `ReferenceError: consol is not defined` and points at the line.

The same greeting in a web page. The script sits at the end of the body, so the elements it looks up exist by then ([[javascript/running-javascript-in-the-browser]]). Each click on the button calls `greet` (rule r6):

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Greeter</title>
  </head>
  <body>
    <input id="name" value="world">
    <button id="go">Greet</button>
    <p id="out"></p>
    <script>
      const field = document.querySelector("#name");
      const out = document.querySelector("#out");

      function greet() {
        out.textContent = `Hello, ${field.value}!`;
        console.log("greeted", field.value);
      }

      document.querySelector("#go").addEventListener("click", greet);
    </script>
  </body>
</html>
```

Save it as `greet.html` and open it in a browser. The paragraph shows the greeting, and the console shows one line per click.

## Pitfalls

### A name used before its line

Code runs from top to bottom (rule r1), so a `const` or `let` name cannot be used above its declaration. MDN shows the message for a button used too early: "Uncaught ReferenceError: Cannot access 'button' before initialization". It adds that "generally items need to be defined before you can use them".[^1]

### Numbers that arrive as text

Command-line arguments and the values of form fields are strings, and `+` joins strings (see [`sum`](#sum)). Convert them with `Number` before arithmetic.[^4] [^3]

### Mismatched quotes

A string starts and ends with the same kind of quote. MDN's course warns that a string cannot open with a double quote and close with a single one.[^3]

### Trying code in the console

Snippets typed into a browser console one after another can interact. MDN's guide wraps its first example in a function that starts with `'use strict'`, and says the wrapper and strict mode "Prevent semantics in JavaScript that trip up beginners" and keep console snippets from interacting with one another.[^5]

## Comparison

| Step | Node.js | Web page |
|---|---|---|
| run | `node first.js Ada Grace` | open the page; its script element runs |
| input | `process.argv`, as strings | events, such as a click, and form fields |
| output | `console.log`, to standard output | the page itself, and the developer console |
| an uncaught error | the process ends with exit code 1 | an error in the developer console |

The rows follow the Node.js documentation and MDN's beginner course.[^2] [^4] [^1] [^3] The same first program in Python is [[python/hello-world-in-python]], and a larger tour of the language is [[javascript/javascript-language-overview]].

## References
