```javascript run id=local-function-javascript
function sumOfScaled(values, factor) {
  function scale(x) {
    return x * factor;
  }

  let total = 0;
  for (const v of values) {
    total += scale(v);
  }
  return total;
}

console.log(sumOfScaled([1, 2, 3], 10));
console.log(sumOfScaled([4], 2));
```

```output for=local-function-javascript
60
8
```

- Verdict: native. A function declaration in a function body is local to that function and can read its parameters.
