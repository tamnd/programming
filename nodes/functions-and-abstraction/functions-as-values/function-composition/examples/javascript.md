```javascript run id=function-composition-javascript
const compose = (f, g) => (x) => f(g(x));
const composeAll = (...fs) => (x) => fs.reduceRight((acc, f) => f(acc), x);

const inc = (x) => x + 1;
const double = (x) => x * 2;

console.log(compose(double, inc)(5));
console.log(compose(inc, double)(5));
console.log(composeAll(inc, double, inc)(5));
```

```output for=function-composition-javascript
12
11
13
```

- Verdict: idiom. JavaScript has no compose function or operator. An arrow function that returns an arrow function composes two functions, and `reduceRight` applies a list of functions from the last to the first.
