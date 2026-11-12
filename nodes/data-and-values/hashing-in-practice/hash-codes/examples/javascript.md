```javascript run id=keys-by-identity
// Map and Set compare object keys by identity, not by contents
const byObject = new Map([[{ x: 1, y: 2 }, "a"]]);
console.log("object key:", byObject.get({ x: 1, y: 2 }));

// The idiom: derive a string key from the fields that define equality
const key = (p) => `${p.x},${p.y}`;
const byKey = new Map([[key({ x: 1, y: 2 }), "a"]]);
console.log("string key:", byKey.get(key({ x: 1, y: 2 })));

// SameValueZero: NaN finds NaN, and -0 is the same key as +0
const m = new Map([[NaN, "nan"], [0, "zero"]]);
console.log(m.get(NaN), m.get(-0), new Set([1, "1", 1.0]).size);

// A plain object turns every key into a string
const counts = {};
for (const k of [1, "1", true, "true"]) counts[k] = (counts[k] ?? 0) + 1;
console.log(counts);
```

```output for=keys-by-identity
object key: undefined
string key: a
nan zero 2
{ '1': 2, true: 2 }
```

- Verdict: idiom. `Map` and `Set` hash their keys inside the engine, but a program cannot define equality or a hash for its own types: objects are equal only to themselves, under the SameValueZero rule. The idiom derives a primitive key, usually a string, from the fields that define equality. A plain object converts every key to a string, so `1` and `"1"` are one key.
