---
tag: 0NBE6N
kind: feature
owner: javascript
slug: iterators
name: Iterators
aliases: [iterator protocol, next method, IteratorResult, Iterator class, Iterator.from, iterator return method, proper iterator]
area: languages/javascript/collections
level: core
summary: >-
  An iterator is an object whose next method returns { value, done }. Built-in iterators inherit from Iterator.prototype, so they are also iterable, and for...of calls return() when it exits early.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html#sec-iterator-interface", locator: "27.1.1.2 The Iterator Interface; 27.1.1.5 The IteratorResult Interface; 27.1.3 Iterator Objects"}
see_also: [javascript/iterators-and-generators, javascript/iterables, iterators, iterator-pattern]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Iteration_protocols
  title: MDN Web Docs, JavaScript Reference, Iteration protocols
  locator: The iterator protocol
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 27
  locator: 27.1.1.2 The Iterator Interface; 27.1.1.5 The IteratorResult Interface
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...of
  title: MDN Web Docs, JavaScript Reference, for...of
  locator: Description; Early exiting
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator
  title: MDN Web Docs, JavaScript Reference, Iterator
  locator: the introduction; Description; Iterator helper objects; Proper iterators
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Iterators_and_generators
  title: MDN Web Docs, JavaScript Guide, Iterators and generators
  locator: Iterators
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/tutorial/classes.html
  title: The Python Tutorial, Python 3.14, Classes
  locator: 9.8 Iterators
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Iterator.html
  title: Java SE 25 API, Interface Iterator
  locator: hasNext; next
  accessed: 2026-11-18
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: The Rust Standard Library, Trait Iterator
  locator: next
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=protocol
const it = ["a", "b"][Symbol.iterator]();
console.log(it.next(), it.next(), it.next());

let n = 0;
const countdown = {
  next() {
    n++;
    return n <= 2 ? { value: 3 - n, done: false } : { value: "end", done: true };
  },
  [Symbol.iterator]() {
    return this;
  },
};
console.log([...countdown]);
console.log(countdown.next());
```

```output for=protocol
{ value: 'a', done: false } { value: 'b', done: false } { value: undefined, done: true }
[ 2, 1 ]
{ value: 'end', done: true }
```

| Member | Required | What it does |
|---|---|---|
| `next(v?)` | yes | returns `{ value, done }` |
| `return(v?)` | no | the caller has stopped early; clean up and return `{ done: true }` |
| `throw(e?)` | no | the caller reports an error |
| `[Symbol.iterator]()` returning `this` | no, but expected | makes the iterator iterable too |

## Semantics

**r1.** "An object is an iterator when it implements a next() method" that "accepts zero or one argument and returns an object conforming to the IteratorResult interface".[^1] In that object, `done` is "false if the iterator was able to produce the next value in the sequence" and "true if the iterator has completed its sequence. In this case, value optionally specifies the return value of the iterator."[^1] The specification lists `"done"` as "the result status of an iterator next method call."[^2]

**r2.** The return value is not one of the values. When `done` is true, `for...of` and spread stop and drop `value`: "A for...of loop exits when the iterator has completed (the next() result is an object with done: true )."[^3] In the [`protocol`](#protocol) example, `"end"` is not in the array.

**r3.** An iterator should stay finished: "If an iterator returns a result with done: true , any subsequent calls to next() are expected to return done: true as well, although this is not enforced on the language level."[^1] If `next` returns something that is not an object while a language feature uses it, "a TypeError ("iterator.next() returned a non-object value" ) will be thrown."[^1]

**r4.** `return` and `throw` are optional. Calling `return` "tells the iterator that the caller does not intend to make any more next() calls and can perform any cleanup actions."[^1] `for...of` uses it: "If the for...of loop exited early (e.g., a break statement is encountered or an error is thrown), the return() method of the iterator is called to perform any cleanup."[^3] "No built-in language feature calls throw() for cleanup purposes".[^1] Example: [`cleanup`](#cleanup).

**r5.** "All built-in iterators inherit from the Iterator class. The Iterator class provides a [Symbol.iterator]() method that returns the iterator object itself, making the iterator also iterable."[^4] A "proper iterator is one that both conforms to the iterator protocol and inherits from Iterator ". To make one, "define a class that extends Iterator , or use the Iterator.from() method."[^4] Proper iterators get the helper methods such as `map` and `take`. Example: [`proper`](#proper).

**r6.** "Iterating over an iterator is said to consume the iterator, because it is generally only possible to do once."[^5] A helper "shares the same data source as the underlying iterator", and "There is no way to "fork" an iterator to allow it to be iterated multiple times."[^4] Example: [`shared`](#shared).

## Examples

`return` runs when a loop stops early (rule r4).

```javascript run id=cleanup
function openLines(lines) {
  let i = 0;
  return {
    next() {
      return i < lines.length ? { value: lines[i++], done: false } : { value: undefined, done: true };
    },
    return() {
      console.log("closed after", i);
      return { value: undefined, done: true };
    },
    [Symbol.iterator]() {
      return this;
    },
  };
}

