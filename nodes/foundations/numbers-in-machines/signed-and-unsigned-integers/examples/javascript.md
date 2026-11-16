```javascript run id=views
console.log("-1 >>> 0:", -1 >>> 0);
console.log("-9 >>> 2:", -9 >>> 2);

const bytes = new Uint8Array([200, 255, 256, -1]);
console.log("Uint8Array:", bytes.join(" "));
console.log("Int8Array view:", new Int8Array(bytes.buffer).join(" "));
```

```output for=views
-1 >>> 0: 4294967295
-9 >>> 2: 1073741821
Uint8Array: 200 255 0 255
Int8Array view: -56 -1 0 -1
```

- Verdict: idiom. A JavaScript number is not an integer of a fixed width, so there is no unsigned number type.
- `x >>> 0` gives the unsigned 32-bit value of `x`. The operator `>>>` fills the left bits with zeros.
- A `Uint8Array` stores each value modulo 256, so 256 becomes 0 and -1 becomes 255. An `Int8Array` on the same buffer reads the same bytes as signed values.
