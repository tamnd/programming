```javascript run id=intl-segmenter
const t = "cafe\u0301 \u{1F1EB}\u{1F1F7} \u{1F469}\u{1F3FD}\u200d\u{1F692}";

// length counts UTF-16 code units, and the spread counts code points.
console.log(t.length, [...t].length);

// Intl.Segmenter with granularity "grapheme" finds the user-perceived characters.
const seg = new Intl.Segmenter("en", { granularity: "grapheme" });
const parts = [...seg.segment(t)];
console.log(parts.length);
console.log(parts.map((p) => `${p.index}:${[...p.segment].length}`).join(" "));

// containing(i) finds the grapheme that holds code unit i.
const g = seg.segment(t).containing(12);
console.log(g.index, g.segment === "\u{1F469}\u{1F3FD}\u200d\u{1F692}");

// Cut to the first 4 graphemes, not to the first 4 code units.
console.log(parts.slice(0, 4).map((p) => p.segment).join(""), t.slice(0, 4));
```

```output for=intl-segmenter
18 13
8
0:1 1:1 2:1 3:2 5:1 6:2 10:1 11:4
11 true
café cafe
```

- Verdict: stdlib. The text is 18 code units and 13 code points. `Intl.Segmenter` with `granularity: "grapheme"` finds 8 segments, and each segment has an `index` in code units. `containing(12)` finds the emoji that starts at index 11. A cut to 4 segments keeps `é`, and `slice(0, 4)` loses the accent.
