---
tag: PE47AY
kind: feature
owner: javascript
slug: iterators-and-generators
name: Iterators and generators
aliases: [JavaScript iterators, JavaScript generators, iteration protocols, function*, yield, lazy sequence in JavaScript]
area: languages/javascript/collections
level: core
summary: >-
  An iterator has a next method that returns { value, done }. An iterable has a Symbol.iterator method that returns one. A generator function written with function* builds both with yield.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html#sec-iteration", locator: "27.1 Iteration; 27.5 Generator Objects"}
see_also: [javascript/loops-and-iteration, iterators, lazy-sequence, iterator-pattern, python/generators]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Iterators_and_generators
  title: MDN Web Docs, JavaScript Guide, Iterators and generators
  locator: the introduction; Iterators; Generator functions; Iterables; Advanced generators
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Iteration_protocols
  title: MDN Web Docs, JavaScript Reference, Iteration protocols
  locator: the introduction; The iterator protocol
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 27
  locator: 27.1.1 Common Iteration Interfaces; 27.5 Generator Objects
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/tutorial/classes.html
  title: The Python Tutorial, Python 3.14, Classes
  locator: 9.8 Iterators; 9.9 Generators
  accessed: 2026-11-18
- url: https://learn.microsoft.com/en-us/dotnet/csharp/iterators
  title: Microsoft Learn, C# guide, Iterators
  locator: Produce elements with yield return; Understand deferred element production
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 14
  locator: 14.14.2 The enhanced for statement
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=both
function makeRangeIterator(start, end) {
  let next = start;
  return {
    next() {
      return next < end ? { value: next++, done: false } : { value: undefined, done: true };
    },
  };
}
const it = makeRangeIterator(1, 3);
console.log(it.next(), it.next(), it.next());

function* range(start, end) {
  for (let i = start; i < end; i++) yield i;
}
console.log([...range(1, 4)]);
for (const n of range(5, 7)) console.log(n);
```

```output for=both
{ value: 1, done: false } { value: 2, done: false } { value: undefined, done: true }
[ 1, 2, 3 ]
5
6
```

| Term | What it is | Made by |
|---|---|---|
| iterator | an object with `next()` that returns `{ value, done }` | an object literal, a class, a generator |
| iterable | an object with a `[Symbol.iterator]()` method that returns an iterator | arrays, strings, maps, sets, your own objects |
| generator function | `function*`, which returns a generator when called | `function* name() { ... yield v; ... }` |
| generator | an iterator that is also iterable | calling a generator function |

## Semantics

**r1.** "Iterators and Generators bring the concept of iteration directly into the core language and provide a mechanism for customizing the behavior of for...of loops."[^1] "There are two protocols: The iterable protocol and the iterator protocol."[^2] The specification names them as interfaces: "27.1.1.1 The Iterable Interface" and "27.1.1.2 The Iterator Interface".[^3]

**r2.** An iterator "is any object which implements the Iterator protocol by having a next() method that returns an object with two properties": `value`, and `done`, which "is true if the last value in the sequence has already been consumed."[^1] "Iterating over an iterator is said to consume the iterator, because it is generally only possible to do once."[^1] See [[javascript/iterators]].

**r3.** An iterator produces values on demand: "Arrays must be allocated in their entirety, but iterators are consumed only as necessary. Because of this, iterators can express sequences of unlimited size".[^1] Example: [`infinite`](#infinite).

**r4.** "In order to be iterable, an object must implement the [Symbol.iterator]() method."[^1] "String , Array , TypedArray , Map and Set are all built-in iterables", and the syntaxes that take iterables include "the for...of loops, spread syntax, yield* , and destructuring syntax."[^1] See [[javascript/iterables]].

**r5.** A hand-written iterator must keep its own state, and "their creation requires careful programming due to the need to explicitly maintain their internal state."[^1] "Generator functions are written using the function* syntax. When called, generator functions do not initially execute their code. Instead, they return a special type of iterator, called a Generator."[^1] The body runs "until it encounters the yield keyword."[^1] Example: [`both`](#both).

**r6.** "A Generator is created by calling a generator function and conforms to both the iterator interface and the iterable interface."[^3] So a generator can go straight into `for...of` or spread. It returns itself from `[Symbol.iterator]()`, so it runs only once. Example: [`once`](#once).

**r7.** A generator can also take input. "A value passed to next() will be received by yield ", and "A value passed to the first invocation of next() is always ignored."[^1] Generators also have `throw()` and a `return()` method "that returns the given value and finishes the generator itself."[^1] Example: [`two-way`](#two-way).

## Examples

An endless sequence, read a few values at a time (rule r3).

```javascript run id=infinite
function* naturals() {
  let n = 0;
  while (true) yield n++;
}

function* take(iterable, count) {
  if (count <= 0) return;
  for (const value of iterable) {
    yield value;
    if (--count === 0) return;
  }
}

console.log([...take(naturals(), 5)]);
```

```output for=infinite
[ 0, 1, 2, 3, 4 ]
```

`naturals` never ends, but `take` asks for only 5 values, and the loop inside `take` stops the generator when it returns.

A generator runs once, and an iterable object can run many times (rule r6).

```javascript run id=once
function* range(start, end) {
  for (let i = start; i < end; i++) yield i;
}
const gen = range(1, 3);
console.log([...gen], [...gen], gen[Symbol.iterator]() === gen);

const pair = {
  *[Symbol.iterator]() {
    yield "a";
    yield "b";
  },
};
console.log([...pair], [...pair]);
```

```output for=once
[ 1, 2 ] [] true
[ 'a', 'b' ] [ 'a', 'b' ]
```

Values going into a generator (rule r7).

```javascript run id=two-way
function* counter() {
  let count = 0;
  while (true) {
    const reset = yield count++;
    if (reset) count = 0;
  }
}
const c = counter();
console.log(c.next().value, c.next().value, c.next().value);
console.log(c.next(true).value, c.next().value);
console.log(c.return(99), c.next());
```

```output for=two-way
0 1 2
0 1
{ value: 99, done: true } { value: undefined, done: true }
```

## Pitfalls

### Reading a generator twice

A generator object is used up after one pass (rule r6). Store the values in an array if you need them again, or call the generator function again.

```javascript wrong id=twice
function* letters() {
  yield "x";
  yield "y";
}
const it = letters();
const count = [...it].length;
console.log(count, [...it]);

const values = [...letters()];
console.log(values.length, values);
```

```output for=twice
2 []
2 [ 'x', 'y' ]
```

### Expecting the body to run at the call

A call to a generator function runs no code until the first `next()` (rule r5).

```javascript wrong id=lazy
const log = [];
function* steps() {
  log.push("started");
  yield 1;
}
const g = steps();
console.log(log);
g.next();
console.log(log);
```

```output for=lazy
[]
[ 'started' ]
```

Code that checks its arguments in a generator function throws at the first `next()`, not at the call.

## Comparison

Python uses the same two-step model: "Behind the scenes, the for statement calls iter() on the container object. The function returns an iterator object that defines the method __next__() which accesses elements in the container one at a time."[^4] Its generators "are written like regular functions but use the yield statement whenever they want to return data."[^4] Python signals the end with an exception, and JavaScript with `done: true` (rule r2). In C#, "An iterator method produces a sequence of values one at a time", with `yield return`, and "Calling an iterator method doesn't run its body immediately."[^5] Java has no `yield` for iterators, and its enhanced `for` takes "an array type (§10.1) or a subtype of the raw type Iterable".[^6] See [[iterators]] and [[lazy-sequence]].

## References
