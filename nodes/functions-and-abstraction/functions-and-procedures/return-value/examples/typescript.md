```typescript run id=return-value-typescript
function absolute(n: number): number {
  if (n < 0) {
    return -n;
  }
  return n;
}

console.log(absolute(-7));
console.log(absolute(5) + 1);
```

```output for=return-value-typescript
7
6
```

- Verdict: native. The return type follows the parameter list. The compiler checks that each `return` gives a value of that type.
