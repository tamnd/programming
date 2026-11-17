---
tag: PEVVRE
kind: feature
owner: javascript
slug: loops-and-iteration
name: Loops and iteration
aliases: [JavaScript loops, iteration statements, for loop, while loop, for...of, for...in, looping over an array]
area: languages/javascript/collections
level: intro
summary: >-
  JavaScript repeats code with for, while, do...while, for...in and for...of, plus break, continue and labels. for...of walks the values of an iterable, and for...in walks property names.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-iteration-statements", locator: "14.7 Iteration Statements"}
see_also: [loop, for-loop, while-loop, javascript/arrays, javascript/maps, iterate-over-a-map]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Loops_and_iteration
  title: MDN Web Docs, JavaScript Guide, Loops and iteration
  locator: the introduction; for statement; do...while statement; while statement; break statement; continue statement; for...in statement; for...of statement
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 14
  locator: 14.7 Iteration Statements; 14.7.4.4 CreatePerIterationEnvironment; 14.7.5 The for-in, for-of, and for-await-of Statements
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...of
  title: MDN Web Docs, JavaScript Reference, for...of
  locator: the introduction; Description; Early exiting
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...in
  title: MDN Web Docs, JavaScript Reference, for...in
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for
  title: MDN Web Docs, JavaScript Reference, for
  locator: Lexical declarations in the initialization block
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/forEach
  title: MDN Web Docs, JavaScript Reference, Array.prototype.forEach()
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/tutorial/controlflow.html
  title: The Python Tutorial, Python 3.14, More Control Flow Tools
  locator: for Statements
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 14
  locator: 14.14.2 The enhanced for statement
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=all
const fruits = ["apple", "pear", "plum"];

for (let i = 0; i < fruits.length; i++) console.log(i, fruits[i]);
for (const fruit of fruits) console.log(fruit);
for (const key in { a: 1, b: 2 }) console.log(key);

let n = 0;
while (n < 2) n++;
do n += 10; while (n < 5);
console.log(n);
```

```output for=all
0 apple
1 pear
2 plum
apple
pear
plum
a
b
12
```

| Statement | Repeats | Use it for |
|---|---|---|
| `for (init; cond; next)` | while `cond` is true | a counter or an index |
| `while (cond)` | while `cond` is true, tested first | an unknown number of passes |
| `do ... while (cond)` | at least once, tested after | a body that must run once |
| `for (x of iterable)` | for each value of an iterable | arrays, strings, maps, sets |
| `for (k in object)` | for each enumerable string key | the property names of a plain object |
| `break`, `continue`, `label:` | stop the loop, skip a pass, name a loop | early exit, nested loops |

## Semantics

**r1.** "Loops offer a quick and easy way to do something repeatedly."[^1] MDN lists the statements: "for statement do...while statement while statement labeled statement break statement continue statement for...in statement for...of statement".[^1] In the specification, an `IterationStatement` is one of `DoWhileStatement`, `WhileStatement`, `ForStatement` and `ForInOfStatement`.[^2]

**r2.** "A for loop repeats until a specified condition evaluates to false."[^1] If the condition is left out, "the condition is assumed to be true."[^1] A `while` loop tests first: "The condition test occurs before statement in the loop is executed."[^1] In a `do...while` loop, the "statement is always executed once before the condition is checked."[^1] Example: [`all`](#all).

**r3.** `break` without a label "terminates the innermost enclosing while , do-while , for , or switch immediately", and with a label "it terminates the specified labeled statement."[^1] `continue` "terminates the current iteration of the innermost enclosing while , do-while , or for statement and continues execution of the loop with the next iteration."[^1] Example: [`labels`](#labels).

**r4.** `for...of` "executes a loop that operates on a sequence of values sourced from an iterable object."[^3] It "first calls the iterable's [Symbol.iterator]() method, which returns an iterator, and then repeatedly calls the resulting iterator's next() method".[^3] So it works on arrays, strings, maps, sets and any user-defined iterable. If the loop exits early, "the return() method of the iterator is called to perform any cleanup."[^3]

**r5.** `for...in` "iterates over all enumerable string properties of an object (ignoring properties keyed by symbols), including inherited enumerable properties."[^4] On an array it gives the indexes as strings, and also any other enumerable property: "for...in iterates over property names, for...of iterates over property values".[^1] Example: [`in-vs-of`](#in-vs-of).

**r6.** A `let` declared in the head of a `for` loop gets a new binding for each pass. The specification copies the per-iteration bindings into a new environment with `CreatePerIterationEnvironment`.[^2] For `for...of`, "Each iteration creates a new variable."[^3] A function made in the loop body sees the value of its own pass, as the [`closures`](#closures) example shows. With `var`, all passes share one variable, because "variables declared with var are only function-scoped, but not lexically scoped".[^5]

**r7.** Array methods such as `forEach` are not statements, and "There is no way to stop or break a forEach() loop other than by throwing an exception."[^6] MDN sends early exits to the statements: "Early termination may be accomplished with looping statements like for , for...of , and for...in ."[^6]

## Examples

Keys and values of the same array (rule r5).

```javascript run id=in-vs-of
const arr = [3, 5, 7];
arr.foo = "hello";
const keys = [];
for (const i in arr) keys.push(i);
const values = [];
for (const v of arr) values.push(v);
console.log(keys, values);

