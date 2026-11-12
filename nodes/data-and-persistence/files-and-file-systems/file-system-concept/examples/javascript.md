```javascript run id=stat-and-statfs
const fs = require("node:fs");

fs.mkdirSync("tree/src", { recursive: true });
fs.writeFileSync("tree/src/main.txt", "main\n");
fs.writeFileSync("tree/readme.txt", "read me\n");

// readdir with withFileTypes gives a Dirent with the type of each entry.
for (const e of fs.readdirSync("tree", { withFileTypes: true }).sort((a, b) => a.name.localeCompare(b.name))) {
  console.log(e.name, e.isDirectory() ? "dir" : "file");
}

// stat gives the device ID, the inode number and the link count.
const before = fs.statSync("tree/readme.txt").ino;
fs.renameSync("tree/readme.txt", "tree/README.txt");
console.log("same inode after rename:", fs.statSync("tree/README.txt").ino === before);

fs.linkSync("tree/src/main.txt", "tree/main-link.txt");
const a = fs.statSync("tree/src/main.txt");
const b = fs.statSync("tree/main-link.txt");
console.log("same file:", a.dev === b.dev && a.ino === b.ino);
console.log("links:", a.nlink);

// statfs describes the file system that holds the path.
const s = fs.statfsSync("tree");
console.log("block size > 0:", s.bsize > 0);
console.log("free bytes for this user > 0:", s.bavail * s.bsize > 0);
```

```output for=stat-and-statfs
readme.txt file
src dir
same inode after rename: true
same file: true
links: 2
block size > 0: true
free bytes for this user > 0: true
```

- Verdict: stdlib. Node.js `fs.readdirSync` with `withFileTypes` gives `Dirent` objects with the type of each entry. `fs.statSync` gives `dev`, `ino` and `nlink`, so the program sees that a rename keeps the inode and that a link is the same file. `fs.statfsSync` describes the file system, as `statvfs` does in C.
