```typescript run id=hof
type Transform<T, U> = (value: T) => U;

function mapAll<T, U>(items: T[], f: Transform<T, U>): U[] {
  const result: U[] = [];
  for (const item of items) {
    result.push(f(item));
  }
  return result;
}

function compose<A, B, C>(f: Transform<B, C>, g: Transform<A, B>): Transform<A, C> {
  return (x) => f(g(x));
}

const words: string[] = ["pear", "fig", "banana"];
const lengths: number[] = mapAll(words, (w) => w.length);
console.log(lengths);
const describe = compose((n: number) => `${n} letters`, (w: string) => w.length);
console.log(mapAll(words, describe));
words.forEach((w) => console.log(w.toUpperCase()));
```

```output for=hof
[ 4, 3, 6 ]
[ '4 letters', '3 letters', '6 letters' ]
PEAR
FIG
BANANA
```

- Verdict: native. TypeScript has the functions of JavaScript and adds function types such as `(value: T) => U`.
- Generic type parameters let one higher-order function work for any element type. The compiler infers `T` and `U` from the arguments, and it infers the type of the parameter `w` from the context.
- A function with fewer parameters can take the place of a function type with more parameters. So `forEach` accepts a callback with one parameter, although `forEach` passes three arguments.
- The checker removes the types before the program runs, so the types give no checks at run time.
