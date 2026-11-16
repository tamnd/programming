```typescript run id=function-pointer-and-delegate-typescript
type BinOp = (a: number, b: number) => number;

function add(a: number, b: number): number {
  return a + b;
}

function mul(a: number, b: number): number {
  return a * b;
}

function apply(f: BinOp, a: number, b: number): number {
  return f(a, b);
}

let op: BinOp = add;
console.log(op(6, 7));
op = mul;
console.log(op(6, 7));
console.log(apply((a, b) => a - b, 6, 7));
```

```output for=function-pointer-and-delegate-typescript
13
42
-1
```

- Verdict: native. The type alias `BinOp` gives the type of the function values, as a function pointer type does in C.
