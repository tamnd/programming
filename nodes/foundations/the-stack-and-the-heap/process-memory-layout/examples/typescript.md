```typescript run id=regions
const fs = require("node:fs");

const found = new Map<string, bigint>();
const text: string = fs.readFileSync("/proc/self/maps", "utf8");
for (const line of text.trim().split("\n")) {
  const fields: string[] = line.trim().split(/\s+/);
  const path: string = fields[5] ?? "";
  if (["[heap]", "[stack]", "[vdso]"].includes(path) && !found.has(path)) {
    found.set(path, BigInt("0x" + fields[0].split("-")[0])); // addresses need more than 53 bits
    console.log(path, fields[1]);
  }
}
console.log("the heap is below the stack:", found.get("[heap]")! < found.get("[stack]")!);
```

```output for=regions
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

- Verdict: native. The program runs as JavaScript in Node.js, so it sees the same regions.
