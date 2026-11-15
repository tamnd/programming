```javascript run id=var-let-and-let-outside-the-loop
const run = (fs) => fs.map((f) => f()).join(" ");

const withVar = [];
for (var i = 0; i < 3; i++) {
  withVar.push(() => i);
}
console.log("var:", run(withVar));

const withLet = [];
for (let j = 0; j < 3; j++) {
  withLet.push(() => j);
}
console.log("let:", run(withLet));

// A let declared before the loop is one variable for all passes.
const outside = [];
let k = 0;
for (; k < 3; k++) {
  outside.push(() => k);
}
console.log("let outside:", run(outside));

// for...of with const gives each pass its own binding.
const names = [];
for (const name of ["ann", "bob"]) {
  names.push(() => name);
}
console.log("for...of:", run(names));

// The old fix: a function call makes a new variable.
const iife = [];
for (var m = 0; m < 3; m++) {
  iife.push(((n) => () => n)(m));
}
console.log("factory:", run(iife));
```

```output for=var-let-and-let-outside-the-loop
var: 3 3 3
let: 0 1 2
let outside: 3 3 3
for...of: ann bob
factory: 0 1 2
```

- Verdict: native. A `var` counter belongs to the function, so the three closures read one variable, which is 3 after the loop. A `let` in the head of the loop gets a new binding for each pass, but a `let` declared before the loop is one variable again. Before `let`, code called a function in each pass, which gave each closure its own parameter.
