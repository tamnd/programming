```javascript run id=thunks
function lazy(compute) {
  let done = false;
  let value;
  return () => {
    if (!done) {
      value = compute();
      done = true;
      compute = undefined;
    }
    return value;
  };
}

const plain = () => {
  console.log("loading");
  return 21;
};
const memo = lazy(plain);

console.log(plain() + plain());
console.log(memo() + memo());
```

```output for=thunks
loading
loading
42
loading
42
```

- Verdict: idiom. An arrow function with no parameters is a plain thunk. `lazy` wraps it in a closure that stores the result and drops `compute`, so the garbage collector can free what the computation captured.
- A getter that replaces itself with a data property on first access is the usual memoized property.
