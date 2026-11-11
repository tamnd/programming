---
tag: JEJJ8S
kind: language
slug: typescript
name: TypeScript
area: languages
level: intro
summary: >-
  TypeScript is a typed superset of JavaScript. A compiler checks its static, structural types and then erases them, so the program runs as JavaScript. The compiler tsc is its reference implementation.
class: programming
tier: 1
paradigms: [imperative, object-oriented, functional]
typing: {discipline: static, strength: weak, checking: [compile-time], inference: local}
memory: garbage-collected
execution: [transpiled]
appeared: 2012
designers: [anders-hejlsberg]
influenced_by: [javascript]
standard: null
reference: {url: "https://www.typescriptlang.org/docs/handbook/", licence: cc-by-4.0}
licence_of_reference_impl: apache-2.0
file_extensions: [ts, mts, cts]
hello_world: examples/typescript.md#hello
same_as: {wikidata: Q978185, wikipedia: TypeScript, pldb: typescript, linguist: TypeScript, rosetta: null, hopl: null}
sources:
  - url: https://github.com/microsoft/TypeScript/blob/main/README.md
    title: TypeScript README, microsoft/TypeScript
    locator: the introduction; Contribute; Code of Conduct
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/typescript-from-scratch.html#a-typed-superset-of-javascript
    title: TypeScript for the New Programmer, TypeScript Handbook
    locator: "TypeScript: A Static Type Checker; A Typed Superset of JavaScript; Runtime Behavior; Erased Types"
    accessed: 2026-11-10
  - url: https://github.com/microsoft/TypeScript/wiki/TypeScript-Design-Goals
    title: TypeScript Design Goals, TypeScript wiki
    locator: Goals; Non-goals
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: Narrowing, TypeScript Handbook
    locator: Discriminated unions; Exhaustiveness checking
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/modules/reference.html#module-format-detection
    title: Modules, Reference, TypeScript Handbook
    locator: Module format detection
    accessed: 2026-11-10
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: TypeScript, extensions; TSX
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/2/type-declarations.html#dts-files
    title: Type Declarations, TypeScript Handbook
    locator: .d.ts files; DefinitelyTyped / @types
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html
    title: Type Compatibility, TypeScript Handbook
    locator: the introduction; A Note on Soundness; Starting out
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/typescript-in-5-minutes-oop.html#erased-structural-types
    title: "TypeScript for Java/C# Programmers, TypeScript Handbook"
    locator: Co-learning JavaScript; Free Functions and Data; OOP in TypeScript; Nominal Reified Type Systems; Erased Structural Types; Reflection
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/type-inference.html
    title: Type Inference, TypeScript Handbook
    locator: the introduction; Best common type; Contextual Typing
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#type-assertions
    title: Everyday Types, TypeScript Handbook
    locator: any; noImplicitAny; Type Assertions; strictNullChecks on
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/2/functions.html#unknown
    title: More on Functions, TypeScript Handbook
    locator: Other Types to Know About, unknown
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/typescript-in-5-minutes-func.html#unions
    title: TypeScript for Functional Programmers, TypeScript Handbook
    locator: Unions; Gradual typing; Structural typing
    accessed: 2026-11-10
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-6-0/
    title: Announcing TypeScript 6.0, TypeScript blog
    locator: the introduction; Simple Default Changes; Preparing for TypeScript 7.0
  - url: https://www.typescriptlang.org/tsconfig/#strict
    title: TSConfig Reference, TypeScript
    locator: strict; erasableSyntaxOnly
    accessed: 2026-11-10
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Memory_management#garbage_collection
    title: Memory management, MDN Web Docs
    locator: the introduction; Garbage collection
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html#tsc-the-typescript-compiler
    title: The Basics, TypeScript Handbook
    locator: tsc, the TypeScript compiler; Emitting with Errors; Erased Types; Downleveling
    accessed: 2026-11-10
  - url: https://nodejs.org/docs/latest-v24.x/api/typescript.html#type-stripping
    title: "Modules: TypeScript, Node.js v24 documentation"
    locator: Type stripping; TypeScript features; History
    accessed: 2026-11-10
  - url: https://docs.deno.com/runtime/fundamentals/typescript/#how-typescript-runs-in-deno
    title: TypeScript support, Deno documentation
    locator: How TypeScript runs in Deno
    accessed: 2026-11-10
  - url: https://bun.com/docs/runtime/file-types#ts
    title: File types, Bun documentation
    locator: ts
    accessed: 2026-11-10
  - url: https://github.com/microsoft/TypeScript/wiki/TypeScript's-Release-Process
    title: TypeScript's Release Process, TypeScript wiki
    locator: How Often Does TypeScript Release?; What Are the Stages of a Release?; What gets done for patch releases?; Tracking Planning and Development
    accessed: 2026-11-10
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-7-0/
    title: Announcing TypeScript 7.0, TypeScript blog
    locator: the introduction; Running Side-by-Side with TypeScript 6.0; The Road Forward
  - url: https://github.com/microsoft/TypeScript/issues/62963
    title: Transition to 6.0 Maintenance Mode, microsoft/TypeScript issue 62963
    locator: PR Merge Criteria for 6.0
    accessed: 2026-11-10
  - url: https://registry.npmjs.org/typescript
    title: typescript, npm registry metadata
    locator: time; dist-tags
    accessed: 2026-11-10
  - url: https://github.com/microsoft/TypeScript/releases/tag/v7.0.2
    title: TypeScript 7.0.2, GitHub releases of microsoft/TypeScript
    locator: the release notes
    accessed: 2026-11-10
  - url: https://github.com/microsoft/TypeScript/issues/63703
    title: TypeScript 7.1 Iteration Plan, microsoft/TypeScript issue 63703
    locator: the release dates
    accessed: 2026-11-10
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-7-1-beta/
    title: Announcing TypeScript 7.1 Beta, TypeScript blog
    locator: What's Next?
  - url: https://devblogs.microsoft.com/typescript/typescript-native-port/
    title: A 10x Faster TypeScript, TypeScript blog
    locator: the introduction; Versioning Roadmap; the author
  - url: https://github.com/microsoft/typescript-go
    title: microsoft/typescript-go README
    locator: This Repo Is Closed
    accessed: 2026-11-10
  - url: https://esbuild.github.io/content-types/#typescript
    title: Content Types, esbuild documentation
    locator: TypeScript; No type system
    accessed: 2026-11-10
  - url: https://nodejs.org/en/blog/release/v26.13.0
    title: Node.js v26.13.0 (LTS), Node.js blog
    locator: the release notes
    accessed: 2026-11-10
  - url: https://github.com/denoland/deno/releases/tag/v2.9.8
    title: Deno release v2.9.8
    locator: the release
    accessed: 2026-11-10
  - url: https://github.com/oven-sh/bun/releases/tag/bun-v1.4.4
    title: Bun release bun-v1.4.4
    locator: the release; LICENSE.md
    accessed: 2026-11-10
  - url: https://github.com/evanw/esbuild/releases/tag/v0.28.2
    title: esbuild release v0.28.2
    locator: the release
    accessed: 2026-11-10
  - url: https://docs.npmjs.com/about-npm
    title: About npm, npm Docs
    locator: the introduction
    accessed: 2026-11-10
  - url: https://www.typescriptlang.org/docs/handbook/tsconfig-json.html#overview
    title: What is a tsconfig.json, TypeScript Handbook
    locator: Overview
    accessed: 2026-11-10
  - url: https://nodejs.org/docs/latest-v24.x/api/test.html
    title: Test runner, Node.js v24 documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://prettier.io/docs/
    title: What is Prettier?, Prettier documentation
    locator: What is Prettier?
    accessed: 2026-11-10
  - url: https://typescript-eslint.io/
    title: typescript-eslint
    locator: the front page
    accessed: 2026-11-10
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-1-0/
    title: Announcing TypeScript 1.0, TypeScript blog
    locator: TypeScript 1.0; Taking Pull Requests
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-2-0/
    title: Announcing TypeScript 2.0, TypeScript blog
    locator: What's New in TypeScript 2.0?, Non-nullable Types
  - url: https://github.com/microsoft/TypeScript/wiki/Writing-Good-Design-Proposals
    title: Writing Good Design Proposals, TypeScript wiki
    locator: Before You Start; Defining a Scenario; Language Feature Checklist
    accessed: 2026-11-10
  - url: https://github.com/microsoft/TypeScript/issues/64399
    title: Design Meeting Notes, 2026-09-22, microsoft/TypeScript issue 64399
    locator: the issue and its label Design Notes
    accessed: 2026-11-10
