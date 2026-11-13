---
tag: QKEG1Z
kind: task
slug: sync-a-file-to-disk
name: Sync a file to disk
aliases: [fsync, fdatasync, flush a file to disk, durable write, sync_all, force a file to disk]
area: data-and-persistence/files-and-file-systems
level: advanced
summary: Write a file and do not return until its data is on the storage device, with fsync on the file and then on its directory, so that the data and the name stay after a crash or a power loss.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fsync.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), fsync
    locator: DESCRIPTION, APPLICATION USAGE, RATIONALE
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fdatasync.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), fdatasync
    locator: DESCRIPTION
  - url: https://man7.org/linux/man-pages/man2/fsync.2.html
    title: Linux manual page, fsync(2)
    locator: DESCRIPTION, ERRORS
  - url: https://man7.org/linux/man-pages/man2/sync.2.html
    title: Linux manual page, sync(2)
    locator: DESCRIPTION
  - url: https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/fsync.2.html
    title: Mac OS X manual page, fsync(2)
    locator: DESCRIPTION
  - url: https://wiki.postgresql.org/wiki/Fsync_Errors
    title: PostgreSQL wiki, Fsync Errors
    locator: Current status, Open source kernels
  - url: https://www.usenix.org/conference/osdi14/technical-sessions/presentation/pillai
    title: "Pillai, Chidambaram, Alagappan, Al-Kiswany, Arpaci-Dusseau and Arpaci-Dusseau, All File Systems Are Not Created Equal: On the Complexity of Crafting Crash-Consistent Applications, OSDI 2014"
    locator: Abstract
  - url: https://docs.python.org/3/library/os.html
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.fsync, os.fdatasync
  - url: https://docs.python.org/3/library/io.html
    title: The Python Standard Library, io, Core tools for working with streams
    locator: IOBase.flush
  - url: https://nodejs.org/api/fs.html
    title: Node.js documentation, File system
    locator: fs.fsync, filehandle.sync()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html
    title: Java SE 25 API, FileChannel
    locator: force(boolean)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestream.flush?view=net-10.0
    title: .NET API reference, FileStream.Flush Method
    locator: Flush(Boolean)
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func (*File) Sync
  - url: https://doc.rust-lang.org/std/fs/struct.File.html
    title: The Rust Standard Library, std::fs::File
    locator: sync_all, sync_data
  - url: https://en.cppreference.com/w/cpp/io/basic_ostream/flush.html
    title: cppreference, std::basic_ostream::flush
    locator: Description
---

## Task

Write a string to a file, and do not return until the data is on the storage device. The entry is `write_synced(path, text)`. It creates or truncates the file, writes all the bytes of `text` as UTF-8, and then:

1. empties the buffer of the language into the kernel;
2. calls `fsync` on the file, or the equal call of the language;
3. closes the file, and checks the error of the close;
4. calls `fsync` on the directory that holds the file.

A write that returns puts the data in the memory of the kernel, not on the disk. The kernel writes it later. When the machine loses power first, the data is lost, and a new file can be lost with its name. POSIX says that `fsync` "is intended to force a physical write of data from the buffer cache, and to assure that after a system crash or other failure that all data up to the time of the fsync () call is recorded on the disk."

The sync of the file does not save its name. Linux says: "Calling fsync () does not necessarily ensure that the entry in the directory containing the file has also reached disk. For that an explicit fsync () on a file descriptor for the directory is also needed." So step 4 is necessary for a new file.

Report an error when the directory of `path` does not exist, and when `path` is a directory. Report each error of the write, the sync and the close. SQL, regular expressions, HTML and CSS do not apply, because they do not write files.

## Solutions

