```typescript run id=scopes
const level: string = "module";

function outer(): string {
  const level = "outer";
  const inner = (): string => level;
  return inner();
}

function readsModule(): string {
  return level;
}

function caller(): string {
  const level = "caller";
  return readsModule() + " (the local " + level + " is not seen)";
}

console.log(outer());
console.log(caller());

for (let i = 0; i < 2; i++) {
  const i2 = i * 2;
  console.log("block", i, i2);
}
```

```output for=scopes
outer
module (the local caller is not seen)
block 0 0
block 1 2
```

- Verdict: native. The run time is JavaScript, so the scope rules are the same. Code in TypeScript usually uses only `let` and `const`, which have block scope.
- The type checker reports a use of a block-scoped variable before its declaration when it can see it in the text, which catches many temporal dead zone errors before the program runs.
