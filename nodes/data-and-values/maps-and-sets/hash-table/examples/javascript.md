```javascript run id=map-keys
// A Map compares keys with SameValueZero. JavaScript gives no hash code to a program.
const m = new Map();
m.set(NaN, "nan");
m.set(0, "zero");
console.log(m.get(NaN), m.get(-0), m.size);

// Objects are keys by identity: two objects with the same fields are two keys.
const a = { x: 1, y: 2 };
const b = { x: 1, y: 2 };
const byObject = new Map([[a, "a"]]);
console.log(byObject.get(a), byObject.get(b));

// To look up by value, make a string key from the fields.
const key = (p) => `${p.x},${p.y}`;
const byValue = new Map([[key(a), "found"]]);
console.log(byValue.get(key(b)));

// A key object that changes keeps its entry, because identity does not change.
a.x = 99;
console.log(byObject.get(a));

// A WeakMap holds its object keys weakly, and primitive keys are an error.
const wm = new WeakMap();
wm.set(a, 1);
try {
  wm.set("s", 1);
} catch (e) {
  console.log(e.constructor.name);
}
```

```output
nan zero 2
a undefined
found
a
TypeError
```

- Verdict: native. Map and Set are hash tables in the engines, but the language gives no hash code to a program. Keys are compared with SameValueZero, so NaN finds NaN and -0 finds 0. Objects are keys by identity, so a program that wants keys by value makes a string key from the fields.
