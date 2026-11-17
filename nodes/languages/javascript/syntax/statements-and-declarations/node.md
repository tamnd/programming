---
tag: 7F6F1T
kind: feature
owner: javascript
slug: statements-and-declarations
name: Statements and declarations
aliases: [statements, declarations, StatementListItem, statement position, single-statement context, completion value]
area: languages/javascript/syntax
level: core
summary: >-
  A body holds statements and declarations. Declarations (let, const, class, function forms) cannot be the bare body of if, a loop or a label; var is a statement. Expressions appear in slots of both.
grammar: "StatementListItem : Statement | Declaration; Declaration : HoistableDeclaration | ClassDeclaration | LexicalDeclaration (ECMA-262, 14 Statements and Declarations; 14.2 Block)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-ecmascript-language-statements-and-declarations", locator: "14 ECMAScript Language: Statements and Declarations"}
see_also: [statements-and-expressions, javascript/automatic-semicolon-insertion, javascript/lexical-grammar, javascript/javascript-language-overview, python/statements-and-suites, python/expression-statements, python/pass-statements]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-ecmascript-language-statements-and-declarations
    title: ECMAScript 2026 Language Specification
    locator: 14 Statements and Declarations, Syntax; 14.2 Block; 14.5 Expression Statement
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements
    title: Statements and declarations, MDN
    locator: introduction; Statements and declarations by category; What are statements, declarations, and expressions?
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7. Simple statements
    accessed: 2026-11-18
---

## Synopsis

| ECMA-262 production | Forms |
|---|---|
| BlockStatement | `{ ... }` |
| VariableStatement | `var x = 1;` |
| EmptyStatement | `;` |
| ExpressionStatement | `f(x);`, `x = 2;` |
| IfStatement | `if (c) s else t` |
| BreakableStatement | `for`, `for...in`, `for...of`, `while`, `do...while`, `switch` |
| ContinueStatement, BreakStatement, ReturnStatement | `continue`, `break`, `return` |
| WithStatement | `with (o) s` |
| LabelledStatement | `name: s` |
| ThrowStatement, TryStatement | `throw e;`, `try { } catch { } finally { }` |
| DebuggerStatement | `debugger;` |
| Declaration | `let`, `const`, `class`, `function`, `function*`, `async function`, `async function*` |

The first rows are the alternatives of Statement, and the last row is Declaration.[^1] "JavaScript code is built from statements and declarations, which can contain expressions."[^2]

## Semantics

**r1. Two sets.** ECMA-262 defines Statement and Declaration as two productions.[^1] A Declaration is a HoistableDeclaration (the four function forms), a ClassDeclaration, or a LexicalDeclaration (`let` and `const`).[^1] MDN: "we often colloquially refer to both as statements, but they are technically two disjoint sets of grammars."[^2] `import` and `export` are declarations too, and "can only appear at the top-level of a module".[^2]

**r2. Where each may stand.** A block, a function body and the top level of a script or module hold a list of StatementListItem, and a StatementListItem is a Statement or a Declaration.[^1] The bodies of `if`, `else`, the loops and a label take a single Statement. So a bare `let` there is an error: "Lexical declaration cannot appear in a single-statement context".[^2] Braces fix it, because a block is a statement that holds a list.[^2] Examples: [`positions`](#positions), [`bare-let`](#bare-let).

**r3. `var` is a statement.** `var` is a VariableStatement, so `if (c) var i = 0;` is valid.[^1] [^2] MDN calls this a special case, because `var` "doesn't follow normal lexical scoping rules": a `var` name is visible outside its block.[^2] Example: [`positions`](#positions).

**r4. Expressions in slots.** "Expressions are not top-level constructs; they can only be used in specific slots inside statements and declarations".[^2] The expression statement is the statement with one such slot, and it lets most expressions stand where a statement can.[^2] It cannot start with `{`, `function`, `class`, `async function` or `let [`: "An ExpressionStatement cannot start with a U+007B (LEFT CURLY BRACKET) because that might make it ambiguous with a Block."[^1] Wrap such an expression in parentheses. Example: [`completion`](#completion).

**r5. Lines and statements.** "A statement can span multiple lines, and a line can contain multiple statements."[^2] Semicolons end most statements, and the parser inserts some of them ([[javascript/automatic-semicolon-insertion]]).

**r6. Completion values.** "In JavaScript, statements and declarations produce values, but these values are almost never observable (other than eval())."[^2] `eval` returns the value of the last statement that produced one. A declaration gives `undefined`. Example: [`completion`](#completion).

## Examples

The examples run with Node.js 24.

`var` as the bare body of an `if`, `let` inside a block, and a labelled block that `break` leaves (rules r2 and r3):

```javascript run id=positions
const flag = true;
if (flag) var viaVar = "var is a statement";
if (flag) {
  let inBlock = "let inside a block";
  console.log(inBlock);
}
console.log(viaVar);

outer: {
  console.log("in the labelled block");
  break outer;
  console.log("skipped");
}
console.log("after the block");
```

```output for=positions
let inside a block
var is a statement
in the labelled block
after the block
```

Completion values through `eval` (rules r4 and r6). An `if` gives the value of the branch that ran, a declaration gives `undefined`, and a loop gives the value of its last pass. At the start of a statement, `{}` is an empty block and `function f() {}` is a declaration. In parentheses, both are expressions:

```javascript run id=completion
console.log(eval("if (true) { 'then' } else { 'else' }"));
console.log(eval("let x = 1;"));
console.log(eval("for (let i = 0; i < 3; i++) i * 10"));
console.log(eval("{}"), eval("({})"));
console.log(eval("function f() {}"), typeof eval("(function f() {})"));
```

```output for=completion
then
undefined
20
undefined {}
undefined function
```

A declaration as the bare body of an `if` (rule r2):

```javascript fails id=bare-let expect=exception:SyntaxError
const flag = true;
if (flag) let i = 0;
```

Node.js reports `SyntaxError: Lexical declaration cannot appear in a single-statement context`.

## Pitfalls

### A declaration after a label

A label attaches only to a statement, so `label: const a = 1;` is the same error as [`bare-let`](#bare-let). Use a block: `label: { const a = 1; }`.[^2]

### An object literal at the start of a statement

`{ a: 1 }` at the start of a statement is a block with the label `a`, not an object (rule r4). Arrow functions have the same trap: `() => { a: 1 }` has a block body. Write `({ a: 1 })`.[^1]

### `var` in a bare body

`if (c) var x = 1;` is valid (rule r3), but `x` exists in the whole function, also when `c` is false. Use `let` inside braces.[^2]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| kinds | statements and declarations[^1] | simple and compound statements[^3] |
| declarations | `let`, `const`, `class`, function forms | `def` and `class` are statements |
| one line | any number of statements, with semicolons[^2] | simple statements separated by semicolons[^3] |
| statement values | completion values, seen through `eval`[^2] | none |

Python says: "A simple statement is comprised within a single logical line."[^3] Python has no separate set of declarations. See [[python/statements-and-suites]].

## References
