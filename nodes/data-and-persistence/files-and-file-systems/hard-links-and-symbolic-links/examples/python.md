```python run id=relative-target-and-loop
import errno
import os

os.makedirs("sub")
with open("data.txt", "w") as f:
    f.write("alpha\n")

# A relative target is read from the directory of the link, not from the
# current directory. So this link points to sub/data.txt, which does not exist.
os.symlink("data.txt", "sub/link")
print("readlink:", os.readlink("sub/link"))
print("exists:  ", os.path.exists("sub/link"))

# Two links that point to each other make a loop. The system stops after
# a fixed number of links and gives ELOOP.
os.symlink("b", "a")
os.symlink("a", "b")
try:
    os.stat("a")
except OSError as e:
    print("loop:    ", errno.errorcode[e.errno])
print("lstat:   ", os.path.islink("a"))
```

```output for=relative-target-and-loop
readlink: data.txt
exists:   False
loop:     ELOOP
lstat:    True
```

- Verdict: stdlib. `os.link`, `os.symlink` and `os.readlink` are thin wrappers of the POSIX calls, and `os.stat` and `os.lstat` choose whether to follow a link. `os.path.exists` follows the link and `os.path.lexists` does not. On Windows, `os.symlink` needs `target_is_directory=True` for a link to a directory that does not exist yet.