const obj = { foo: 1, bar: 2 };
for (const [key, val] of Object.entries(obj)) console.log(key, val);
```

```output for=in-vs-of
[ '0', '1', '2', 'foo' ] [ 3, 5, 7 ]
foo 1
bar 2
```

`for...of` with `Object.entries` gives the keys and values of a plain object, which is not iterable itself.

Leaving two loops at once with a label (rule r3).

```javascript run id=labels
const grid = [[1, 2], [3, 4], [5, 6]];
let found = null;
outer: for (const row of grid) {
  for (const cell of row) {
    if (cell % 2 === 0) continue;
    if (cell > 2) {
      found = cell;
      break outer;
    }
  }
}
console.log(found);
```

```output for=labels
3
```

One binding per pass (rule r6).

```javascript run id=closures
const fromLet = [];
for (let i = 0; i < 3; i++) fromLet.push(() => i);

const fromVar = [];
for (var j = 0; j < 3; j++) fromVar.push(() => j);

console.log(fromLet.map((f) => f()), fromVar.map((f) => f()));
```

```output for=closures
[ 0, 1, 2 ] [ 3, 3, 3 ]
```

## Pitfalls

### `for...in` over an array

`for...in` gives string keys, and it also visits added properties (rule r5). MDN: "it is better to use a traditional for loop with a numeric index when iterating over arrays".[^1]

```javascript wrong id=in-array
const scores = [10, 20];
scores.label = "round 1";
let sum = 0;
for (const i in scores) sum += scores[i];
console.log(sum);

let total = 0;
for (const s of scores) total += s;
console.log(total);
```

```output for=in-array
30round 1
30
```

### `return` inside `forEach`

A `return` in the callback ends that call only, so the loop goes on (rule r7).

```javascript wrong id=foreach-return
const seen = [];
[1, 2, 3, 4].forEach((n) => {
  if (n === 2) return;
  seen.push(n);
});
console.log(seen);

const before = [];
for (const n of [1, 2, 3, 4]) {
  if (n === 2) break;
  before.push(n);
}
console.log(before);
```

```output for=foreach-return
[ 1, 3, 4 ]
[ 1 ]
```

## Comparison

"The JavaScript for loop is similar to the Java and C for loop."[^1] Python has no counting form: "Python’s for statement iterates over the items of any sequence (a list or a string), in the order that they appear in the sequence."[^7] That is the job of `for...of` in JavaScript (rule r4). Java's enhanced `for` needs an array or an `Iterable`: "The type of the Expression must be an array type (§10.1) or a subtype of the raw type Iterable , or a compile-time error occurs."[^8] Go has one loop keyword for all forms: "A "for" statement specifies repeated execution of a block. There are three forms: The iteration may be controlled by a single condition, a "for" clause, or a "range" clause."[^9] See [[loop]], [[for-loop]] and [[while-loop]].

## References
