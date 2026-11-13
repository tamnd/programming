---
tag: GDJXNM
kind: task
slug: rename-a-file
name: Rename a file
aliases: [move a file, rename a file atomically, replace a file atomically, mv]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Give a file a new name, also in another directory, with one rename call that replaces a file at the new name, so that a reader sees the old file or the new file, never a part.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/os.html#os.replace
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.rename, os.replace, os.fsync
  - url: https://docs.python.org/3/library/shutil.html#shutil.move
    title: The Python Standard Library, shutil, High-level file operations
    locator: shutil.move
  - url: https://nodejs.org/api/fs.html#fspromisesrenameoldpath-newpath
    title: Node.js documentation, File system
    locator: fsPromises.rename(oldPath, newPath), fs.rename(oldPath, newPath, callback)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: move(Path, Path, CopyOption...), ATOMIC_MOVE
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.move?view=net-10.0
    title: .NET API reference, File.Move Method
    locator: Remarks
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Rename
  - url: https://doc.rust-lang.org/std/fs/fn.rename.html
    title: The Rust Standard Library, std::fs::rename
    locator: Platform-specific behavior
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), rename, renameat
    locator: DESCRIPTION, ERRORS (EXDEV)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/mv.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), mv
    locator: DESCRIPTION, RATIONALE
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/df.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), df
    locator: OPTIONS (-P)
  - url: https://man7.org/linux/man-pages/man2/rename.2.html
    title: Linux manual page, rename(2)
    locator: DESCRIPTION, RENAME_NOREPLACE, ERRORS (EXDEV)
  - url: https://man7.org/linux/man-pages/man2/fsync.2.html
    title: Linux manual page, fsync(2)
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/filesystem/rename
    title: cppreference, std::filesystem::rename
    locator: Description
  - url: https://en.cppreference.com/w/c/io/rename
    title: cppreference, rename (C)
    locator: Description
  - url: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefileexw
    title: Windows API reference, MoveFileExW function (winbase.h)
    locator: MOVEFILE_REPLACE_EXISTING, MOVEFILE_COPY_ALLOWED
---

## Task

Give the file at path `src` the new name `dst`. The new name can be in another directory, but on the same file system. If a file is at `dst`, replace it. After the call, `dst` has the data of `src`, and no entry is at `src`. If `src` and `dst` name the same file, do nothing.

Do it with one rename call of the operating system. On POSIX, that call is atomic: "a directory entry named new shall remain visible to other threads throughout the renaming operation and refer either to the file referred to by new or old before the operation began." So a program that opens `dst` at any time sees the old file or the new file, never an empty file and never a part of one. This is why programs save a file in two steps: write a temporary file, then rename it over the old file. See the pitfalls.

Report an error, and change nothing, when `src` is missing, when the directory of `dst` is missing, when a directory is at `dst`, and when the two paths are on different file systems. A function that copies the file in the last case is a move, not a rename, and a copy is not atomic.

SQL, regular expressions, HTML and CSS do not apply, because they do not rename files.

## Solutions

Each solution calls the rename function of its language. Most of those functions call POSIX `rename` on Unix. The Linux manual says: "If newpath already exists, it will be atomically replaced, so that there is no point at which another process attempting to access newpath will find it missing." Across file systems, `rename` fails with `EXDEV`: "oldpath and newpath are not on the same mounted filesystem."

