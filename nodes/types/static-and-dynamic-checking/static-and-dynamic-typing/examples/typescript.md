```typescript run id=types
type Shape = { kind: "circle"; radius: number } | { kind: "square"; side: number };

function area(shape: Shape): number {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "square":
      return shape.side ** 2;
  }
}

function isString(value: unknown): value is string {
  return typeof value === "string";
}

console.log(area({ kind: "square", side: 3 }), area({ kind: "circle", radius: 1 }).toFixed(4));
const inputs: unknown[] = ["ab", 3, null];
for (const input of inputs) {
  console.log(isString(input) ? input.toUpperCase() : `not a string: ${input}`);
}
```

```output
9 3.1416
AB
not a string: 3
not a string: null
```

- Verdict: native. TypeScript adds static types to JavaScript. The compiler `tsc` checks them, and then it removes them from the output.
- A test such as `typeof` or a check of the `kind` field narrows a union type. A function with the return type `value is string` is a type predicate, which the compiler uses in the same way.
- The run here strips the types and does no type check. The run-time behaviour is the behaviour of JavaScript.
- `any` and `as` turn off the check. Give outside data the type `unknown` and narrow it.
