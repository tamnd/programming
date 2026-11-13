```python run id=stat-lstat-scandir
import os
import stat
from pathlib import Path

os.mkdir("box")
os.chdir("box")
Path("data.txt").write_text("hello", encoding="utf-8")
os.chmod("data.txt", 0o640)
os.symlink("data.txt", "link")
os.symlink("missing.txt", "dangling")

# stat follows the link; lstat, or follow_symlinks=False, does not.
st = os.stat("link")
lst = os.stat("link", follow_symlinks=False)
print("stat:  ", stat.filemode(st.st_mode), st.st_size)
print("lstat: ", "symlink" if stat.S_ISLNK(lst.st_mode) else "?", lst.st_size)
print("same as lstat:", lst == os.lstat("link"))

# A dangling link has metadata of its own, but no target.
try:
    os.stat("dangling")
except FileNotFoundError:
    print("stat dangling: FileNotFoundError")
print("lstat dangling: size", os.lstat("dangling").st_size)

# scandir gives the type of each entry without a stat call on most systems.
for e in sorted(os.scandir("."), key=lambda e: e.name):
    print(f"{e.name:9} link={e.is_symlink()!s:5} file={e.is_file()!s:5}"
          f" file-no-follow={e.is_file(follow_symlinks=False)}")
```

```output for=stat-lstat-scandir
stat:   -rw-r----- 5
lstat:  symlink 8
same as lstat: True
stat dangling: FileNotFoundError
lstat dangling: size 11
dangling  link=True  file=False file-no-follow=False
data.txt  link=False file=True  file-no-follow=True
link      link=True  file=True  file-no-follow=False
```

- Verdict: stdlib. `os.stat` returns a `stat_result`, and `stat.filemode` writes its `st_mode` as `ls` does. With `follow_symlinks=False`, or with `os.lstat`, the result describes the link, and the size of a link is the length of the path in it. A dangling link has no target, so `os.stat` fails, but `os.lstat` works. `os.DirEntry` has `is_file`, `is_dir` and `is_symlink`, and each takes `follow_symlinks`.
