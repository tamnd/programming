```typescript run id=refutable-typescript
for (const line of ["width=80", "oops", "height=24"]) {
  // Array destructuring never fails: a missing element is undefined.
  const [key, value]: (string | undefined)[] = line.split("=", 2);
  if (value === undefined) {
    console.log(`skip: ${line}`);
    continue;
  }
  console.log(`${key} -> ${value}`);
}
```

```output for=refutable-typescript
width -> 80
skip: oops
height -> 24
```

- Verdict: absent. As in JavaScript, array destructuring never fails, and a missing element is `undefined`. The type annotation makes the compiler require the test for `undefined`.