---

## Overview

TypeScript is "a language for application-scale JavaScript". It adds optional types to JavaScript and compiles to readable, standards-based JavaScript.[^1] The handbook calls it a static type checker, because it finds errors before the program runs.[^2] It is a typed superset of JavaScript, so legal JavaScript syntax is also legal TypeScript syntax.[^2]

The design goals on the project wiki ask TypeScript to statically identify constructs that are likely to be errors. It must impose no runtime overhead, emit clean JavaScript and preserve the runtime behaviour of all JavaScript code.[^3] One goal asks for "a consistent, fully erasable, structural type system". A sound or "provably correct" type system is a non-goal, and so is run-time type information.[^3]

## A first program

```typescript run id=hello
console.log("Hello, world!");
```

```output for=hello
Hello, world!
```

The program below defines a union of two object types and a generic function; see [[parametric-polymorphism]]. The `kind` property tells the members of the union apart, so each `case` narrows `shape` to one member.[^4]

```typescript run id=shapes
type Shape =
  | { kind: "circle"; radius: number }
  | { kind: "rect"; width: number; height: number };

function area(shape: Shape): number {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "rect":
      return shape.width * shape.height;
  }
}

function largest<T>(xs: readonly T[], size: (x: T) => number): T | undefined {
  let top: T | undefined;
  for (const x of xs) {
    if (top === undefined || size(x) > size(top)) top = x;
  }
  return top;
}

const shapes: readonly Shape[] = [
  { kind: "circle", radius: 1 },
  { kind: "rect", width: 2, height: 3 },
];

for (const shape of shapes) {
  console.log(`${shape.kind}: ${area(shape).toFixed(2)}`);
}
console.log(largest(shapes, area)?.kind);
```

