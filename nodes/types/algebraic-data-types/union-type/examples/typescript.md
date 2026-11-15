```typescript run id=either
function describe(value: number | string): string {
  if (typeof value === "number") {
    return `number ${value}, doubled ${value * 2}`;
  }
  return `text ${value}, upper ${value.toUpperCase()}`;
}

type Shape = { kind: "circle"; radius: number } | { kind: "square"; side: number };

function area(shape: Shape): number {
  switch (shape.kind) {
    case "circle":
      return Math.round(Math.PI * shape.radius ** 2);
    case "square":
      return shape.side ** 2;
  }
}

for (const v of [21, "ok"]) {
  console.log(describe(v));
}
console.log(`areas: ${area({ kind: "circle", radius: 2 })} ${area({ kind: "square", side: 3 })}`);
```

```output for=either
number 21, doubled 42
text ok, upper OK
areas: 13 9
```

- Verdict: native. `number | string` is a union type. Inside the `if`, `typeof` narrows `value` to `number`, and after it to `string`.
- `Shape` is a discriminated union: the common field `kind` has a different literal type in each member, so a `switch` on it narrows the shape.
- The types exist only at compile time. At run time the values are plain numbers, strings and objects.