for (const line of openLines(["one", "two", "three"])) {
  if (line === "two") break;
  console.log(line);
}
```

```output for=cleanup
one
closed after 2
```

A class that extends `Iterator`, and `Iterator.from` (rule r5).

```javascript run id=proper
class Countdown extends Iterator {
  #n;
  constructor(n) {
    super();
    this.#n = n;
  }
  next() {
    return this.#n > 0 ? { value: this.#n--, done: false } : { value: undefined, done: true };
  }
}
console.log([...new Countdown(3).map((x) => x * 10)]);

let k = 0;
const plain = { next: () => ({ value: k++, done: k > 3 }) };
console.log(typeof plain.take, [...Iterator.from(plain).take(2)]);
```

```output for=proper
[ 30, 20, 10 ]
undefined [ 0, 1 ]
```

`plain` follows the protocol but has no helpers. `Iterator.from` wraps it in an object that inherits from `Iterator.prototype`.

Two names for one source (rule r6).

```javascript run id=shared
const it = [1, 2, 3, 4].values();
const doubled = it.map((x) => x * 2);
console.log(it.next().value, doubled.next().value, it.next().value);
console.log([...doubled]);
```

```output for=shared
1 4 3
[ 8 ]
```

## Pitfalls

### Expecting the return value in a loop

A value that comes with `done: true` is dropped by `for...of` and spread (rule r2).

```javascript wrong id=lost-return
function* job() {
  yield "step";
  return "result";
}
console.log([...job()]);

const g = job();
let r = g.next();
while (!r.done) r = g.next();
console.log(r.value);
```

```output for=lost-return
[ 'step' ]
result
```

Call `next` yourself if you need the return value.

### An iterator without `[Symbol.iterator]`

An object with only `next` is an iterator but not an iterable, so `for...of` and spread reject it (rule r5).

```javascript wrong id=not-iterable
let i = 0;
const bare = { next: () => ({ value: i, done: i++ >= 2 }) };
try {
  console.log([...bare]);
} catch (e) {
  console.log(e.name);
}
console.log([...Iterator.from(bare)]);
```

```output for=not-iterable
TypeError
[ 0, 1 ]
```

## Comparison

Python's iterator object "defines the method __next__() which accesses elements in the container one at a time. When there are no more elements, __next__() raises a StopIteration exception".[^6] JavaScript returns `done: true` instead of raising (rule r1). Java splits the test from the step: `hasNext` "Returns true if the iteration has more elements", and `next` "Returns the next element in the iteration."[^7] Rust's `next` "Advances the iterator and returns the next value. Returns None when iteration is finished."[^8] See [[iterators]] and [[iterator-pattern]].

## References
