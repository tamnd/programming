---
tag: S84VJG
kind: feature
owner: javascript
slug: what-is-javascript
name: What is JavaScript?
aliases: [JavaScript, JS, ECMAScript language overview]
area: languages/javascript/overview
level: intro
summary: >-
  JavaScript is a dynamic, prototype-based language with first-class functions. It runs inside a host, such as a browser or Node.js, which gives it input and output.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/overview.html#sec-overview", locator: "4 Overview; 4.3 ECMAScript Overview; 4.3.1 Objects"}
see_also: [javascript, javascript/javascript-and-the-ecmascript-specification, javascript/history-of-javascript, prototype-based-programming, multi-paradigm-programming, static-and-dynamic-typing, garbage-collection, just-in-time-compilation]
realizes: [prototype-based-programming, multi-paradigm-programming]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/overview.html#sec-overview
    title: ECMAScript 2026 Language Specification
    locator: 4 Overview; 4.1 Web Scripting; 4.3 ECMAScript Overview; 4.3.1 Objects
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript
    title: JavaScript, MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Language_overview
    title: JavaScript language overview, MDN
    locator: Data types; Numbers; Variables; Operators; Asynchronous programming; Language and runtime
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/JavaScript_technologies_overview
    title: JavaScript technologies overview, MDN
    locator: JavaScript implementations
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: What is JavaScript?; JavaScript and Java
    accessed: 2026-11-18
  - url: https://www.lua.org/manual/5.4/manual.html
    title: Lua 5.4 Reference Manual
    locator: 1 Introduction
    accessed: 2026-11-18
---

## Synopsis

ECMA-262 describes the language in one sentence: "ECMAScript is an object-oriented programming language for performing computations and manipulating computational objects within a host environment."[^1] MDN calls JavaScript "a prototype-based , garbage-collected , dynamic language, supporting multiple paradigms such as imperative, functional, and object-oriented".[^2]

| Trait | What it means in JavaScript | Rule |
|---|---|---|
| dynamic typing | a variable has no type; a value has one | r3 |
| seven primitive types | Undefined, Null, Boolean, Number, BigInt, String, Symbol; all else is an Object | r4 |
| prototype-based | an object inherits from another object, through a prototype chain | r5, r6 |
| first-class functions | a function is an object that can be passed and returned | r7 |
| hosted | input, output, timers and the event loop come from the host | r1, r8 |

## Semantics

**r1.** The language does no input or output by itself. ECMA-262 says that "there are no provisions in this specification for input of external data or output of computed results." The host environment provides those objects.[^1] MDN puts it this way: without APIs of the runtime, "most notably console.log() ", "a JavaScript program's behavior is entirely unobservable."[^3]

**r2.** MDN describes JavaScript as "a lightweight interpreted (or just-in-time compiled ) programming language with first-class functions" ([[just-in-time-compilation]]).[^2] Whether the code is interpreted or compiled is a choice of the engine, not of the language.

**r3.** Types belong to values. In MDN's words, types "are only associated with values, but not with variables", so a `let` variable can hold a number and then a string.[^3] ECMA-262 says the syntax is relaxed: "a variable is not required to have its type declared nor are types associated with properties" ([[static-and-dynamic-typing]]).[^1] Example: [`dynamic`](#dynamic).

