```typescript run id=anonymous-function-typescript
const words: string[] = ["house", "car", "bike"];
console.log([...words].sort((a, b) => a.length - b.length).join(" "));

console.log([1, 2, 3].map((x) => x * x).join(" "));

const n = 5;
const add = (x: number): number => x + n;
console.log(add(10));
```

```output for=anonymous-function-typescript
car bike house
1 4 9
15
```

- Verdict: native. The arrow functions have no type annotations. TypeScript gets the types of `a`, `b` and `x` from the calls to `sort` and `map`.
