```javascript run id=packed
function toPacked(n) {
  const digits = [...String(Math.abs(n))].map(Number);
  digits.push(n < 0 ? 0xd : 0xc);
  if (digits.length % 2) {
    digits.unshift(0);
  }
  const out = new Uint8Array(digits.length / 2);
  for (let i = 0; i < out.length; i++) {
    out[i] = (digits[2 * i] << 4) | digits[2 * i + 1];
  }
  return out;
}

function fromPacked(bytes) {
  let value = 0;
  for (let i = 0; i < bytes.length; i++) {
    const hi = bytes[i] >> 4;
    const lo = bytes[i] & 0x0f;
    if (hi > 9) {
      throw new RangeError("not packed BCD");
    }
    value = value * 10 + hi;
    if (i < bytes.length - 1) {
      if (lo > 9) {
        throw new RangeError("not packed BCD");
      }
      value = value * 10 + lo;
    } else if (lo === 0xd) {
      value = -value;
    }
  }
  return value;
}

const hex = (b) => [...b].map((x) => x.toString(16).toUpperCase().padStart(2, "0")).join(" ");
const p = toPacked(-12345);
console.log(`packed -12345: ${hex(p)}`);
console.log(`back: ${fromPacked(p)}`);
console.log(`0x09 + 0x08 in binary: ${(0x09 + 0x08).toString(16)}`);
try {
  fromPacked(Uint8Array.of(0x1a, 0x3c));
} catch (e) {
  console.log(`1A 3C: ${e.message}`);
}
```

```output for=packed
packed -12345: 12 34 5D
back: -12345
0x09 + 0x08 in binary: 11
1A 3C: not packed BCD
```

- Verdict: idiom. JavaScript has no BCD type. The block packs the digits into a `Uint8Array` with shifts and masks.
- A plain binary addition of two BCD bytes gives `0x11`, not 17, so a BCD adder needs the add-6 correction.
