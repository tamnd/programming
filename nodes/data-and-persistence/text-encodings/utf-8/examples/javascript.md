```javascript run id=lengths
const text = "nai\u0308ve \u{1F469}\u200D\u{1F4BB}";

const bytes = new TextEncoder().encode(text);
console.log("UTF-8 bytes", bytes.length);
console.log("UTF-16 code units", text.length);
console.log("code points", [...text].length);

const segmenter = new Intl.Segmenter("en", { granularity: "grapheme" });
const graphemes = [...segmenter.segment(text)].map((part) => part.segment);
console.log("grapheme clusters", graphemes.length);

const composed = text.normalize("NFC");
console.log("after NFC", composed.length, new TextEncoder().encode(composed).length);
```

```output for=lengths
UTF-8 bytes 19
UTF-16 code units 12
code points 10
grapheme clusters 7
after NFC 11 18
```

```javascript run id=decode
const bad = new Uint8Array([0x61, 0xc0, 0xaf, 0xed, 0xa0, 0x80, 0x62]);
console.log(JSON.stringify(new TextDecoder().decode(bad)));

try {
  new TextDecoder("utf-8", { fatal: true }).decode(bad);
} catch (error) {
  console.log(error.name, error instanceof TypeError);
}

const withBom = new Uint8Array([0xef, 0xbb, 0xbf, 0x68, 0x69]);
console.log(new TextDecoder().decode(withBom).length);
console.log(new TextDecoder("utf-8", { ignoreBOM: true }).decode(withBom).length);

const lone = "a\uD800b";
console.log(lone.isWellFormed(), lone.toWellFormed() === "a\uFFFDb");
const encoded = new TextEncoder().encode(lone);
console.log([...encoded].map((byte) => byte.toString(16).padStart(2, "0")).join(" "));
```

```output for=decode
"a�����b"
TypeError true
2
3
false true
61 ef bf bd 62
```

- Verdict: native. `TextEncoder` and `TextDecoder` of the WHATWG Encoding Standard convert between strings and UTF-8 bytes. Browsers, Node.js and Deno have them.
- A JavaScript string is a sequence of UTF-16 code units. `length` counts code units. The spread `[...text]` iterates by code point. `Intl.Segmenter` with the granularity `"grapheme"` finds the grapheme clusters.
- By default `TextDecoder` replaces each maximal subpart of an ill-formed sequence with U+FFFD. The option `fatal: true` makes it throw a `TypeError`.
- The decoder removes a leading byte order mark. The option `ignoreBOM: true` keeps it in the string.
- A string can hold a lone surrogate. `TextEncoder` writes it as `EF BF BD`, the encoding of U+FFFD. `isWellFormed` finds lone surrogates, and `toWellFormed` replaces them.
