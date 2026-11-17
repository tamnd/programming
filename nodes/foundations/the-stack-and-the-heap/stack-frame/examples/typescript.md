```typescript run id=frames
function count(n: number): void {
  const here: number = n * 10; // a local: one copy in each call
  console.log(`enter n=${n} here=${here}`);
  if (n > 1) {
    count(n - 1);
  }
  console.log(`leave n=${n} here=${here}`);
}

count(3);
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. TypeScript compiles to JavaScript, so the frames are JavaScript execution contexts. The types add nothing to the frame.
