```javascript run id=utf16-and-segmenter
const s = "cafe\u0301 \u{1F44D}\u{1F3FD}";
console.log(s.length, [...s].length, new TextEncoder().encode(s).length);

const seg = new Intl.Segmenter("en", { granularity: "grapheme" });
console.log([...seg.segment(s)].length);

console.log(s.normalize("NFC").length, s.normalize("NFC") === "caf\u00e9 \u{1F44D}\u{1F3FD}");
console.log(s.codePointAt(6).toString(16), s.charCodeAt(6).toString(16), s.charCodeAt(7).toString(16));
```

```output for=utf16-and-segmenter
10 8 15
6
9 true
1f44d d83d dc4d
```

```javascript wrong id=split-surrogate-pair
const s = "\u{1F44D}\u{1F3FD} done";
const cut = s.slice(0, 3); // three UTF-16 units: one emoji and half of the next
console.log(cut.length, cut.isWellFormed());
console.log(encodeURIComponent(s.slice(0, 2)));
try {
  encodeURIComponent(cut);
} catch (e) {
  console.log(e.name);
}
```

```output for=split-surrogate-pair
3 false
%F0%9F%91%8D
URIError
```

- Verdict: native. A JavaScript string is a sequence of UTF-16 code units, so `length` counts units: 10 for the sample, where each emoji takes two. Spreading a string with `...` iterates over code points, and `Intl.Segmenter` with `granularity: "grapheme"` gives the characters a reader sees. `codePointAt` reads a whole code point, while `charCodeAt` reads one unit, here the high surrogate `d83d`.
- The wrong block cuts a string after three units, which splits the second emoji between its two surrogates. The result is not well-formed, and `encodeURIComponent` throws `URIError` for it. Cut strings at grapheme boundaries from `Intl.Segmenter`, or at least at code point boundaries.
