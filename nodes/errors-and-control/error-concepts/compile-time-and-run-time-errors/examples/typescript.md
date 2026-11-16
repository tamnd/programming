```typescript run id=when-typescript
const items: number[] = [10, 20, 30];
for (const i of [1, 5]) {
  console.log(`items[${i}] = ${items[i]}`);
}
```

```output for=when-typescript
items[1] = 20
items[5] = undefined
```

- Verdict: absent. The TypeScript compiler finds type errors before the program runs, but here the type of `items[i]` is `number`, so the compiler does not report the index past the end. The run-time result is `undefined`, as in JavaScript.
