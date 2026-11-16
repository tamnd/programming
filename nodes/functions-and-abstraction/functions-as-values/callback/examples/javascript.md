```javascript run id=callback-javascript
function repeat(n, callback) {
  for (let i = 1; i <= n; i++) {
    callback(i);
  }
}

function tick(i) {
  console.log(`tick ${i}`);
}

console.log([3, 1, 2].sort((a, b) => b - a).join(" "));
repeat(2, tick);
```

```output for=callback-javascript
3 2 1
tick 1
tick 2
```

- Verdict: native. `sort` calls the comparison function, which returns a negative number, zero or a positive number. `repeat` calls `tick` before it returns, so both callbacks are synchronous.
