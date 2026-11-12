```javascript run id=utf16-index-and-segmenter
const s = "naïve 😀";

// s[i] and charAt give one UTF-16 code unit.
console.log(s[2], s.length, JSON.stringify(s[6]), JSON.stringify(s.charAt(7)));

// codePointAt reads a whole code point if i is at its start.
console.log(s.codePointAt(6).toString(16), s.codePointAt(7).toString(16));

// at accepts a negative index. It also counts code units.
console.log(JSON.stringify(s.at(-1)), s.at(20));

// A spread makes an array of code points, which takes O(n) time.
const cps = [...s];
console.log(cps.length, cps[6]);

// Intl.Segmenter finds the user-perceived characters (graphemes).
const t = "cafe\u0301 🇫🇷";
const graphemes = [...new Intl.Segmenter("en", { granularity: "grapheme" }).segment(t)].map((g) => g.segment);
console.log(t.length, [...t].length, graphemes.length, graphemes[3]);
```

```output for=utf16-index-and-segmenter
ï 8 "\ud83d" "\ude00"
1f600 de00
"\ude00" undefined
7 😀
10 8 6 é
```

- Verdict: native. `s[2]` is `ï`, but `s.length` is 8 and `s[6]` is only the high surrogate of the emoji. `codePointAt(6)` reads the whole code point, and `codePointAt(7)` reads the low surrogate. `at(-1)` counts from the end in code units, and `at(20)` is `undefined`. The spread gives 7 code points. `Intl.Segmenter` finds 6 graphemes in a text of 10 code units and 8 code points.
