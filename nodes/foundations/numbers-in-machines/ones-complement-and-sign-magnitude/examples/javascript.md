```javascript run id=forms
const toSignMagnitude = (n) => (n < 0 ? 0x80 | -n : n);
const toOnes = (n) => (n < 0 ? ~-n & 0xff : n);
const fromSignMagnitude = (b) => (b & 0x80 ? -(b & 0x7f) : b);
const fromOnes = (b) => (b & 0x80 ? -(~b & 0xff) : b);
const bin8 = (b) => b.toString(2).padStart(8, "0");
const yesNo = (b) => (b ? "yes" : "no");

console.log(`-43 in sign-magnitude: ${bin8(toSignMagnitude(-43))}`);
console.log(`-43 in ones' complement: ${bin8(toOnes(-43))}`);
console.log(`10101011 in sign-magnitude: ${fromSignMagnitude(0b10101011)}`);
console.log(`11010100 in ones' complement: ${fromOnes(0b11010100)}`);
console.log(`10000000 in sign-magnitude: ${fromSignMagnitude(0b10000000)}`);
console.log(`11111111 in ones' complement: ${fromOnes(0b11111111)}`);

const z = fromSignMagnitude(0b10000000);
console.log(`the result is -0: ${yesNo(Object.is(z, -0))}`);
console.log(`-0 === 0: ${yesNo(-0 === 0)}`);
console.log(`Object.is(-0, 0): ${yesNo(Object.is(-0, 0))}`);
console.log(`1 / -0: ${1 / -0}`);
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
the result is -0: yes
-0 === 0: yes
Object.is(-0, 0): no
1 / -0: -Infinity
```

- Verdict: idiom. The JavaScript bitwise operators use 32-bit two's complement values, so a program makes the other two forms by hand.
- A JavaScript number is a float, so `-(0)` gives `-0`. A template string prints `-0` as `0`, but `Object.is` sees the difference.
- `-0 === 0` is true, so code that must find a negative zero uses `Object.is` or `1 / x`.
