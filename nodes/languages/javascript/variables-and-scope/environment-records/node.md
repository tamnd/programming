---
tag: NEW-javascript-environment-records
kind: feature
owner: javascript
slug: environment-records
name: Environment Records
aliases: [Environment Record, lexical environment, Declarative Environment Record, Object Environment Record, Function Environment Record, Module Environment Record, scope analysis]
area: languages/javascript/variables-and-scope
level: advanced
summary: >-
  An Environment Record is the specification type that maps names to variables for one scope. Records link to an outer record, and a name lookup walks that chain.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-environment-records", locator: "9.1 Environment Records; 8.2 Scope Analysis"}
see_also: [javascript/lexical-scoping, javascript/the-temporal-dead-zone, javascript/the-global-scope-and-globalthis, javascript/execution-contexts-and-realms, javascript/function-scopes-and-closures, javascript/specification-types, binding, python/python-scopes-and-namespaces]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-environment-records
    title: ECMAScript 2026 Language Specification
    locator: 9.1 Environment Records; 9.1.1 The Environment Record Type Hierarchy, Table 14; 9.1.2.1 GetIdentifierReference
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-block
    title: ECMAScript 2026 Language Specification
    locator: 14.2.1 Early Errors; 14.2.3 BlockDeclarationInstantiation; 14.11 The with Statement
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/syntax-directed-operations.html#sec-syntax-directed-operations-scope-analysis
    title: ECMAScript 2026 Language Specification
    locator: 8.2 Scope Analysis
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

| Record | Made for |
|---|---|
| Declarative | a block, a catch clause, a function's declarations |
| Function | one call of a function; holds `this` for non-arrow functions |
| Module | the top level of a module, with its imports |
| Object | a `with` statement; the global object |
| Global | the outermost scope of the scripts of a realm |

"Environment Record is a specification type used to define the association of Identifiers to specific variables and functions, based upon the lexical nesting structure of ECMAScript code."[^1]

```javascript run id=synopsis
let n = "outer";
function f() {
  let n = "f";
  return () => n;
}
const g = f();
n = "changed";
console.log(g());
```

```output for=synopsis
f
```

## Semantics

**r1. One record for each run of the code.** "Usually an Environment Record is associated with some specific syntactic structure of ECMAScript code such as a FunctionDeclaration, a BlockStatement, or a Catch clause of a TryStatement. Each time such code is evaluated, a new Environment Record is created to record the identifier bindings that are created by that code."[^1]

**r2. The hierarchy.** "Environment Record is an abstract class with three concrete subclasses: Declarative Environment Record, Object Environment Record, and Global Environment Record. Function Environment Records and Module Environment Records are subclasses of Declarative Environment Record."[^1]

**r3. Bindings.** CreateMutableBinding "creates a new but uninitialized mutable binding". InitializeBinding "sets the value of an already existing but uninitialized binding". For an immutable binding with S true, "attempts to set it after it has been initialized will always throw an exception, regardless of the strict mode setting".[^1] BlockDeclarationInstantiation gives a `const` such a binding: "If IsConstantDeclaration of d is true, then Perform ! env.CreateImmutableBinding ( dn, true )."[^2] Example: [`immutable`](#immutable).

**r4. Name lookup.** GetIdentifierReference asks the record HasBinding. If it has none, the steps say "Let outer be env.`[[OuterEnv]]`" and run again on the outer record. "If env is null", the result is a Reference Record with `[[Base]]` unresolvable.[^1]

**r5. Object records.** "Each Object Environment Record is associated with an object called its binding object." "Both own and inherited properties are included in the set regardless of the setting of their `[[Enumerable]]` attribute."[^1] "The with statement adds an Object Environment Record for a computed object to the lexical environment of the running execution context."[^2] Example: [`with`](#with).

**r6. Scope analysis.** Before code runs, the syntax-directed operations of 8.2 collect the names that each record will hold. Examples are BoundNames, LexicallyDeclaredNames and VarDeclaredNames.[^3] A Block has an early error "if any element of the LexicallyDeclaredNames of StatementList also occurs in the VarDeclaredNames of StatementList."[^2]

## Examples

The examples run with Node.js 24.

An immutable binding for `const` (rule r3):

```javascript fails id=immutable expect=exception:TypeError
const limit = 1;
limit = 2;
```

An Object Environment Record from `with`, in sloppy mode (rule r5). Inherited properties are names too:

```javascript run id=with
const proto = { inherited: "yes" };
const scopeObj = Object.create(proto);
scopeObj.own = 1;
with (scopeObj) {
  console.log(own, inherited);
}
```

```output for=with
1 yes
```

## Pitfalls

### Records are not objects you can reach

"Environment Records are purely specification mechanisms and need not correspond to any specific artefact of an ECMAScript implementation. It is impossible for an ECMAScript program to directly access or manipulate such values."[^1]

### A closure keeps a record, not a copy

In example [`synopsis`](#synopsis), the arrow function reads `n` from the record of the call to `f` (rule r1). A change to the outer `n` does not reach it.

### with makes names depend on an object

With `with`, a name can resolve to a property that appears at run time (rule r5). For `with`, "It is a Syntax Error if IsStrict (this production) is true."[^2] See [[javascript/strict-mode]].

## Comparison

| Topic | JavaScript spec | Python |
|---|---|---|
| unit of scope | Environment Record[^1] | namespace of a block[^4] |
| lookup | the chain of `[[OuterEnv]]`[^1] | "the nearest enclosing scope"[^4] |

Python names its lookup set the environment: "The set of all such scopes visible to a code block is called the block’s environment."[^4] See [[python/python-scopes-and-namespaces]] and [[binding]].

## References
