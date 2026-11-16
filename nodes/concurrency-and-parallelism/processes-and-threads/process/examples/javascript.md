```javascript run id=child
import { spawnSync } from "node:child_process";

const script =
  'echo "child sees GREETING=$GREETING"; GREETING=changed; echo "child set GREETING=$GREETING"; exit 3';

const greeting = "hello";
const child = spawnSync("sh", ["-c", script], {
  env: { ...process.env, GREETING: greeting },
  encoding: "utf8",
});
process.stdout.write(child.stdout);
console.log("child exit status:", child.status);
console.log("child pid differs:", child.pid !== process.pid);
console.log("parent greeting:", greeting);
```

```output for=child
child sees GREETING=hello
child set GREETING=changed
child exit status: 3
child pid differs: true
parent greeting: hello
```

- Verdict: stdlib. The language has no processes; Node.js gives the module `node:child_process`.
- `spawnSync` blocks until the child ends and returns its output, its exit status and its process ID. `spawn` is the form that does not block, with events for the output and the exit.
- The `env` option gives the child its own environment. The default is a copy of `process.env`.
- Each Node.js process has its own event loop and its own heap. Worker threads share the process but not the heap.
