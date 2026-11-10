```javascript run id=stack
const inspector = require("node:inspector");

// The frames of this file on the stack, innermost first, without the paths.
// A line of a V8 stack trace reads "at name (file:line:column)".
function whereAmI() {
  const lines = new Error().stack.split("\n").slice(2); // skip the message and whereAmI
  return lines
    .map((line) => line.match(/at (\S+) \((.*):(\d+):\d+\)$/))
    .filter((match) => match !== null && match[2] === __filename)
    .map(([, name, , line]) => `${name} line ${line}`);
}

function price(item) {
  if (item === "bread") {
    debugger; // stops here under a debugger; does nothing without one
    console.log("  " + whereAmI().join(" <- "));
    return 250;
  }
  return 30;
}

function total(cart) {
  return cart.reduce((sum, item) => sum + price(item), 0);
}

console.log("total:", total(["apple", "bread"]));
// inspector.url() returns undefined when no inspector is listening.
console.log("inspector url:", inspector.url());
```

```output for=stack
  price line 16 <- total line 23 <- Object.<anonymous> line 26
total: 280
inspector url: undefined
```

V8 keeps a fixed number of frames in the `stack` of an error. A deep chain of calls loses its start.

```javascript wrong id=stack-limit
// A recursive parser fails deep in the input. V8 keeps only the top 10
// frames of a stack trace by default, so the start of the chain is lost.
function parse(depth) {
  if (depth === 0) {
    throw new Error("unexpected end of input");
  }
  return parse(depth - 1);
}

try {
  parse(15);
} catch (err) {
  const frames = err.stack.split("\n").slice(1);
  console.log("frames in the trace:", frames.length);
  console.log("frames of parse:", frames.filter((f) => f.includes("at parse")).length);
}
```

```output for=stack-limit
frames in the trace: 10
frames of parse: 10
```

The call chain had 16 frames of `parse` and one more for the top level, but the trace kept 10. The fix is `Error.stackTraceLimit = Infinity` before the error is made, or the `--stack-trace-limit` option of V8.

- Verdict: native. The `debugger` statement is part of the language. It stops the program when a debugger is active and has no effect otherwise.
- `node inspect script.js` starts the command-line debugger of Node. `node --inspect-brk script.js` stops at the first line and waits for a client such as Chrome DevTools.
- The `node:inspector` module opens and closes the inspector from the code. `inspector.url()` returns `undefined` when it is closed.
