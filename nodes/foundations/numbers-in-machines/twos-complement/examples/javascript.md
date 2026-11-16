```javascript run id=int32
console.log("~5:", ~5);
console.log("-6 >>> 0 in hex:", (-6 >>> 0).toString(16));
console.log("-(2 ** 31) | 0:", -(2 ** 31) | 0);
console.log("2 ** 31 | 0:", 2 ** 31 | 0);
console.log("BigInt.asIntN(8, 250n):", BigInt.asIntN(8, 250n));
```

```output for=int32
~5: -6
-6 >>> 0 in hex: fffffffa
-(2 ** 31) | 0: -2147483648
2 ** 31 | 0: -2147483648
BigInt.asIntN(8, 250n): -6n
```

- Verdict: native. The bitwise operators convert a number to a 32-bit two's complement integer, so `~x` is `-(x + 1)`.
- `x | 0` converts to a signed 32-bit value. 2³¹ does not fit, so it becomes the most negative 32-bit value.
- `BigInt.asIntN(8, x)` reads the low 8 bits of `x` as a two's complement value.