- **Python.** `f.flush()`, then `os.fsync(f.fileno())`. Python says: "If you’re starting with a buffered Python file object f , first do f.flush() , and then do os.fsync(f.fileno()) , to ensure that all internal buffers associated with f are written to disk." On Unix, `os.fsync` "calls the native fsync() function". For the directory, the solution opens it with `os.open(dir, os.O_RDONLY)` and calls `os.fsync` on that descriptor. On macOS it first tries `fcntl(fd, F_FULLFSYNC)` (see Pitfalls).
- **JavaScript and TypeScript (Node.js).** `fs.openSync`, `fs.writeFileSync(fd, text)`, `fs.fsyncSync(fd)` and `fs.closeSync(fd)`. The promise API has `filehandle.sync()`, which will "Request that all data for the open file descriptor is flushed to the storage device." A directory opens with the flag `"r"`, and `fsyncSync` works on it on Linux and macOS.
- **Java.** `FileChannel.force(true)`. Java says: "If this channel's file resides on a local storage device then when this method returns it is guaranteed that all changes made to the file since this channel was created, or since this method was last invoked, will have been written to that device." The argument `false` asks only for the content, as `fdatasync` does. Java has no call for a directory, but on Linux and macOS `FileChannel.open(dir, READ)` opens it, and `force` syncs it.
- **C#.** `FileStream.Flush(true)`. It is for when "you want to ensure that all buffered data in intermediate file buffers is written to disk". .NET cannot open a directory as a stream, so the solution calls `open`, `fsync` and `close` of the C library through `DllImport` for the directory.
- **Go.** `(*os.File).Sync`. Go says that `Sync` "commits the current contents of the file to stable storage." An `*os.File` has no buffer, so no flush comes first. For the directory, `os.Open(dir)` and then `Sync`.
- **Rust.** `File::sync_all`, which "Attempts to sync all OS-internal file content and metadata to disk." It also gives the error that a drop of the file loses: it "can be used to handle errors that would otherwise only be caught when the File is closed, as dropping a File will ignore all errors." For the directory, `File::open(dir)?.sync_all()`.
- **C.** `open`, a loop of `write`, `fsync`, `close`, and then `open(dir, O_RDONLY | O_DIRECTORY)` and `fsync` on the directory. On macOS, `fcntl(fd, F_FULLFSYNC)` comes first.
- **C++.** `std::ofstream::flush` does not reach the device. It "Writes uncommitted changes to the underlying output sequence", which is the kernel. The standard library has no sync call, so the solution uses the POSIX calls, as in C, and reports errors through `std::error_code`.
- **Bash.** GNU `sync` syncs the files that it is given: `sync -- "$path" && sync -- "$dir"`. The `sync` of macOS and BSD takes no file, so there the solution syncs all file systems. Linux says that `sync` "causes all pending modifications to filesystem metadata and cached file data to be written to the underlying filesystems."

In Python, the order of the steps is: write, flush, sync the file, close, sync the directory. The example prints each sync:

```python run id=python-write-synced
import os
import tempfile


def sync_directory(directory):
    fd = os.open(directory, os.O_RDONLY)
    try:
        os.fsync(fd)
    finally:
        os.close(fd)


def write_synced(path, text):
    with open(path, "w", encoding="utf-8", newline="") as f:
        f.write(text)
        f.flush()
        os.fsync(f.fileno())
    sync_directory(os.path.dirname(os.path.abspath(path)))


real_fsync = os.fsync


def show_fsync(fd):
    kind = "directory" if os.path.isdir(f"/dev/fd/{fd}") else "file"
    print("fsync of the", kind)
    real_fsync(fd)


os.fsync = show_fsync
os.chdir(tempfile.mkdtemp())
write_synced("orders.txt", "order 42: paid\n")
with open("orders.txt", encoding="utf-8") as f:
    print("orders.txt holds:", f.read(), end="")
```

```output
fsync of the file
fsync of the directory
orders.txt holds: order 42: paid
```

In Go:

```go run id=go-write-synced
package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func writeSynced(path, text string) error {
	f, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0o644)
	if err != nil {
		return err
	}
	if _, err := f.WriteString(text); err != nil {
		f.Close()
		return err
	}
	if err := f.Sync(); err != nil {
		f.Close()
		return err
	}
	if err := f.Close(); err != nil {
		return err
	}
	d, err := os.Open(filepath.Dir(path))
	if err != nil {
		return err
	}
	defer d.Close()
	return d.Sync()
}

func main() {
	work, err := os.MkdirTemp("", "sync-")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(work)
	path := filepath.Join(work, "orders.txt")
	if err := writeSynced(path, "order 42: paid\n"); err != nil {
		panic(err)
	}
	b, _ := os.ReadFile(path)
	fmt.Printf("orders.txt holds: %s", b)
	fmt.Println("error for a missing directory:", writeSynced(filepath.Join(work, "missing", "x.txt"), "x") != nil)
}
```

