```javascript run id=combinator-javascript
const identity = (x) => x;
const constant = (x) => () => x;
const flip = (f) => (a, b) => f(b, a);
const both = (p, q) => (x) => p(x) && q(x);

const sub = (a, b) => a - b;
console.log(identity(5));
console.log(constant(7)(99));
console.log(flip(sub)(2, 10));
const keep = both((x) => x % 2 === 0, (x) => x > 0);
console.log([-2, -1, 0, 1, 2, 3, 4].filter(keep).join(" "));
```

```output for=combinator-javascript
5
7
8
2 4
```

- Verdict: native. An arrow function can return an arrow function, so each combinator is one line.
