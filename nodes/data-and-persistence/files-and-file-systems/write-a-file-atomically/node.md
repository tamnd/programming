---
tag: 1W9Y7Z
kind: technique
slug: write-a-file-atomically
name: Write a file atomically
aliases: [atomic write, atomic file write, write and rename, safe save, atomic replace]
area: data-and-persistence/files-and-file-systems
level: advanced
summary: >-
  Write the new content to a temporary file in the same directory, sync it, and rename it over the target, so that a reader or a crash sees the old file or the new file, never half of one.
see_also: [create-a-temporary-file, rename-a-file, write-a-file, sync-a-file-to-disk, truncate-a-file, hard-links-and-symbolic-links]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), rename, renameat
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fsync.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), fsync
    locator: DESCRIPTION
  - url: https://man7.org/linux/man-pages/man2/rename.2.html
    title: Linux manual page, rename(2)
    locator: DESCRIPTION, ERRORS (EXDEV)
  - url: https://man7.org/linux/man-pages/man2/fsync.2.html
    title: Linux manual page, fsync(2)
    locator: DESCRIPTION
  - url: https://www.usenix.org/conference/osdi14/technical-sessions/presentation/pillai
    title: "Pillai, Chidambaram, Alagappan, Al-Kiswany, Arpaci-Dusseau and Arpaci-Dusseau, All File Systems Are Not Created Equal: On the Complexity of Crafting Crash-Consistent Applications, OSDI 2014"
    locator: Abstract
  - url: https://docs.python.org/3/library/os.html
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.replace, os.fsync
  - url: https://docs.python.org/3/library/tempfile.html
    title: The Python Standard Library, tempfile, Generate temporary files and directories
    locator: tempfile.mkstemp
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Rename, func (*File) Sync, func CreateTemp
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: move (StandardCopyOption.ATOMIC_MOVE)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html
    title: Java SE 25 API, FileChannel
    locator: force(boolean)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.move?view=net-10.0
    title: .NET API reference, File.Move Method
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestream.flush?view=net-10.0
    title: .NET API reference, FileStream.Flush Method
    locator: Flush(Boolean)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.replace?view=net-10.0
    title: .NET API reference, File.Replace Method
    locator: Definition
  - url: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefileexw
    title: Windows API reference, MoveFileExW function (winbase.h)
    locator: Parameters (dwFlags)
  - url: https://nodejs.org/api/fs.html#fspromisesrenameoldpath-newpath
    title: Node.js documentation, File system
    locator: fsPromises.rename, filehandle.sync()
  - url: https://doc.rust-lang.org/std/fs/fn.rename.html
    title: The Rust Standard Library, std::fs::rename
    locator: Description, Platform-specific behavior
  - url: https://doc.rust-lang.org/std/fs/struct.File.html
    title: The Rust Standard Library, std::fs::File
    locator: sync_all, sync_data
---

## Purpose

A program that saves a file in place first truncates it and then writes the new content. When the program crashes, or the machine loses power between the two steps, the file is empty or holds half of the new content. A reader that opens the file during the write also sees half of it. For a configuration file or a saved state, this is data loss.

The technique uses the guarantee of `rename`. POSIX says that when the new name exists, "it shall be removed and old renamed to new . In this case, a directory entry named new shall remain visible to other threads throughout the renaming operation and refer either to the file referred to by new or old before the operation began." Linux says the same in other words: the new path "will be atomically replaced, so that there is no point at which another process attempting to access newpath will find it missing." So the program writes the new content to another file, and then renames that file over the target in one step.

## Steps

