```typescript run id=trace
function f<T>(name: string, value: T): T {
  console.log("evaluate", name);
  return value;
}

const point = { x: f("x", 1), y: f("y", 2) };
const sum = f("left", point.x) + f("right", point.y);
console.log(sum);
```

```output for=trace
evaluate x
evaluate y
evaluate left
evaluate right
3
```

- Verdict: native. TypeScript compiles to JavaScript and keeps its order of evaluation. Property values in an object literal and the operands of `+` are evaluated from left to right.
- The type checker does not look at side effects, so it does not warn about code that depends on the order.
