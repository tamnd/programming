```javascript run id=return-value-javascript
function absolute(n) {
  if (n < 0) {
    return -n;
  }
  return n;
}

console.log(absolute(-7));
console.log(absolute(5) + 1);
```

```output for=return-value-javascript
7
6
```

- Verdict: native. `return` ends the function and gives the value. A function with no `return` gives back `undefined`.
