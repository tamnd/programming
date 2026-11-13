---
tag: KBHB5J
kind: task
slug: check-whether-a-file-exists
name: Check whether a file exists
aliases: [file exists, test if a file exists, check that a file exists, path exists]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Tell whether a path names a file, a directory, another entry, a broken link or nothing, report an error when the program cannot tell, and know that the answer can be old at once.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/os.path.html#os.path.exists
    title: The Python Standard Library, os.path, Common pathname manipulations
    locator: os.path.exists, os.path.lexists
  - url: https://docs.python.org/3/library/os.html#os.stat
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.stat, os.lstat
  - url: https://docs.python.org/3/library/functions.html#open
    title: The Python Standard Library, Built-in Functions
    locator: open, the mode 'x'
  - url: https://nodejs.org/api/fs.html
    title: Node.js documentation, File system
    locator: fsPromises.access(path[, mode]), fsPromises.stat(path[, options]), fs.existsSync(path)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: exists(Path, LinkOption...), notExists(Path, LinkOption...), readAttributes(Path, Class, LinkOption...)
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func IsNotExist, func Stat, func Lstat
  - url: https://pkg.go.dev/io/fs
    title: Go standard library, package io/fs
    locator: ErrNotExist, type FileMode
  - url: https://doc.rust-lang.org/std/path/struct.Path.html
    title: The Rust Standard Library, std::path::Path
    locator: exists, try_exists
  - url: https://doc.rust-lang.org/std/fs/fn.metadata.html
    title: The Rust Standard Library, std::fs::metadata
    locator: the function documentation
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/stat.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), fstatat, lstat, stat
    locator: ERRORS
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/test.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), test
    locator: OPERANDS (-e, -L)
  - url: https://en.cppreference.com/w/cpp/filesystem/status
    title: cppreference, std::filesystem::status, std::filesystem::symlink_status
    locator: Return value
---

## Task

Tell what is at a path. Give one of five answers:

- `file`: a regular file, also an empty one.
- `directory`: a directory.
- `other`: an entry of another type, such as a device or a named pipe.
- `missing`: there is no entry at the path.
- `broken-link`: a symbolic link whose target does not exist.

Follow symbolic links: a link to a file gives `file`. A path through a regular file, such as `a.txt/b.txt`, gives `missing`, because no entry can be there.

If the program cannot tell, report an error and do not give `missing`. For example, if a parent directory does not let the program search it, the file can be there or not, and the program cannot know which.

A check is correct only for the moment of the check. Another program can make or delete the file just after it. To make a file only if there is none, or to read a file that can be missing, do not check first: open the file and handle the error. See the pitfalls.

SQL, regular expressions, HTML and CSS do not apply, because they do not look at files.

## Solutions

Each solution calls the stat function of the operating system, which follows symbolic links. If the error is `ENOENT` (no such entry) or `ENOTDIR` (a part of the path is not a directory), it calls lstat, which does not follow links, to tell a broken link from a missing entry. Each other error goes to the caller. The solutions do not use the short functions that give only true or false, because those functions give false also for an error.

