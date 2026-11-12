```javascript run id=typed-arrays-and-dataview
// Two views of the same byte: unsigned and two's complement.
const buf = new ArrayBuffer(1);
const u8 = new Uint8Array(buf);
const i8 = new Int8Array(buf);
u8[0] = 251;
console.log(u8[0], i8[0]);
i8[0] = 200; // stored modulo 256, then read as signed
console.log(i8[0], u8[0]);

// Bitwise operators convert to a 32-bit signed integer.
console.log(2 ** 31 | 0, (2 ** 32 + 5) | 0, -1 >>> 0);
console.log(Math.imul(0x7fffffff, 2)); // 32-bit multiply that wraps

// BigInt wraps only when asked.
console.log(BigInt.asIntN(8, 200n), BigInt.asUintN(64, -1n));

// A DataView reads integers of a given size and byte order.
const v = new DataView(new ArrayBuffer(2));
v.setInt16(0, -1000); // big-endian by default
console.log(v.getUint8(0).toString(16), v.getUint8(1).toString(16), v.getInt16(0, true));
```

```output for=typed-arrays-and-dataview
251 -5
-56 200
-2147483648 5 4294967295
-2
-56n 18446744073709551615n
fc 18 6396
```

- Verdict: native. A JavaScript number is a 64-bit float, but the bitwise operators convert it to a 32-bit two's complement integer, so `2 ** 31 | 0` is negative, and `>>> 0` reads the bits as unsigned. A `Uint8Array` and an `Int8Array` on the same buffer read one byte as 251 and as −5. `Math.imul` multiplies with 32-bit wraparound, `BigInt.asIntN` wraps a `BigInt` to a given width, and a `DataView` reads and writes in either byte order.
