```typescript run id=options
interface Options {
  host: string;
  port?: number;
  retries?: number;
}

function connect({ host, port = 5432, retries = 3 }: Options): string {
  return `${host}:${port} retries=${retries}`;
}

function sum(first: number, ...rest: number[]): number {
  return rest.reduce((total, n) => total + n, first);
}

console.log(connect({ host: "db" }));
console.log(connect({ host: "db", retries: 0 }));
console.log(sum(1, 2, 3));
```

```output for=options
db:5432 retries=3
db:5432 retries=0
6
```

- Verdict: native. TypeScript keeps the rules of JavaScript, and the type checker adds a check of the number and types of the arguments. A call with a missing required argument is error TS2554.
- An optional parameter has a `?` or a default value. An options object with an interface is the usual way to get named arguments.
- The runner of this page strips the types without a check, so the page cannot show the error as a `fails` block.
