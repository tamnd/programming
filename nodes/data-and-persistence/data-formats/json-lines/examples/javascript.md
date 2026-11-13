```javascript run id=read-write
import { createReadStream, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { createInterface } from "node:readline";

// JSON.stringify writes U+2028 and U+2029 as themselves. The escapes \u2028 and \u2029 are also valid JSON.
const toLine = (value) =>
  JSON.stringify(value).replace(/\u2028/g, "\\u2028").replace(/\u2029/g, "\\u2029") + "\n";

async function readLines(path) {
  // crlfDelay: Infinity makes \r\n one line end, also when the bytes come in two reads.
  const lines = createInterface({ input: createReadStream(path), crlfDelay: Infinity });
  const out = [];
  for await (const line of lines) out.push(line);
  return out;
}

const dir = mkdtempSync(join(tmpdir(), "jsonl-"));
try {
  const events = [{ level: "info", note: "a\u2028b" }, { level: "warn", n: 2 }];
  const last = '{"level": "info", "n": 3}\r\n';

  const raw = join(dir, "raw.jsonl");
  writeFileSync(raw, events.map((e) => JSON.stringify(e) + "\n").join("") + last);
  console.log("raw:", (await readLines(raw)).length, "lines for 3 records");

  const safe = join(dir, "safe.jsonl");
  writeFileSync(safe, events.map(toLine).join("") + last);
  let number = 0;
  for (const line of await readLines(safe)) {
    const value = JSON.parse(line);
    console.log(++number, value.level, value.note === "a\u2028b");
  }
} finally {
  rmSync(dir, { recursive: true, force: true });
}
```

```output for=read-write
raw: 4 lines for 3 records
1 info true
2 warn false
3 info false
```

- Verdict: stdlib. `JSON.stringify` writes one line, and `readline.createInterface` with `for await` reads one line at a time from a stream. The last record ends with `\r\n`, and with `crlfDelay: Infinity` the reader sees one line end. `JSON.stringify` writes U+2028 in the first record as itself, not as an escape, and `readline` ends a line at U+2028 and U+2029 as well as at `\n` and `\r`. So the raw file gives 4 lines for 3 records. The function `toLine` writes the two characters as the escapes `\u2028` and `\u2029`, which are valid JSON, and then each record is one line.
