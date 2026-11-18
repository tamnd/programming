---
tag: YXG70D
kind: feature
owner: javascript
slug: static-initialization-blocks
name: Static initialization blocks
aliases: [static blocks, static initializer, class static block, static block in a class body]
area: languages/javascript/user-types
level: advanced
summary: >-
  A static block, written static { ... } in a JavaScript class body, runs once when the class is defined, with this set to the class. It can use statements, try...catch and the class's private names.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-runtime-semantics-classstaticblockdefinitionevaluation", locator: "15.7.11 ClassStaticBlockDefinitionEvaluation; 15.7.14 ClassDefinitionEvaluation"}
see_also: [javascript/static-properties, javascript/private-fields, javascript/classes]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Static_initialization_blocks
  title: MDN Web Docs, JavaScript Reference, Static initialization blocks
  locator: the introduction; Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.7.11 ClassStaticBlockDefinitionEvaluation; 15.7.14 ClassDefinitionEvaluation
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Class Static Block, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.7 Static Initializers
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Config {
  static defaults = { retries: 3 };
  static retries;
  static timeout;
  static {
    const { retries } = this.defaults;
    this.retries = retries;
    this.timeout = retries * 1000;
  }
}
console.log(Config.retries, Config.timeout);
```

```output for=basics
3 3000
```

| Question | Answer |
|---|---|
| when it runs | once, while the class definition is evaluated |
| `this` | the class |
| order | with the static field initializers, top to bottom |
| `var`, `let`, `function` inside | local to the block |
| `await`, `yield`, `arguments`, `super()` | not allowed |
| private names of the class | usable |

## Semantics

**r1.** Static blocks "contain statements to be evaluated during class initialization. This permits more flexible initialization logic than static properties, such as using try...catch or setting multiple fields from a single value."[^1] ECMA-262 turns the block body into a function, "Let bodyFunction be OrdinaryFunctionCreate(%Function.prototype%, sourceText, formalParameters, ClassStaticBlockBody, non-lexical-this, lex, privateEnv).", and calls it with the class as `this`: "Let result be Completion(Call(elementRecord.[[BodyFunction]], F))."[^2] Example: [`basics`](#basics), [`fallback`](#fallback).

**r2.** "A class can have any number of static {} initialization blocks in its class body. These are evaluated, along with any interleaved static field initializers, in the order they are declared. Any static initialization of a super class is performed first, before that of its sub classes."[^1] "super.property can be used to access static properties of the super class."[^1] Example: [`order`](#order).

**r3.** "The scope of the variables declared inside the static block is local to the block. This includes var , function , const , and let declarations. var declarations will not be hoisted out of the static block."[^1] Example: [`scope`](#scope).

**r4.** "The scope of the static block is nested within the lexical scope of the class body, and can access private names declared within the class without causing a syntax error."[^1] So a block can hand a function that reads private state to code outside the class. Example: [`friend`](#friend).

**r5.** Some syntax is refused: "it is a syntax error to call super() in a class static initialization block, or to use the arguments object."[^1] "You cannot use await or yield in this block."[^1] Example: [`syntax`](#syntax).

## Examples

`try...catch` during class setup (rule r1).

```javascript run id=fallback
class Settings {
  static parsed;
  static {
    try {
      this.parsed = JSON.parse("{bad json");
    } catch (e) {
      this.parsed = {};
      console.log("fallback after", e.name);
    }
  }
}
console.log(Settings.parsed);
```

```output for=fallback
fallback after SyntaxError
{}
```

Evaluation order (rule r2).

```javascript run id=order
class A {
  static label = "from A";
  static first = console.log("A field 1");
  static {
    console.log("A block 1");
  }
  static second = console.log("A field 2");
  static {
    console.log("A block 2");
  }
}
class B extends A {
  static {
    console.log("B block sees", super.label);
  }
}
```

```output for=order
A field 1
A block 1
A field 2
A block 2
B block sees from A
```

Variables in a block (rule r3).

```javascript run id=scope
var y = "outer y";
class C {
  static field = "inner y";
  static {
    console.log(y);
    var y = this.field;
    console.log(y);
  }
}
console.log(y);
```

```output for=scope
undefined
inner y
outer y
```

The inner `var y` is hoisted to the top of the block, not out of it.

Sharing private state with code outside the class (rule r4).

```javascript run id=friend
let readSecret;
class Vault {
  #secret;
  constructor(secret) {
    this.#secret = secret;
  }
  static {
    readSecret = (vault) => vault.#secret;
  }
}
console.log(readSecret(new Vault("gold")));
```

```output for=friend
gold
```

Syntax that a static block refuses (rule r5). Each class is compiled with `eval` so that the error can be caught.

```javascript run id=syntax
const sources = [
  "class A extends Object { static { super(); } }",
  "class B { static { arguments; } }",
  "class C { static { await 1; } }",
];
for (const src of sources) {
  try {
    eval(src);
  } catch (e) {
    console.log(e.name);
  }
}
```

```output for=syntax
SyntaxError
SyntaxError
SyntaxError
```

## Versions

| Feature | Edition |
|---|---|
| static initialization blocks | ECMAScript 2022[^3] |

The TC39 list of finished proposals gives 2022 for Class Static Block.[^3]

## Pitfalls

### Reading a static field declared below the block

```javascript wrong id=below
class Order {
  static {
    console.log("rate is", this.rate);
  }
  static rate = 0.2;
}
console.log(Order.rate);
try {
  class Secret {
    static {
      console.log(this.#rate);
    }
    static #rate = 0.2;
  }
} catch (e) {
  console.log(e.name);
}
```

```output for=below
rate is undefined
0.2
TypeError
```

MDN: "The initialization block can refer to field values above it, but not below it."[^1] A public field below reads as undefined, and a private one throws, because "accessing a non-initialized private field throws a TypeError , even if the private field is declared below."[^1] Put the block after the fields it reads.

## Comparison

Java has the same construct: "A static initializer declared in a class is executed when the class is initialized (§12.4.2)."[^4] But in Java "the keywords this and super are prohibited in a static context".[^4] A JavaScript static block has `this` bound to the class and can use `super.property` (rules r1 and r2).

## References
