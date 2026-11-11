```javascript run id=function-constructor
const area = new Function("w", "h", "return w * h;"); // compile source at run time
console.log(area(3, 4), typeof area);

const formula = "x * 2 + 1";
const f = new Function("x", `return ${formula};`);
console.log([0, 1, 2, 3].map(f));

try {
  new Function("return (1 + ;");
} catch (e) {
  console.log(e.name); // the engine parses the source before it runs any of it
}

function hot(n) {
  let s = 0;
  for (let i = 0; i < n; i++) s += i; // a hot loop: the engine compiles it to machine code
  return s;
}
console.log(hot(1_000_000));
```

```output for=function-constructor
12 function
[ 1, 3, 5, 7 ]
SyntaxError
499999500000
```

- Verdict: native. A JavaScript engine parses the source, compiles it to bytecode for an interpreter, and compiles hot functions to machine code with one or more JIT compilers. The `Function` constructor and `eval` compile source text at run time, which is how template engines and some formula evaluators work. A syntax error in the text throws `SyntaxError` before any of the new code runs.
- The engine watches `hot` as it runs the loop a million times, and moves it from the interpreter to compiled code. The program cannot see the change, except in speed.
