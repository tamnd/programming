```typescript run id=first-class-function-typescript
type IntFn = (x: number) => number;

function double(x: number): number {
  return x * 2;
}

const square: IntFn = (x) => x * x;

function applyTwice(g: IntFn, x: number): number {
  return g(g(x));
}

function adder(n: number): IntFn {
  return (x) => x + n;
}

const f: IntFn = double;
console.log(f(5));

const ops: { [name: string]: IntFn } = { double, square };
console.log(ops["square"](4));

console.log(applyTwice(double, 3));

const add5 = adder(5);
console.log(add5(10));
```

```output for=first-class-function-typescript
10
16
12
15
```

- Verdict: native. The function type expression `(x: number) => number` is the type of the variables, the parameter `g` and the result of `adder`.
