```javascript run id=shapes
function area(shape) {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "rect":
      return shape.width * shape.height;
    case "triangle":
      return (shape.base * shape.height) / 2;
    default:
      throw new Error(`unknown shape: ${shape.kind}`);
  }
}

const shapes = [
  { kind: "circle", radius: 1 },
  { kind: "rect", width: 2, height: 3 },
  { kind: "triangle", base: 4, height: 5 },
];
for (const shape of shapes) {
  console.log(area(shape).toFixed(2));
}

try {
  area({ kind: "square", side: 2 });
} catch (e) {
  console.log(e.message);
}
```

```output for=shapes
3.14
6.00
10.00
unknown shape: square
```

- Verdict: idiom. A plain object with a `kind` field acts as a tagged union, and a `switch` on the tag selects the case.
- Nothing checks that the cases are complete or that an object has the fields of its tag. The `default` branch that throws is the only guard, and it works at run time.
