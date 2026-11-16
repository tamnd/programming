```typescript run id=function-object-typescript
type Multiplier = {
  (x: number): number;
  calls: number;
};

function makeMultiplier(factor: number): Multiplier {
  const multiply: Multiplier = Object.assign(
    (x: number): number => {
      multiply.calls += 1;
      return x * factor;
    },
    { calls: 0 },
  );
  return multiply;
}

const triple = makeMultiplier(3);
console.log(triple(5));
console.log([1, 2, 3].map((x) => triple(x)).join(" "));
console.log(triple.calls);
```

```output for=function-object-typescript
15
3 6 9
4
```

- Verdict: native. The type `Multiplier` has a call signature and a property, so it describes a function that is also an object with a field. `Object.assign` adds the property `calls` to the arrow function.
