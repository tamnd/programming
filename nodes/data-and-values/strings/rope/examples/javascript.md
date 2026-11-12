```javascript run id=persistent-versions-and-undo
// Each edit makes a new rope and keeps the old one, so an undo history is cheap.
const leaf = (s) => ({ s, n: s.length });
const EMPTY = leaf("");
const concat = (a, b) => (a.n === 0 ? b : b.n === 0 ? a : { left: a, right: b, n: a.n + b.n });

function split(t, i) {
  if (i <= 0) return [EMPTY, t];
  if (i >= t.n) return [t, EMPTY];
  if (t.s !== undefined) return [leaf(t.s.slice(0, i)), leaf(t.s.slice(i))];
  if (i < t.left.n) {
    const [a, b] = split(t.left, i);
    return [a, concat(b, t.right)];
  }
  const [a, b] = split(t.right, i - t.left.n);
  return [concat(t.left, a), b];
}

const insert = (t, i, s) => {
  const [a, b] = split(t, i);
  return concat(concat(a, leaf(s)), b);
};
const remove = (t, i, j) => {
  const [a, rest] = split(t, i);
  const [, b] = split(rest, j - i);
  return concat(a, b);
};
const text = (t) => (t.s !== undefined ? t.s : text(t.left) + text(t.right));
const leafCount = (t) => (t.s !== undefined ? 1 : leafCount(t.left) + leafCount(t.right));

const history = [leaf("let x = 1;")];
const edit = (f) => history.push(f(history.at(-1)));
edit((t) => remove(t, 4, 5));
edit((t) => insert(t, 4, "total"));
edit((t) => insert(t, t.n, " // sum"));

for (const [k, v] of history.entries()) {
  console.log(k, v.n, leafCount(v), JSON.stringify(text(v)));
}
// Undo is a step back in the array. No version was copied or changed.
console.log("undo:", JSON.stringify(text(history[history.length - 2])));
```

```output for=persistent-versions-and-undo
0 10 1 "let x = 1;"
1 9 2 "let  = 1;"
2 14 3 "let total = 1;"
3 21 4 "let total = 1; // sum"
undo: "let total = 1;"
```

- Verdict: idiom. JavaScript has no rope type. Each edit returns a new rope and does not change the old one, so the array `history` holds every version of the text, and an undo reads the version before the last. The edits split leaves, so the last version has 4 leaves.
