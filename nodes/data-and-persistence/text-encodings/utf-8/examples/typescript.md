```typescript run id=validate
type Result = { ok: true } | { ok: false; offset: number };

function sequenceLength(lead: number): number {
  if (lead <= 0x7f) return 1;
  if (lead >= 0xc2 && lead <= 0xdf) return 2;
  if (lead >= 0xe0 && lead <= 0xef) return 3;
  if (lead >= 0xf0 && lead <= 0xf4) return 4;
  return 0;
}

function secondByteRange(lead: number): [number, number] {
  switch (lead) {
    case 0xe0: return [0xa0, 0xbf];
    case 0xed: return [0x80, 0x9f];
    case 0xf0: return [0x90, 0xbf];
    case 0xf4: return [0x80, 0x8f];
    default: return [0x80, 0xbf];
  }
}

function validate(bytes: Uint8Array): Result {
  let index = 0;
  while (index < bytes.length) {
    const length = sequenceLength(bytes[index]);
    if (length === 0 || index + length > bytes.length) {
      return { ok: false, offset: index };
    }
    for (let next = 1; next < length; next++) {
      const [low, high] = next === 1 ? secondByteRange(bytes[index]) : [0x80, 0xbf];
      const byte = bytes[index + next];
      if (byte < low || byte > high) {
        return { ok: false, offset: index };
      }
    }
    index += length;
  }
  return { ok: true };
}

const samples: Record<string, number[]> = {
  "A e-acute euro clef": [0x41, 0xc3, 0xa9, 0xe2, 0x82, 0xac, 0xf0, 0x9d, 0x84, 0x9e],
  "overlong slash": [0x2f, 0xc0, 0xaf],
  "overlong euro": [0xe0, 0x82, 0xac],
  "surrogate D800": [0xed, 0xa0, 0x80],
  "above 10FFFF": [0xf4, 0x90, 0x80, 0x80],
  "truncated": [0x41, 0xe2, 0x82],
};

const strict = new TextDecoder("utf-8", { fatal: true });
for (const [name, list] of Object.entries(samples)) {
  const bytes = new Uint8Array(list);
  let agrees: boolean;
  try {
    strict.decode(bytes);
    agrees = validate(bytes).ok;
  } catch {
    agrees = !validate(bytes).ok;
  }
  console.log(name.padEnd(20), JSON.stringify(validate(bytes)), agrees);
}
```

```output for=validate
A e-acute euro clef  {"ok":true} true
overlong slash       {"ok":false,"offset":1} true
overlong euro        {"ok":false,"offset":0} true
surrogate D800       {"ok":false,"offset":0} true
above 10FFFF         {"ok":false,"offset":0} true
truncated            {"ok":false,"offset":1} true
```

- Verdict: native. TypeScript runs on JavaScript, so it has `TextEncoder`, `TextDecoder` and `Intl.Segmenter`. The example is an idiom that shows the rules of the format.
- The function `validate` follows Table 3-7 of the Unicode Standard. The lead byte gives the length. The range of the second byte depends on the lead byte, and this one check rejects overlong forms, surrogates and values above U+10FFFF.
- The bytes C0, C1 and F5 to FF can never start a sequence, so `sequenceLength` returns 0 for them.
- The last column compares the result with `TextDecoder` in its fatal mode. The two agree on each sample.
