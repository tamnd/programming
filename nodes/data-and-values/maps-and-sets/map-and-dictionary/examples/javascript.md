```javascript run id=map-and-object
// A Map takes keys of any type and keeps insertion order.
const m = new Map([["x", 1], ["y", 2]]);
m.set("z", 3);
m.set("x", 10);
console.log(m.get("x"), m.get("w"), m.has("y"), m.size);
m.delete("y");
console.log([...m]);

// Map keys keep their type: the number 1 and the string "1" are two keys.
const n = new Map();
n.set(1, "number").set("1", "string");
console.log(n.size, n.get(1), n.get("1"));

// Object keys are strings, so 1 and "1" are the same key.
const o = {};
o[1] = "number";
o["1"] = "string";
console.log(Object.keys(o), o[1]);

// A plain object has keys from its prototype, such as toString.
const counts = {};
console.log("toString" in counts, typeof counts.toString);
const safe = Object.create(null);
console.log("toString" in safe);

// Map compares keys with SameValueZero: NaN finds NaN, and an object is found by identity.
const k = { id: 1 };
const byObj = new Map([[NaN, "nan"], [k, "obj"]]);
console.log(byObj.get(NaN), byObj.get(k), byObj.get({ id: 1 }));
```

```output
10 undefined true 3
[ [ 'x', 10 ], [ 'z', 3 ] ]
2 number string
[ '1' ] string
true function
false
nan obj undefined
```

- Verdict: native. A Map keeps the type of each key, so 1 and "1" are two keys, and it compares keys with SameValueZero. A plain object turns each key into a string and has keys such as toString from its prototype. Object.create(null) makes an object with no prototype.
