```javascript run id=objects-and-arrays
// An object is a record with named properties, and an array holds items by index.
const p = { name: "Ann", age: 31, tags: ["admin"] };
console.log(typeof p, Array.isArray(p.tags), p.tags.length);

// === compares objects by reference, not by contents.
const a = [1, 2];
const b = [1, 2];
console.log(a === b, a === a, JSON.stringify(a) === JSON.stringify(b));

// A spread copy is shallow: the inner array is shared.
const q = { ...p };
q.name = "Bob";
q.tags.push("dev");
console.log(p.name, p.tags);

// structuredClone makes a deep copy.
const r = structuredClone(p);
r.tags.push("ops");
console.log(p.tags.length, r.tags.length);

// A Map takes keys of any type, and it compares object keys by reference.
const m = new Map([[a, "first"]]);
console.log(m.get(a), m.get(b));
```

```output for=objects-and-arrays
object true 1
false true true
Ann [ 'admin', 'dev' ]
2 3
first undefined
```

- Verdict: native. Objects and arrays are the composite values of JavaScript, and `typeof` gives `"object"` for both. `===` is true only for the same object, so two arrays with the same items are not equal. A spread copy gets its own `name`, but it shares the array in `tags`. `structuredClone` copies the inner array too. A `Map` finds an object key by reference, so `b` does not find the entry of `a`.
