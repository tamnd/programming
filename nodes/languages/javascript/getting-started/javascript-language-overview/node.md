---
tag: N7C7AE
kind: feature
owner: javascript
slug: javascript-language-overview
name: JavaScript language overview
aliases: [JavaScript tour, JavaScript for programmers, JavaScript basics]
area: languages/javascript/getting-started
level: intro
summary: >-
  A short tour of JavaScript for a reader who knows another language: the types, variables, operators, control flow, objects, arrays, functions, classes, async code and modules.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types", locator: "6.1 ECMAScript Language Types"}
see_also: [javascript, javascript/what-is-javascript, javascript/a-first-program, javascript/language-and-host, type-conversion, closures, async-await, event-loop-pattern]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Language_overview
    title: JavaScript language overview, MDN
    locator: Data types; Variables; Operators; Grammar; Control structures; Objects; Arrays; Functions; Classes; Asynchronous programming; Modules
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types
    title: ECMAScript 2026 Language Specification
    locator: 6.1 ECMAScript Language Types
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: JavaScript and Java
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Modules
    title: JavaScript modules, MDN
    locator: What's a module?
    accessed: 2026-11-18
  - url: https://nodejs.org/api/packages.html#determining-module-system
    title: Modules, Packages, Node.js documentation
    locator: Determining module system; Syntax detection
    accessed: 2026-11-18
---

## Synopsis

This tour is for a reader who already programs in another language. It follows the MDN language overview.[^1]

| Topic | In one line |
|---|---|
| types | seven primitive types and objects; a variable has no type |
| variables | `let` and `const` are block-scoped; `var` is function-scoped |
| operators | `+` also joins strings; `===` compares without conversion |
| control flow | the C family: `if`, `switch`, `for`, `while`, `try`; plus `for...of` |
| objects | bags of properties, always handled by reference |
| functions | first-class values with closures, rest and default parameters |
| classes | syntax over prototypes, with `#private` members |
| async code | callbacks, promises and `async`/`await` on one thread |
| modules | `import` and `export`, resolved by the host |

## Semantics

