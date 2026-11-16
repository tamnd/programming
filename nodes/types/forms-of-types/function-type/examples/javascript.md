```javascript run id=functions-as-values
const inc = (n) => n + 1;
function twice(f, x) {
  return f(f(x));
}
console.log(typeof inc, inc.length, twice(inc, 5));

// A function has no parameter types, so any function fits any call.
console.log(["1", "2", "3"].map(parseInt));
console.log(["1", "2", "3"].map((s) => parseInt(s, 10)));

// A function returns a function.
const adder = (a) => (b) => a + b;
const add10 = adder(10);
console.log(add10(5), adder(1)(2));

// Extra arguments are ignored, and missing arguments are undefined.
const show = (a, b) => `${a} ${b}`;
console.log(show(1, 2, 3), show(1));
```

```output for=functions-as-values
function 1 7
[ 1, NaN, NaN ]
[ 1, 2, 3 ]
15 3
1 2 1 undefined
```

- Verdict: native. A function is an object, `typeof` gives `"function"`, and `length` gives the number of declared parameters. JavaScript does not check the number of arguments, so `map` passes the element and the index to `parseInt`, and the index becomes the radix. A wrapper that passes only the element gives the expected result. `adder` returns a function, so its result has a function type.
