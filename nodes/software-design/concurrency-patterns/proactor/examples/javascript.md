```javascript run id=proactor
const { readFile, writeFile, mkdtemp } = require("node:fs/promises");
const { tmpdir } = require("node:os");
const path = require("node:path");

function onRead(name, size) {
  console.log(`${name} done: ${size} bytes`);
}

async function main() {
  const dir = await mkdtemp(path.join(tmpdir(), "proactor-"));
  await writeFile(path.join(dir, "a.txt"), "hello");
  await writeFile(path.join(dir, "b.txt"), "abc");
  const reads = ["a.txt", "b.txt"].map((name) => {
    console.log("start read", name);
    // the runtime does the read; the promise is the completion
    return readFile(path.join(dir, name)).then((data) => [name, data.length]);
  });
  for (const done of reads) {
    const [name, size] = await done; // the event loop dispatches completions
    onRead(name, size);
  }
}

main();
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: native. A call to `fs.promises.readFile` starts the operation and returns at once. The event loop of the runtime is the completion dispatcher, and the code after `await` is the completion handler.
