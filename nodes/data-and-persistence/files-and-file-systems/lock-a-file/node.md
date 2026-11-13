---
tag: ZKJVPE
kind: task
slug: lock-a-file
name: Lock a file
aliases: [file lock, flock, lockf, advisory lock, exclusive lock, shared lock, FileChannel.lock, try_lock]
area: data-and-persistence/files-and-file-systems
level: advanced
summary: Take a shared or an exclusive advisory lock on a whole file without a wait, so that programs which use the same lock do not change the file at the same time, and release it when the work is done.
contract: contract.yaml
category: file-output
languages: [python, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://man7.org/linux/man-pages/man2/flock.2.html
    title: Linux manual page, flock(2)
    locator: DESCRIPTION, NFS details, NOTES
  - url: https://man7.org/linux/man-pages/man2/fcntl_locking.2.html
    title: Linux manual page, fcntl_locking(2)
    locator: Advisory record locking, Open file description locks
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fcntl.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), fcntl
    locator: DESCRIPTION (F_SETLK)
  - url: https://man7.org/linux/man-pages/man1/flock.1.html
    title: Linux manual page, flock(1)
    locator: DESCRIPTION, OPTIONS
  - url: https://www.sqlite.org/howtocorrupt.html
    title: SQLite, How To Corrupt An SQLite Database File
    locator: 2.2 Posix advisory locks canceled by a separate thread doing close()
  - url: https://docs.python.org/3/library/fcntl.html
    title: The Python Standard Library, fcntl, The fcntl and ioctl system calls
    locator: fcntl.flock, fcntl.lockf
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html
    title: Java SE 25 API, FileChannel
    locator: tryLock(long, long, boolean)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileLock.html
    title: Java SE 25 API, FileLock
    locator: Description, Platform dependencies
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.fileshare?view=net-10.0
    title: .NET API reference, FileShare Enum
    locator: Fields, Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/core/whats-new/dotnet-11/runtime
    title: What's new in the .NET 11 runtime
    locator: Advisory file locking on iOS and tvOS
  - url: https://learn.microsoft.com/en-us/dotnet/core/compatibility/core-libraries/6.0/filestream-file-locks-unix
    title: .NET 6 breaking change, FileStream locks files with shared lock on Unix
    locator: New behavior
  - url: https://pkg.go.dev/syscall
    title: Go standard library, package syscall
    locator: func Flock
  - url: https://doc.rust-lang.org/std/fs/struct.File.html
    title: The Rust Standard Library, std::fs::File
    locator: try_lock, try_lock_shared, Platform-specific behavior
---

## Task

Take a lock on a whole file, so that two programs do not change it at the same time. The task has two functions:

- `try_lock_file(path, mode)` opens the file at `path`, which must exist, and asks for a lock without a wait. `mode` is `shared` or `exclusive`. It returns a lock object, or nothing when another open file has a lock that conflicts. A release of the lock object, or a close of its file, releases the lock.
- `try_lock(path, held, want)` is the entry of the cases. It takes the lock `held` (`none`, `shared` or `exclusive`) with one open of the file, then calls `try_lock_file(path, want)` with a second open. It returns true when the second call got the lock.

The rules of the two modes are the rules of `flock`. Linux says of a shared lock: "More than one process may hold a shared lock for a given file at a given time." Of an exclusive lock: "Only one process may hold an exclusive lock for a given file at a given time." And: "A single file may not simultaneously have both shared and exclusive locks." So a reader takes a shared lock, and a writer takes an exclusive lock.

The locks are advisory. They stop only the programs that ask for the same lock. Linux says: "flock () places advisory locks only; given suitable permissions on a file, a process is free to ignore the use of flock () and perform I/O on the file."

Report an error when the file does not exist, and do not make it. JavaScript and TypeScript are not in the solutions, because Node.js has no call that locks a file (see Pitfalls). SQL, regular expressions, HTML and CSS do not apply, because they do not open files.

## Solutions

