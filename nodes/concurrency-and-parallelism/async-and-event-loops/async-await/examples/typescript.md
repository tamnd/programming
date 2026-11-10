```typescript run id=load
const sleep = (ms: number): Promise<void> =>
  new Promise((resolve) => setTimeout(resolve, ms));

async function load(name: string, ms: number): Promise<string> {
  console.log("start", name);
  await sleep(ms);
  console.log("end", name);
  return name;
}

async function main(): Promise<void> {
  const results = await Promise.all([load("slow", 200), load("fast", 100)]);
  console.log(results.join(", "));
}

main();
```

```output for=load
start slow
start fast
end fast
end slow
slow, fast
```

- Verdict: native. TypeScript has the async functions of JavaScript, and it types the result as `Promise<T>`.
- The return type of an async function must be a `Promise` type. A `return name` in `load` gives a `Promise<string>`.
- The type checker finds some missing awaits, for example a `Promise<number>` used where a `number` is needed.
