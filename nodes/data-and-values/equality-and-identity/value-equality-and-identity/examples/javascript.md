```javascript run id=points
const a = { x: 1, y: 2 };
const b = { x: 1, y: 2 };
const c = a;
console.log("a === b:", a === b);
console.log("a === c:", a === c);

const samePoint = (p, q) => p.x === q.x && p.y === q.y;
console.log("samePoint(a, b):", samePoint(a, b));
console.log("distinct objects:", new Set([a, b, c]).size);

const key = (p) => `${p.x},${p.y}`;
console.log("distinct keys:", new Set([a, b, c].map(key)).size);

console.log("NaN === NaN:", NaN === NaN);
console.log("Object.is(NaN, NaN):", Object.is(NaN, NaN));
console.log("[NaN].indexOf(NaN):", [NaN].indexOf(NaN));
console.log("[NaN].includes(NaN):", [NaN].includes(NaN));
console.log("0 === -0:", 0 === -0);
console.log("Object.is(0, -0):", Object.is(0, -0));
```

```output for=points
a === b: false
a === c: true
samePoint(a, b): true
distinct objects: 2
distinct keys: 1
NaN === NaN: false
Object.is(NaN, NaN): true
[NaN].indexOf(NaN): -1
[NaN].includes(NaN): true
0 === -0: true
Object.is(0, -0): false
```

- Verdict: idiom. `===` compares primitives by value and objects by identity. JavaScript has no hook that changes what `===` does for a class, so value equality of objects is a function that the program writes.
- A `Set` and a `Map` compare keys with SameValueZero, which is identity for objects. A common idiom is a string key made from the fields.
- There are four equality algorithms. `==` converts types. `===` does not. `Object.is` treats `NaN` as equal to itself and `0` as different from `-0`. SameValueZero, used by `includes`, `Set` and `Map`, treats `NaN` as equal to itself and `0` as equal to `-0`.

```javascript wrong id=loose
console.log(0 == "");
console.log("0" == false);
console.log(null == undefined);
console.log(null == 0);
console.log([] == false);
console.log("1,2" == [1, 2]);
```

```output for=loose
true
true
true
false
true
true
```

`==` converts the operands before it compares them. A string becomes a number, a boolean becomes a number and an object becomes a primitive. So `0 == ""` is true, and an empty array is equal to `false`. `null` is loosely equal to `undefined` and to nothing else. Use `===`.

```javascript wrong id=object-key
const visits = new Map();
visits.set([1, 2], "first");
console.log(visits.get([1, 2]));
console.log(visits.size);
```

```output for=object-key
undefined
1
```

Each array literal makes a new array, and the map compares keys by identity. The second `[1, 2]` is a different key, so `get` finds nothing. Use a string key such as `"1,2"`, or keep the array object and use it again.
