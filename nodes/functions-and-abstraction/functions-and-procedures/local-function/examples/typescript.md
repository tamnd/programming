```typescript run id=local-function-typescript
function sumOfScaled(values: number[], factor: number): number {
  function scale(x: number): number {
    return x * factor;
  }

  let total = 0;
  for (const v of values) {
    total += scale(v);
  }
  return total;
}

console.log(sumOfScaled([1, 2, 3], 10));
console.log(sumOfScaled([4], 2));
```

```output for=local-function-typescript
60
8
```

- Verdict: native. A nested function declaration can have type annotations like any other function.
