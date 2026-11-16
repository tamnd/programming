```typescript run id=match-guards-typescript
function describe(n: number): string {
  if (n === 0) {
    return "zero";
  }
  if (n < 0) {
    return "negative";
  }
  if (n < 10 && n % 2 === 0) {
    return "small even";
  }
  if (n < 10) {
    return "small odd";
  }
  return "large";
}

console.log([-5, 0, 4, 7, 12].map(describe).join(", "));
```

```output for=match-guards-typescript
negative, zero, small even, small odd, large
```

- Verdict: absent. TypeScript adds no pattern matching to JavaScript, so it has no guards. A chain of `if` statements tests the conditions in order.
