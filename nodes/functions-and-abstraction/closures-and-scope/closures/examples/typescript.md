```typescript run id=counter
function makeCounter(): () => number {
  let count = 0;
  return () => {
    count += 1;
    return count;
  };
}

const first = makeCounter();
const second = makeCounter();
console.log(first(), first(), first());
console.log(second());
```

```output for=counter
1 2 3
1
```

- Verdict: native. TypeScript compiles to JavaScript and has the same closure rules.
- The return type `() => number` is a function type. The compiler infers it when the annotation is left out.
