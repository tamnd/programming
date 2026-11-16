```javascript run id=undefined-results
function log(msg) {
  console.log("log:", msg);
}

const r = log("start");
console.log(r, typeof r, r === undefined);

// The void operator evaluates an expression and gives undefined.
console.log(void 0, void log("again"));

// An arrow function with a block body and no return gives undefined.
const f = () => {
  1 + 1;
};
const g = () => 1 + 1;
console.log(f(), g());

// forEach runs a callback for its effect and returns undefined.
console.log([1, 2].forEach((x) => x * 2), [1, 2].map((x) => x * 2));
```

```output for=undefined-results
log: start
undefined undefined true
log: again
undefined undefined
undefined 2
undefined [ 2, 4 ]
```

- Verdict: idiom. JavaScript has no unit type by name, but a function without `return` gives `undefined`, and `undefined` is the only value of its type. The `void` operator evaluates its operand, so `log("again")` prints its line, and then gives `undefined`. An arrow function with braces needs `return`, so `f` gives `undefined` and `g` gives `2`. `forEach` returns `undefined`, and `map` returns an array.
