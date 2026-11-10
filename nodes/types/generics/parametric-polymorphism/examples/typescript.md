```typescript run id=first
interface HasLength {
  length: number;
}

function first<T>(items: readonly T[]): T | undefined {
  return items[0];
}

function longest<T extends HasLength>(a: T, b: T): T {
  return a.length >= b.length ? a : b;
}

const n: number | undefined = first([3, 1, 2]);
console.log(n);
console.log(longest("apple", "fig"));
console.log(longest([1, 2], [1, 2, 3]).join(" "));
```

```output for=first
3
apple
1 2 3
```

- Verdict: native. Functions, classes, interfaces and type aliases take type parameters in angle brackets. The compiler usually infers the type arguments from the arguments of a call.
- `T extends HasLength` is a constraint. The body may read `length` because every allowed type has it.
- The compiler erases all types. The JavaScript that runs has no `T`, so a type argument cannot be read or checked at run time (see Pitfalls on the concept page).
- This runner strips the types and does not type-check, so a type error here does not stop the run.
