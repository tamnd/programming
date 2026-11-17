```typescript run id=deep
function depth(n: number): number {
  return n === 0 ? 0 : 1 + depth(n - 1); // one frame for each step
}

function loop(n: number): number {
  let count = 0;
  while (n > 0) { // one frame for the whole loop
    n--;
    count++;
  }
  return count;
}

console.log("depth(100) =", depth(100));
try {
  depth(1_000_000);
} catch (e) {
  console.log("depth(1000000):", e instanceof RangeError ? "RangeError" : "another error");
}
console.log("loop(1000000) =", loop(1_000_000));
```

```output for=deep
depth(100) = 100
depth(1000000): RangeError
loop(1000000) = 1000000
```

- Verdict: native. The program runs as JavaScript, so the error is the one that the engine throws. In a `catch`, the type of `e` is `unknown`, so the code tests it with `instanceof`.