- **Python.** `os.stat(path)`, then `stat.S_ISREG` and `stat.S_ISDIR` on `st_mode`. On an `OSError` with `errno.ENOENT` or `errno.ENOTDIR`, `os.lstat` and `stat.S_ISLNK`. The code does not use `os.path.exists`, which "Returns False for broken symbolic links. On some platforms, this function may return False if permission is not granted to execute os.stat() on the requested file, even if the path physically exists."
- **JavaScript and TypeScript (Node.js).** `fsPromises.stat(path)` and `stats.isFile()`, `stats.isDirectory()`. On an error with `code` `ENOENT` or `ENOTDIR`, `fsPromises.lstat` and `isSymbolicLink()`. "fs.exists() is deprecated, but fs.existsSync() is not." `fs.existsSync` gives `false` also for a permission error, so the code does not use it.
- **Java.** `Files.readAttributes(path, BasicFileAttributes.class)`, then `isRegularFile()` and `isDirectory()`. A missing entry gives `NoSuchFileException`, and a permission error gives `AccessDeniedException`. Java has no exception type for `ENOTDIR`, so for another `FileSystemException` the code looks for the nearest parent that is there: if it is not a directory, the answer is `missing`. The code does not use `Files.exists` and `Files.notExists`: "Where it is not possible to determine if a file exists or not then both methods return false."
- **Go.** `os.Stat(path)`, then `info.Mode().IsRegular()` and `info.IsDir()`. On an error, `errors.Is(err, fs.ErrNotExist)` or `errors.Is(err, syscall.ENOTDIR)`, then `os.Lstat` and `fs.ModeSymlink`. The documentation of `os.IsNotExist` says: "New code should use errors.Is(err, fs.ErrNotExist)." `fs.ErrNotExist` does not match `ENOTDIR`; see the pitfalls.
- **Rust.** `std::fs::metadata(path)`, which "will traverse symbolic links to query information about the destination file", then `is_file()` and `is_dir()`. On an error of kind `ErrorKind::NotFound` or `ErrorKind::NotADirectory`, `std::fs::symlink_metadata` and `file_type().is_symlink()`. The code does not use `Path::exists`: "If you cannot access the metadata of the file, e.g. because of a permission error or broken symbolic links, this will return false." `Path::try_exists` reports the error, but it does not tell a file from a directory.
- **C.** POSIX `stat` and `lstat` with `S_ISREG`, `S_ISDIR` and `S_ISLNK`. The function returns a string, or `NULL` with `errno` from `stat` for an error. POSIX gives `ENOTDIR` when "A component of the path prefix names an existing file that is neither a directory nor a symbolic link to a directory".
- **C++.** `std::filesystem::status(path, ec)`, which follows links. "If p does not exist, returns file_status ( file_type :: not_found ) ." For other errors it sets `ec`. Then `std::filesystem::symlink_status` tells a broken link from a missing entry. The function returns `std::expected<std::string_view, std::error_code>` (C++23).
- **Bash.** The tests `[[ -f ]]`, `[[ -d ]]`, `[[ -e ]]` and `[[ -L ]]`. `-e` is "True if pathname resolves to an existing directory entry. False if pathname cannot be resolved." It gives false also for a permission error. So for a path that seems missing, the function looks at the nearest parent that is there, and it returns status 1 if that parent is a directory that the shell cannot search.

There is no C# solution. `File.GetAttributes` reports `Normal` for a regular file, for `/dev/null` and for a named pipe, and .NET has no public function that gives the type of a file on Unix, so a C# solution cannot give `other`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with the files in `fixtures/`.

- `file` and `missing`: a file with text, and a name that is not there.
- `directory`: a directory gives `directory`, not `file`.
- `empty-file`: an empty file is there; its size does not matter.
- `missing-parent`: `fixtures/no-such-dir/a.txt`. stat gives `ENOENT`.
- `through-a-file`: `fixtures/a.txt/b.txt`. stat gives `ENOTDIR`, not `ENOENT`, but the answer is `missing`.
- `trailing-slash-on-a-directory`: `fixtures/dir/` gives `directory`.

The native tests make the cases that need symbolic links or permissions in a temporary directory. A link to a file gives `file`, a link to a directory gives `directory`, and a link to a missing file gives `broken-link`. `/dev/null` and a named pipe give `other`. A path in a directory with mode `000` gives a permission error, not `missing`. The tests skip that case when they run as root, because root can search each directory.

## Pitfalls

- **A function that gives false for an error.** `os.path.exists` gives `False` when it cannot tell. Here the name is there, but it is a loop of symbolic links, so stat fails with `ELOOP`:

