```javascript run id=truthy-conditions-and-ternary
// A condition is truthy or falsy: "0" and [] are truthy, 0 and "" are falsy.
for (const v of [0, "0", "", [], null]) {
  if (v) {
    console.log(JSON.stringify(v), "truthy");
  } else {
    console.log(JSON.stringify(v), "falsy");
  }
}

// else if is an if statement inside the else branch.
function size(n) {
  if (n < 10) return "small";
  else if (n < 100) return "medium";
  else return "large";
}
console.log(size(5), size(50), size(500));

// The conditional operator gives a value, and it can be chained.
const t = 30;
console.log(t < 10 ? "cold" : t < 25 ? "mild" : "hot");

// An object lookup replaces a chain of conditions for fixed keys.
const handlers = { add: (a, b) => a + b, mul: (a, b) => a * b };
for (const op of ["add", "mul", "pow"]) {
  const f = handlers[op];
  console.log(op, f ? f(3, 4) : "unknown");
}

// ??= assigns only when the value is null or undefined.
const opts = { retries: 0 };
opts.retries ??= 3;
opts.timeout ??= 1000;
console.log(opts.retries, opts.timeout);
```

```output for=truthy-conditions-and-ternary
0 falsy
"0" truthy
"" falsy
[] truthy
null falsy
small medium large
hot
add 7
mul 12
pow unknown
0 1000
```

- Verdict: native. `0`, `""` and `null` are falsy, and `"0"` and `[]` are truthy. `else if` is an `if` statement in the `else` branch. A chained conditional operator chooses `"hot"`. An object lookup replaces a chain of tests, and `??=` assigns the default only to the missing `timeout`, not to the `0` of `retries`.
