```javascript run id=fields
function bits32(x) {
  const view = new DataView(new ArrayBuffer(4));
  view.setFloat32(0, x);
  return view.getUint32(0);
}

function bits64(x) {
  const view = new DataView(new ArrayBuffer(8));
  view.setFloat64(0, x);
  return view.getBigUint64(0);
}

for (const x of [1.0, -2.5, 0.1]) {
  const b32 = bits32(x).toString(16).padStart(8, "0");
  const b64 = bits64(x).toString(16).padStart(16, "0");
  console.log(`${String(x).padStart(4)} binary32 ${b32} binary64 ${b64}`);
}

const b = bits64(-2.5);
const exponent = Number((b >> 52n) & 0x7ffn) - 1023;
const fraction = (b & ((1n << 52n) - 1n)).toString(16);
console.log(`-2.5: sign ${b >> 63n}, exponent ${exponent}, fraction ${fraction}`);

console.log("Math.fround(0.1):", Math.fround(0.1));
console.log("Math.f16round(0.1):", Math.f16round(0.1));
console.log("Math.f16round(65504):", Math.f16round(65504));
console.log("Math.f16round(65520):", Math.f16round(65520));
```

```output for=fields
   1 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary32 c0200000 binary64 c004000000000000
 0.1 binary32 3dcccccd binary64 3fb999999999999a
-2.5: sign 1, exponent 1, fraction 4000000000000
Math.fround(0.1): 0.10000000149011612
Math.f16round(0.1): 0.0999755859375
Math.f16round(65504): 65504
Math.f16round(65520): Infinity
```

- Verdict: native. A JavaScript number is a binary64 value.
- `DataView` writes a number into bytes as binary32 or binary64, and reads the same bytes back as an unsigned integer. The default byte order of `DataView` is big-endian, so the hex digits are in the order of the standard: sign, exponent, fraction.
- `Math.fround` and `Math.f16round` round a number to binary32 and binary16, and return the result as a binary64 number again. The value of 0.1 in binary16 has only 11 significant bits, so it is 0.0999755859375.
- 65520 is halfway between the largest binary16 value, 65504, and 65536. Ties to even goes to 65536, which is too large, so the result is `Infinity`.
