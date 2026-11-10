```typescript run id=shapes
type Shape =
  | { kind: "circle"; radius: number }
  | { kind: "rect"; width: number; height: number }
  | { kind: "triangle"; base: number; height: number };

function area(shape: Shape): number {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "rect":
      return shape.width * shape.height;
    case "triangle":
      return (shape.base * shape.height) / 2;
    default: {
      const unreachable: never = shape;
      throw new Error(`unknown shape: ${JSON.stringify(unreachable)}`);
    }
  }
}

const shapes: Shape[] = [
  { kind: "circle", radius: 1 },
  { kind: "rect", width: 2, height: 3 },
  { kind: "triangle", base: 4, height: 5 },
];
for (const shape of shapes) {
  console.log(area(shape).toFixed(2));
}

const fromJson = JSON.parse('{"kind":"square","side":2}') as Shape;
try {
  area(fromJson);
} catch (e) {
  console.log((e as Error).message);
}
```

```output for=shapes
3.14
6.00
10.00
unknown shape: {"kind":"square","side":2}
```

- Verdict: native. A union of object types that share a literal `kind` property is a discriminated union, and a check of `kind` narrows the type in each branch.
- The assignment to a `never` variable makes the compiler check that the cases are complete. If a case is missing, `tsc` reports that the missing type is not assignable to `never`. The runner of these examples strips the types and does not run `tsc`.
- The types do not exist at run time. Data from `JSON.parse` can have any tag, so the `throw` in the `default` branch is still needed.
