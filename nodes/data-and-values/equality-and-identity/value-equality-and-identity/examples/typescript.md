```typescript run id=points
type Point = { readonly x: number; readonly y: number };

function samePoint(p: Point, q: Point): boolean {
  return p.x === q.x && p.y === q.y;
}

const a: Point = { x: 1, y: 2 };
const b: Point = { x: 1, y: 2 };
const c = a;
console.log("a === b:", a === b);
console.log("a === c:", a === c);
console.log("samePoint(a, b):", samePoint(a, b));

const key = (p: Point): string => `${p.x},${p.y}`;
const seen = new Map<string, Point>();
for (const p of [a, b, c]) seen.set(key(p), p);
console.log("distinct keys:", seen.size);
```

```output for=points
a === b: false
a === c: true
samePoint(a, b): true
distinct keys: 1
```

- Verdict: idiom. The run time is JavaScript, so `===` compares objects by identity, and value equality is a function. The types make the function easier to write correctly.
- The type checker reports a comparison of two types that have no value in common.

```typescript wrong id=no-overlap
const count: number = Number("3");
if (count === "3") {
  console.log("three");
} else {
  console.log("not three");
}
```

```output for=no-overlap
not three
```

The type checker reports error TS2367: "This comparison appears to be unintentional because the types 'number' and 'string' have no overlap." A runner that only strips the types, such as Node, runs the program, and the comparison is always false. Compare values of one type: `count === 3`.
