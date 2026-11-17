```typescript run id=extends-bound
interface Lengthwise {
  length: number;
}

function longest<T extends Lengthwise>(a: T, b: T): T {
  return b.length > a.length ? b : a;
}

function maxBy<T, K extends number | string>(items: T[], key: (x: T) => K): T {
  let best = items[0];
  for (const x of items) {
    if (key(x) > key(best)) best = x;
  }
  return best;
}

console.log(longest("ox", "cat"));
console.log(longest([1, 2, 3], [4]));
console.log(maxBy([{ n: "a", age: 30 }, { n: "b", age: 41 }], (p) => p.age).n);
```

```output for=extends-bound
cat
[ 1, 2, 3 ]
b
```

- Verdict: native. `T extends Lengthwise` accepts strings and arrays, because both have a `length` property. `K extends number | string` limits the key to types that `>` can compare. The checker removes the bounds before the program runs.