```output for=shapes
circle: 3.14
rect: 6.00
rect
```

The compiler accepts the `switch` with no `default` branch, because the two cases cover every member of `Shape`.[^4] The array can be empty, so `largest` returns `T | undefined`, and the last line uses `?.`.

## The language at a glance

A source file has the extension `.ts`. A `.mts` file is always an ECMAScript module, and a `.cts` file is always a CommonJS module.[^5] Linguist lists these three extensions for TypeScript. It lists `.tsx` under TSX, a separate language in the TypeScript group.[^6] A declaration file, `.d.ts`, contains only type information and produces no JavaScript output.[^7]

**Types.** TypeScript checks types at compile time. Then the compiler erases them, so the JavaScript output has no type information.[^2] Type compatibility is based on structural subtyping: a type is compatible with another type when it has at least the same members.[^8] C# and Java use nominal typing instead, where types relate through their declarations.[^9] Where the code has no annotation, the compiler infers types from initializers, defaults, return statements and the context.[^10]

```typescript run id=types
interface Point {
  x: number;
  y: number;
}

class Pixel {
  x: number;
  y: number;
  color: string;

  constructor(x: number, y: number, color: string) {
    this.x = x;
    this.y = y;
    this.color = color;
  }
}

function length(p: Point): number {
  return Math.hypot(p.x, p.y);
}

console.log(length(new Pixel(3, 4, "red")));
console.log(length({ x: 6, y: 8 }));
console.log("1" + 2);

const data: unknown = JSON.parse('{"x": "3", "y": 4}');
const point = data as Point;
console.log(typeof point.x);
```

