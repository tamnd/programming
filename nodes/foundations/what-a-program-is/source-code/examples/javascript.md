```javascript run id=function-source-text
function area(w, h) {
  // The engine keeps the exact source text.
  return w * h;
}
console.log(area.toString());

// new Function compiles source text when the program runs.
const double = new Function("x", "return x * 2;");
console.log(double(21));
console.log(double.toString().split("\n").join(" | "));

console.log(Math.max.toString());
```

```output for=function-source-text
function area(w, h) {
  // The engine keeps the exact source text.
  return w * h;
}
42
function anonymous(x | ) { | return x * 2; | }
function max() { [native code] }
```

- Verdict: native. A function's `toString` gives the exact text that defined it, with its comment and its layout. `new Function` compiles source text while the program runs, and the engine wraps the body in a function named `anonymous`. A built-in function such as `Math.max` has no JavaScript source, so its text says `[native code]`.
