```javascript run id=write-at-position
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

const dir = fs.mkdtempSync(path.join(os.tmpdir(), "sparse-"));
const file = path.join(dir, "sparse.bin");

const fd = fs.openSync(file, "w");
// A write at a position past the end leaves a hole before it.
fs.writeSync(fd, Buffer.from("end"), 0, 3, 64 * 1024 * 1024);
fs.closeSync(fd);

const st = fs.statSync(file);
console.log("size:           ", st.size);
// stats.blocks counts 512-byte units on POSIX systems.
console.log("uses less space:", st.blocks * 512 < st.size);

const data = fs.readFileSync(file);
console.log("first byte:     ", data[0]);
console.log("last bytes:     ", data.subarray(-3).toString());
fs.rmSync(dir, { recursive: true });
```

```output for=write-at-position
size:            67108867
uses less space: true
first byte:      0
last bytes:      end
```

- Verdict: stdlib. The `position` argument of `fs.writeSync` writes past the end of the file, and `fs.ftruncateSync` makes a file longer. `stats.blocks` gives the allocated space. Node.js has no `SEEK_DATA` or `SEEK_HOLE`.