```output for=types
5
10
12
string
```

`Pixel` never names `Point`, but it has the members `x` and `y`, so `length` accepts it; see [[subtyping]].[^9] The checker rejects some implicit conversions, such as a division of a number by an array.[^2] It accepts `"1" + 2`, and JavaScript converts the number to a string at run time. The compiler removes the type assertion `as Point`, so nothing checks it at run time, and `point.x` stays a string.[^11] This is [[type-erasure]].

The type system is not sound on purpose. It allows some operations that the compiler cannot prove safe.[^8] The type `any` turns off type checking for a value.[^11] The type `unknown` also holds any value, but the checker allows no operation on it until the code narrows it.[^12] Union types are untagged, so a program uses a literal property such as `kind` to tell the members apart; see [[sum-types]].[^13] With `strictNullChecks` on, the code must test for `null` and `undefined` before it uses a value; see [[null-references]].[^11] From TypeScript 6.0, the `strict` option, which turns on this check and others, is on by default.[^14][^15]

**Memory.** TypeScript adds nothing to the memory model. It uses the same runtime as JavaScript.[^9] JavaScript allocates memory when a program creates values, and it frees memory automatically when the values are no longer used, through [[garbage-collection]].[^16]

**Execution.** The compiler `tsc` checks a program and writes JavaScript files. By default it writes the output even when it reports type errors.[^17] It can also rewrite newer syntax for older ECMAScript targets.[^17] Node.js, Deno and Bun run `.ts` files directly. They strip the types and do not check them.[^18][^19][^20] Type stripping works only for erasable syntax. The compiler option `erasableSyntaxOnly` reports the other constructs, such as `enum` and parameter properties, as errors.[^15]

**Paradigms.** TypeScript keeps the paradigms of JavaScript, where free functions that work on plain data are the preferred model.[^9] Classes, interfaces, inheritance and static methods support an object-oriented style.[^9] The handbook for functional programmers covers union types, structural typing and gradual typing.[^13]

## The guide

The guide to TypeScript has the 25 parts of the language template. Each row says what the part covers for TypeScript. Only this page exists so far, so the table has no links.

| Part | What it covers in TypeScript |
|---|---|
| Overview and history | this page: the design goals, the versions, the implementations, the history |
| Getting started | installing `typescript` from npm, `tsc` and `tsconfig.json`, running a `.ts` file with Node.js |
| Lexical structure and syntax | the JavaScript grammar with type annotations, type-only imports, `as` and `satisfies`, the non-null operator `!` |
| Values and types | the JavaScript primitives, literal types, `any`, `unknown`, `never`, `null` and `undefined` under `strictNullChecks` |
| Variables, binding and scope | `let`, `const` and `var`, block scope, `readonly`, `as const`, inference from initializers |
| Control flow | `if`, `switch` and loops, narrowing with `typeof`, `in`, `instanceof` and discriminants, exhaustiveness with `never` |
| Functions | parameter and return types, optional and default parameters, overloads, generic functions, closures, `this` types |
| Built-in collections | arrays and read-only arrays, tuples, `Map`, `Set`, iterators and `for...of` |
| Strings and text | `string`, template literals, template literal types, `RegExp`, UTF-16 code units |
| User-defined types | interfaces, type aliases, union and intersection types, classes, generics, mapped and conditional types |
| Errors and exceptions | `throw`, `try`, `catch` and `finally`, the `unknown` type of a caught value, `Error` and its subclasses |
| Memory and resources | the garbage collector of the JavaScript engine, `using` declarations and `Symbol.dispose`, `WeakMap` and `WeakRef` |
| Modules, packages and builds | ECMAScript modules and CommonJS, `.mts` and `.cts`, module resolution, npm, declaration files, project references |
| Concurrency and parallelism | the event loop, promises, `async` and `await`, workers |
| Metaprogramming | decorators, `keyof` and `typeof` in types, indexed access types, mapped and conditional types, `infer` |
| The standard library | the JavaScript built-ins and the `lib` declaration files; the api cards hang here |
| I/O, files and the OS | the APIs of the runtime, such as `node:fs`, `node:path` and `node:process` |
| Testing | `node:test`, test files run by type stripping, tests of types |
| Tooling | `tsc`, `tsconfig.json`, the language service in editors, Prettier, typescript-eslint |
| Ecosystem | npm, DefinitelyTyped and `@types`, the design notes and the iteration plans, libraries by need |
| Performance | the cost of type checking, incremental builds and project references, the native compiler of TypeScript 7 |
| Interoperability | JavaScript files with `allowJs` and `checkJs`, JSDoc types, declaration files for JavaScript libraries |
| Idioms and style | `unknown` instead of `any`, discriminated unions, `as const`, erasable syntax only, the Prettier style |
| Pitfalls | type assertions with no runtime check, the unsound places, `any` that spreads, syntax that is not erasable |
| Versions and migration | the move from 5.9 to 6.0 and 7.0, deprecated compiler options, the changed default options |

