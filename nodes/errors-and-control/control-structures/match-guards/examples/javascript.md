```javascript run id=match-guards-javascript
function describe(n) {
  if (n === 0) {
    return "zero";
  }
  if (n < 0) {
    return "negative";
  }
  if (n < 10 && n % 2 === 0) {
    return "small even";
  }
  if (n < 10) {
    return "small odd";
  }
  return "large";
}

console.log([-5, 0, 4, 7, 12].map(describe).join(", "));
```

```output for=match-guards-javascript
negative, zero, small even, small odd, large
```

- Verdict: absent. JavaScript has no `match` statement, so it has no guards. A chain of `if` statements tests the conditions in order. A TC39 proposal to add pattern matching is at stage 1.
