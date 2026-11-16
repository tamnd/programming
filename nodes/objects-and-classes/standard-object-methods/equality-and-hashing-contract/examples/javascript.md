```javascript run id=map-keys-are-identity
const m = new Map();
m.set({ x: 1 }, "first");
m.set({ x: 1 }, "second");
console.log(m.size, m.get({ x: 1 }));

const byKey = new Map();
for (const p of [{ x: 1 }, { x: 1 }, { x: 2 }]) {
  byKey.set(JSON.stringify(p), p);
}
console.log(byKey.size, byKey.has(JSON.stringify({ x: 1 })));

const nums = new Set([NaN, NaN, 0, -0]);
console.log(nums.size, NaN === NaN);
```

```output for=map-keys-are-identity
2 undefined
2 true
2 false
```

- Verdict: absent. A `Map` or `Set` compares keys with SameValueZero, which for objects is identity, and code cannot give a class its own equality or hash.
- Two objects with the same fields are two keys. The usual work-around is a key of a primitive type, such as a string built from the fields; `JSON.stringify` works here only because the property order is the same.
- SameValueZero treats `NaN` as equal to `NaN` and `-0` as equal to `0`, so the set of numbers has 2 items although `NaN === NaN` is false.
