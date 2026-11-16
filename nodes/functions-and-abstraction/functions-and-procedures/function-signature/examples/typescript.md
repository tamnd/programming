```typescript run id=function-signature-typescript
function average(a: number, b: number): number {
  return (a + b) / 2;
}

console.log(average(3, 4));
```

```output for=function-signature-typescript
3.5
```

- Verdict: native. The signature gives the types of the parameters and the result, and the compiler checks each call against it.
