```javascript run id=radix-and-bigint
const n = 0b1010_1100; // binary literal with a numeric separator
console.log(n, n.toString(2), n.toString(8), n.toString(16));
console.log(parseInt("101101", 2), Number("0b1010"), parseInt("z", 36));
console.log((5).toString(2).padStart(8, "0"));

// Numbers are 64-bit floats: integers are exact only up to 2^53.
console.log(2 ** 53 === 2 ** 53 + 1, Number.MAX_SAFE_INTEGER.toString(2).length);
// Bitwise operators work on 32-bit two's complement integers.
console.log((-5 >>> 0).toString(2), (-5).toString(2));
// BigInt has no fixed width.
console.log((2n ** 70n).toString(2).length, BigInt.asUintN(8, -5n).toString(2));
```

```output for=radix-and-bigint
172 10101100 254 ac
45 10 35
00000101
true 53
11111111111111111111111111111011 -101
71 11111011
```

- Verdict: native. `toString` and `parseInt` take a radix from 2 to 36, and `Number` reads the `0b` prefix. A JavaScript number is a 64-bit float, so integers are exact only up to 2⁵³, and `Number.MAX_SAFE_INTEGER` has 53 one bits. The bitwise operators work on 32-bit two's complement, so `-5 >>> 0` shows its 32 bits, while `toString(2)` prints a minus sign. `BigInt.asUintN` gives the low bits at any width.
