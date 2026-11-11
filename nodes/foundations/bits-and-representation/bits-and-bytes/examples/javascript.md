```javascript run id=int32-bigint-and-dataview
const READ = 1 << 0, WRITE = 1 << 1, EXEC = 1 << 2;
let perm = READ | WRITE;
perm &= ~WRITE;
perm ^= EXEC;
console.log(perm.toString(2).padStart(3, "0"), (perm & EXEC) !== 0);

console.log(1 << 31, (1 << 31) >>> 0, -8 >> 1, -8 >>> 28);
console.log(1 << 32, Math.clz32(1));  // the shift count is taken mod 32
console.log(1n << 40n, (0xFFn << 56n).toString(16));

const view = new DataView(new ArrayBuffer(4));
view.setUint32(0, 0x12345678, true);  // little-endian
console.log(Array.from(new Uint8Array(view.buffer), b => b.toString(16)).join(" "));
console.log(view.getUint32(0, false).toString(16));  // read as big-endian
```

```output for=int32-bigint-and-dataview
101 true
-2147483648 2147483648 -4 15
1 31
1099511627776n ff00000000000000
78 56 34 12
78563412
```

```javascript wrong id=or-zero-truncates
const ms = 3000000000;  // 3e9 milliseconds, about 35 days
const whole = ms | 0;   // meant to drop a fraction, as Math.trunc does
console.log(whole, Math.trunc(ms));
console.log((2 ** 53 + 2) & 1, 2 ** 32 + 7 | 0);
```

```output for=or-zero-truncates
-1294967296 3000000000
0 7
```

- Verdict: native. The bitwise operators of JavaScript convert their operands to 32-bit signed integers, operate, and return a number. `>>>` shifts in zeros and gives an unsigned result, so `x >>> 0` reads the 32 bits as unsigned. The shift count is taken modulo 32, so `1 << 32` is `1`. For wider values, `BigInt` has the same operators with no fixed width. `DataView` reads and writes integers in a buffer with a stated byte order.
- The wrong block uses `x | 0` to drop the fraction of a number, a common trick. It works only for values that fit in 32 bits: 3 billion becomes a negative number, and the low bits of larger numbers vanish. Use `Math.trunc` for numbers and `BigInt` for wide bit patterns.
