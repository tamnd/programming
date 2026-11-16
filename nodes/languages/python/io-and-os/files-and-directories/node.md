---
tag: HHJBS3
kind: feature
owner: python
slug: files-and-directories
name: Files and directories
aliases: [os.path, shutil.copy, shutil.move, shutil.rmtree, os.walk, os.scandir, os.stat, tempfile, file permissions, os.chmod]
area: languages/python/io-and-os
level: core
summary: >-
  os.path splits and joins paths, os.scandir() and os.walk() list directories, os.stat() reads the size, times and mode of a file, shutil copies and removes trees, and tempfile makes scratch files.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/os.html#files-and-directories", locator: "os, Files and Directories"}
see_also: [python/operating-system-interface, python/pathlib, python/file-wildcards, list-files-in-a-directory, create-a-temporary-file]
sources:
  - url: https://docs.python.org/3.15/library/os.path.html
    title: os.path, Python 3.15 documentation
    locator: module introduction; os.path.join(); os.path.split(); os.path.splitext(); os.path.normpath(); os.path.getsize()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/os.html#files-and-directories
    title: os, Python 3.15 documentation
    locator: os.scandir(); os.DirEntry; os.stat(); os.stat_result; os.chmod(); os.rmdir(); os.walk()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/stat.html
    title: stat, Python 3.15 documentation
    locator: stat.S_IMODE(); stat.filemode()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/shutil.html
    title: shutil, Python 3.15 documentation
    locator: shutil.copy(); shutil.copy2(); shutil.copytree(); shutil.rmtree(); shutil.move()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tempfile.html
    title: tempfile, Python 3.15 documentation
    locator: module introduction; TemporaryDirectory; mkstemp(); Deprecated functions and variables
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os#ReadDir
    title: Package os, Go standard library
    locator: func ReadDir
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import shutil

os.makedirs('site/css', exist_ok=True)
with open('site/index.html', 'w', encoding='utf-8') as f:
    f.write('<h1>Hi</h1>\n')
with open('site/css/main.css', 'w', encoding='utf-8') as f:
    f.write('h1 {}\n')

shutil.copytree('site', 'backup')
for dirpath, dirnames, filenames in os.walk('backup'):
    dirnames.sort()
    for name in sorted(filenames):
        path = os.path.join(dirpath, name)
        print(path, os.path.getsize(path))

shutil.rmtree('site')
print(os.path.exists('site'), os.path.isdir('backup'))
```

```output for=synopsis
backup/index.html 12
backup/css/main.css 6
False True
```

`copytree()` copies the whole tree, `os.walk()` visits each directory of the copy, and `rmtree()` deletes the original tree.

## Semantics

**r1.** `os.path` "implements some useful functions on pathnames."[^1] The functions work on strings. "Unlike a Unix shell, Python does not do any automatic path expansions."[^1] `os.path.join()` joins segments "with exactly one directory separator following each non-empty part, except the last."[^1] "If a segment is an absolute path (which on Windows requires both a drive and a root), then all previous segments are ignored and joining continues from the absolute path segment."[^1] Example: [`paths`](#paths).

**r2.** `os.path.split()` returns "a pair, (head, tail) where tail is the last pathname component and head is everything leading up to that."[^1] `os.path.splitext()` returns "a pair (root, ext) such that root + ext == path".[^1] `os.path.normpath()` collapses "redundant separators and up-level references so that A//B, A/B/, A/./B and A/foo/../B all become A/B."[^1]

**r3.** `os.scandir()` returns "an iterator of os.DirEntry objects corresponding to the entries in the directory given by path."[^2] "Using scandir() instead of listdir() can significantly increase the performance of code that also needs file type or file attribute information".[^2] Example: [`scandir`](#scandir).

**r4.** `os.walk()` yields "a 3-tuple (dirpath, dirnames, filenames)" for each directory in a tree.[^2] "Note that the names in the lists contain no path components. To get a full path (which begins with top) to a file or directory in dirpath, do os.path.join(dirpath, name)."[^2] "When topdown is True, the caller can modify the dirnames list in-place (perhaps using del or slice assignment), and walk() will only recurse into the subdirectories whose names remain in dirnames".[^2]

**r5.** `os.stat()` returns a `stat_result`.[^2] `st_size` is the "Size of the file in bytes", `st_mtime` is the "Time of most recent content modification expressed in seconds", and `st_mode` holds the "File mode: file type and file mode bits (permissions)."[^2] `stat.S_IMODE()` takes out "the file’s permission bits", and `stat.filemode()` makes "a string of the form ‘-rwxrwxrwx’."[^3] `os.chmod(path, mode)` sets the mode.[^2]

**r6.** `shutil.copy()` "copies the file data and the file’s permission mode".[^4] "Other metadata, like the file’s creation and modification times, is not preserved. To preserve all file metadata from the original, use copy2() instead."[^4] `shutil.copytree()` copies "an entire directory tree rooted at src".[^4] `shutil.rmtree()` deletes "an entire directory tree".[^4] Example: [`copy2`](#copy2).

**r7.** `os.rmdir()` removes one directory. "If the directory does not exist or is not empty, a FileNotFoundError or an OSError is raised respectively. In order to remove whole directory trees, shutil.rmtree() can be used."[^2]

**r8.** The `tempfile` module "creates temporary files and directories."[^5] "TemporaryFile, NamedTemporaryFile, TemporaryDirectory, and SpooledTemporaryFile are high-level interfaces which provide automatic cleanup and can be used as context managers."[^5] Example: [`tempdir`](#tempdir).

## Examples

```python run id=paths
import os.path

