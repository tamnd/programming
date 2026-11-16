---
tag: 1486JZ
kind: feature
owner: python
slug: file-wildcards
name: File Wildcards
aliases: [glob, glob.glob, wildcard file search, recursive glob, "**", fnmatch, file name patterns]
area: languages/python/io-and-os
level: intro
summary: >-
  glob.glob() returns the paths that match a shell-style pattern with *, ? and [...]. With recursive=True, ** matches any number of directories. The result has no fixed order, so sort it.
grammar: "glob.glob(pathname, *, root_dir=None, dir_fd=None, recursive=False, include_hidden=False)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/glob.html", locator: "glob.glob()"}
see_also: [python/operating-system-interface, python/pathlib, python/files-and-directories, list-files-in-a-directory]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#file-wildcards
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.2. File wildcards
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/glob.html
    title: glob, Python 3.15 documentation
    locator: module introduction; glob.glob(); glob.iglob(); glob.escape(); Examples
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/fnmatch.html
    title: fnmatch, Python 3.15 documentation
    locator: module introduction; fnmatch.fnmatch(); fnmatch.filter()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/path/filepath#Glob
    title: Package path/filepath, Go standard library
    locator: func Glob; func Match
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import glob
import os

os.makedirs('pics/sub', exist_ok=True)
for name in ['1.gif', '2.txt', 'card.gif', '.card.gif', 'sub/3.txt']:
    open(os.path.join('pics', name), 'w', encoding='utf-8').close()

print(sorted(glob.glob('pics/*.gif')))
print(sorted(glob.glob('**/*.txt', root_dir='pics', recursive=True)))
```

```output for=synopsis
['pics/1.gif', 'pics/card.gif']
['2.txt', 'sub/3.txt']
```

The first pattern finds the GIF files in `pics`, and the second one finds the text files in `pics` and in all its subdirectories.

## Semantics

**r1.** "The glob module provides a function for making file lists from directory wildcard searches".[^1] It "finds pathnames using pattern matching rules similar to the Unix shell."[^2] It works "by using the os.scandir() and fnmatch.fnmatch() functions in concert, and not by actually invoking a subshell."[^2]

**r2.** The wildcards are those of the shell.[^3]

| Pattern | Meaning |
|---|---|
| `*` | "matches everything" |
| `?` | "matches any single character" |
| `[seq]` | "matches any character in seq" |
| `[!seq]` | "matches any character not in seq" |

In `glob`, a wildcard matches inside one path segment. "No tilde expansion is done".[^2] Example: [`patterns`](#patterns).

**r3.** `glob.glob()` returns "a possibly empty list of path names that match pathname".[^2] The result keeps the leading parts of the pattern: "Notice how any leading components of the path are preserved."[^2] With `root_dir`, the search starts there, and "If pathname is relative, the result will contain paths relative to root_dir."[^2]

**r4.** "If recursive is true, the pattern “**” will match any files and zero or more directories, subdirectories and symbolic links to directories."[^2] Example: [`recursive`](#recursive).

**r5.** "By default, files beginning with a dot (.) can only be matched by patterns that also start with a dot".[^2] "If include_hidden is true, wildcards can match path segments that begin with a dot (.)."[^2]

**r6.** "The pathnames are returned in no particular order. If you need a specific order, sort the results."[^2]

**r7.** `glob.iglob()` returns "an iterator which yields the same values as glob() without actually storing them all simultaneously."[^2] `glob.escape()` escapes "all special characters ('?', '*' and '[')."[^2] Example: [`escape`](#escape).

**r8.** The `fnmatch` module matches one name against a pattern, without a directory search. "Note that the filename separator ('/' on Unix) is not special to this module."[^3]

## Examples

```python run id=patterns
import glob
import os

os.makedirs('pics/sub', exist_ok=True)
for name in ['1.gif', '2.txt', 'card.gif', '.card.gif', 'sub/3.txt']:
    open(os.path.join('pics', name), 'w', encoding='utf-8').close()

