```python run id=inodes-and-statvfs
import os
from pathlib import Path

root = Path("tree")
(root / "src").mkdir(parents=True)
(root / "src" / "main.txt").write_text("main\n", encoding="utf-8")
(root / "readme.txt").write_text("read me\n", encoding="utf-8")

# A directory is a list of names. scandir gives the type of each entry.
for entry in sorted(os.scandir(root), key=lambda e: e.name):
    print(entry.name, "dir" if entry.is_dir() else "file")

# A rename changes a name, not the file: the inode number stays.
before = (root / "readme.txt").stat().st_ino
(root / "readme.txt").rename(root / "README.txt")
print("same inode after rename:", (root / "README.txt").stat().st_ino == before)

# A hard link is one more name; samefile compares device and inode.
os.link(root / "src" / "main.txt", root / "main-link.txt")
print("samefile:", (root / "main-link.txt").samefile(root / "src" / "main.txt"))
print("links:", (root / "src" / "main.txt").stat().st_nlink)

# statvfs describes the file system that holds the path.
vfs = os.statvfs(root)
print("block size > 0:", vfs.f_frsize > 0)
print("free bytes for this user > 0:", vfs.f_bavail * vfs.f_frsize > 0)
```

```output for=inodes-and-statvfs
readme.txt file
src dir
same inode after rename: True
samefile: True
links: 2
block size > 0: True
free bytes for this user > 0: True
```

- Verdict: stdlib. `os.scandir` gives the type of each entry, and `Path.rename` keeps the inode number, because a rename changes only the directory entry. `os.link` adds a second name, `Path.samefile` compares the device and the inode, and `st_nlink` counts the names. `os.statvfs` describes the file system that holds a path, and `f_bavail` times `f_frsize` is the free space for a normal user.
