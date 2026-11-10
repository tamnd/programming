```typescript run id=traced
// Instrumentation: traced() wraps a function and records the path of calls
// that led to each call, one line per path, as a flame graph expects.
const stack: string[] = [];
const folded = new Map<string, number>();

function traced<A extends unknown[], R>(name: string, fn: (...args: A) => R): (...args: A) => R {
  return (...args: A): R => {
    stack.push(name);
    const path = stack.join(";");
    folded.set(path, (folded.get(path) ?? 0) + 1);
    try {
      return fn(...args);
    } finally {
      stack.pop();
    }
  };
}

const parse = traced("parse", (line: string): string[] => line.split(","));
const check = traced("check", (fields: string[]): boolean => fields.every((f) => f !== ""));
const load = traced("load", (lines: string[]): number => lines.filter((line) => check(parse(line))).length);
const main = traced("main", (): void => {
  console.log("good lines:", load(["a,b", "c,d", "e,"]));
  check(["x"]);
});

main();
for (const [path, count] of [...folded].sort(([a], [b]) => (a < b ? -1 : 1))) {
  console.log(path, count);
}
```

```output for=traced
good lines: 2
main 1
main;check 1
main;load 1
main;load;check 3
main;load;parse 3
```

- Verdict: stdlib. TypeScript runs on a JavaScript engine and uses its profiler: `node --cpu-prof` or `node:inspector`, as in the JavaScript example.
- The generic `traced` keeps the parameter types and the return type of the function that it wraps, so the wrapped function has the same type.
- `finally` pops the stack also when the function throws.
