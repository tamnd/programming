```javascript run id=refutable-javascript
for (const line of ["width=80", "oops", "height=24"]) {
  // Array destructuring never fails: a missing element is undefined.
  const [key, value] = line.split("=", 2);
  if (value === undefined) {
    console.log(`skip: ${line}`);
    continue;
  }
  console.log(`${key} -> ${value}`);
}
```

```output for=refutable-javascript
width -> 80
skip: oops
height -> 24
```

- Verdict: absent. JavaScript has no patterns that can fail and no `match` statement. Array destructuring gives `undefined` for a missing element, so the program tests the value after the destructuring. Destructuring `null` or `undefined` throws a `TypeError`.
