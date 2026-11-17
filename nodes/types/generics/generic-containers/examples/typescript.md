```typescript run id=typed-containers
const scores: Array<number> = [90, 85];
const same: number[] = scores;
const byName = new Map<string, number[]>();

byName.set("ann", same);
byName.get("ann")?.push(77);

for (const [name, list] of byName) {
  console.log(name, list.length, list.reduce((a, b) => a + b, 0));
}
console.log(new Set<string>(["a", "b", "a"]).size);
```

```output for=typed-containers
ann 3 252
2
```

- Verdict: native. `Array<number>` and `number[]` are the same type. `Map<string, number[]>` and `Set<string>` take type arguments. The checker removes the types before the program runs.
