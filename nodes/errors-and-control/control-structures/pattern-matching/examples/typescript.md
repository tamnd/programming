```typescript run id=points
type Point = readonly [number, number];

function describe([x, y]: Point): string {
  if (x === 0 && y === 0) return "origin";
  if (y === 0) return `on the x axis at ${x}`;
  if (x === 0) return `on the y axis at ${y}`;
  if (x === y) return `on the diagonal at ${x}`;
  return `at ${x}, ${y}`;
}

const points: Point[] = [[0, 0], [3, 0], [0, -2], [4, 4], [1, 5]];
for (const point of points) {
  console.log(`(${point.join(", ")}): ${describe(point)}`);
}
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
```

- Verdict: idiom. TypeScript has no `match`. Destructuring in the parameter list takes the tuple apart, and a chain of `if` tests does the matching.
- For a union with a `kind` field, a `switch` on `kind` narrows the type in each branch. [[sum-types]] shows this form and its exhaustiveness check.
- The TC39 pattern matching proposal, at Stage 1, would add a `match` expression to JavaScript and so to TypeScript.
