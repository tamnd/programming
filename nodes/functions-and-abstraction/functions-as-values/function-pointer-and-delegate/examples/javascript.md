```javascript run id=function-pointer-and-delegate-javascript
function add(a, b) {
  return a + b;
}

function mul(a, b) {
  return a * b;
}

function apply(f, a, b) {
  return f(a, b);
}

let op = add;
console.log(op(6, 7));
op = mul;
console.log(op(6, 7));
console.log(apply((a, b) => a - b, 6, 7));
```

```output for=function-pointer-and-delegate-javascript
13
42
-1
```

- Verdict: native. A JavaScript variable holds the function itself. `op` holds `add`, then `mul`.