1. **Make a temporary file in the directory of the target.** `rename` works only on one file system. Linux gives `EXDEV` when "oldpath and newpath are not on the same mounted filesystem". So do not use `/tmp`: use the same directory as the target. Make the file with a call that checks the name, as in [[create-a-temporary-file]].
2. **Write all the new content to the temporary file.** If the target has a mode that the program must keep, set it on the temporary file now. A file from `mkstemp` "is readable and writable only by the creating user ID".
3. **Sync the temporary file.** POSIX says that `fsync` "shall request that all data for the open file descriptor named by fildes is to be transferred to the storage device". Without this step, the rename can reach the disk before the data does. Then, after a crash, the target can have the new name but not all of the new data.
4. **Close the temporary file**, and check the error of the close.
5. **Rename the temporary file over the target**, with `rename` in C, `os.replace` in Python, or the call of your language. Python says: "If successful, the renaming will be an atomic operation (this is a POSIX requirement)."
6. **Sync the directory.** The rename changes the directory, not the file. Linux says that `fsync` "does not necessarily ensure that the entry in the directory containing the file has also reached disk. For that an explicit fsync () on a file descriptor for the directory is also needed."
7. **On an error, remove the temporary file**, so that it is not left in the directory. The target stays as it was.

## Example

In Python. The function `write_atomically` does the steps. The test then makes a crash in the middle of a save: the target still holds its old content, and no temporary file is left.

```python run id=python-write-atomically
import os
import tempfile


def write_atomically(path, text, fail_after=None):
    directory = os.path.dirname(os.path.abspath(path))
    fd, tmp = tempfile.mkstemp(prefix=".tmp-", dir=directory)
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            f.write(text if fail_after is None else text[:fail_after])
            if fail_after is not None:
                raise RuntimeError("crash in the middle of the write")
            f.flush()
            os.fsync(f.fileno())
        os.replace(tmp, path)
    except BaseException:
        os.remove(tmp)
        raise
    dir_fd = os.open(directory, os.O_RDONLY)
    try:
        os.fsync(dir_fd)
    finally:
        os.close(dir_fd)


os.chdir(tempfile.mkdtemp())  # an empty directory for the example
write_atomically("config.txt", "colour = blue\n")
try:
    write_atomically("config.txt", "colour = green\n", fail_after=5)
except RuntimeError as e:
    print("error:", e)
with open("config.txt", encoding="utf-8") as f:
    print("config.txt holds:", f.read(), end="")
print("files:", sorted(os.listdir(".")))
```

```output
error: crash in the middle of the write
config.txt holds: colour = blue
files: ['config.txt']
```

In place, the same crash leaves half a file:

```python wrong id=python-write-in-place
import os
import tempfile

os.chdir(tempfile.mkdtemp())
with open("config.txt", "w", encoding="utf-8") as f:
    f.write("colour = blue\n")

try:
    with open("config.txt", "w", encoding="utf-8") as f:  # "w" truncates the file first
        f.write("colou")
        raise RuntimeError("crash in the middle of the write")
except RuntimeError:
    pass
with open("config.txt", encoding="utf-8") as f:
    print("config.txt holds:", repr(f.read()))
```

```output
config.txt holds: 'colou'
```

In Go, with `os.CreateTemp`, `File.Sync`, `os.Rename` and a sync of the directory:

```go run id=go-write-atomically
package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func writeAtomically(path string, data []byte) (err error) {
	dir := filepath.Dir(path)
	f, err := os.CreateTemp(dir, ".tmp-*")
	if err != nil {
		return err
	}
	defer func() {
		if err != nil {
			os.Remove(f.Name())
		}
	}()
	if _, err = f.Write(data); err != nil {
		f.Close()
		return err
	}
	if err = f.Sync(); err != nil {
		f.Close()
		return err
	}
	if err = f.Close(); err != nil {
		return err
	}
	if err = os.Rename(f.Name(), path); err != nil {
		return err
	}
	d, err := os.Open(dir)
	if err != nil {
		return err
	}
	defer d.Close()
	return d.Sync()
}

func main() {
	work, err := os.MkdirTemp("", "atomic-") // an empty directory for the example
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(work)
	os.Chdir(work)
	if err := writeAtomically("config.txt", []byte("colour = blue\n")); err != nil {
		panic(err)
	}
	b, _ := os.ReadFile("config.txt")
	entries, _ := os.ReadDir(".")
	fmt.Printf("config.txt holds: %s", b)
	fmt.Println("files:", len(entries))
}
```