## Versions and support

TypeScript releases about every three months. Each release has a beta, a release candidate and a stable release, and patch releases follow.[^21] At the end of each month, the team decides if the current stable release needs a patch.[^21] The release process names no support period for older versions.[^21]

TypeScript 6.0 is the last release on the JavaScript codebase and the bridge between 5.9 and 7.0.[^14] TypeScript 7.0 is a native port of the compiler.[^22] The 6.0 line is in maintenance mode, and it takes only a limited set of changes.[^23] TypeScript 7.0 has no API yet, so the package `@typescript/typescript6` gives the 6.0 compiler and API as `tsc6`.[^22]

TypeScript has no support policy. So the pages of this guide state facts for the releases of the last five years, TypeScript 4.3 to 7.0. TypeScript 4.3 was the stable release on 2021-07-08, five years before 7.0.[^24] A fact that holds only from a later version gives that version.

| Version | Status | First release | Newest release | End of life |
|---|---|---|---|---|
| 7.1 | beta | none | 7.1.0-beta (2026-10-20) | not published |
| 7.0 | newest | 2026-07-08 | 7.0.2 (2026-07-08) | not published |
| 6.0 | superseded; the last release on the JavaScript codebase | 2026-03-23 | 6.0.3 (2026-04-16) | not published |
| 5.9 | superseded | 2025-07-31 | 5.9.3 (2025-09-30) | not published |
| 5.8 | superseded | 2025-02-28 | 5.8.3 (2025-04-05) | not published |
| 5.7 | superseded | 2024-11-22 | 5.7.3 (2025-01-08) | not published |

The dates come from the npm registry.[^24] The GitHub releases give later dates for some versions. GitHub dates v7.0.2 2026-08-20, and the release says that it first came out in the typescript-go repository.[^25] The iteration plan of 7.1 gave 2026-10-06 for the beta and 2026-11-24 for the stable release.[^26] The beta came out on 2026-10-20, and its announcement expects a release candidate about a month later.[^27] So the table gives no date for the stable release of 7.1.

## Implementations

The compiler `tsc` is the reference implementation. TypeScript 7.0 is a native port of it, written in Go.[^22] The team announced the port in March 2025 and named the JavaScript line TypeScript 6 and the native line TypeScript 7.[^28] The staging repository microsoft/typescript-go is closed, and its code is now in microsoft/TypeScript.[^29]