**r4.** A value is either a primitive or an object. The primitive types are Undefined, Null, Boolean, Number, BigInt, String and Symbol. An object is a member of the type Object, and "a function is a callable object."[^1] The Number type is "an IEEE 754 64-bit double-precision floating point value", so `3 / 2` is `1.5`.[^3] Example: [`dynamic`](#dynamic).

**r5.** Objects are not built from classes. "Even though ECMAScript includes syntax for class definitions, ECMAScript objects are not fundamentally class-based such as those in C++, Smalltalk, or Java." Each object has a prototype, which may have its own prototype, and so on; this is the prototype chain. A property lookup checks the object first and then each prototype in turn ([[prototype-based-programming]]).[^1] Example: [`prototype`](#prototype).

**r6.** "Unlike most class-based object languages, properties can be added to objects dynamically by assigning values to them." The `class` syntax that ECMAScript 2015 added builds the same constructor and prototype pattern that the built-in objects use.[^1] Example: [`prototype`](#prototype).

**r7.** Functions are values. MDN says JavaScript "supports object-oriented programming with object prototypes and classes", and functional programming "since functions are first-class objects" ([[multi-paradigm-programming]]).[^3] Example: [`paradigms`](#paradigms).

**r8.** "JavaScript is single-threaded by nature." Asynchronous code runs on an event loop, with callbacks, promises or `async` and `await` ([[event-driven-programming]]).[^3] Memory is managed by a garbage collector ([[garbage-collection]]).[^2]

**r9.** The language was first meant for web pages. In a browser, "there is no need for a main program": the code reacts to events from the user.[^1] It also runs outside browsers, "such as Node.js , Apache CouchDB and Adobe Acrobat".[^2] The engines of the main browsers are V8 (Chrome), SpiderMonkey (Firefox) and JavaScriptCore (Safari); Node.js and Deno use V8, and Bun uses JavaScriptCore.[^4]

**r10.** The syntax looks like Java on purpose: "ECMAScript syntax intentionally resembles Java syntax."[^1] The type system and the object model are not Java's. MDN says JavaScript "does not have Java's static typing and strong type checking", and that it "has a prototype-based object model instead of the more common class-based object model."[^5]

## Examples

Dynamic typing and dynamic objects (rules r3, r4 and r6). The variable changes type, the object gains a property and loses one, and the numbers are floating point.

```javascript run id=dynamic
let value = 42;
console.log(typeof value);
value = "forty-two";
console.log(typeof value);

const point = { x: 1 };
point.y = 2;
delete point.x;
console.log(point);

console.log(3 / 2, 7 % 2, 2 ** 10, 0.1 + 0.2);
```

```output for=dynamic
number
string
{ y: 2 }
1.5 1 1024 0.30000000000000004
```

Inheritance from an object, then the same with `class` (rules r5 and r6). `dog` has no `describe` property of its own; the lookup finds it on the prototype `animal`.

```javascript run id=prototype
const animal = {
  describe() {
    return `${this.name} says ${this.sound}`;
  },
};

const dog = Object.create(animal);
dog.name = "Rex";
dog.sound = "woof";

console.log(dog.describe());
console.log(Object.getPrototypeOf(dog) === animal);
console.log(Object.hasOwn(dog, "describe"), "describe" in dog);

class Cat {
  constructor(name) {
    this.name = name;
  }
  describe() {
    return `${this.name} says meow`;
  }
}
const tom = new Cat("Tom");
console.log(tom.describe(), Object.getPrototypeOf(tom) === Cat.prototype);
```

```output for=prototype
Rex says woof
true
false true
Tom says meow true
```

Three styles in one program (rule r7): a loop that updates a variable, functions passed to `filter` and `map`, and a closure that keeps its own count ([[closures]], [[higher-order-functions]]).

```javascript run id=paradigms
const scores = [72, 95, 88, 61];

// imperative: a loop that updates a variable
let best = -Infinity;
for (const s of scores) {
  if (s > best) best = s;
}

// functional: functions passed as values
const passed = scores.filter((s) => s >= 70).map((s) => `${s}%`);

// a closure keeps its own state
function counter() {
  let n = 0;
  return () => ++n;
}
const next = counter();
next();

console.log(best, passed, next());
```

```output for=paradigms
95 [ '72%', '95%', '88%' ] 2
```

## Pitfalls

### Type errors come at run time

No compiler checks the types before the program runs (rule r3). A missing property is `undefined`, and the error comes only when the code calls a method on it. Here the first line runs and prints `ADA`, and the second throws a `TypeError`.

```javascript fails id=not-a-function expect=exception:TypeError
const user = { name: "Ada" };
console.log(user.name.toUpperCase());
console.log(user.email.toUpperCase());
```

Test for the value first, or use optional chaining, `user.email?.toUpperCase()`, which gives `undefined` instead of an error.

### `+` joins strings, and `==` converts

"If you add a string to a number (or other value) everything is converted into a string first." The `==` operator also converts its operands, and `===` does not, so MDN says `===` "is usually preferred".[^3]

```javascript run id=coercion
console.log("3" + 4 + 5, 3 + 4 + "5", "5" - 1);
console.log(123 == "123", 123 === "123");
console.log(Number("3") + 4);
```

```output for=coercion
345 75 4
true false
7
```

### JavaScript is not Java

The two names are alike and the syntax is alike (rule r10), but MDN lists the differences: in JavaScript "Variable data types are not declared (dynamic typing, loosely typed)", while in Java "Variable data types must be declared (static typing, strongly typed)".[^5] Code that looks like Java can still mean something else, as `"5" - 1` above shows.

## Comparison

**Java** is class-based. MDN's table says that in Java "Objects are divided into classes and instances with all inheritance through the class hierarchy", and "Classes and instances cannot have properties or methods added dynamically."[^5] In JavaScript an object inherits from another object, and a property can be added at any time (rule r6).

**Lua** is also a hosted language. Its manual says: "As an extension language, Lua has no notion of a \"main\" program: it works embedded in a host client".[^6] JavaScript has the same split between the language and the host (rule r1), and in a browser it also has no main program (rule r9).

## References
