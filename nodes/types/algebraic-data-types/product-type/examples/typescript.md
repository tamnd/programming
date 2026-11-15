```typescript run id=parts
type Size = [width: number, height: number]; // parts by position
type Point = { readonly x: number; readonly y: number }; // parts by name

const yn = (b: boolean): string => (b ? "yes" : "no");

const s: Size = [640, 480];
const [w, h] = s;
console.log(`tuple: width = ${w} height = ${h} length = ${s.length}`);

const p: Point = { x: 3, y: 4 };
const same: Point = { x: 3, y: 4 };
const eq = (a: Point, b: Point): boolean => a.x === b.x && a.y === b.y;
console.log(`object: x = ${p.x} y = ${p.y}`);
console.log("same parts, ===: " + yn(p === same));
console.log("same parts, by field: " + yn(eq(p, same)));
```

```output for=parts
tuple: width = 640 height = 480 length = 2
object: x = 3 y = 4
same parts, ===: no
same parts, by field: yes
```

- Verdict: native. A tuple type such as `[width: number, height: number]` is a product by position, and an object type is a product by name.
- The labels of a tuple type are for reading only. The values are still read by position.
- The types do not change equality at run time: `===` still compares identity.
