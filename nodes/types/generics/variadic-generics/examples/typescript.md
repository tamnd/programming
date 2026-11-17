```typescript run id=variadic-tuples
function concat<T extends unknown[], U extends unknown[]>(a: [...T], b: [...U]): [...T, ...U] {
  return [...a, ...b];
}

function tail<T extends unknown[]>(arr: readonly [unknown, ...T]): T {
  const [, ...rest] = arr;
  return rest as T;
}

const joined = concat([1, "a"], [true]); // [number, string, boolean]
const rest = tail([0, "x", false] as const); // ["x", false]
console.log(joined, joined.length);
console.log(rest);
```

```output for=variadic-tuples
[ 1, 'a', true ] 3
[ 'x', false ]
```

- Verdict: native. `T` and `U` stand for tuple types of any length, and `[...T, ...U]` joins them, so the checker knows the type of each element of `joined`. The types are removed before the program runs.
