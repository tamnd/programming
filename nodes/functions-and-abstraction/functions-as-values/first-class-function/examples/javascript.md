```javascript run id=first-class-function-javascript
function double(x) {
  return x * 2;
}

const square = (x) => x * x;

function applyTwice(g, x) {
  return g(g(x));
}

function adder(n) {
  return (x) => x + n;
}

const f = double;
console.log(f(5));

const ops = { double, square };
console.log(ops.square(4));

console.log(applyTwice(double, 3));

const add5 = adder(5);
console.log(add5(10));
```

```output for=first-class-function-javascript
10
16
12
15
```

- Verdict: native. A function declaration and an arrow function both make function values. An object can hold them as properties.
