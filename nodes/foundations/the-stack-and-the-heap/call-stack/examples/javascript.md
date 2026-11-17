```javascript run id=trace
function where() {
  // Error.prototype.stack is not in the standard; Node.js writes "    at name (place)".
  const names = new Error().stack
    .split("\n")
    .map((line) => line.trim().split(" ")[1])
    .filter((name) => ["a", "b", "c", "main"].includes(name));
  return names.join(" <- ");
}

function c() {
  console.log("      stack:", where());
}

function b() {
  console.log("    call c");
  c();
  console.log("    back in b");
}

function a() {
  console.log("  call b");
  b();
  console.log("  back in a");
}

function main() {
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

- Verdict: native. The engine keeps the call stack. A program sees it only through the `stack` string of an `Error`, which is not in the standard, so the code keeps only the names it knows.
- The text of each line depends on the engine. This example reads the format of Node.js.
