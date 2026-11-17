---
tag: WMR7JT
kind: feature
owner: javascript
slug: async-iteration
name: Async iteration
aliases: [async iterator, async iterable, Symbol.asyncIterator, async generator, async function*, for await...of, Array.fromAsync]
area: languages/javascript/collections
level: advanced
summary: >-
  An async iterator's next returns a promise for { value, done }. Async generators (async function*) make them, for await...of reads them, and Array.fromAsync collects them into an array.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html#sec-asynciterable-interface", locator: "27.1.1.3 The Async Iterable Interface; 27.1.1.4 The Async Iterator Interface; 27.6 AsyncGenerator Objects"}
see_also: [javascript/iterators, javascript/iterables, javascript/iterators-and-generators, async-await, python/async-for-and-async-with]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Iteration_protocols
  title: MDN Web Docs, JavaScript Reference, Iteration protocols
  locator: The async iterator and async iterable protocols; Interactions between the language and iteration protocols
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 27
  locator: 27.1.1.4 The Async Iterator Interface; 27.6 AsyncGenerator Objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/AsyncGenerator
  title: MDN Web Docs, JavaScript Reference, AsyncGenerator
  locator: the introduction; Instance methods
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for-await...of
  title: MDN Web Docs, JavaScript Reference, for await...of
  locator: the introduction; Description; Iterating over sync iterables and generators
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/fromAsync
  title: MDN Web Docs, JavaScript Reference, Array.fromAsync()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/readline.html
  title: Node.js v24 documentation, Readline
  locator: rl[Symbol.asyncIterator]()
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/asyncIterator
  title: MDN Web Docs, JavaScript Reference, Symbol.asyncIterator
  locator: the introduction; Built-in async iterables
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Asynchronous Iteration; Array.fromAsync, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/compound_stmts.html
  title: The Python Language Reference, Python 3.14, Compound statements
  locator: 8.9.2 The async for statement
  accessed: 2026-11-18
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Microsoft Learn, C# language reference, Iteration statements
  locator: await foreach
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const wait = (ms, value) => new Promise((resolve) => setTimeout(() => resolve(value), ms));

async function* pages() {
  for (let page = 1; page <= 3; page++) {
    yield await wait(10, `page ${page}`);
  }
}

async function main() {
  for await (const page of pages()) console.log(page);

  const it = pages();
  const step = it.next();
  console.log(step instanceof Promise, await step);
  console.log(await Array.fromAsync(pages(), (p) => p.toUpperCase()));
}
main();
```

```output for=basics
page 1
page 2
page 3
true { value: 'page 1', done: false }
[ 'PAGE 1', 'PAGE 2', 'PAGE 3' ]
```

| Sync | Async |
|---|---|
| `[Symbol.iterator]()` | `[Symbol.asyncIterator]()` |
| `next()` returns `{ value, done }` | `next()` returns a promise for `{ value, done }` |
| `function*` | `async function*` |
| `for (x of it)` | `for await (x of it)`, only where `await` is allowed |
| `Array.from(it)` | `await Array.fromAsync(it)` |

## Semantics

**r1.** The async protocols "have very similar interfaces compared to the iterable and iterator protocols, except that each return value from the calls to the iterator methods is wrapped in a promise."[^1] The specification asks `next` to be "a function that returns a promise for an IteratorResult object".[^2] An object is async iterable when it has a `[Symbol.asyncIterator]()` method that returns such an iterator.[^1]

**r2.** "The AsyncGenerator object is returned by an async generator function and it conforms to both the async iterable protocol and the async iterator protocol."[^3] Its `next` "Returns a Promise which will be resolved with the given value yielded by the yield expression."[^3] Inside the body, `await` and `yield` can both be used, so each value can wait for an I/O result. Example: [`basics`](#basics).

**r3.** "The for await...of statement creates a loop iterating over async iterable objects as well as sync iterables. This statement can only be used in contexts where await can be used, which includes inside an async function body and in a module."[^4] It looks for `[Symbol.asyncIterator]()` first; if that is missing it uses `[Symbol.iterator]()` and wraps the sync iterator.[^4] If the loop exits early, `return()` is called, and "The returned promise is awaited before the loop exits."[^4]

**r4.** With a sync iterable that gives promises, the two loops differ: "for await...of would produce a sequence of resolved values, while for...of would produce a sequence of promises."[^4] The wrapping has a cost: "Even when the iterable is sync, the loop still awaits the return value for every iteration, leading to slower execution due to repeated promise unwrapping."[^4] Example: [`sync-source`](#sync-source).

**r5.** `Array.fromAsync` collects an async iterable, an iterable or an array-like object into an array, and returns "A new Promise whose fulfillment value is a new Array instance."[^5] It "iterates the async iterable in a fashion very similar to for await...of ".[^5]

**r6.** The core language has few async iterables of its own: "Other than AsyncGenerator , there is no object in the core JavaScript language that is async iterable."[^1] Hosts add them. Node's readline interface has `rl[Symbol.asyncIterator]()`, which creates "an AsyncIterator object that iterates through each line in the input stream as a string."[^6] MDN advises that "async iterables are designed to represent something iterable — like a stream of data or a list —, not to completely replace callbacks and events in most situations."[^7]

## Examples

A hand-written async iterable (rule r1).

```javascript run id=manual
const ticks = {
  [Symbol.asyncIterator]() {
    let i = 0;
    return {
      next: () => Promise.resolve(i < 3 ? { value: i++, done: false } : { value: undefined, done: true }),
      return: () => {
        console.log("closed at", i);
        return Promise.resolve({ value: undefined, done: true });
      },
    };
  },
};