```output
config.txt holds: colour = blue
files: 1
```

## When to use

Use the technique when a program replaces the whole content of a small or medium file that other programs read, or that must survive a crash: configuration files, saved state, caches, and generated files. A reader opens the old file or the new file, and an open file stays as it was, because the rename gives the name to a new file.

Do not use it to add to a log or to change a part of a large file. The technique writes the whole file again each time. It also makes a new file, so the target loses what belongs to the old file: other hard links still point to the old content, and the owner, the mode and other attributes come from the temporary file unless the program copies them. When the target is a symbolic link, POSIX says: "If the new argument points to a pathname of a symbolic link, the symbolic link shall be removed." So the link becomes a regular file, and the file that it pointed to does not change. To write through a link, rename over the target of the link. See [[hard-links-and-symbolic-links]].

## Pitfalls

- **A temporary file on another file system.** A file in `/tmp` cannot be renamed into `/home` when they are two file systems. Rust says that `rename` "will not work if the new name is on a different mount point". In Java, with `ATOMIC_MOVE`, "If the move cannot be performed as an atomic file system operation then AtomicMoveNotSupportedException is thrown. This can arise, for example, when the target location is on a different FileStore". A move that falls back to a copy is not atomic.
- **No sync before the rename.** A rename that reaches the disk before the data can leave an empty or short file after a power loss. A study of eleven applications found that their update protocols depend on "subtle behaviors of the underlying file system", and found "a total of 60 vulnerabilities".[^5] Sync the file before the rename, and the directory after it.
- **Windows.** Go says: "Even within the same directory, on non-Unix platforms Rename is not an atomic operation." On Windows, `MoveFileExW` with `MOVEFILE_REPLACE_EXISTING` replaces the target, and `MOVEFILE_WRITE_THROUGH` means that "The function does not return until the file is actually moved on the disk." .NET has `File.Move(source, dest, overwrite: true)` and `File.Replace`, which "Replaces the contents of a specified file with the contents of another file, deleting the original file and creating a backup of the replaced file."
- **A copy is not a rename.** .NET says of copy and delete: "This strategy isn't atomic, because a system or program crash during the Copy may leave a partially-written destination file".
- **Java does not say what happens to the target.** With `ATOMIC_MOVE`, "If the target file exists then it is implementation specific if the existing file is replaced or this method fails by throwing an IOException ." Test the call on each platform that the program supports.
- **Two writers.** Two processes that write the same target with this technique do not corrupt it, but the last rename wins, and the other change is lost. To stop that, use a lock. See [[lock-a-file]].

## Tools that do it

Each language has the calls for the steps. None of the standard libraries below has one call that does all the steps.

| Language | Temporary file | Sync the file | Rename over the target |
|---|---|---|---|
| C (POSIX) | `mkstemp` | `fsync` | `rename` |
| Python | `tempfile.mkstemp` | `os.fsync` | `os.replace` |
| Go | `os.CreateTemp` | `(*os.File).Sync` | `os.Rename` |
| Rust | `OpenOptions::create_new` | `File::sync_all` | `std::fs::rename` |
| Java | `Files.createTempFile` | `FileChannel.force` | `Files.move` with `ATOMIC_MOVE` |
| C# | `FileMode.CreateNew` | `FileStream.Flush(true)` | `File.Move` with `overwrite: true`, or `File.Replace` |
| Node.js | `open` with `"wx"` | `filehandle.sync()` | `fsPromises.rename` |

Go says that `Sync` "commits the current contents of the file to stable storage." Rust has `sync_all`, which "Attempts to sync all OS-internal file content and metadata to disk", and `sync_data`, which "might not synchronize file metadata to the filesystem". C# `Flush(true)` is for when "you want to ensure that all buffered data in intermediate file buffers is written to disk". Java `force` "Forces any updates to this channel's file to be written to the storage device that contains it". Node.js `filehandle.sync()` will "Request that all data for the open file descriptor is flushed to the storage device." Node.js `rename` says: "In the case that newPath already exists, it will be overwritten."

## References
