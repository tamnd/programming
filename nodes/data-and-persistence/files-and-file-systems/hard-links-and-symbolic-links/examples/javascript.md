```javascript run id=stat-and-lstat
import { link, lstat, readFile, readlink, stat, symlink, unlink, writeFile } from "node:fs/promises";

await writeFile("data.txt", "alpha\n");
await link("data.txt", "hard.txt");
await symlink("data.txt", "soft.txt");

const s = await stat("soft.txt");  // follows the link
const l = await lstat("soft.txt"); // the link itself
console.log("stat  isSymbolicLink:", s.isSymbolicLink(), "nlink:", s.nlink);
console.log("lstat isSymbolicLink:", l.isSymbolicLink());
console.log("same inode:", s.ino === (await stat("hard.txt")).ino);
console.log("readlink:", await readlink("soft.txt"));

await unlink("data.txt");
console.log("hard.txt:", (await readFile("hard.txt", "utf8")).trim());
try {
  await readFile("soft.txt");
} catch (e) {
  console.log("soft.txt:", e.code);
}
```

```output for=stat-and-lstat
stat  isSymbolicLink: false nlink: 2
lstat isSymbolicLink: true
same inode: true
readlink: data.txt
hard.txt: alpha
soft.txt: ENOENT
```

- Verdict: stdlib. Node.js has `fs.link`, `fs.symlink`, `fs.readlink`, and `fs.lstat` beside `fs.stat`. The `type` argument of `symlink` is read only on Windows, where it can be `'dir'`, `'file'` or `'junction'`.
