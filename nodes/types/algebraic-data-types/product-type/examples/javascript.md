```javascript run id=parts
const yn = (b) => (b ? "yes" : "no");

const t = [3, 4]; // parts by position
const [x, y] = t;
console.log(`array: x = ${x} y = ${y}`);

const p = { x: 3, y: 4 }; // parts by name
console.log(`object: x = ${p.x} y = ${p.y}`);

const same = { x: 3, y: 4 };
const eq = (a, b) => a.x === b.x && a.y === b.y;
console.log("same parts, ===: " + yn(p === same));
console.log("same parts, by field: " + yn(eq(p, same)));
console.log("swapped parts, by field: " + yn(eq(p, { x: 4, y: 3 })));
```

```output for=parts
array: x = 3 y = 4
object: x = 3 y = 4
same parts, ===: no
same parts, by field: yes
swapped parts, by field: no
```

- Verdict: idiom. An object is a product by name, and an array of fixed length is a product by position by convention.
- `===` on two objects compares identity, so equal fields are not enough. A function compares the fields.
