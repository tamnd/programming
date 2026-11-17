```typescript run id=erased
function first<T>(items: T[]): T | undefined {
  return items[0];
}

const n = first<number>([3, 4]);
const s = first(["a", "b"]);
console.log(n, s);
console.log(first.length, typeof first);
```

```output for=erased
3 a
1 function
```

- Verdict: native. The checker uses `T` to give `n` the type `number | undefined` and `s` the type `string | undefined`. The JavaScript that runs has one `first` function with one parameter and no type parameter.
