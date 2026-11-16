```typescript run id=switch-ts
type Shape =
  | { kind: "circle"; radius: number }
  | { kind: "square"; side: number }
  | { kind: "rectangle"; width: number; height: number };

function area(shape: Shape): number {
  switch (shape.kind) {
    case "circle":
      // Here shape has the type { kind: "circle"; radius: number }.
      return Math.round(Math.PI * shape.radius ** 2);
    case "square":
      return shape.side ** 2;
    case "rectangle":
      return shape.width * shape.height;
  }
}

const shapes: Shape[] = [
  { kind: "circle", radius: 2 },
  { kind: "square", side: 3 },
  { kind: "rectangle", width: 2, height: 5 },
];
for (const shape of shapes) {
  console.log(shape.kind, area(shape));
}
```

```output for=switch-ts
circle 13
square 9
rectangle 10
```

- Verdict: native. TypeScript has the `switch` of JavaScript. In each case, the compiler narrows `shape` to the member of the union whose `kind` is that string, so `shape.radius` is allowed only in the case for `"circle"`. The three cases cover the union, so the compiler accepts the function with no `return` after the `switch`.