- **Python.** `fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)`, or `LOCK_SH` for a shared lock. A lock that is busy raises `BlockingIOError`. Python says: "If the flock() call fails, an OSError exception is raised." The solution opens the file again for each lock, with `os.open(path, os.O_RDWR)`, because a flock lock belongs to the open file.
- **Java.** `FileChannel.tryLock(0, Long.MAX_VALUE, shared)`. Java says: "If it fails to acquire a lock because an overlapping lock is held by another program then it returns null ." But "File locks are held on behalf of the entire Java virtual machine. They are not suitable for controlling access to a file by multiple threads within the same virtual machine." A second lock in the same JVM throws `OverlappingFileLockException`, and the solution gives null for it too. So the Java tests take the first lock in a child JVM.
- **C#.** .NET has no call that locks a whole file. The `FileShare` argument of `FileStream` does it: `FileShare.None` "Declines sharing of the current file. Any request to open the file (by this process or another process) will fail until the file is closed." On Unix, .NET enforces `FileShare` with advisory locks: the .NET 11 notes say that on iOS and tvOS "FileShare restrictions are no longer enforced by those locks", and that "Mac Catalyst and other platforms keep their existing defaults". The solution opens with `FileShare.None` for an exclusive lock, and with read access and `FileShare.ReadWrite` for a shared lock. A lock that is busy gives an `IOException`.
- **Go.** The `os` package has no lock call. The solution calls `syscall.Flock(int(f.Fd()), syscall.LOCK_EX|syscall.LOCK_NB)`, and a lock that is busy gives `syscall.EWOULDBLOCK`. The file has the build constraint `unix`.
- **Rust.** `File::try_lock` and `File::try_lock_shared`, in the standard library. `try_lock` "Returns Err(TryLockError::WouldBlock) if a different lock is already held on this file (via another handle/descriptor)." On Unix, it "currently corresponds to the flock function on Unix with the LOCK_EX flag". A drop of the `File` releases the lock.
- **C.** `open(path, O_RDWR)`, then `flock(fd, LOCK_EX | LOCK_NB)`. `flock` is not in POSIX, but Linux, macOS and the BSDs have it. A lock that is busy gives -1 with `errno` set to `EWOULDBLOCK`.
- **C++.** The C++ library has no file lock. The solution calls `open` and `flock` as in C, and puts the descriptor in a class whose destructor releases the lock, so an exception does not leave the lock in place.
- **Bash.** The `flock` command of util-linux "manages flock(2) locks from within shell scripts or from the command line". The solution opens the file on a new descriptor with `exec {fd}<"$path"`, then runs `flock --nonblock --exclusive "$fd"`. Linux has the command, and macOS does not have it by default, so the test skips when it is not installed. A read-only descriptor is sufficient: "A shared or exclusive lock can be placed on a file regardless of the mode in which the file was opened."

In Python, a second open of the file gets no lock while the first open holds it, and gets it after the release:

```python run id=python-flock-two-opens
import fcntl
import os
import tempfile

os.chdir(tempfile.mkdtemp())
with open("data.txt", "w", encoding="utf-8") as f:
    f.write("alpha\n")


def try_lock_file(path, op):
    fd = os.open(path, os.O_RDWR)
    try:
        fcntl.flock(fd, op | fcntl.LOCK_NB)
    except BlockingIOError:
        os.close(fd)
        return None
    return fd


first = try_lock_file("data.txt", fcntl.LOCK_EX)
print("first open, exclusive:", first is not None)
print("second open, shared:", try_lock_file("data.txt", fcntl.LOCK_SH) is not None)
os.close(first)  # the close releases the lock
second = try_lock_file("data.txt", fcntl.LOCK_SH)
print("after the release, shared:", second is not None)
os.close(second)
```

```output
first open, exclusive: True
second open, shared: False
after the release, shared: True
```

In Go, with `syscall.Flock`:

```go run id=go-flock
package main

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"syscall"
)

func tryLock(path string, how int) (*os.File, error) {
	f, err := os.OpenFile(path, os.O_RDWR, 0)
	if err != nil {
		return nil, err
	}
	if err := syscall.Flock(int(f.Fd()), how|syscall.LOCK_NB); err != nil {
		f.Close()
		if errors.Is(err, syscall.EWOULDBLOCK) {
			return nil, nil
		}
		return nil, err
	}
	return f, nil
}

func main() {
	dir, err := os.MkdirTemp("", "lock-")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "data.txt")
	if err := os.WriteFile(path, []byte("alpha\n"), 0o644); err != nil {
		panic(err)
	}
	a, _ := tryLock(path, syscall.LOCK_SH)
	b, _ := tryLock(path, syscall.LOCK_SH)
	c, _ := tryLock(path, syscall.LOCK_EX)
	fmt.Println("two shared locks:", a != nil && b != nil)
	fmt.Println("exclusive lock while they are held:", c != nil)
	a.Close()
	b.Close()
	_, err = tryLock(filepath.Join(dir, "missing.txt"), syscall.LOCK_EX)
	fmt.Println("missing file is an error:", errors.Is(err, os.ErrNotExist))
}
```