print(sorted(glob.glob('*.gif', root_dir='pics')))
print(sorted(glob.glob('?.gif', root_dir='pics')))
print(sorted(glob.glob('[0-9].*', root_dir='pics')))
print(sorted(glob.glob('[!0-9]*', root_dir='pics')))
print(sorted(glob.glob('*.gif', root_dir='pics', include_hidden=True)))
```

```output for=patterns
['1.gif', 'card.gif']
['1.gif']
['1.gif', '2.txt']
['card.gif', 'sub']
['.card.gif', '1.gif', 'card.gif']
```

These are the files of the example in the docs. `*.gif` skips `.card.gif` until `include_hidden=True` (rule r5). `[!0-9]*` matches the directory `sub` too, because a pattern matches names of any kind (rule r2).

```python run id=recursive
import glob
import os

os.makedirs('src/pkg/deep', exist_ok=True)
for name in ['main.py', 'pkg/util.py', 'pkg/deep/core.py', 'pkg/notes.txt']:
    open(os.path.join('src', name), 'w', encoding='utf-8').close()

print(sorted(glob.glob('**/*.py', root_dir='src', recursive=True)))
print(sorted(glob.glob('**/*.py', root_dir='src')))
print(sorted(glob.glob('**/', root_dir='src', recursive=True)))
```

```output for=recursive
['main.py', 'pkg/deep/core.py', 'pkg/util.py']
['pkg/util.py']
['pkg/', 'pkg/deep/']
```

With `recursive=True`, `**` matches zero or more directories, so `main.py` at the top matches too (rule r4). Without it, `**` acts like `*` and matches one directory name. "If the pattern is followed by an os.sep or os.altsep then files will not match."[^2]

```python run id=escape
import glob
import os

os.makedirs('q', exist_ok=True)
for name in ['what?.txt', 'whatX.txt']:
    open(os.path.join('q', name), 'w', encoding='utf-8').close()

print(sorted(glob.glob('what?.txt', root_dir='q')))
print(glob.escape('what?.txt'))
print(glob.glob(glob.escape('what?.txt'), root_dir='q'))
```

```output for=escape
['what?.txt', 'whatX.txt']
what[?].txt
['what?.txt']
```

The `?` in the name is a wildcard until `glob.escape()` wraps it in brackets (rule r7).

```python run id=fnmatch
import fnmatch

names = ['setup.py', 'README.md', 'test_io.py', '.env']
print(fnmatch.filter(names, '*.py'))
print(fnmatch.fnmatch('.env', '*'))
```

```output for=fnmatch
['setup.py', 'test_io.py']
True
```

`fnmatch` works on a list of names (rule r8). Unlike `glob`, it lets `*` match a name that starts with a dot: "filenames starting with a period are not special for this module".[^3]

## Pitfalls

A wrong path gives an empty list, not an error:

```python run id=missing
import glob

print(glob.glob('no-such-dir/*.txt'))
```

```output for=missing
[]
```

"Any OSError exceptions raised from scanning the filesystem are suppressed. This includes PermissionError when accessing directories without read permission."[^2] Check that the directory exists when an empty result could be a mistake.

The order depends on the file system. "Whether or not the results are sorted depends on the file system."[^2] A program that works on one machine can process the files in a different order on another. Wrap the call in `sorted()` (rule r6).

`**` is slow on big trees. "Using the “**” pattern in large directory trees may consume an inordinate amount of time."[^2] Start the search in the smallest directory that holds the files, and use `iglob()` to stop early.

## Comparison

Go has the same function in `path/filepath`. "Glob returns the names of all files matching pattern or nil if there is no matching file."[^4] Go also ignores file system errors: "Glob ignores file system errors such as I/O errors reading directories."[^4] In Go, `'*'` "matches any sequence of non-Separator characters", and the syntax has no `**`.[^4] Python's `glob` crosses directories with `**` and `recursive=True`.
