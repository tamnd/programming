```javascript run id=chmod-and-umask
const fs = require("node:fs");

// The umask removes bits from the mode of each new file.
const old = process.umask(0o027);
fs.writeFileSync("new.txt", "x", { mode: 0o666 });
const show = (p) => (fs.statSync(p).mode & 0o7777).toString(8).padStart(4, "0");
console.log("new file with umask 027:", show("new.txt"));
process.umask(old);

// chmod takes a number or a string of octal digits.
fs.chmodSync("new.txt", "755");
console.log("after chmod 755:", show("new.txt"));

// fs.constants has the bits by name.
const { S_IXGRP, S_IXOTH } = fs.constants;
fs.chmodSync("new.txt", (fs.statSync("new.txt").mode & 0o777) & ~(S_IXGRP | S_IXOTH));
console.log("after go-x:", show("new.txt"));

// access with X_OK fails when no class has the execute bit.
fs.writeFileSync("data.txt", "x");
fs.chmodSync("data.txt", 0o644);
try {
  fs.accessSync("data.txt", fs.constants.X_OK);
  console.log("data.txt: executable");
} catch (e) {
  console.log("data.txt:", e.code);
}
```

```output for=chmod-and-umask
new file with umask 027: 0640
after chmod 755: 0755
after go-x: 0744
data.txt: EACCES
```

- Verdict: stdlib. `fs.chmod` takes the mode as a number or as a string of octal digits, and `fs.constants` names the bits, such as `S_IRUSR` and `S_IXOTH`. `process.umask` reads and sets the mask of the process, which removes bits from the `mode` of each new file. `fs.access` checks the access of the process. On Windows, `chmod` changes only the write permission.