- **Python.** `os.replace(src, dst)`: "If dst exists and is a file, it will be replaced silently if the user has permission. The operation may fail if src and dst are on different filesystems. If successful, the renaming will be an atomic operation (this is a POSIX requirement)." The code does not use `os.rename`, because "On Windows, if dst exists a FileExistsError is always raised."
- **JavaScript and TypeScript (Node.js).** `fsPromises.rename(src, dst)`. "In the case that newPath already exists, it will be overwritten. If there is a directory at newPath , an error will be raised instead."
- **Java.** `Files.move(src, dst, StandardCopyOption.ATOMIC_MOVE)`. With no options, `Files.move` can copy the file, for example when the target "is on a different FileStore and would require that the file be copied". With `ATOMIC_MOVE`, "If the move cannot be performed as an atomic file system operation then AtomicMoveNotSupportedException is thrown." For a file at `dst`, "it is implementation specific if the existing file is replaced or this method fails by throwing an IOException". On Unix it is replaced.
- **Go.** `os.Rename(src, dst)`. "If newpath already exists and is not a directory, Rename replaces it. If newpath already exists and is a directory, Rename returns an error." The error is a `*os.LinkError`, and `errors.Is(err, syscall.EXDEV)` finds a cross-device error.
- **Rust.** `std::fs::rename(src, dst)` renames "replacing the original file if to already exists. This will not work if the new name is on a different mount point." A cross-device error has the kind `ErrorKind::CrossesDevices`.
- **C.** POSIX `rename(src, dst)`, which returns 0, or -1 with `errno`. ISO C has `rename` too, but it does not say what occurs to a file at the new name: "If new_filename exists, the behavior is implementation-defined." POSIX says what occurs, so the code is for POSIX systems.
- **C++.** `std::filesystem::rename(src, dst, ec)` works "as if by the POSIX rename". For a file at `dst`: "new_p is first deleted, then, without allowing other processes to observe new_p as deleted, the pathname new_p is linked to the file and old_p is unlinked from the file." The function takes a `std::error_code&`, so it gives no exception.
- **Bash.** `mv -f -- "$src" "$dst"`. `mv` is a rename call only when the two paths are on one file system. When `rename` fails with `EXDEV`, `mv` copies the file and deletes `src`. So the function first compares the mount points of the two directories, from the last field of `df -P`, and returns status 1 if they are not the same. `mv` also moves a file into a directory at `dst`, so the function returns status 1 for a directory. For the same file, `mv` gives an error, so the function returns 0 first.

C# is not on this page. `File.Move` is not one rename call: "This method works across disk volumes", and "Moving the file across disk volumes is equivalent to copying the file and deleting it from the source if the copying was successful." .NET has no option that makes the call fail in place of a copy. The Windows function under it is `MoveFileEx`. With `MOVEFILE_REPLACE_EXISTING`, "If a file named lpNewFileName exists, the function replaces its contents with the contents of the lpExistingFileName file". With `MOVEFILE_COPY_ALLOWED`, "If the file is to be moved to a different volume, the function simulates the move by using the CopyFile and DeleteFile functions."

On Linux, `renameat2` with `RENAME_NOREPLACE` gives an error if `dst` exists: "Don't overwrite newpath of the rename. Return an error if newpath already exists." Use it when a file at the new name must not be lost.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each test makes the files in a new temporary directory, calls the function, and checks the content at `dst` and that no entry is at `src`.

- `rename` and `replace-existing`: a new name, and a name that has a file. The old data at `dst` is gone.
- `same-path`: `src` and `dst` are the same. POSIX says: "If the old argument and the new argument resolve to either the same existing directory entry or different directory entries for the same existing file, rename () shall return successfully and perform no other action." The file stays.
- `empty-file`: an empty file replaces a file with text.
- `other-directory`: `a.txt` to `sub/a.txt`. The directory `sub` must exist; the rename does not make it.
- `name-with-spaces` and `non-ascii-name`: `my notes.txt` and `café 日本.txt`. A shell solution must quote the names.
- Errors, in `cases/errors.jsonl`: a missing `src` and a missing directory of `dst` give not-found, and a directory at `dst` gives is-a-directory. POSIX: "If the old argument names a file that is not a directory and the new argument names a directory, or old names a directory and new names a file that is not a directory, or new names a directory that is not empty, rename () shall fail."

The native tests also check that a symbolic link at `src` is renamed and its target stays; that a program that has the old `dst` open still reads the old data; and, on Linux, when `/dev/shm` is on another file system, that a rename to it gives a cross-device error and `src` stays. The Python test also has threads that read `dst` while other threads rename new files over it, and each read gets a whole file.

## Pitfalls

- **A save that writes in place.** Opening a file for writing makes it empty at once. If the program fails before the write, or a reader opens the file during the write, the data is gone or is a part:

