---
tag: VPR8VW
kind: task
slug: delete-a-file
name: Delete a file
aliases: [remove a file, unlink a file, delete a file if it exists, rm]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Delete the file at a path and tell whether a file was there, with no error for a missing file but an error for a directory and for each other failure, such as a permission error.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/os.html#os.remove
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.remove, os.unlink
  - url: https://nodejs.org/api/fs.html#fspromisesunlinkpath
    title: Node.js documentation, File system
    locator: fsPromises.unlink(path), fs.unlink(path, callback)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: delete(Path), deleteIfExists(Path)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.desktop/java/awt/Desktop.html
    title: Java SE 25 API, Desktop
    locator: moveToTrash(File)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.delete?view=net-10.0
    title: .NET API reference, File.Delete(String) Method
    locator: Remarks, Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualbasic.fileio.filesystem.deletefile?view=net-10.0
    title: .NET API reference, FileSystem.DeleteFile Method
    locator: DeleteFile(String, UIOption, RecycleOption)
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Remove, func Lstat
  - url: https://doc.rust-lang.org/std/fs/fn.remove_file.html
    title: The Rust Standard Library, std::fs::remove_file
    locator: Platform-specific behavior, Errors
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/unlink.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), unlink, unlinkat
    locator: DESCRIPTION, ERRORS
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/remove.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), remove
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/rm.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), rm
    locator: DESCRIPTION, OPTIONS (-f)
  - url: https://man7.org/linux/man-pages/man2/unlink.2.html
    title: Linux manual page, unlink(2)
    locator: DESCRIPTION, ERRORS (EISDIR)
  - url: https://en.cppreference.com/w/cpp/filesystem/remove
    title: cppreference, std::filesystem::remove, std::filesystem::remove_all
    locator: Return value
  - url: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-deletefilew
    title: Windows API reference, DeleteFileW function (fileapi.h)
    locator: Remarks
  - url: https://specifications.freedesktop.org/trash/latest/
    title: freedesktop.org, Trash specification
    locator: Abstract
---

## Task

Delete the file at a path. Return true if a file was there and the call deleted it, and false if no file was there. A missing file is not an error: the result is the same, because no file is at the path after the call. A missing directory in the path, or a path through a regular file such as `a.txt/b.txt`, also gives false.

Report each other error, and do not take it for a missing file. For example, if the directory does not let the program write to it, the file stays, and the call must say so. A directory at the path is an error, also an empty one: this task deletes files, not directories.

If the path names a symbolic link, delete the link, not its target. A broken link is an entry too, so the call deletes it and returns true.

Do not check first and then delete. The answer of the check can be old when the program acts on it, and a check that gives false for a broken link leaves the link there. Delete, then look at the error. See the pitfalls.

SQL, regular expressions, HTML and CSS do not apply, because they do not delete files.

## Solutions

On Unix, a file has no call that deletes the data. The `unlink` call removes one name of the file from its directory: "When the file's link count becomes 0 and no process has a reference to the file via an open file descriptor or a memory mapping (see mmap () ), the space occupied by the file shall be freed and the file shall no longer be accessible." So the right to delete a file comes from the directory, not from the file: `unlink` fails with `EACCES` when "write permission is denied on the directory containing the directory entry to be removed".

Each solution calls the delete function of its language. If the error is `ENOENT` (no such entry) or `ENOTDIR` (a part of the path is not a directory), the solution returns false. Each other error goes to the caller. Some delete functions also delete an empty directory, so those solutions look at the entry first, with no link followed, and report an error for a directory.

