```typescript run id=stack
// The frames of this file on the stack, innermost first. Node removes the
// types and keeps each line where it was, so the line numbers in a stack
// trace are the line numbers of this TypeScript file.
interface Frame {
  name: string;
  line: number;
}

function whereAmI(): Frame[] {
  const lines: string[] = (new Error().stack ?? "").split("\n").slice(2);
  const frames: Frame[] = [];
  for (const text of lines) {
    const match = text.match(/at (\S+) \((.*):(\d+):\d+\)$/);
    if (match !== null && match[2] === import.meta.url) {
      frames.push({ name: match[1], line: Number(match[3]) });
    }
  }
  return frames;
}

type Item = "apple" | "bread";

function price(item: Item): number {
  if (item === "bread") {
    debugger; // a breakpoint under a debugger; no effect without one
    for (const frame of whereAmI()) {
      console.log(`  ${frame.name} line ${frame.line}`);
    }
    return 250;
  }
  return 30;
}

function total(cart: Item[]): number {
  let sum = 0;
  for (const item of cart) {
    sum += price(item);
  }
  return sum;
}

console.log("total:", total(["apple", "bread"]));
```

```output for=stack
  price line 26
  total line 37
total: 280
```

- Verdict: native. TypeScript has the `debugger` statement of JavaScript, and the program runs under the debugger of its JavaScript engine.
- Node runs this file after it removes the types. The line numbers in the trace match the TypeScript source, so no source map is needed.
- The module is an ES module, so a stack frame names the file with a `file:` URL. The example compares it with `import.meta.url`.