```python wrong id=python-write-in-place
import os
import tempfile


def make_config():
    raise RuntimeError("disk full")  # the program fails before it writes


with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "app.conf")
    with open(path, "w") as f:
        f.write("port = 8080\n")
    try:
        with open(path, "w") as f:  # "w" makes the file empty now
            f.write(make_config())
    except RuntimeError as e:
        print("error:", e)
    print("size:", os.path.getsize(path))
```

```output
error: disk full
size: 0
```

Write a temporary file in the same directory, and rename it over the old file. The temporary file must be on the same file system as the old file, so do not put it in `/tmp`:

```python run id=python-write-temp-then-rename
import os
import tempfile


def save(path, text):
    dir = os.path.dirname(path)
    fd, tmp = tempfile.mkstemp(dir=dir, prefix=".tmp-")
    try:
        with os.fdopen(fd, "w") as f:
            f.write(text)
            f.flush()
            os.fsync(f.fileno())  # the data is on the disk
        os.replace(tmp, path)  # one atomic step
    except BaseException:
        os.unlink(tmp)
        raise
    dfd = os.open(dir, os.O_RDONLY)
    try:
        os.fsync(dfd)  # the new name is on the disk
    finally:
        os.close(dfd)


with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "app.conf")
    save(path, "port = 8080\n")
    save(path, "port = 9090\n")
    with open(path) as f:
        print(f.read(), end="")
    print(os.listdir(dir))
```

```output
port = 9090
['app.conf']
```

- **A rename that is not on the disk.** After a power failure, the new name can be lost, or the new name can point to a file with no data. `os.fsync` on the file before the rename puts the data on the disk. Then `fsync` on the directory puts the new name on the disk, because "fsync () does not necessarily ensure that the entry in the directory containing the file has also reached disk. For that an explicit fsync () on a file descriptor for the directory is also needed." The block above does both.

- **A move that is a copy.** `shutil.move`, `mv`, Java `Files.move` with no options and C# `File.Move` copy the file when the two paths are on different file systems. For `shutil.move`: "In case os.rename() fails due to OSError (e.g. the user has write permission to the destination file but not to its parent directory), this method falls back to using copy_function , in which case src is copied to the destination using copy_function and then removed." During the copy, `dst` is a part of the file, and a failure can leave two files or one part. Use the rename function, and handle the cross-device error: write the temporary file on the file system of `dst`.

- **A directory at the new name.** `mv` and `shutil.move` move the file into a directory at `dst`, with no error:

```bash wrong id=bash-mv-into-directory
dir=$(mktemp -d)
cd "$dir" || exit
printf 'alpha\n' >report.txt
mkdir backup  # a directory with the name the program wants for the file
mv -f report.txt backup
find . -type f
cd / && rm -r "$dir"
```

```output
./backup/report.txt
```

The program now looks for `backup` and finds a directory. The rename functions give an error for this case.

- **A rename that replaces only on Unix.** Python `os.rename` replaces a file at `dst` on Unix, but "On Windows, if dst exists a FileExistsError is always raised." Code that is correct on Linux then fails on Windows. Use `os.replace`, which replaces the file on each platform.

- **A reader that has the old file open.** A rename changes the name, not the data. A program that opened `dst` before the rename reads the old file to the end:

```python run id=python-rename-open-reader
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "app.conf")
    new = os.path.join(dir, "app.conf.new")
    with open(path, "w") as f:
        f.write("port = 8080\n")
    with open(new, "w") as f:
        f.write("port = 9090\n")
    with open(path) as reader:  # a reader that opened the old file
        os.replace(new, path)
        print("open reader:", reader.read(), end="")
    with open(path) as f:
        print("new open:", f.read(), end="")
```

```output
open reader: port = 8080
new open: port = 9090
```

A server that reads its configuration only at the start must open the file again to see the new data.

## Related tasks

- [[check-whether-a-file-exists]]
- [[delete-a-file]]
- [[copy-a-file]]
- [[create-a-temporary-file]]
- [[hard-links-and-symbolic-links]]

## References
