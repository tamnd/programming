```javascript run id=parser-combinators
// Each parser takes (text, pos) and returns [value, newPos], or null on failure.
const lit = (s) => (t, i) => (t.startsWith(s, i) ? [s, i + s.length] : null);
const num = (t, i) => {
  const re = /\d+/y;
  re.lastIndex = i;
  const m = re.exec(t);
  return m ? [Number(m[0]), re.lastIndex] : null;
};
const seq = (...ps) => (t, i) => {
  const out = [];
  for (const p of ps) {
    const r = p(t, i);
    if (!r) return null;
    out.push(r[0]);
    i = r[1];
  }
  return [out, i];
};
const alt = (...ps) => (t, i) => {
  for (const p of ps) {
    const r = p(t, i);
    if (r) return r;
  }
  return null;
};
const many = (p) => (t, i) => {
  const out = [];
  for (let r; (r = p(t, i)); i = r[1]) out.push(r[0]);
  return [out, i];
};
const opt = (p) => (t, i) => p(t, i) ?? [null, i];
const map = (p, f) => (t, i) => {
  const r = p(t, i);
  return r && [f(r[0]), r[1]];
};

// list  = "[" [ item { "," item } ] "]" .
// item  = number | list .
const item = (t, i) => alt(num, list)(t, i);
const rest = many(map(seq(lit(","), item), ([, x]) => x));
const items = map(seq(item, rest), ([x, xs]) => [x, ...xs]);
const list = map(seq(lit("["), opt(items), lit("]")), ([, xs]) => xs ?? []);

for (const text of ["[1,[2,3],[]]", "[10,20]", "[1,]", "[1 2]"]) {
  const r = list(text, 0);
  const ok = r && r[1] === text.length;
  console.log(text.padEnd(13), ok ? JSON.stringify(r[0]) : "syntax error");
}
```

```output for=parser-combinators
[1,[2,3],[]]  [1,[2,3],[]]
[10,20]       [10,20]
[1,]          syntax error
[1 2]         syntax error
```

- Verdict: library. Peggy, nearley, Chevrotain and the JavaScript target of ANTLR turn grammars into parsers. The example writes parser combinators, small functions that each match one construct and combine like the operators of EBNF: `seq` for a sequence, `alt` for a choice, `many` for braces and `opt` for brackets. The code for `list` and `item` then reads like the two grammar rules in the comment above it, and each rule returns the value that the rule describes.
