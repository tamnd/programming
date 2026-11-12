```javascript run id=number-and-bigint
// A JavaScript number is a 64-bit float, so there is one numeric type for both 7 and 7.5.
console.log(typeof 7, typeof 7.5, Number.isInteger(7.0), 7 / 2);

// Integers are exact only up to 2**53 - 1.
console.log(Number.MAX_SAFE_INTEGER, 2 ** 53 === 2 ** 53 + 1, Number.isSafeInteger(2 ** 53));

// There is no integer division; use Math.trunc or Math.floor. % takes the sign of the dividend.
console.log(Math.trunc(-7 / 2), Math.floor(-7 / 2), -7 % 2);

// Division by zero gives Infinity or NaN, not an error.
console.log(1 / 0, -1 / 0, 0 / 0, Number.isNaN(0 / 0));

// BigInt is the second numeric type: exact integers of any size.
console.log(typeof 1n, 2n ** 64n, 7n / 2n, -7n / 2n);

// A BigInt and a number do not mix in arithmetic.
try {
  console.log(1n + 1);
} catch (e) {
  console.log(e.name);
}
console.log(1n + BigInt(1), Number(2n ** 53n) + 1, 1n < 2, 2n == 2);

// 0.1 has no exact binary form.
console.log(0.1 + 0.2, (0.1 + 0.2).toFixed(2), 0.1 + 0.2 === 0.3);
```

```output for=number-and-bigint
number number true 3.5
9007199254740991 true false
-3 -4 -1
Infinity -Infinity NaN true
bigint 18446744073709551616n 3n -3n
TypeError
2n 9007199254740992 true true
0.30000000000000004 0.30 false
```

- Verdict: native. `typeof` gives `"number"` for both `7` and `7.5`, and `7 / 2` is `3.5`. Integers are exact only up to `Number.MAX_SAFE_INTEGER`, so `2 ** 53 === 2 ** 53 + 1` is true. Integer division needs `Math.trunc` or `Math.floor`. Division by zero gives `Infinity` or `NaN`. `BigInt` is exact, and `7n / 2n` truncates to `3n`. `1n + 1` throws `TypeError`, but comparisons such as `1n < 2` work.
