---
tag: B8VD8J
kind: feature
owner: python
slug: pathlib
name: The pathlib module
aliases: [pathlib, Path, PurePath, Path objects, path / operator, read_text, write_text, iterdir]
area: languages/python/io-and-os
level: core
summary: >-
  pathlib.Path is an object for a file system path. The / operator joins parts, attributes such as name and suffix split it, and methods such as read_text(), iterdir() and glob() do the I/O.
grammar: "pathlib.Path(*pathsegments); path / 'child'"
spec: {standard: null, url: "https://docs.python.org/3.15/library/pathlib.html", locator: "pathlib module"}
see_also: [python/operating-system-interface, python/file-wildcards, python/files-and-directories, python/reading-and-writing-files, list-files-in-a-directory]
sources:
  - url: https://docs.python.org/3.15/library/pathlib.html
    title: pathlib, Python 3.15 documentation
    locator: module introduction; Basic use; Pure paths; Path.read_text(); Path.iterdir(); Path.glob(); Comparison to the glob module; Corresponding tools
    accessed: 2026-11-16
  - url: https://pkg.go.dev/path/filepath#Join
    title: Package path/filepath, Go standard library
    locator: Overview; func Join
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
from pathlib import Path

base = Path('project')
docs = base / 'docs'
docs.mkdir(parents=True, exist_ok=True)

readme = docs / 'README.txt'
readme.write_text('Hello, pathlib\n', encoding='utf-8')
print(readme)
print(readme.name, readme.stem, readme.suffix)
print(readme.read_text(encoding='utf-8'), end='')
print([p.name for p in base.iterdir()])
```

```output for=synopsis
project/docs/README.txt
README.txt README .txt
Hello, pathlib
['docs']
```

The `/` operator builds the path. `mkdir()` makes the directories, `write_text()` and `read_text()` write and read the file, and `iterdir()` lists a directory.

## Semantics

**r1.** "This module offers classes representing filesystem paths with semantics appropriate for different operating systems."[^1] "Path classes are divided between pure paths, which provide purely computational operations without I/O, and concrete paths, which inherit from pure paths but also provide I/O operations."[^1] "If you’ve never used this module before or just aren’t sure which class is right for your task, Path is most likely what you need."[^1]

**r2.** "The slash operator helps create child paths, like os.path.join(). If the argument is an absolute path, the previous path is ignored."[^1] The constructor takes several segments in the same way. Example: [`join`](#join).

**r3.** Attributes split a path without I/O. `name` is "the final path component", `suffix` is "The last dot-separated portion of the final component, if any", `stem` is "The final path component, without its suffix", and `parent` is "The logical parent of the path".[^1] `with_suffix()` returns "a new path with the suffix changed."[^1]

**r4.** `read_text()` returns "the decoded contents of the pointed-to file as a string".[^1] `write_text()` opens "the file pointed to in text mode, write data to it, and close the file".[^1] For both, "The optional parameters have the same meaning as in open()."[^1] `read_bytes()` and `write_bytes()` do the same in binary mode.[^1]

**r5.** `iterdir()` yields "path objects of the directory contents".[^1] "The children are yielded in arbitrary order, and the special entries '.' and '..' are not included."[^1]

**r6.** `glob()` yields "all matching files (of any kind)" for a relative pattern.[^1] `rglob()` "is like calling Path.glob() with “**/” added in front of the pattern."[^1] The rules differ from the `glob` module: "Files beginning with a dot are not special in pathlib" and "“**” pattern components are always recursive in pathlib."[^1] Example: [`glob`](#glob).

**r7.** Most functions of `os` and `os.path` have a `Path` form.[^1]

| `os` and `os.path` | `pathlib` |
|---|---|
| `os.path.join()` | `PurePath.joinpath()`, `/` |
| `os.path.basename()`, `os.path.dirname()` | `PurePath.name`, `PurePath.parent` |
| `os.path.splitext()` | `PurePath.stem`, `PurePath.suffix` |
| `os.path.exists()`, `os.path.isfile()`, `os.path.isdir()` | `Path.exists()`, `Path.is_file()`, `Path.is_dir()` |
| `os.getcwd()` | `Path.cwd()` |
| `os.listdir()` | `Path.iterdir()` |
| `os.mkdir()`, `os.makedirs()` | `Path.mkdir()` |
| `os.remove()`, `os.rename()` | `Path.unlink()`, `Path.rename()` |

## Examples

```python run id=join
from pathlib import Path, PurePosixPath, PureWindowsPath

print(Path('etc') / 'init.d' / 'reboot')
print(PurePosixPath('/etc', '/usr', 'lib64'))
print(PureWindowsPath('c:/Downloads') / 'pathlib.tar.gz')
print(PurePosixPath('my/library.tar.gz').suffixes)
print(PurePosixPath('/a/b/c/d').parent)
```

```output for=join
etc/init.d/reboot
/usr/lib64
c:\Downloads\pathlib.tar.gz
['.tar', '.gz']
/a/b/c
```

An absolute segment drops the segments before it (rule r2). A `PureWindowsPath` uses backslashes on any system, because pure paths do no I/O (rule r1).

```python run id=glob
from pathlib import Path

root = Path('tree')
for name in ['a.py', '.hidden.py', 'pkg/b.py', 'pkg/notes.txt']:
    path = root / name
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text('', encoding='utf-8')

print(sorted(p.as_posix() for p in root.glob('*.py')))
print(sorted(p.as_posix() for p in root.rglob('*.py')))
print(sorted(p.name for p in root.iterdir() if p.is_dir()))
```

```output for=glob
['tree/.hidden.py', 'tree/a.py']
['tree/.hidden.py', 'tree/a.py', 'tree/pkg/b.py']
['pkg']
```

`glob('*.py')` finds `.hidden.py`, which `glob.glob('*.py')` would skip. `rglob()` also looks in `pkg` (rule r6). The results include the base path `tree` as a prefix.

```python run id=with-suffix
from pathlib import Path

report = Path('out/report.csv')
print(report.with_suffix('.json'))
print(report.with_name('summary.csv'))
print(report.parent.name, report.is_absolute())
```

```output for=with-suffix
out/report.json
out/summary.csv
out False
```

The methods return new paths and do not touch the file system (rule r3).

## Pitfalls

A `Path` is not a string:

```python fails id=concat expect=exception:TypeError
from pathlib import Path

name = Path('data') + '.txt'
```

Use `/` to add a part, `with_suffix()` to change the extension, or `str(path)` when an old function needs a string.

`parent` does not follow `..`. "This is a purely lexical operation", so `PurePosixPath('foo/..').parent` is `PurePosixPath('foo')`.[^1] The docs recommend to "first call Path.resolve() so as to resolve symlinks and eliminate \"..\" components."[^1]

`read_text()` without an encoding uses the default encoding of `open()`. Before Python 3.15, that is the locale encoding, so pass `encoding='utf-8'` when the code must run on older versions. See [reading and writing files](python/reading-and-writing-files).

The results of `iterdir()` and `glob()` have no fixed order. "The paths are returned in no particular order. If you need a specific order, sort the results."[^1]

## Comparison

Go joins paths with a function on strings: "Join joins any number of path elements into a single path, separating them with an OS specific Separator."[^2] `filepath.Join` takes strings and returns a string.[^2] In Python, a `Path` object holds the path, and its methods do the work.