```output
two shared locks: true
exclusive lock while they are held: false
missing file is an error: true
```

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/errors.jsonl`. Each test runs in a new directory that holds the file `data.txt` with the text `alpha`.

- `no-lock-exclusive` and `no-lock-shared`: with no other lock, each mode is given.
- `exclusive-with-exclusive`: the second exclusive lock is not given, and the call does not wait.
- `shared-with-shared`: two shared locks are given together.
- `shared-with-exclusive` and `exclusive-with-shared`: a shared lock and an exclusive lock are not given together.
- Errors: `missing.txt` gives not-found, and the call does not make the file.

In each case, the content of the file stays `alpha`. The native tests also check that a release or a close frees the lock, so that a new call gets it, and the Python, C and C++ tests check that a child process sees the lock too.

## Pitfalls

- **fcntl and lockf locks belong to the process.** POSIX record locks (`fcntl` with `F_SETLK`, and `lockf`) are not the same as `flock`. Linux says: "If a process closes any file descriptor referring to a file, then all of the process's locks on that file are released, regardless of the file descriptor(s) on which the locks were obtained." SQLite, which uses these locks, says that "the close() system call will cancel all POSIX advisory locks on the same file for all threads and all file descriptors in the process." In this example, a read of the file by another part of the program drops the lock:

```python wrong id=python-lockf-lost-on-close
import fcntl
import os
import tempfile

os.chdir(tempfile.mkdtemp())
with open("data.txt", "w", encoding="utf-8") as f:
    f.write("alpha\n")

fd = os.open("data.txt", os.O_RDWR)
fcntl.lockf(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
with open("data.txt", encoding="utf-8") as other:  # a library reads the file
    other.read()

pid = os.fork()
if pid == 0:
    try:
        fcntl.lockf(os.open("data.txt", os.O_RDWR), fcntl.LOCK_EX | fcntl.LOCK_NB)
        print("another process got the lock", flush=True)
    except OSError:
        print("another process must wait", flush=True)
    os._exit(0)
os.waitpid(pid, 0)
```

```output
another process got the lock
```

  Also, "The threads in a process share locks." Linux has open file description locks (`F_OFD_SETLK`), which belong to the open file as `flock` locks do, but they are Linux-specific.
- **Java locks do not stop the threads of the same JVM.** Use a lock in memory for the threads of the program, and the file lock for other programs.
- **An advisory lock does not stop a program that does not ask for it.** A text editor or a `cp` can still change the file. Use the lock only among programs that you control, and give each the same rule.
- **Node.js has no file lock.** The usual way is a lock file: make `data.txt.lock` with the flag `"wx"`, which fails when the file exists, and remove it after the work. A program that stops before the remove leaves the lock file, so the lock file must hold the process ID or a time, and the next program must check it. See [[create-a-temporary-file]] for the `"wx"` flag.
- **NFS and other network file systems.** Linux says that newer NFS clients "support flock () locks by emulating them as fcntl(2) byte-range locks on the entire file. This means that fcntl(2) and flock () locks do interact with one another over NFS. It also means that in order to place an exclusive lock, the file must be opened for writing." The solutions open the file for reading and writing for this reason, except for the shared locks of C# and Bash.
- **A change of mode is not atomic.** A program that holds a shared lock and asks for an exclusive lock does not keep the shared lock between the two. Linux says: "the existing lock is first removed, and then a new lock is established." Release the lock, take the exclusive lock, and read the file again.
- **Lock the file, not the name.** When a writer replaces the file with a rename, as in [[write-a-file-atomically]], the lock stays on the old file, and a new reader opens the new file with no lock. Lock a separate file that never changes, for example `data.txt.lock`, and keep it.

## Related tasks

- [[write-a-file-atomically]]
- [[sync-a-file-to-disk]]
- [[create-a-temporary-file]]
- [[append-to-a-file]]
- [[file-permissions]]

## References
