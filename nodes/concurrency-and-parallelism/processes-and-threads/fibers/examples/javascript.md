```javascript run id=generator-fringe
function* walk(tree) {
  if (Array.isArray(tree)) {
    for (const child of tree) {
      yield* walk(child); // each level of the recursion is a generator
    }
  } else {
    yield tree;
  }
}

function sameFringe(x, y) {
  const gx = walk(x);
  const gy = walk(y);
  for (;;) {
    const a = gx.next();
    const b = gy.next();
    if (a.done !== b.done || a.value !== b.value) return false;
    if (a.done) return true;
  }
}

const a = [[1, 2], [3, [4]]];
const b = [1, [2, [3, 4]]];
const c = [[1, 2], [3, 5]];

for (const leaf of walk(a)) {
  console.log("got", leaf);
}
console.log("same fringe a b:", sameFringe(a, b));
console.log("same fringe a c:", sameFringe(a, c));
```

```output for=generator-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: absent. JavaScript has no fibers. Generator functions are the alternative, and like Python generators they are stackless.
- `yield*` delegates to the inner generator, so each level of the recursion must be a `function*`.
- `next()` returns an object with `value` and `done`, so the caller can see when a walk is finished.
