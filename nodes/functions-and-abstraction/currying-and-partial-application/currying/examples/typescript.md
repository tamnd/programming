```typescript run id=typed
function curry2<A, B, R>(f: (a: A, b: B) => R): (a: A) => (b: B) => R {
  return (a) => (b) => f(a, b);
}

function partial<A, B, R>(f: (a: A, b: B) => R, a: A): (b: B) => R {
  return (b) => f(a, b);
}

const repeat = (s: string, n: number): string => s.repeat(n);
const stars = curry2(repeat)("*");
const dashes = partial(repeat, "-");
console.log(stars(3), dashes(5));
```

```output for=typed
*** -----
```

- Verdict: native. The rules are those of JavaScript. Generic helpers keep the types of the parameters and of the result, so the type checker rejects `stars("3")`.
- A generic `curry` for functions with any number of parameters needs variadic tuple types and conditional types, and its error messages are hard to read. Most code writes small closures.
