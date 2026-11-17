```typescript run id=to-the-interface
function total(xs: Iterable<number>): number {
  let sum = 0;
  for (const x of xs) sum += x;
  return sum;
}

type Sized = { readonly size: number } | { readonly length: number };

function describe(c: Iterable<number> & Sized): string {
  const size = "size" in c ? c.size : c.length;
  return `size ${size}, total ${total(c)}`;
}

const list: readonly number[] = [1, 2, 3];
const set: ReadonlySet<number> = new Set([4, 5]);
const prices: ReadonlyMap<string, number> = new Map([["tea", 3], ["cake", 4]]);

console.log("list:", describe(list));
console.log("set:", describe(set));
console.log("map values:", describe([...prices.values()]));
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
```

- Verdict: native. `Iterable<T>`, `ReadonlyArray<T>`, `ReadonlySet<T>` and `ReadonlyMap<K, V>` are structural types: a value matches when it has the members, with no `implements` clause.
- The read-only types have no `push`, `add` or `set`, so the checker stops code that takes them from changing them.
- TypeScript has no shared size interface either, so `Sized` accepts `size` or `length`.