**r1. Types.** ECMA-262 lists eight language types: Undefined, Null, Boolean, String, Symbol, Number, BigInt and Object.[^2] The first seven are the primitive types, and MDN sums up the rest: "Everything else is known as an Object."[^1] A Number is a 64-bit IEEE 754 double, so integers and fractions share one type. A BigInt is an integer of any size, and arithmetic cannot mix the two.[^1] Example: [`types`](#types).

**r2. Dynamic typing.** In MDN's words, types "are only associated with values, but not with variables".[^1] MDN contrasts this with Java: in JavaScript, variable data types are not declared, while in Java they must be.[^3]

**r3. Variables.** `let` declares a block-scoped variable, and `const` declares one that cannot be reassigned. A `const` object can still change inside. MDN calls `var` declarations surprising and discourages them.[^1] A `let` or `const` name occupies its whole block, so reading it before the declaration line throws an error (the temporal dead zone).[^1] Example: [`tdz`](#tdz).

**r4. Operators and conversion.** When one operand of `+` is a string, `+` joins strings, so `'3' + 4 + 5` is `'345'` and `3 + 4 + '5'` is `'75'`. The `==` operator converts its operands, and `===` does not and "is usually preferred".[^1] The values `false`, `0`, the empty string, `NaN`, `null` and `undefined` are falsy, and every other value is truthy ([[type-conversion]]).[^1] Example: [`coerce`](#coerce).

**r5. Grammar and control flow.** The grammar "is very similar to the C family". Semicolons are optional, because the language inserts them where needed, but they stay part of the syntax.[^1] The control structures are `if`/`else`, `switch` (with fall-through), `for`, `while`, `do...while`, `for...of` over iterables, `for...in` over property names, and `try...catch` with `throw`. The control structures are statements, so a program cannot assign an `if` to a variable.[^1]

**r6. Objects and arrays.** An object is a collection of key-value pairs with no fixed shape. A key is a string or a symbol. "Objects are always references", so two names can point at one object, and two separately created objects are never equal.[^1] An array is an object with a `length` property that is one more than the highest index. Reading past the end gives `undefined` and does not throw.[^1] Example: [`objects`](#objects).

**r7. Functions.** A function is an object that can be called. A call can pass more or fewer arguments than the parameters; the missing ones are `undefined`. Rest parameters (`...args`), defaults (`x = 0`) and destructured parameters (`{ width, height }`) cover the other cases. An arrow function `(a, b) => a + b` is a short form, and an inner function can read the variables of the outer one ([[closures]]).[^1] Example: [`functions`](#functions).

**r8. Classes.** `class` gives syntax close to Java, but "JavaScript classes are just functions that must be instantiated with the new operator". `static` makes a static member, and a `#` prefix makes a private one that no code outside the class body can read.[^1] Under the classes, inheritance goes through prototypes ([[javascript/what-is-javascript]]). Example: [`classes`](#classes).

**r9. Asynchronous code.** JavaScript "is single-threaded by nature" and runs asynchronous work through an event loop ([[event-loop-pattern]]). The idioms are callbacks, promises, and `async`/`await` as "syntactic sugar for Promises" ([[async-await]]).[^1] "Promises are never blocking". Their callbacks run as jobs after the running code ends ([[javascript/language-and-host]]). Example: [`order`](#order).

**r10. Modules.** A module is usually a file. It shares names with `export` and takes them with `import`, and the host resolves the module paths.[^1] Code runs either as a classic script or as a module. A module is in strict mode and has its own scope.[^4] Node.js treats a `.mjs` file as a module, and it treats a `.js` file as one when the nearest `package.json` has `"type": "module"` or when the code uses module syntax such as `import`.[^5]

## Examples

The examples run with Node.js 24. Each one is a CommonJS script except where it says so.

The `typeof` operator names the type of each kind of value (rule r1). Two results are surprises: `typeof null` is `"object"`, and a function gets its own answer, `"function"`.

```javascript run id=types
const values = [42, 42n, "text", true, Symbol("id"), undefined, null, {}, [], function () {}];
console.log(values.map((value) => typeof value).join(" "));
```

```output for=types
number bigint string boolean symbol undefined object object object function
```

Conversions of rule r4, the binary double of `0.1 + 0.2`, and BigInt division that truncates toward zero:

```javascript run id=coerce
console.log("3" + 4 + 5, 3 + 4 + "5");
console.log(123 == "123", 123 === "123");
console.log(0.1 + 0.2, 3 / 2, -3n / 2n);
console.log(Boolean(""), Boolean("0"), Boolean([]));
```

```output for=coerce
345 75
true false
0.30000000000000004 1.5 -1n
false true true
```

The string `'0'` and the empty array are truthy; only the six values of rule r4 are falsy.

Two names for one object, a missing property, and an array method that returns a fresh array (rule r6):

```javascript run id=objects
const cat = { name: "Tom", details: { age: 3 } };
const same = cat;
same.name = "Kit";
console.log(cat.name, cat["details"].age, cat.owner);

const pets = ["dog", "cat"];
pets.push("hen");
console.log(pets.length, pets.map((pet) => pet.toUpperCase()));
```

```output for=objects
Kit 3 undefined
3 [ 'DOG', 'CAT', 'HEN' ]
```

Rest parameters, a destructured parameter with a default, and a closure that keeps its own counter (rule r7):

```javascript run id=functions
function average(...numbers) {
  let sum = 0;
  for (const n of numbers) sum += n;
  return sum / numbers.length;
}

function area({ width, height = 1 }) {
  return width * height;
}

function makeCounter() {
  let count = 0;
  return () => ++count;
}

const next = makeCounter();
console.log(average(2, 3, 4, 5), area({ width: 4 }), area({ width: 2, height: 3 }));
console.log(next(), next(), next());
```

```output for=functions
3.5 4 6
1 2 3
```

A class with a static field and a private field (rule r8). `Object.keys` lists only `owner`, because the private field is not a property, and `typeof` shows that the class is a function.

```javascript run id=classes
class Account {
  static count = 0;
  #balance = 0;

  constructor(owner) {
    this.owner = owner;
    Account.count++;
  }

  deposit(amount) {
    this.#balance += amount;
    return this.#balance;
  }
}

const a = new Account("Ada");
a.deposit(10);
console.log(a.owner, a.deposit(5), Account.count, Object.keys(a));
console.log(typeof Account);
```

```output for=classes
Ada 15 1 [ 'owner' ]
function
```

The order of asynchronous work (rule r9). The script runs to its end first. Then the promise callbacks run, in the order they were queued, and the timer comes last.

```javascript run id=order
setTimeout(() => console.log("timer"), 0);
Promise.resolve().then(() => console.log("promise callback"));
(async () => {
  console.log("async function starts");
  await null;
  console.log("after await");
})();
console.log("end of script");
```

```output for=order
async function starts
end of script
promise callback
after await
timer
```

The temporal dead zone of rule r3. Inside the block, `x` names the `const`, which does not have a value yet, so the parameter `x` cannot be seen there.

```javascript fails id=tdz expect=exception:ReferenceError
function show(x) {
  if (x) {
    console.log(x);
    const x = 2;
  }
}
show(1);
```

Node.js reports `ReferenceError: Cannot access 'x' before initialization`.

## Pitfalls

### `+` with a string operand

Input from a form, a file or the command line is a string. Adding a number to it joins the text: `'3' + 4` is `'34'`. Convert first with `Number(text)` or unary `+text`.[^1]

### Floating-point results

`0.1 + 0.2` is not `0.3` (see [`coerce`](#coerce)). MDN warns: "Because of IEEE 754 encoding, sometimes floating point arithmetic can be imprecise."[^1] See [[floating-point-arithmetic]].

### Promises run later

Code after a call to an `async` function runs before the code after its first `await` (see [`order`](#order)). Use `await`, or `then`, when the next step needs the result.[^1]

## Comparison

MDN compares JavaScript with Python and Java at several points of its tour.[^1] [^3] A cell says not compared where the source makes no comparison for that row.

| Topic | JavaScript | Python | Java |
|---|---|---|---|
| typing | dynamic | not compared | static |
| object model | prototypes, with `class` syntax | not compared | classes |
| key-value data | an object | a dictionary | a `HashMap` |
| embedded expressions in text | template literals in backticks | f-strings | not compared |
| extra arguments | a rest parameter, `...args` | `*args` | not compared |
| chained conditions | `else if` | `elif` | not compared |
| comments | `//` and `/* */` | `#` | not compared |

For the reasons JavaScript looks like Java in its syntax and differs in its object model, see [[javascript/what-is-javascript]].

## References
