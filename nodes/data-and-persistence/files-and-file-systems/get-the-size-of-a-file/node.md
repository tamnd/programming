---
tag: P9HCNV
kind: task
slug: get-the-size-of-a-file
name: Get the size of a file
aliases: [file size, size of a file in bytes, length of a file, stat size, st_size]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Get the size in bytes of a regular file from the file system, with no read of its data, and know why it is not the count of characters or the space on the disk.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/os.html#os.stat_result
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.stat, stat_result.st_size, stat_result.st_blocks
  - url: https://docs.python.org/3/library/os.path.html#os.path.getsize
    title: The Python Standard Library, os.path, Common pathname manipulations
    locator: os.path.getsize
  - url: https://docs.python.org/3/library/io.html#io.IOBase.flush
    title: The Python Standard Library, io, Core tools for working with streams
    locator: IOBase.flush
  - url: https://nodejs.org/api/fs.html#class-fsstats
    title: Node.js documentation, File system
    locator: Class fs.Stats, stats.size, stats.blocks
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: size(Path), readAttributes(Path, Class, LinkOption...)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.fileinfo.length?view=net-10.0
    title: .NET API reference, FileInfo.Length Property
    locator: Exceptions, Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filesysteminfo.resolvelinktarget?view=net-10.0
    title: .NET API reference, FileSystemInfo.ResolveLinkTarget(Boolean) Method
    locator: Description
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Stat, func (*File) Stat
  - url: https://doc.rust-lang.org/std/fs/struct.Metadata.html
    title: The Rust Standard Library, std::fs::Metadata
    locator: len
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), sys/stat.h
    locator: DESCRIPTION (st_size, st_blocks)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/wc.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), wc
    locator: OPTIONS (-c, -m), STDOUT
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/du.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), du
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/filesystem/file_size
    title: cppreference, std::filesystem::file_size
    locator: Description
  - url: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesizeex
    title: Windows API reference, GetFileSizeEx function (fileapi.h)
    locator: Syntax
---

## Task

Return the size in bytes of the regular file at `path`. The size comes from the file system, as the `st_size` field of `stat` gives it. POSIX defines that field: "For regular files, the file size in bytes. For symbolic links, the length in bytes of the pathname contained in the symbolic link." So the function must follow a symbolic link, and give the size of its target. The function does not read the data of the file, so a file of some gigabytes takes no more time than a small one.

Report an error when no file is at `path`, and when `path` is a directory. Most calls give a number for a directory, but that number is not the size of a file, and it is not the same on each file system. C++ says: "The result of attempting to determine the size of a directory (as well as any other file that is not a regular file or a symlink) is implementation-defined."

The size is a count of bytes. It is not a count of characters, and it is not the space that the file uses on the disk. The pitfalls show both.

SQL, regular expressions, HTML and CSS do not apply, because they do not get information about files.

## Solutions

Each language has a call that gets the status of a file, and the size is one field of it. Some languages also have a call that gives only the size. Each solution gets the status one time, and uses it for the directory check and for the size, so that the two values come from the same file.

- **Python.** `os.stat(path).st_size`. `os.stat` does "the equivalent of a stat() system call on the given path", and `st_size` is the "Size of the file in bytes, if it is a regular file or a symbolic link." The code raises `IsADirectoryError` for a directory. `os.path.getsize(path)` is shorter: "Return the size, in bytes, of path . Raise OSError if the file does not exist or is inaccessible." But it gives a number for a directory.
- **JavaScript and TypeScript (Node.js).** `(await fsPromises.stat(path)).size`. `stats.size` is "The size of the file in bytes. If the underlying file system does not support getting the size of the file, this will be 0 ." The code throws an error with the code `EISDIR` for a directory. Use `stat(path, { bigint: true })` to get a `bigint`.
- **Java.** `Files.readAttributes(path, BasicFileAttributes.class).size()`. `Files.size(path)` "Returns the size of a file (in bytes)", but "The size of files that are not regular files is implementation specific and therefore unspecified." So the code reads the attributes, and throws a `FileSystemException` for a directory. The size is a `long`.
- **C#.** `new FileInfo(path).Length`. It "Gets the size, in bytes, of the current file". It throws `FileNotFoundException` when "The file does not exist. -or- The Length property is called for a directory." So the code throws an `IOException` for a directory first, to keep the two errors apart. For a symbolic link, `Length` is the size of the link, not the size of the target. The native test shows this on macOS. So the code calls `ResolveLinkTarget(returnFinalTarget: true)` first.
- **Go.** `os.Stat(path)` and then `fi.Size()`, an `int64`. "Stat returns a FileInfo describing the named file. If there is an error, it will be of type *PathError ." The code returns a `*fs.PathError` with `syscall.EISDIR` for a directory. For a file that is open, `f.Stat()` gives the same `FileInfo`.
- **Rust.** `fs::metadata(path)?.len()`, a `u64`. It "Returns the size of the file, in bytes, this metadata is for." The code returns `ErrorKind::IsADirectory` for a directory.
- **C.** POSIX `stat(path, &st)` and then `st.st_size`, an `off_t`. The function returns 0 or -1, and puts the size in an argument, because -1 is not a size. For a directory it sets `errno` to `EISDIR`. ISO C has no call that gets the size of a file. The code `fseek(f, 0, SEEK_END)` and then `ftell(f)` is common, but it opens the file, and `ftell` returns a `long`.
- **C++.** `std::filesystem::file_size(path, ec)`. "For a regular file p , returns the size determined as if by reading the st_size member of the structure obtained by POSIX stat (symlinks are followed)." The code checks for a directory first, because the result for a directory is implementation-defined. The function is in a namespace, because a call with `std::filesystem::path` arguments also finds `std::filesystem::file_size`.
- **Bash.** POSIX has no `stat` utility, so the code uses `wc -c <"$path"`. The `-c` option will "Write to the standard output the number of bytes in each input file". The input comes from a redirection, and "If no input file operands are specified, no name shall be written". Some versions of `wc` put spaces before the number, so the code removes them. On macOS and on Linux, `wc -c` gets the size of a regular file from the file system, with no read: the native test of a sparse file of 5 GiB takes less than one second. The function returns status 1 for a missing file and for a directory.

