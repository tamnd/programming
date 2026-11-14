```javascript run id=split-and-readline
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const readline = require("node:readline");

const text = "one\ntwo\r\nthree\rfour";
// split("\n") leaves the CR of a CRLF in the line and does not split at a CR alone.
console.log(JSON.stringify(text.split("\n")));
console.log(JSON.stringify(text.split(/\r\n|\r|\n/)));
console.log("os.EOL is", JSON.stringify(os.EOL));

// readline ends a line at LF, CR or CRLF. crlfDelay: Infinity keeps a CRLF as one ending.
const dir = fs.mkdtempSync(path.join(os.tmpdir(), "eol-"));
const file = path.join(dir, "mixed.txt");
fs.writeFileSync(file, text);
const lines = [];
const rl = readline.createInterface({ input: fs.createReadStream(file), crlfDelay: Infinity });
rl.on("line", (line) => lines.push(line));
rl.on("close", () => {
  console.log(JSON.stringify(lines));
  fs.rmSync(dir, { recursive: true });
});
```

```output for=split-and-readline
["one","two\r","three\rfour"]
["one","two","three","four"]
os.EOL is "\n"
["one","two","three","four"]
```

- Verdict: stdlib. A JavaScript string has no line functions, so `split` with the pattern `/\r\n|\r|\n/` splits at all three endings. In Node.js, `os.EOL` is the ending of the platform, and the `readline` module ends a line at LF, CR or CRLF. With `crlfDelay: Infinity`, a CR and the LF after it always count as one ending, also when time passes between them.
