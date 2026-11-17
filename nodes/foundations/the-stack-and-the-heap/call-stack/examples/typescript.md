```typescript run id=trace
function where(): string {
  // Error.prototype.stack is not in the standard; Node.js writes "    at name (place)".
  const known = new Set(["a", "b", "c", "main"]);
  const names = (new Error().stack ?? "")
    .split("\n")
    .map((line) => line.trim().split(" ")[1])
    .filter((name): name is string => name !== undefined && known.has(name));
  return names.join(" <- ");
}

function c(): void {
  console.log("      stack:", where());
}

function b(): void {
  console.log("    call c");
  c();
  console.log("    back in b");
}

function a(): void {
  console.log("  call b");
  b();
  console.log("  back in a");
}

function main(): void {
  console.log("call a");
  a();
  console.log("back in main");
}

main();
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. TypeScript runs as JavaScript, so it has the same call stack and the same `stack` string. The type of `stack` is `string | undefined`, so the code gives an empty string when it is missing.
