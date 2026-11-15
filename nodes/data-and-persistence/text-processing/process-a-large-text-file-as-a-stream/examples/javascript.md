```javascript run id=javascript-stream
import { createReadStream, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { createInterface } from "node:readline";

async function stream(path) {
  const lines = createInterface({ input: createReadStream(path), crlfDelay: Infinity });
  let count = 0, total = 0, largest;
  for await (const line of lines) { // one line at a time, with no LF
    const n = Number(line);
    count++;
    total += n;
    largest = largest === undefined ? n : Math.max(largest, n);
  }
  return { count, total, largest };
}

const dir = mkdtempSync(join(tmpdir(), "stream-"));
try {
  const path = join(dir, "numbers.txt");
  const parts = [];
  for (let i = 0; i < 1_000_000; i++) parts.push(`${i % 1000}\n`);
  writeFileSync(path, parts.join(""));
  console.log(await stream(path));
} finally {
  rmSync(dir, { recursive: true });
}
```

```output
{ count: 1000000, total: 499500000, largest: 999 }
```

`readline.createInterface` reads a stream from `createReadStream` and gives one line at a time, with no LF, to `for await`. With `crlfDelay: Infinity`, a CR and an LF are one end of line. The file of numbers is written with one call, so this example needs memory for the whole file when it writes it, but not when it reads it.
