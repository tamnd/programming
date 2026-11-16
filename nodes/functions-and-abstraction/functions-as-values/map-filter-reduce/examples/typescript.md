```typescript run id=map-filter-reduce-typescript
const nums: number[] = [1, 2, 3, 4, 5, 6];
const squares: number[] = nums
  .filter((x) => x % 2 === 0)
  .map((x) => x * x);
console.log(squares.join(" "));
const total: number = squares.reduce((acc, x) => acc + x, 0);
console.log(total);
```

```output for=map-filter-reduce-typescript
4 16 36
56
```

- Verdict: native. The array methods of JavaScript, with types. TypeScript gets the types of `x` and `acc` from the array and the initial value.
