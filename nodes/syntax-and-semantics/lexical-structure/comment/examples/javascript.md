```javascript run id=block-comments-and-asi
// A function's source text keeps its comments, so toString shows them.
function add(a, b) {
  /* the sum */ return a + b; // done
}
console.log(add.toString().split("\n")[1].trim());

// Block comments do not nest: the first */ ends the comment.
for (const src of ["/* a /* b */ 1", "/* a /* b */ c */ 1"]) {
  try {
    console.log(JSON.stringify(src), "=>", new Function(`return ${src}`)());
  } catch (e) {
    console.log(JSON.stringify(src), "=>", e.name);
  }
}

// A comment that contains a line break acts as a line break for automatic semicolon insertion.
const f = new Function("return /*\n*/ 42");
console.log(f());

// JSON has no comments.
try {
  JSON.parse('{"a": 1 /* no */}');
} catch (e) {
  console.log("JSON:", e.name);
}
```

```output for=block-comments-and-asi
/* the sum */ return a + b; // done
"/* a /* b */ 1" => 1
"/* a /* b */ c */ 1" => SyntaxError
undefined
JSON: SyntaxError
```

- Verdict: native. `//` and `/* */` are the two usual comment forms, and a hashbang line `#!` at the start of a script is a third form. `Function.prototype.toString` returns the source text, so it keeps the comments. Block comments do not nest, so the second string is a syntax error. A block comment that contains a line break ends the `return` statement, so the function returns `undefined`. `JSON.parse` rejects comments.
