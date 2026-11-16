```javascript run id=strategy-javascript
function divide(a, b) {
  if (b === 0) {
    throw new RangeError("division by zero");
  }
  return Math.trunc(a / b);
}

for (const [a, b] of [[10, 2], [10, 0]]) {
  try {
    console.log(`${a} / ${b} = ${divide(a, b)}`);
  } catch (e) {
    console.log(`${a} / ${b}: error: ${e.message}`);
  }
}
```

```output for=strategy-javascript
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exceptions for synchronous code. Asynchronous code reports an error in another way: a promise is rejected with the error, and older Node.js functions pass the error as the first argument of a callback.
