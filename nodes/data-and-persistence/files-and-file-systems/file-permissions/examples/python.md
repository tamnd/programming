```python run id=stat-constants-and-copymode
import os
import shutil
import stat
from pathlib import Path

Path("a.txt").write_text("a", encoding="utf-8")
Path("b.txt").write_text("b", encoding="utf-8")

# The stat module names each bit. OR them to make a mode.
mode = stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP
os.chmod("a.txt", mode)
st = os.stat("a.txt")
print("mode:", oct(stat.S_IMODE(st.st_mode)), stat.filemode(st.st_mode))

# Test one bit with &.
print("group can write:", bool(st.st_mode & stat.S_IWGRP))
print("owner can read:", bool(st.st_mode & stat.S_IRUSR))

# Add a bit to the current mode, as chmod u+x does.
os.chmod("a.txt", stat.S_IMODE(st.st_mode) | stat.S_IXUSR)
print("after u+x:", stat.filemode(os.stat("a.txt").st_mode))

# copymode copies only the permission bits to another file.
shutil.copymode("a.txt", "b.txt")
print("b.txt:", stat.filemode(os.stat("b.txt").st_mode))
```

```output for=stat-constants-and-copymode
mode: 0o640 -rw-r-----
group can write: False
owner can read: True
after u+x: -rwxr-----
b.txt: -rwxr-----
```

- Verdict: stdlib. The `stat` module has a constant for each bit, such as `S_IRUSR` and `S_IXGRP`, and `S_IMODE` keeps only the permission bits of `st_mode`. `os.chmod` sets the whole mode, so a program that adds one bit reads the mode first. `shutil.copymode` copies the permission bits from one file to another. On Windows, `os.chmod` can change only the read-only flag.
