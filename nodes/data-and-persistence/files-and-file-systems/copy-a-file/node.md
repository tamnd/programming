---
tag: 7DWYBX
kind: task
slug: copy-a-file
name: Copy a file
aliases: [duplicate a file, cp, copy a file with its permissions, file copy]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Copy the bytes and the permission bits of a file to a new path, replace a file at that path, and report an error when the two paths name the same file.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/shutil.html#shutil.copy
    title: The Python Standard Library, shutil, High-level file operations
    locator: shutil.copyfile, shutil.copy, shutil.copy2, Platform-dependent efficient copy operations
  - url: https://nodejs.org/api/fs.html#fspromisescopyfilesrc-dest-mode
    title: Node.js documentation, File system
    locator: fsPromises.copyFile(src, dest[, mode])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: copy(Path, Path, CopyOption...), StandardCopyOption
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.copy?view=net-10.0
    title: .NET API reference, File.Copy Method
    locator: Copy(String, String, Boolean), Remarks
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func SameFile, func CopyFS
  - url: https://pkg.go.dev/io
    title: Go standard library, package io
    locator: func Copy
  - url: https://doc.rust-lang.org/std/fs/fn.copy.html
    title: The Rust Standard Library, std::fs::copy
    locator: Description, Platform-specific behavior
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/cp.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), cp
    locator: DESCRIPTION, OPTIONS (-p)
  - url: https://man7.org/linux/man-pages/man2/copy_file_range.2.html
    title: Linux manual page, copy_file_range(2)
    locator: DESCRIPTION
  - url: https://man7.org/linux/man-pages/man2/ioctl_ficlone.2.html
    title: Linux manual page, ioctl_ficlone(2)
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/filesystem/copy_file
    title: cppreference, std::filesystem::copy_file
    locator: Description
  - url: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfileexw
    title: Windows API reference, CopyFileExW function (winbase.h)
    locator: Syntax
---

## Task

Copy the file at path `src` to the path `dst`. After the call, `dst` has the same bytes as `src`, and the same permission bits. If a file is at `dst`, replace it. `src` does not change. Copy the bytes with no change: no text mode, no change of line ends, and a NUL byte is a byte.

Report an error, and change no file, when `src` is missing, when the directory of `dst` is missing, when `src` or `dst` is a directory, and when `src` and `dst` name the same file. The last case is important: a copy that opens `dst` for writing first makes the file empty, and then it has no data to read. See the pitfalls.

The permission bits are part of the task, because a copy of a script must stay executable. The times, the owner, the access control lists and other metadata are not part of the task. No copy function copies all of them. Python says: "Even the higher-level file copying functions ( shutil.copy() , shutil.copy2() ) cannot copy all file metadata. On POSIX platforms, this means that file owner and group are lost as well as ACLs."

SQL, regular expressions, HTML and CSS do not apply, because they do not copy files.

## Solutions

Most languages have a copy function, and it is better than a loop that reads and writes, because it can use a fast call of the operating system. The Python documentation says that the copy functions "may use platform-specific “fast-copy” syscalls in order to copy the file more efficiently". On Linux, `copy_file_range` "performs an in-kernel copy between two file descriptors without the additional cost of transferring data from the kernel to user space and then back into the kernel." On a file system with reflinks, a copy can share the blocks of the file: "this ioctl(2) operation can be used to make some of the data in the src_fd file appear in the dest_fd file by sharing the underlying storage, which is faster than making a separate physical copy of the data." The new file gets its own blocks only when a program writes to it ("copy on write").

Each solution checks the directory and same-file cases first, because the copy functions do not agree on them.