```output
orders.txt holds: order 42: paid
error for a missing directory: true
```

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/errors.jsonl`. Each test runs in a new, empty directory, and the file is `out.txt`.

- `new-file`, `replace-longer` and `many-lines`: the file holds the bytes of `text` and no old byte, and it is the only file in the directory.
- `empty-replace` and `empty-new`: an empty `text` gives an empty file. The sync still runs, because the truncation is a change too.
- `no-final-newline`, `non-ascii` and `crlf`: the bytes stay as they are. A solution must not add a newline or change `\r\n`.
- Errors: `missing/out.txt` gives not-found, and no file is made. When `out.txt` is a directory, the solution gives is-a-directory, and the directory stays.

A test cannot cut the power, so the cases check only the content. The native tests also call the directory sync alone, and the Python test checks the order: the file first, then its directory, and an error of `fsync` comes back to the caller.

## Pitfalls

- **A flush is not a sync.** A flush moves the data from the buffer of the language to the kernel. Python `flush` will "Flush the write buffers of the stream if applicable." The data is then in memory, not on the disk. A sync without a flush first also fails: the data is still in the buffer of the language, so `fsync` does not see it.

```python wrong id=python-fsync-without-flush
import os
import tempfile

os.chdir(tempfile.mkdtemp())
f = open("orders.txt", "w", encoding="utf-8")
f.write("order 42: paid\n")
os.fsync(f.fileno())  # no f.flush() first
with open("orders.txt", encoding="utf-8") as g:
    print("the kernel had at the fsync:", repr(g.read()))
f.close()
```

```output
the kernel had at the fsync: ''
```

- **No sync of the directory.** After a crash, a new file that had a sync can be missing, because its name was not on the disk. POSIX says: "An application that modifies a directory, for example, by creating a file in the directory, can invoke fsync () on the directory to ensure that the directory's entries and file attributes are synchronized." A rename also changes a directory, so [[write-a-file-atomically]] syncs the directory after the rename.
- **Do not try the sync again after an error.** When `fsync` fails, the kernel can mark the data clean and drop it. The PostgreSQL wiki says that on Linux "you still only get the error once (so retrying fsync() is not OK)". After the problems of 2018, "PostgreSQL will now PANIC on fsync() failure." Treat a failed sync as lost data: report it, and write the data again from a copy that you trust.
- **macOS and the cache of the drive.** On macOS, `fsync` sends the data to the drive, but the drive can keep the data in its cache for some time, and "it may be written in an out-of-order sequence." For a stronger rule, "Mac OS X provides the F_FULLFSYNC fcntl. The F_FULLFSYNC fcntl asks the drive to flush all buffered data to permanent storage." The Python and C solutions use it. The other languages call only `fsync`.
- **fdatasync and the size of the file.** `fdatasync` (Python `os.fdatasync`, Rust `sync_data`, Java `force(false)`) does not sync all the metadata, so it is faster. Linux says that it "does not flush modified metadata unless that metadata is needed in order to allow a subsequent data retrieval to be correctly handled", and "a change to the file size ( st_size , as made by say ftruncate(2) ), would require a metadata flush". Rust says that `sync_data` "might not synchronize file metadata to the filesystem".
- **The promise is weak.** The POSIX rationale of `fsync` says: "It is explicitly intended that a null implementation is permitted." A file system, a virtual machine or a drive can say that the data is on the disk when it is not. A study of file systems found that application update protocols depend on "subtle behaviors of the underlying file system", and the study used them to find crash bugs in eleven applications.[^7] Test the full protocol of a program on the file system that it uses.
- **A sync is slow.** The call "blocks until the device reports that the transfer has completed". A program that syncs after each small write can be many times slower. Put many changes in one file write, and sync once for each group of changes.
- **An error can come from another descriptor.** Linux says of `EIO`: "This error may relate to data written to some other file descriptor on the same file." So a sync can fail for data that another part of the program wrote.

## Related tasks

- [[write-a-file]]
- [[write-a-file-atomically]]
- [[create-a-temporary-file]]
- [[read-and-write-with-buffers]]
- [[truncate-a-file]]

## References