(async () => {
  for await (const t of ticks) {
    console.log("tick", t);
    if (t === 1) break;
  }
})();
```

```output for=manual
tick 0
tick 1
closed at 2
```

A sync array of promises in both loops (rule r4).

```javascript run id=sync-source
const values = [Promise.resolve("a"), "b", Promise.resolve("c")];

(async () => {
  for (const v of values) console.log(typeof v === "string" ? v : "a promise");
  for await (const v of values) console.log(v);
})();
```

```output for=sync-source
a promise
b
a promise
a
b
c
```

## Versions

The TC39 list of finished proposals gives Asynchronous Iteration the expected publication year 2018, so async generators, `Symbol.asyncIterator` and `for await...of` are part of ECMAScript 2018.[^8] It gives `Array.fromAsync` the year 2026, so that method is part of ECMAScript 2026; Node v24.10.0 has it, as the [`basics`](#basics) example shows.[^8]

## Pitfalls

### `for await` in a plain function

Outside an async function and outside a module, `for await` is a syntax error, because the loop "can only be used in contexts where await can be used" (rule r3). The run below checks the source with the `Function` constructor, which compiles a plain function body.

```javascript wrong id=context
const body = "for await (const x of []) {}";
try {
  new Function(body);
} catch (e) {
  console.log(e.name);
}
const AsyncFunction = (async () => {}).constructor;
console.log(typeof new AsyncFunction(body));
```

```output for=context
SyntaxError
function
```

### Spreading an async generator

An async generator has `[Symbol.asyncIterator]` but no `[Symbol.iterator]`, so sync syntax rejects it (rule r1). MDN: such use "will throw a TypeError: x is not iterable."[^1]

```javascript wrong id=spread
async function* letters() {
  yield "x";
  yield "y";
}
try {
  console.log([...letters()]);
} catch (e) {
  console.log(e.name);
}
Array.fromAsync(letters()).then((arr) => console.log(arr));
```

```output for=spread
TypeError
[ 'x', 'y' ]
```

## Comparison

Python has the same pair of protocols: "An asynchronous iterable provides an __aiter__ method that directly returns an asynchronous iterator, which can call asynchronous code in its __anext__ method", and "The async for statement allows convenient iteration over asynchronous iterables."[^9] C# uses `await foreach` "to consume an asynchronous stream of data, that is, a collection type that implements the IAsyncEnumerable<T> interface", and "Each iteration of the loop can suspend while the next element is retrieved asynchronously."[^10] In JavaScript the protocol is a symbol-keyed method, and `for await` also accepts sync iterables (rule r3). See [[async-await]] and [[python/async-for-and-async-with]].

## References