| Implementation | Runs on | TypeScript version | Newest release | Licence |
|---|---|---|---|---|
| `tsc`, TypeScript 7 | a native program, installed from npm | 7.0 | 7.0.2 (2026-07-08) | Apache-2.0 |
| `tsc`, TypeScript 6 | Node.js; the JavaScript codebase | 6.0 | 6.0.3 (2026-04-16) | Apache-2.0 |
| Node.js type stripping | Node.js; no type check, erasable syntax only | not stated | 26.13.0 (2026-10-28) | MIT |
| Deno | Deno; `deno check` runs the type checker | not stated | 2.9.8 (2026-10-21) | MIT |
| Bun | Bun; no type check | not stated | 1.4.4 (2026-10-22) | MIT |
| esbuild | a bundler; no type check | not stated | 0.28.2 (2026-08-08) | MIT |

Node.js replaces the type syntax with whitespace and ignores `tsconfig.json`. `enum`, namespaces with runtime code and parameter properties need the flag `--experimental-transform-types`.[^18] `deno check` runs the TypeScript type checker.[^19] esbuild does no type checking.[^30] Sources of the releases: Node.js[^31], Deno[^32], Bun[^33] and esbuild[^34]. The compat tables of the feature pages have one column, for `tsc`.

## Ecosystem

npm is the package registry of JavaScript.[^35] A `tsconfig.json` file marks the root of a TypeScript project and holds its compiler options.[^36] The DefinitelyTyped repository holds declaration files for thousands of JavaScript libraries. It publishes them to npm under the `@types` scope.[^7]

| Need | Tool |
|---|---|
| install packages | npm[^35] |
| check types and compile | `tsc` and `tsconfig.json`[^36] |
| get types for JavaScript packages | DefinitelyTyped, the `@types` packages[^7] |
| run tests | `node:test` in Node.js[^37] |
| format | Prettier[^38] |
| lint | typescript-eslint[^39] |

The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

TypeScript is a Microsoft project. Its repository is microsoft/TypeScript on GitHub, and it follows the Microsoft Open Source Code of Conduct.[^1] The first release, TypeScript 0.8, came out in October 2012.[^40] Anders Hejlsberg is the lead architect of TypeScript. He is also the original designer of C#, Delphi and Turbo Pascal.[^28]

| Date | Event |
|---|---|
| October 2012 | TypeScript 0.8, the first release[^40] |
| 2014-04-02 | TypeScript 1.0; the team starts to take pull requests for the compiler and the language service[^40] |
| 2016-09-22 | TypeScript 2.0, with non-nullable types[^41] |
| 2025-03-11 | the announcement of a native port of the compiler[^28] |
| 2026-03-23 | TypeScript 6.0, the last release on the JavaScript codebase[^14][^24] |
| 2026-07-08 | TypeScript 7.0, the native port[^22] |

The TypeScript team plans each release in an iteration plan on GitHub.[^21] A design proposal must fit the design goals, and its author first looks for related suggestions in the issue tracker.[^42] The team discusses proposals in design meetings and publishes the notes as GitHub issues with the label "Design Notes".[^43]

TypeScript keeps the syntax and the runtime behaviour of JavaScript.[^2] Its design goals ask it to align with current and future ECMAScript proposals.[^3]

## Coming from another language

- From JavaScript: the types exist only at compile time. The compiler erases them, so the type system has no effect on how a program runs; see [[type-erasure]].[^2]
- From JavaScript: a value with no annotation and no inferred type usually gets `any`, which the compiler does not check. `noImplicitAny` makes this an error, and `unknown` is the checked choice.[^11][^12]
- From JavaScript: with `strictNullChecks` on, a `string` cannot be `null`. The type must say `string | null`, and the code must narrow it before use; see [[null-references]].[^11]
- From Java and C#: types are structural, not nominal. An object fits an interface when it has the members, even if it never names the interface; see [[subtyping]].[^9]
- From Java and C#: the types are not reified. There is no run-time type of a generic parameter, and `typeof` and `instanceof` see only the JavaScript values; see [[static-and-dynamic-typing]].[^9]
- From Haskell: union types are untagged. A discriminated union needs a literal property, such as `kind`, in each member; see [[sum-types]].[^13][^4]

## References
