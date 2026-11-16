```javascript run id=expressions
let a, b;
a = b = 5;
console.log(a, b);
[a, b] = [1, 2];
[a, b] = [b, a];
console.log(a, b);
let n;
console.log((n = 3) * 2, n);
```

```output for=expressions
5 5
2 1
6 3
```

- Verdict: native. `=` is an expression, and its value is the value that it assigns, so `a = b = 5` works from the right and `(n = 3) * 2` is 6. A destructuring assignment such as `[a, b] = [b, a]` builds the array on the right first, so it swaps the values.
