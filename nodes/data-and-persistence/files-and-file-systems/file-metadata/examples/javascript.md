```javascript run id=stats-and-lstat
const fs = require("node:fs");

fs.writeFileSync("data.txt", "hello");
fs.chmodSync("data.txt", 0o640);
fs.utimesSync("data.txt", new Date(1700000000000), new Date(1700000000000));
fs.linkSync("data.txt", "second-name.txt");
fs.symlinkSync("data.txt", "link");

// statSync follows the link, lstatSync does not.
const st = fs.statSync("link");
const lst = fs.lstatSync("link");
console.log("stat:  file", st.isFile(), "size", st.size,
  "mode", (st.mode & 0o777).toString(8), "links", st.nlink);
console.log("lstat: symlink", lst.isSymbolicLink(), "size", lst.size);
console.log("mtime:", st.mtime.toISOString());

// The two names have one inode, so the stats are the same.
const other = fs.statSync("second-name.txt");
console.log("same file:", st.dev === other.dev && st.ino === other.ino);

// With bigint: true, the times also come in nanoseconds.
const big = fs.statSync("data.txt", { bigint: true });
console.log("mtimeNs:", big.mtimeNs);
```

```output for=stats-and-lstat
stat:  file true size 5 mode 640 links 2
lstat: symlink true size 8
mtime: 2023-11-14T22:13:20.000Z
same file: true
mtimeNs: 1700000000000000000n
```

- Verdict: stdlib. `fs.statSync` and `fs.lstatSync` return an `fs.Stats` object with `dev`, `ino`, `mode`, `nlink`, `uid`, `gid`, `size` and four times. The methods `isFile`, `isDirectory` and `isSymbolicLink` read the file type from `mode`. With `bigint: true`, the numbers are `bigint` values and the times are also in nanoseconds, such as `mtimeNs`.
