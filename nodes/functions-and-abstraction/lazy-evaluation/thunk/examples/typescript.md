```typescript run id=thunks
type Thunk<T> = () => T;

function lazy<T>(compute: Thunk<T>): Thunk<T> {
  let state: { done: false; compute: Thunk<T> } | { done: true; value: T } = { done: false, compute };
  return () => {
    if (!state.done) {
      state = { done: true, value: state.compute() };
    }
    return state.value;
  };
}

let calls = 0;
const total: Thunk<number> = lazy(() => {
  calls += 1;
  return [1, 2, 3].reduce((a, b) => a + b, 0);
});

console.log(total(), total(), calls);
```

```output for=thunks
6 6 1
```

- Verdict: idiom. The type `() => T` is a plain thunk. The union type of `state` makes the two states of a memoized thunk explicit: before the force it holds code, and after the force it holds a value.
