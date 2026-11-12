```javascript run id=semicolon-insertion-and-sugar
// A syntax error stops the parse; new Function parses text at run time.
try { new Function("let x = ;"); } catch (e) { console.log(e.name + ":", e.message); }

// Automatic semicolon insertion: a line break after return ends the statement.
function f() {
  return
    42;
}
console.log("f() =", f());

// A { at the start of a statement starts a block, not an object.
console.log(eval("{ a: 1 }"), eval("({ a: 1 })"));

// Template literals are sugar for a call when a tag is used.
const tag = (strings, ...values) => strings.raw.join("|") + " / " + values.join(",");
console.log(tag`a${1}b${2}c`);

// Class syntax is mostly sugar over constructor functions and prototypes.
class Point { constructor(x) { this.x = x; } get double() { return 2 * this.x; } }
console.log(typeof Point, new Point(4).double);
```

```output for=semicolon-insertion-and-sugar
SyntaxError: Unexpected token ';'
f() = undefined
1 { a: 1 }
a|b|c / 1,2
function 8
```

- Verdict: native. `new Function` parses text at run time and throws `SyntaxError`. A line break after `return` ends the statement, so `f()` returns `undefined`. `{ a: 1 }` at the start of a statement is a block with a label, and in parentheses it is an object. A tagged template is sugar for a call. A class is a function.
