```javascript run id=inspector
const { Session } = require("node:inspector");

// Instrumentation: wrap a function so that each call is counted.
const counts = new Map();
function counted(name, fn) {
  return function (...args) {
    counts.set(name, (counts.get(name) ?? 0) + 1);
    return fn.apply(this, args);
  };
}

const fib = counted("fib", (n) => (n < 2 ? n : fib(n - 1) + fib(n - 2)));

// Sampling: the V8 CPU profiler through the inspector protocol.
const session = new Session();
session.connect();
session.post("Profiler.enable", () => {
  session.post("Profiler.start", () => {
    const result = fib(25);
    session.post("Profiler.stop", (err, { profile }) => {
      console.log("fib(25) =", result);
      console.log("profile fields:", Object.keys(profile).sort().join(", "));
      console.log("root node:", profile.nodes[0].callFrame.functionName);
      console.log("fib calls:", counts.get("fib"));
      session.disconnect();
    });
  });
});
```

```output for=inspector
fib(25) = 75025
profile fields: endTime, nodes, samples, startTime, timeDeltas
root node: (root)
fib calls: 242785
```

- Verdict: stdlib. Node.js runs the V8 CPU profiler through `node:inspector`, and `node --cpu-prof` writes a profile of the whole program before it exits.
- The profile is a tree of nodes with a call frame each, plus a list of samples and the time between them. The example prints only the field names, because the samples change on each run.
- A wrapper function counts calls. It is instrumentation by hand, and it adds a call to each call.
