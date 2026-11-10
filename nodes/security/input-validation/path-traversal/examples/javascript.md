```javascript wrong id=join
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

const base = fs.mkdtempSync(path.join(os.tmpdir(), "traversal-"));
const publicDir = path.join(base, "public");
fs.mkdirSync(publicDir);
fs.writeFileSync(path.join(publicDir, "report.txt"), "quarterly report");
fs.writeFileSync(path.join(base, "secret.txt"), "the secret key");

function readFile(name) {
  return fs.readFileSync(path.join(publicDir, name), "utf8");
}

console.log(readFile("report.txt"));
console.log(readFile("../secret.txt"));
fs.rmSync(base, { recursive: true });
```

```output for=join
quarterly report
the secret key
```

```javascript run id=relative
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

const base = fs.realpathSync(fs.mkdtempSync(path.join(os.tmpdir(), "traversal-")));
const publicDir = path.join(base, "public");
fs.mkdirSync(publicDir);
fs.writeFileSync(path.join(publicDir, "report.txt"), "quarterly report");
fs.writeFileSync(path.join(base, "secret.txt"), "the secret key");
fs.symlinkSync("../secret.txt", path.join(publicDir, "link.txt"));

function readFile(name) {
  const full = fs.realpathSync(path.resolve(publicDir, name));
  const rel = path.relative(publicDir, full);
  if (rel === "" || rel.startsWith("..") || path.isAbsolute(rel)) {
    return "refused, the path is outside the public directory";
  }
  return fs.readFileSync(full, "utf8");
}

console.log(readFile("report.txt"));
console.log(readFile("../secret.txt"));
console.log(readFile(path.join(base, "secret.txt")));
console.log(readFile("link.txt"));
fs.rmSync(base, { recursive: true });
```

```output for=relative
quarterly report
refused, the path is outside the public directory
refused, the path is outside the public directory
refused, the path is outside the public directory
```

- Verdict: stdlib. `path.resolve` makes an absolute path and removes `.` and `..`. It processes its arguments from right to left and stops at the first absolute one, so an absolute name replaces the directory. `fs.realpathSync` also follows symbolic links.
- `path.relative(root, full)` gives the way from the root to the file. If it starts with `..` or is absolute, the file is outside the root. This avoids the sibling problem of a string prefix check.
- `path.join` does not stop `..`. It removes `..` by climbing up, which is the bug.
- `fs.realpathSync` fails when the file does not exist. For a file that the program will create, resolve the directory part, check it, and then add the last name.
- Check the path that the file system call gets, after all decoding of the request. A check on the raw URL can miss a name that a later step decodes to `..`.
- CodeQL query `js/path-injection` follows input from a request to file system calls.