On Windows, the system call that gives the size of an open file is `GetFileSizeEx`. It "Retrieves the size of the specified file" through a handle.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each test writes the file in a new temporary directory, and checks the size that the function returns.

- `text`: `alpha\n` is 6 bytes.
- `non-ascii-text`: `café\n` is 5 characters, but 6 bytes in UTF-8.
- `empty-file`: the size is 0. That is a result, not an error.
- `binary-data`: the bytes `00 0D 0A C3 BF 00`. The size does not change in text mode: a NUL byte and a line end are bytes.
- `crlf-line-ends`: `a\r\nb\r\n` is 6 bytes, also on Windows.
- `name-with-spaces` and `non-ascii-name`: `my notes.txt` and `café 日本.txt`. A shell solution must quote the names.
- Errors, in `cases/errors.jsonl`: a missing file gives not-found, and a directory gives is-a-directory.

The native tests also check that a symbolic link gives the size of its target (11, not the 5 bytes of the name `a.txt`); that a sparse file of 5368709120 bytes (5 GiB) gives that size, more than a 32-bit integer can hold; and that a file that a program has open gives the new size after the program flushes its data.

## Pitfalls

- **A size before the flush.** A buffered stream keeps the data in memory until the buffer is full, or until a flush or a close. Until then, the file system does not have the data, and the size does not include it:

```python wrong id=python-size-before-flush
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "app.log")
    with open(path, "w") as log:
        log.write("alpha\n")
        print("size:", os.path.getsize(path))  # the data is still in the buffer
```

```output
size: 0
```

Call `flush()` first, to "Flush the write buffers of the stream if applicable." The C test shows the same with `fputs` and `fflush`. An `*os.File` in Go and a `File` in Rust have no buffer, so a write goes to the file system at once.

- **A count of characters is not a size.** The length of the text that a program reads is a count of characters after decoding, not a count of bytes:

```python wrong id=python-len-of-text
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "a.txt")
    with open(path, "w", encoding="utf-8") as f:
        f.write("café\n")
    with open(path, encoding="utf-8") as f:
        print("characters:", len(f.read()))
    print("bytes:", os.path.getsize(path))
```

```output
characters: 5
bytes: 6
```

The same is true for `wc -m` against `wc -c`. `-m` will "Write to the standard output the number of characters in each input file." Also, a read of all the data to count it is slow for a large file, and a program does not need it.

- **The size is not the space on the disk.** A file can have holes, parts that have no blocks on the disk and that read as zero bytes. Python says that `st_blocks` is the "Number of 512-byte blocks allocated for file. This may be smaller than st_size /512 when the file has holes." A file system that compresses data can also use less space than the size. Java says: "The size may differ from the actual size on the file system due to compression, support for sparse files, or other reasons."

```python run id=python-sparse-size-on-disk
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "sparse.bin")
    with open(path, "wb") as f:
        f.truncate(1024**3)  # 1 GiB, with no data written
    st = os.stat(path)
    print("size:", st.st_size)
    print("allocated less than size:", st.st_blocks * 512 < st.st_size)
```

```output
size: 1073741824
allocated less than size: True
```

To find the space on the disk, use `st_blocks`, or `du`. By default, du writes "the size of the file space allocated to" each file, not the size.

- **The size of a symbolic link.** `lstat`, and each call that does not follow links, gives the length of the target path, not the size of the target. In C#, `FileInfo.Length` does the same.

- **A size that changes.** The size is correct at the time of the call. Another program can write to the file, or truncate it, before the next step. To read a whole file, read until the end, and do not trust a buffer of the size that `stat` gave. Some files give a size of 0 although they have data. Node says that "If the underlying file system does not support getting the size of the file, this will be 0".

- **A size in a 32-bit integer.** A file can be larger than 2 GiB. Keep the size in a 64-bit type: `off_t` with 64-bit file offsets, `long` in Java and C#, `int64` in Go, `u64` in Rust.

## Related tasks

- [[truncate-a-file]]
- [[read-a-whole-file]]
- [[check-whether-a-file-exists]]
- [[get-the-modification-time-of-a-file]]
- [[file-metadata]]
- [[sparse-file]]
- [[hard-links-and-symbolic-links]]

## References