- **Python.** `os.remove(path)`. "If path is a directory, an OSError is raised." A missing file gives `FileNotFoundError`, and a path through a file gives `NotADirectoryError`, so the code looks at `errno` for both.
- **JavaScript and TypeScript (Node.js).** `fsPromises.unlink(path)`, which "Asynchronously removes a file or symbolic link." On an error with `code` `ENOENT` or `ENOTDIR`, the code returns `false`.
- **Java.** `Files.delete(path)` also deletes a directory: "If the file is a directory then the directory must be empty." So the code reads the attributes with `LinkOption.NOFOLLOW_LINKS` first and throws a `FileSystemException` for a directory. For a link, "the symbolic link itself, not the final target of the link, is deleted." Java has no exception type for `ENOTDIR`, so the code looks for the nearest parent that is there, as in [[check-whether-a-file-exists]]. `Files.deleteIfExists` returns a boolean too, but it also deletes an empty directory.
- **C#.** `File.Delete(path)` gives no result: "If the file to be deleted does not exist, no exception is thrown." So the code calls `File.GetAttributes` first. That call does not follow a link, and it throws `FileNotFoundException` or `DirectoryNotFoundException` when no entry is there. For a directory, `File.Delete` throws `UnauthorizedAccessException`, and the code throws an `IOException` first.
- **Go.** `os.Remove` "removes the named file or (empty) directory." So the code calls `os.Lstat` first and returns an error for a directory. Then `os.Remove`. `errors.Is(err, fs.ErrNotExist)` or `errors.Is(err, syscall.ENOTDIR)` gives false.
- **Rust.** `std::fs::remove_file(path)`. "This function currently corresponds to the unlink function on Unix." An error of kind `ErrorKind::NotFound` or `ErrorKind::NotADirectory` gives `Ok(false)`.
- **C.** POSIX `unlink(path)`. It returns 1, 0, or -1 with `errno` for an error. The code does not use ISO C `remove`: "If path names a directory, remove ( path ) shall be equivalent to rmdir ( path )."
- **C++.** `std::filesystem::remove` deletes "The file or empty directory identified by the path p", and it returns "true if the file was deleted, false if it did not exist." So the code calls `std::filesystem::symlink_status` first and gives `std::errc::is_a_directory` for a directory. The function returns `std::expected<bool, std::error_code>` (C++23).
- **Bash.** `rm -- "$path"`, with standard input from `/dev/null`. `--` stops options, so a name such as `-n` is a file name. `rm` asks before it deletes a read-only file when standard input is a terminal: "If the -f option is not specified, and either the permissions of file do not permit writing and the standard input is a terminal or the -i option is specified, rm shall write a prompt to the standard error and read a line from the standard input." The code does not use `-f`, because it hides a missing file. If `rm` fails, the function looks at what is at the path, as in [[check-whether-a-file-exists]].

A program for people can move a file to the trash, and not delete it. There is no portable call for that. Java has `Desktop.moveToTrash` (since Java 9), which "Moves the specified file to the trash", and which throws `UnsupportedOperationException` on a platform with no trash. In .NET, `Microsoft.VisualBasic.FileIO.FileSystem.DeleteFile` takes a `RecycleOption`, for the Windows Recycle Bin. Linux desktops use the freedesktop.org Trash specification: "By complying with this Specification, various Trash implementations will be able to work with the same devices and use the same Trash storage."

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each test makes the file in a new temporary directory, calls the function, and checks the result and that no entry is at the path after the call.

- `delete` and `missing`: a file with text gives true, and a name that is not there gives false.
- `empty-file`: an empty file is a file; its size does not matter.
- `missing-parent`: `no-such-dir/a.txt` gives false, not an error.
- `name-with-spaces` and `non-ascii-name`: `my notes.txt` and `café 日本.txt`. A shell solution must quote the name.
- Errors, in `cases/errors.jsonl`: an empty directory at the path gives an error, and the directory stays.

The native tests also check that a path through a regular file gives false; that a link to a file, a link to a directory and a broken link are deleted, and their targets stay; that a read-only file is deleted; that a file in a directory with mode `500` gives a permission error and stays (skipped for root, because root can write to each directory); and that an open file is deleted while the open stream can still read it.

