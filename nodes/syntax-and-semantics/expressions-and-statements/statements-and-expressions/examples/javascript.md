```javascript run id=ternary-and-assignment-value
const label = (score) => (score >= 90 ? "A" : score >= 50 ? "pass" : "fail");
console.log([95, 70, 30].map(label));

let a, b;
a = b = 5; // assignment is an expression whose value is the assigned value
console.log(a, b);

const data = [3, 1, 4, 0, 5];
let i = 0, n, total = 0;
while ((n = data[i++]) !== 0) total += n;
console.log(total);

const x = (console.log("side effect"), 42); // the comma operator
console.log(x);
```

```output for=ternary-and-assignment-value
[ 'A', 'pass', 'fail' ]
5 5
8
side effect
42
```

- Verdict: native. JavaScript follows C: `if`, `for` and `while` are statements, while assignment, the conditional operator `?:` and the comma operator are expressions. `a = b = 5` works because the value of `b = 5` is 5. An arrow function with an expression body returns that expression, so short functions need no `return` statement.
