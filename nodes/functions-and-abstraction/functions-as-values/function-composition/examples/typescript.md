```typescript run id=function-composition-typescript
function compose<A, B, C>(f: (b: B) => C, g: (a: A) => B): (a: A) => C {
  return (x) => f(g(x));
}

const inc = (x: number): number => x + 1;
const double = (x: number): number => x * 2;
const len = (s: string): number => s.length;

console.log(compose(double, inc)(5));
console.log(compose(inc, double)(5));
console.log(compose(inc, compose(double, len))("hello"));
```

```output for=function-composition-typescript
12
11
11
```

- Verdict: idiom. TypeScript has no compose function. The type parameters `A`, `B` and `C` make the compiler check that `f` accepts what `g` returns, so `compose(double, len)` takes a string and gives a number.