- **Python.** `shutil.copy(src, dst)`. "copy() copies the file data and the file’s permission mode (see os.chmod() ). Other metadata, like the file’s creation and modification times, is not preserved." For the same file it raises `shutil.SameFileError`. But "If dst specifies a directory, the file will be copied into dst using the base filename from src", so the code raises `IsADirectoryError` first. `shutil.copyfile` copies only the bytes: "Copy the contents (no metadata) of the file named src to a file named dst".
- **JavaScript and TypeScript (Node.js).** `fsPromises.copyFile(src, dst)`. "By default, dest is overwritten if it already exists." The code compares `dev` and `ino` from `stat` for the same file, gives `EISDIR` for a directory, and calls `chmod` after the copy. `COPYFILE_FICLONE` in the mode argument asks for a reflink: "If the platform does not support copy-on-write, then a fallback copy mechanism is used."
- **Java.** `Files.copy(src, dst, StandardCopyOption.REPLACE_EXISTING)`, then `Files.setPosixFilePermissions`, because "File attributes are not required to be copied to the target file." The code throws a `FileSystemException` for the other cases, because `Files.copy` gives no error for them: "except if the source and target are the same file, in which case the method completes without copying the file", and "If the file is a directory then an empty directory is created in the target location".
- **C#.** `File.Copy(src, dst, overwrite: true)`. "The attributes of the original file are retained in the copied file." On Unix, the code also calls `File.SetUnixFileMode` after the copy. For a directory at `src` or `dst`, it throws an `IOException`. For the same file, `File.Copy` throws an `IOException`; the native tests check this for the same path and for a hard link.
- **Go.** Go has no function that copies one file. (`os.CopyFS` copies a tree, and it "will not overwrite existing files".) So the code opens `src`, checks `os.SameFile`, opens `dst` with `O_TRUNC`, and calls `io.Copy`. "If src implements WriterTo , the copy is implemented by calling src.WriteTo(dst). Otherwise, if dst implements ReaderFrom , the copy is implemented by calling dst.ReadFrom(src)." So on Linux, the copy uses the fast calls. The code returns the error of `Close` and calls `Chmod`.
- **Rust.** `std::fs::copy(src, dst)`. "This function will also copy the permission bits of the original file to the destination file." It is not safe for the same file: "Note that if from and to both point to the same file, then the file will likely get truncated by this operation." So the code compares the device and inode numbers first.
- **C.** C has no copy function. The code opens `src` with `open`, checks the same file with `fstat` and `stat`, opens `dst` with `O_WRONLY | O_CREAT | O_TRUNC`, and copies with a loop of `read` and `write`. Each call can do less than the full count, and `EINTR` can stop it, so both are loops. Then `fchmod`, and the result of `close` is checked.
- **C++.** `std::filesystem::copy_file(src, dst, copy_options::overwrite_existing, ec)`. For a new file, it "copies the contents and the attributes of the file". For the other cases it reports an error ("If !filesystem::is_regular_file(from) (either because the source file doesn't exist or because it is not a regular file), report an error"), but the error code is not the same in each library, so the code checks the cases first. The function is in a namespace, because a call with `std::filesystem::path` arguments also finds `std::filesystem::copy_file`.
- **Bash.** `cp -p -- "$src" "$dst"`. With no `-p`, `cp` opens a file at `dst` "using dest_file as the path argument, and the bitwise-inclusive OR of O_WRONLY and O_TRUNC as the oflag argument", so the file keeps its old permission bits. `-p` copies "The file permission bits and the S_ISUID and S_ISGID bits", and also the times. `cp` copies into a directory at `dst`, so the function returns status 1 for a directory, and for the same file (`-ef`).

On Windows, the copy function is `CopyFileEx`. Rust says: "On Windows, this function currently corresponds to CopyFileEx", and "On MacOS, this function corresponds to fclonefileat and fcopyfile".

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each test makes the files in a new temporary directory, calls the function, and checks the bytes at `dst` and at `src`.

- `copy` and `replace-existing`: a new path, and a path with a longer file. The copy must truncate the old file, or the end of the old text stays.
- `empty-file`: an empty file replaces a file with text.
- `other-directory`: `a.txt` to `sub/a.txt`. The directory must exist.
- `name-with-spaces` and `non-ascii-name`: `copy of my notes.txt` and `café 日本.txt`. A shell solution must quote the names.
- `binary-data`: the bytes `00 0D 0A C3 BF 00`. A copy in text mode can change `0D 0A` on Windows, and a copy with C strings stops at the first `00`.
- Errors, in `cases/errors.jsonl`: a missing `src` and a missing directory of `dst` give not-found, a directory at `src` or `dst` gives is-a-directory, and the same path gives same-file.

The native tests also check that `dst` gets the permission bits `0750` of `src`, also when it replaces a file with `0640`; that a file of 5000003 bytes, more than one buffer, is copied byte for byte; that a symbolic link at `src` gives a copy of its target, not a link; and that a hard link to `src` at `dst` gives same-file, and the file keeps its data.

## Pitfalls

- **A copy of a file onto itself.** A copy that opens `dst` with truncation makes the file empty before it reads it. The two paths can be different and still name one file, for example through a hard link or a symbolic link:

```python wrong id=python-copy-onto-itself
import os
import tempfile


def naive_copy(src, dst):
    with open(src, "rb") as f, open(dst, "wb") as out:  # "wb" makes dst empty
        out.write(f.read())


with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "report.txt")
    with open(path, "w") as f:
        f.write("alpha\n")
    naive_copy(path, path)  # src and dst are the same file
    print("size:", os.path.getsize(path))
```

```output
size: 0
```

Compare the device and inode numbers of the two files before the copy, as `shutil.copy` and `cp` do. A check of the two path strings is not sufficient.

- **A copy that keeps the old permission bits.** `cp` with no `-p` keeps the mode of a file at `dst`, so a new version of a script is not executable:

```bash wrong id=bash-cp-keeps-old-mode
dir=$(mktemp -d)
printf '#!/bin/sh\necho new\n' >"$dir/new.sh"
printf '#!/bin/sh\necho old\n' >"$dir/run.sh"
chmod 755 "$dir/new.sh"
chmod 644 "$dir/run.sh"  # the old copy is not executable
cp "$dir/new.sh" "$dir/run.sh"
ls -l "$dir/run.sh" | cut -c1-10
rm -r "$dir"
```

```output
-rw-r--r--
```

`shutil.copyfile`, Java `Files.copy` and a loop of `read` and `write` can also give `dst` other bits than `src`. Set the bits after the copy.

- **A copy that changes the time.** `shutil.copy` gives the copy the time of the copy. Tools that compare times, such as build tools and backup programs, then take the copy for a new file. `shutil.copy2` also copies the times:

```python run id=python-copy-and-copy2-times
import os
import shutil
import tempfile

with tempfile.TemporaryDirectory() as dir:
    src = os.path.join(dir, "data.csv")
    with open(src, "w") as f:
        f.write("a,b\n")
    os.utime(src, (1_600_000_000, 1_600_000_000))  # an old file
    shutil.copy(src, os.path.join(dir, "copy.csv"))
    shutil.copy2(src, os.path.join(dir, "copy2.csv"))
    for name in ("copy.csv", "copy2.csv"):
        same = os.path.getmtime(os.path.join(dir, name)) == os.path.getmtime(src)
        print(name, "has the time of src:", same)
```

```output
copy.csv has the time of src: False
copy2.csv has the time of src: True
```

- **A copy that a reader sees in parts.** A copy is not atomic. Node says: "No guarantees are made about the atomicity of the copy operation." A program that opens `dst` during the copy reads a part of the file. If `dst` must always be a full file, copy to a temporary name in the directory of `dst`, and then rename it over `dst`, as in [[rename-a-file]]:

```python run id=python-copy-then-rename
import os
import shutil
import tempfile


def copy_whole(src, dst):
    dir = os.path.dirname(os.path.abspath(dst))
    fd, tmp = tempfile.mkstemp(dir=dir, prefix=".tmp-")
    os.close(fd)
    try:
        shutil.copy(src, tmp)
        os.replace(tmp, dst)  # a reader sees the old dst or the full copy
    except BaseException:
        os.unlink(tmp)
        raise


with tempfile.TemporaryDirectory() as dir:
    src, dst = os.path.join(dir, "new.conf"), os.path.join(dir, "app.conf")
    for path, text in ((src, "port = 9090\n"), (dst, "port = 8080\n")):
        with open(path, "w") as f:
            f.write(text)
    copy_whole(src, dst)
    with open(dst) as f:
        print(f.read(), end="")
    print(sorted(os.listdir(dir)))
```

```output
port = 9090
['app.conf', 'new.conf']
```

- **A copy that reads the whole file into memory.** Code such as `out.write(f.read())` needs as much memory as the file. For a large file, use the copy function of the language, or a loop with a buffer of a fixed size, as the C solution does.

- **A copy of a symbolic link.** Most copy functions follow a link at `src` and copy the target. Node says: "If src is a symbolic link, the target file is copied." To copy the link itself, use `follow_symlinks=False` in Python or `LinkOption.NOFOLLOW_LINKS` in Java.

## Related tasks

- [[rename-a-file]]
- [[check-whether-a-file-exists]]
- [[read-and-write-with-buffers]]
- [[create-a-temporary-file]]
- [[get-the-size-of-a-file]]
- [[file-permissions]]

## References
