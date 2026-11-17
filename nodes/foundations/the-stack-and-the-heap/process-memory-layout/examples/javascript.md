```javascript run id=regions
const fs = require("node:fs");

const found = new Map();
for (const line of fs.readFileSync("/proc/self/maps", "utf8").trim().split("\n")) {
  const fields = line.trim().split(/\s+/);
  const path = fields[5] ?? "";
  if (["[heap]", "[stack]", "[vdso]"].includes(path) && !found.has(path)) {
    found.set(path, BigInt("0x" + fields[0].split("-")[0])); // addresses need more than 53 bits
    console.log(path, fields[1]);
  }
}
console.log("the heap is below the stack:", found.get("[heap]") < found.get("[stack]"));
```

```output for=regions
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

- Verdict: native. JavaScript has no addresses, but Node.js runs in a process with the usual layout, and it can read `/proc/self/maps` as a file.
- An address can be larger than `Number.MAX_SAFE_INTEGER`, so the program compares them as `BigInt` values.
