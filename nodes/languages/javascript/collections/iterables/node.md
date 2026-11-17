---
tag: HS2492
kind: feature
owner: javascript
slug: iterables
name: Iterables
aliases: [iterable protocol, Symbol.iterator, built-in iterables, iterable object, making an object iterable, syntaxes expecting iterables]
area: languages/javascript/collections
level: core
summary: >-
  An iterable has a [Symbol.iterator]() method that returns an iterator. for...of, spread, destructuring, Array.from, new Map and new Set all take iterables, and plain objects are not iterable.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html#sec-iterable-interface", locator: "27.1.1.1 The Iterable Interface"}
see_also: [javascript/iterators, javascript/iterators-and-generators, javascript/loops-and-iteration, iterators]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Iteration_protocols
  title: MDN Web Docs, JavaScript Reference, Iteration protocols
  locator: The iterable protocol; Built-in iterables; Built-in APIs accepting iterables; Syntaxes expecting iterables; Non-well-formed iterables
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 27
  locator: 27.1.1.1 The Iterable Interface
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Iterators_and_generators
  title: MDN Web Docs, JavaScript Guide, Iterators and generators
  locator: Iterables; User-defined iterables; Built-in iterables
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...of
  title: MDN Web Docs, JavaScript Reference, for...of
  locator: Iterating over a string
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/tutorial/classes.html
  title: The Python Tutorial, Python 3.14, Classes
  locator: 9.8 Iterators
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 14
  locator: 14.14.2 The enhanced for statement
  accessed: 2026-11-18
- url: https://learn.microsoft.com/en-us/dotnet/csharp/iterators
  title: Microsoft Learn, C# guide, Iterators
  locator: the introduction
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=uses
const word = "héllo";
const tags = new Set(["a", "b"]);

console.log([...word], Array.from(tags));
const [first, ...rest] = tags;
console.log(first, rest);
console.log(new Map([["k", 1]].values()).get("k"), Math.max(...[3, 9, 4]));
console.log(typeof [][Symbol.iterator], typeof {}[Symbol.iterator]);
```

```output for=uses
[ 'h', 'é', 'l', 'l', 'o' ] [ 'a', 'b' ]
a [ 'b' ]
1 9
function undefined
```

| Takes an iterable | Example |
|---|---|
| `for...of` | `for (const x of it)` |
| spread in arrays and calls | `[...it]`, `f(...it)` |
| array destructuring | `const [a, b] = it` |
| `yield*` | `yield* it` |
| constructors and functions | `new Map(it)`, `new Set(it)`, `Array.from(it)`, `Promise.all(it)`, `Object.groupBy(it, fn)` |

## Semantics

**r1.** "The iterable protocol allows JavaScript objects to define or customize their iteration behavior, such as what values are looped over in a for...of construct."[^1] "In order to be iterable, an object must implement the [Symbol.iterator]() method, meaning that the object (or one of the objects up its prototype chain) must have a property with a [Symbol.iterator] key".[^1] The specification's table gives the requirement for `%Symbol.iterator%`: "a function that returns an iterator object".[^2]

**r2.** "String , Array , TypedArray , Map , Set , and Segments (returned by Intl.Segmenter.prototype.segment() ) are all built-in iterables, because each of their prototype objects implements a [Symbol.iterator]() method."[^1] A plain object is not: built-in types "such as Array or Map , have a default iteration behavior, while other types (such as Object ) do not."[^3] "Strings are iterated by Unicode code points."[^4] Example: [`uses`](#uses).

**r3.** The language takes iterables in many places: "for...of loops, array and parameter spreading, yield* , and array destructuring".[^1] Among the functions, MDN lists `Map()`, `WeakMap()`, `Set()`, `WeakSet()`, `Promise.all()`, `Array.from()` and `Object.groupBy()`.[^1]

**r4.** The method is called once per use: "Whenever an object needs to be iterated (such as at the beginning of a for...of loop), its [Symbol.iterator]() method is called with no arguments, and the returned iterator is used to obtain the values to be iterated."[^1] It is called as a method, so "the this keyword can be used to access the properties of the iterable object".[^1] It may be a generator function.[^1] Example: [`custom`](#custom).

**r5.** An iterable may run once or many times. "Iterables which can iterate only once (such as Generators) customarily return this from their [Symbol.iterator]() method, whereas iterables which can be iterated many times must return a new iterator on each invocation".[^3] MDN prefers the second kind: "it's better for iterable[Symbol.iterator]() to return different iterators that always start from the beginning, like Set.prototype[Symbol.iterator]() does."[^1]

**r6.** Destructuring and `break` stop early, and then the language calls `return` on the iterator, "if all identifiers are already bound in an array destructuring".[^1] A broken iterable throws: it must have "a callable [Symbol.iterator]() method", that method must return an object, and the object must have "a callable next() method"; if not, "a TypeError is thrown."[^1]

## Examples

A class whose instances can be looped over many times (rules r4 and r5).

```javascript run id=custom
class Range {
  constructor(start, end) {
    this.start = start;
    this.end = end;
  }
  *[Symbol.iterator]() {
    for (let i = this.start; i < this.end; i++) yield i;
  }
}

const r = new Range(1, 4);
console.log([...r], [...r]);
for (const n of r) if (n === 2) console.log("found", n);
const [a, b] = r;
console.log(a + b, Array.from(r, (n) => n * n));
```

```output for=custom
[ 1, 2, 3 ] [ 1, 2, 3 ]
found 2
3 [ 1, 4, 9 ]
```

Each spread calls `[Symbol.iterator]()` again, and the generator method starts from `this.start` each time.

Iterating a plain object through its entries (rule r2).

```javascript run id=object
const prices = { tea: 3, cake: 5 };
try {
  for (const p of prices) console.log(p);
} catch (e) {
  console.log(e.name);
}
for (const [item, price] of Object.entries(prices)) console.log(item, price);
console.log(new Map(Object.entries(prices)).size);
```

```output for=object
TypeError
tea 3
cake 5
2
```

## Pitfalls

### A one-shot iterable passed twice

A generator object returns itself from `[Symbol.iterator]()`, so the second use gets nothing (rule r5).

```javascript wrong id=one-shot
function* ids() {
  yield 1;
  yield 2;
}
const source = ids();
const asSet = new Set(source);
const asArray = Array.from(source);
console.log(asSet.size, asArray);

const again = { [Symbol.iterator]: ids };
console.log(new Set(again).size, Array.from(again));
```

```output for=one-shot
2 []
2 [ 1, 2 ]
```

An object whose `[Symbol.iterator]` is the generator function gives a fresh generator each time.

### A `[Symbol.iterator]` that is not a method returning an iterator

The language checks each step and throws a TypeError (rule r6).

```javascript wrong id=bad
for (const value of [1, () => 1, () => ({})]) {
  try {
    [...{ [Symbol.iterator]: value }];
  } catch (e) {
    console.log(typeof value, e.name);
  }
}
```

```output for=bad
number TypeError
function TypeError
function TypeError
```

## Comparison

Python works the same way with special methods: "Define an __iter__() method which returns an object with a __next__() method. If the class defines __next__() , then __iter__() can just return self".[^5] JavaScript uses a symbol, `Symbol.iterator`, in place of a reserved name (rule r1). Java's enhanced `for` needs an array or "a subtype of the raw type Iterable", which a class declares in its type.[^6] C# iterator methods "return IEnumerable<T>, whose values callers usually consume with a foreach statement."[^7] In JavaScript no type declaration is needed: any object with the method is iterable. See [[iterators]].

## References