On Windows, an open file is not the same. `os.remove` says: "On Windows, attempting to remove a file that is in use causes an exception to be raised; on Unix, the directory entry is removed but the storage allocated to the file is not made available until the original file is no longer in use." The Windows `DeleteFile` function can delete an open file only if each other handle allows it ("FILE_SHARE_DELETE must have been specified when other handles were opened"), and then "the file deletion does not occur until the last handle to the file is closed". Also on Windows, a read-only file cannot be deleted: "If the file is a read-only file, the function fails with ERROR_ACCESS_DENIED ." The native tests run on Unix only.

## Pitfalls

- **A check before the delete.** `os.path.exists` follows links, so it gives `False` for a broken link, and the code does not delete the link:

```python wrong id=python-check-then-delete
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    link = os.path.join(dir, "report.txt")
    os.symlink("old-report.txt", link)  # a link to a file that is gone
    if os.path.exists(link):  # False for a broken link
        os.remove(link)
    print(os.listdir(dir))
```

```output
['report.txt']
```

The name `report.txt` is not free, so a new file with that name goes through the link to `old-report.txt`. A check also has a race: another program can delete the file between the check and the delete, and then the delete fails. Call the delete function and look at the error.

- **A delete function that also deletes a directory.** Go `os.Remove`, Java `Files.delete`, C `remove` and C++ `std::filesystem::remove` delete an empty directory with no error. If the path names a directory by mistake, the directory is gone:

```go wrong id=go-remove-directory
package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	dir, _ := os.MkdirTemp("", "remove")
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "cache") // the program thinks this is a file
	os.Mkdir(path, 0o755)
	fmt.Println(os.Remove(path))
	_, err := os.Stat(path)
	fmt.Println(os.IsNotExist(err))
}
```

```output
<nil>
true
```

- **An error that is hidden.** `rm -f` and C# `File.Delete` give no error for a missing file. "-f Do not prompt for confirmation. Do not write diagnostic messages or modify the exit status in the case of no file operands, or in the case of operands that do not exist." A typo in the name then deletes nothing, and the program does not know:

```bash wrong id=bash-rm-f-typo
dir=$(mktemp -d)
printf 'alpha\n' >"$dir/report.txt"
rm -f "$dir/reprot.txt"  # a typo in the name
echo "rm -f gives $?"
ls "$dir"
rm -r "$dir"
```

```output
rm -f gives 0
report.txt
```

The same is true for code that catches each error and takes it for a missing file. Rust says: "This function will only ever return an error of kind NotFound if the given path does not exist. Note that the inverse is not true".

- **The space comes back only at the close.** On Unix, a delete removes the name, but a program that has the file open can still read it, and the disk space is free only when the last program closes the file. A large log file that a server keeps open takes its space after the delete:

```python run id=python-delete-open-file
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "big.log")
    with open(path, "w") as f:
        f.write("alpha\n")
    with open(path) as f:
        os.remove(path)
        print("name there:", os.path.exists(path))
        print("links to the data:", os.fstat(f.fileno()).st_nlink)
        print("data:", f.read(), end="")
```

```output
name there: False
links to the data: 0
data: alpha
```

The Linux manual says: "If the name was the last link to a file but any processes still have the file open, the file will remain in existence until the last file descriptor referring to it is closed." To give back the space of a log that is open, truncate the file, or make the program close it.

- **A read-only file is not safe from a delete.** On Unix, the mode of the file does not decide; the mode of the directory does:

```python run id=python-delete-read-only
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "keep.txt")
    with open(path, "w") as f:
        f.write("alpha\n")
    os.chmod(path, 0o444)  # read-only, but the directory lets us write to it
    os.remove(path)
    print("deleted:", not os.path.exists(path))
```

```output
deleted: True
```

To keep a file safe, take the write permission away from its directory. On Windows it is the other way: `DeleteFile` fails for a read-only file.

## Related tasks

- [[check-whether-a-file-exists]]
- [[rename-a-file]]
- [[create-a-temporary-file]]
- [[truncate-a-file]]
- [[file-permissions]]
- [[hard-links-and-symbolic-links]]

## References
