```javascript wrong id=precision
function addPoints(balance, points) {
  return balance + points;
}

const balance = Number.MAX_SAFE_INTEGER;
console.log(addPoints(balance, 1));
console.log(addPoints(balance, 2));
console.log(addPoints(balance, 1) === addPoints(balance, 2));
```

```output for=precision
9007199254740992
9007199254740992
true
```

A JavaScript number is a double. Above 2⁵³ − 1, it cannot hold every integer, so two different sums compare as equal.

```javascript run id=safe-integer
function addPoints(balance, points) {
  const sum = balance + points;
  if (!Number.isSafeInteger(sum)) {
    throw new RangeError(`the sum is not a safe integer`);
  }
  return sum;
}

const balance = Number.MAX_SAFE_INTEGER;
console.log(addPoints(100, 1));
try {
  addPoints(balance, 2);
} catch (error) {
  console.log(`refused: ${error.message}`);
}
console.log(BigInt(balance) + 2n);
```

```output for=safe-integer
101
refused: the sum is not a safe integer
9007199254740993n
```

- Verdict: idiom. Numbers do not wrap, but integers above `Number.MAX_SAFE_INTEGER` lose precision. Check results with `Number.isSafeInteger`, or use `BigInt`, which has no fixed width.
- Bitwise operators convert their operands to 32-bit integers, and typed arrays such as `Int32Array` wrap values that do not fit.
- Overflow in the engine and in native add-ons, which are written in C and C++, is outside the reach of script.
