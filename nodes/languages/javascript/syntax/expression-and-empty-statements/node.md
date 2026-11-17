---
tag: KKX8GC
kind: feature
owner: javascript
slug: expression-and-empty-statements
name: Expression and empty statements
aliases: [expression statement, empty statement, "ExpressionStatement", "EmptyStatement", completion value]
area: languages/javascript/syntax
level: advanced
summary: >-
  An expression statement evaluates an expression and discards the result. It can not start with {, function, async function, class or let [. A lone ; is an empty statement.
grammar: "ExpressionStatement : [lookahead not in { {, function, async function, class, let [ }] Expression ; EmptyStatement : ; (ECMA-262, 14.4 and 14.5)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-expression-statement", locator: "14.4 Empty Statement; 14.5 Expression Statement"}
see_also: [javascript/statements-and-declarations, javascript/automatic-semicolon-insertion, javascript/strict-mode, javascript/comma-operator, javascript/expressions-and-operators, python/expression-statements, python/pass-statements]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/Expression_statement
    title: Expression statement, MDN
    locator: introduction; Description; Forbidden expressions
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/Empty
    title: Empty statement, MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-expression-statement
    title: ECMAScript 2026 Language Specification
    locator: 14.2.2 Block evaluation, Note 2; 14.4 Empty Statement; 14.5 Expression Statement
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.1 Expression statements; 7.4 The pass statement
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
let count = 0;
count++;
console.log("count is", count);
count + 100;
;
for (let i = 0; i < 3; i++, count++);
console.log(count);
```

```output for=synopsis
count is 1
4
```

MDN: "An expression statement is an expression used in a place where a statement is expected. The expression is evaluated and its result is discarded".[^1] `count + 100;` has no effect. The lone `;` and the loop body are empty statements. "An empty statement is used to provide no statement, although the JavaScript syntax would expect one."[^2]

## Semantics

**r1. Grammar.** `ExpressionStatement : Expression ;` has a lookahead restriction. The statement can not start with `{`, `function`, `async function` with no line break between the two words, `class`, or the two tokens `let [`.[^3]

**r2. Why the restriction.** The spec gives the reasons. A `{` start "might make it ambiguous with a Block". A `function` or `class` start "would make it ambiguous with a FunctionDeclaration , a GeneratorDeclaration , or a ClassDeclaration". A `let [` start would be ambiguous "with a let LexicalDeclaration whose first LexicalBinding was an ArrayBindingPattern".[^3] Example: [`forbidden`](#forbidden).

**r3. Parentheses remove the restriction.** "To avoid these problems, you can use parentheses, so that the statement is unambiguously an expression statement."[^1] Example: [`parens`](#parens).

**r4. Evaluation.** The statement evaluates the expression and returns `GetValue` of the result. An empty statement returns `empty`.[^3] The semicolon of an expression statement can come from automatic semicolon insertion.[^1]

**r5. Completion values.** "The value of a StatementList is the value of the last value-producing item in the StatementList ." An empty statement and a `var` declaration produce no value. So `eval('1;;;;;')`, `eval('1;{}')` and `eval('1;var a;')` all return 1.[^3] Example: [`completion`](#completion).

**r6. Useful statements have side effects.** "Because the expression is evaluated and then discarded, the result of the expression is not available. Therefore, the expression must have some side effect for it to be useful."[^1] MDN lists function calls, tagged templates, assignments, `++` and `--`, `delete`, `import()`, `yield` and `yield*`.[^1]

**r7. The empty statement.** "The empty statement is a semicolon ( ; ) indicating that no statement will be executed, even if JavaScript syntax requires one."[^2] A block statement does the opposite: it puts many statements where one is expected.[^2]

## Examples

The examples run with Node.js 24.

A statement that starts with `function` is a declaration, so a call after it is a syntax error (rule r2):

```javascript fails id=forbidden expect=exception:SyntaxError
function foo() {
  console.log("foo");
}();
```

Node.js reports a SyntaxError at the `)`. The parser reads a function declaration, and then `()`, which is not a valid expression.

Parentheses make an expression statement (rule r3):

```javascript run id=parens
(function foo() {
  console.log("foo");
})();
({ foo: 1 }).foo;
const result = eval("({ foo: 1 })");
const block = eval("{ foo: 1 }");
console.log(result, block);
```

```output for=parens
foo
{ foo: 1 } 1
```

Without parentheses, `{ foo: 1 }` is a block. MDN: "`foo` is a label and `1` is an expression statement."[^1] So `eval` returns 1, the value of the last expression statement in the block (rule r5).

Completion values with `eval` (rule r5). These are the examples from the spec:[^3]

```javascript run id=completion
console.log(eval("1;;;;;"), eval("1;{}"), eval("1;var a;"));
console.log(eval("2; if (true) { 3; }"), eval("let b = 5;"));
```

```output for=completion
1 1 1
3 undefined
```

A `let` declaration produces no value, so the script `let b = 5;` has the value `undefined`.

A `let` variable in sloppy mode (rule r1). With the name `let`, `let[index] = value` is a declaration, not an assignment:[^1]

```javascript run id=let-name
var let = [1, 2, 3];
function setIndex(index, value) {
  if (index >= 0) {
    let[index] = value;
  }
}
setIndex(0, [1, 2]);
console.log(let);
```

```output for=let-name
[ 1, 2, 3 ]
```

In `setIndex`, `let[index] = value` declares the local variables `index` from the array `[1, 2]`. The global array does not change. "let can only be an identifier in non-strict mode".[^1]

## Pitfalls

### A semicolon after `if`

`if (condition);` has an empty statement as its body. The next line runs every time:[^2]

```javascript run id=if-semicolon
let calls = 0;
const condition = false;
if (condition);
calls++;
console.log(calls);
```

```output for=if-semicolon
1
```

MDN: "It is a good idea to comment intentional use of the empty statement, as it is not really obvious to distinguish from a normal semicolon."[^2]

### An object literal at the start of a line

A line that starts with `{` is a block, not an object literal (rule r2). Put the literal in parentheses, as in the [`parens`](#parens) example.

### Expressions as control flow

MDN shows that `&&`, `? :` and array methods can replace `if` and loops, and warns: "Excessive use of expression statements as a substitute for control-flow statements can make code much less readable."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| expression statement | `f();`, value discarded[^1] | `f()`; "used (mostly interactively) to compute and write a value, or (usually) to call a procedure"[^4] |
| no-op statement | `;`[^2] | `pass`, "a null operation"[^4] |
| start restrictions | not `{`, `function`, `class`, `let [`[^3] | `expression_stmt : starred_expression`, with no lookahead list[^4] |

The Python reference says of `pass`: "when it is executed, nothing happens."[^4] See [[python/expression-statements]] and [[python/pass-statements]].

## References