p = os.path.join('data', 'raw', 'report.tar.gz')
print(p)
print(os.path.split(p))
print(os.path.splitext(p))
print(os.path.join('/home/foo', '/home/bar'))
print(os.path.normpath('a//b/./c/../d/'))
```

```output for=paths
data/raw/report.tar.gz
('data/raw', 'report.tar.gz')
('data/raw/report.tar', '.gz')
/home/bar
a/b/d
```

`splitext()` splits off only the last extension (rule r2). The absolute segment `/home/bar` drops `/home/foo` (rule r1).

```python run id=scandir
import os
import stat

os.makedirs('d/sub', exist_ok=True)
with open('d/a.txt', 'w', encoding='utf-8') as f:
    f.write('12345')
os.chmod('d/a.txt', 0o640)

with os.scandir('d') as it:
    for entry in sorted(it, key=lambda e: e.name):
        if entry.is_file():
            st = entry.stat()
            print(entry.name, st.st_size, oct(stat.S_IMODE(st.st_mode)), stat.filemode(st.st_mode))
        else:
            print(entry.name, 'directory')
```

```output for=scandir
a.txt 5 0o640 -rw-r-----
sub directory
```

Each `DirEntry` knows its name and type, and `stat()` gives the size and the mode (rules r3 and r5). Mode `0o640` lets the owner read and write and the group read.

```python run id=copy2
import os
import shutil

with open('orig.txt', 'w', encoding='utf-8') as f:
    f.write('x')
os.utime('orig.txt', (1_000_000_000, 1_000_000_000))

shutil.copy('orig.txt', 'c1.txt')
shutil.copy2('orig.txt', 'c2.txt')
print(os.stat('c1.txt').st_mtime == 1e9)
print(os.stat('c2.txt').st_mtime == 1e9)
```

```output for=copy2
False
True
```

The original has a modification time in 2001. `copy()` gives the copy the current time, and `copy2()` keeps the old time (rule r6).

```python run id=tempdir
import os
import tempfile

with tempfile.TemporaryDirectory() as tmp:
    path = os.path.join(tmp, 'scratch.txt')
    with open(path, 'w', encoding='utf-8') as f:
        f.write('temp')
    print(os.listdir(tmp))
print(os.path.exists(tmp))
```

```output for=tempdir
['scratch.txt']
False
```

"On completion of the context or destruction of the temporary directory object, the newly created temporary directory and all its contents are removed from the filesystem."[^5]

## Pitfalls

`os.rmdir()` refuses a directory that has files:

```python fails id=rmdir expect=exception:OSError
import os

os.makedirs('full', exist_ok=True)
open('full/file.txt', 'w', encoding='utf-8').close()
os.rmdir('full')
```

Use `shutil.rmtree()` for a tree (rule r7), and check the path first: it deletes everything below it.

Do not make a temporary name first and the file later. `tempfile.mktemp()` is deprecated. The docs say: "Unfortunately this is not secure, because a different process may create a file with this name in the time between the call to mktemp() and the subsequent attempt to create the file by the first process."[^5] Use `mkstemp()`, `NamedTemporaryFile()` or `TemporaryDirectory()`, which create the file at once.

The copy functions lose metadata. "Even the higher-level file copying functions (shutil.copy(), shutil.copy2()) cannot copy all file metadata. On POSIX platforms, this means that file owner and group are lost as well as ACLs."[^4]

`os.walk()` and `os.scandir()` have no fixed order. "Whether or not the lists are sorted depends on the file system."[^2] Sort `dirnames` in place and sort `filenames`, as the synopsis does, when the output must be stable.

## Comparison

Go sorts for you: "ReadDir reads the named directory, returning all its directory entries sorted by filename."[^6] Python's `os.scandir()` and `os.listdir()` return the entries "in arbitrary order", so Python code sorts them itself.[^2]
