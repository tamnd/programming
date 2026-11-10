```javascript fails expect=exception:TypeError id=undefined-property
const users = new Map([
  ["alice", { quota: 10 }],
  ["bob", { quota: 20 }],
]);

for (const name of ["alice", "nobody"]) {
  const user = users.get(name); // undefined when the key is missing
  console.log(`${name}: quota ${user.quota}`);
}
```

`Map.prototype.get` returns `undefined` for a missing key. Reading a property of `undefined` throws `TypeError: Cannot read properties of undefined`.

```javascript run id=optional-chaining
const users = new Map([
  ["alice", { quota: 10 }],
  ["bob", { quota: 20 }],
]);

for (const name of ["alice", "nobody"]) {
  const quota = users.get(name)?.quota;
  if (quota === undefined) {
    console.log(`${name}: no such user`);
    continue;
  }
  console.log(`${name}: quota ${quota}`);
}
```

```output for=optional-chaining
alice: quota 10
nobody: no such user
```

- Verdict: native. `null` and `undefined` exist in the language, and the error is found only at run time. The operator `?.` stops and gives `undefined` when the value on its left is `null` or `undefined`.
- `?.` hides the absence. Handle the `undefined` result, as the fixed block does, so that the absence does not move to a later line.
- Use TypeScript with `strictNullChecks` to find a missing check before the code runs.