```python wrong id=python-exists-hides-error
import errno
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    a, b = os.path.join(dir, "a"), os.path.join(dir, "b")
    os.symlink("b", a)  # a loop of links: a gives b, b gives a
    os.symlink("a", b)
    print(os.path.exists(a), os.path.lexists(a))
    try:
        os.stat(a)
    except OSError as err:
        print(errno.errorcode[err.errno])
```

```output
False True
ELOOP
```

A parent directory that the program cannot search gives `False` in the same way, as the Python documentation says. The same is true for `fs.existsSync` in Node.js, `Files.exists` in Java, `Path::exists` in Rust and `test -e` in the shell. The Java documentation of `Files.notExists` says: "Note that this method is not the complement of the exists method."

- **A broken link takes a name.** `os.path.exists` gives `False` for a link to nothing, but the name is not free, so a new file or link with that name fails:

```python run id=python-broken-link
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    link = os.path.join(dir, "link")
    os.symlink("missing.txt", link)
    print(os.path.exists(link), os.path.lexists(link))
    try:
        os.symlink("other.txt", link)  # the name is not free
    except FileExistsError as err:
        print(type(err).__name__)
```

```output
False True
FileExistsError
```

- **A check, then an action.** The result of a check can be old when the program acts on it. This is a time-of-check to time-of-use (TOCTOU) race. Here another program makes the file between the check and the open, and the open with `"w"` deletes its data. The program below does the two steps in order, to show what a race can do:

```python wrong id=python-check-then-open
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "report.txt")
    if not os.path.exists(path):
        # Between the check and the open, another program makes the file.
        with open(path, "w") as other:
            other.write("the data of another program\n")
        with open(path, "w") as f:  # "w" empties the file
            f.write("my data\n")
    print(open(path).read(), end="")
```

```output
my data
```

Make the file and check in one step. Python mode `'x'` is "open for exclusive creation, failing if the file already exists":

```python run id=python-exclusive-create
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "report.txt")
    with open(path, "w") as other:  # another program made the file first
        other.write("the data of another program\n")
    try:
        with open(path, "x") as f:  # make the file only if there is none
            f.write("my data\n")
    except FileExistsError:
        print("the file is there; nothing is lost")
    print(open(path).read(), end="")
```

```output
the file is there; nothing is lost
the data of another program
```

The Node.js documentation says the same of `access`: "Using fsPromises.access() to check for the accessibility of a file before calling fsPromises.open() is not recommended. Doing so introduces a race condition, since other processes may change the file's state between the two calls."

- **`ENOTDIR` is not `ENOENT`.** In Go, `errors.Is(err, fs.ErrNotExist)` is true for `ENOENT` but not for `ENOTDIR`. A path through a regular file gives `ENOTDIR`:

```go wrong id=go-enotdir
package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
)

func main() {
	dir, _ := os.MkdirTemp("", "notdir")
	defer os.RemoveAll(dir)
	file := filepath.Join(dir, "a.txt")
	os.WriteFile(file, []byte("alpha\n"), 0o644)
	_, err := os.Stat(filepath.Join(file, "b.txt")) // a path through a file
	fmt.Println(errors.Is(err, fs.ErrNotExist))
	fmt.Println(errors.Unwrap(err))
}
```

```output
false
not a directory
```

A program that reports each error that is not `fs.ErrNotExist` then reports an error for a path that cannot exist. The Go solution checks `syscall.ENOTDIR` too.

- **A path is not the same in each language.** Java `Path.of("a.txt/")` drops the trailing slash, so it names the file `a.txt`, but POSIX stat of `a.txt/` gives `ENOTDIR`. Java `Path.of("")` is the current directory, but POSIX stat of an empty string gives `ENOENT`: "A component of path does not name an existing file or path is an empty string." The cases do not use these paths.

## Related tasks

- [[read-a-whole-file]]
- [[delete-a-file]]
- [[create-a-temporary-file]]
- [[get-the-size-of-a-file]]
- [[file-metadata]]
- [[hard-links-and-symbolic-links]]

## References
