```typescript run id=callback-typescript
function repeat(n: number, callback: (i: number) => void): void {
  for (let i = 1; i <= n; i++) {
    callback(i);
  }
}

function tick(i: number): void {
  console.log(`tick ${i}`);
}

console.log([3, 1, 2].sort((a, b) => b - a).join(" "));
repeat(2, tick);
```

```output for=callback-typescript
3 2 1
tick 1
tick 2
```

- Verdict: native. The type `(i: number) => void` tells the compiler which functions `repeat` accepts as its callback.
