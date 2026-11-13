---
tag: XQK5XQ
kind: task
slug: watch-a-file-for-changes
name: Watch a file for changes
aliases: [file watcher, detect file changes, poll a file, inotify, FSEvents, kqueue, ReadDirectoryChangesW, WatchService, FileSystemWatcher, fs.watch, hot reload]
area: data-and-persistence/files-and-file-systems
level: advanced
summary: Tell when a file is created, changed or deleted, by polling its size and modification time or by an event API of the operating system, and act once for each burst of changes.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://man7.org/linux/man-pages/man7/inotify.7.html
    title: Linux manual page, inotify(7)
    locator: DESCRIPTION, NOTES (Limitations and caveats)
  - url: https://man7.org/linux/man-pages/man7/inode.7.html
    title: Linux manual page, inode(7)
    locator: Last modification timestamp, File timestamps
  - url: https://man.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2&format=html
    title: FreeBSD System Calls Manual, kqueue(2)
    locator: EVFILT_VNODE
  - url: https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/FSEvents_ProgGuide/TechnologyOverview/TechnologyOverview.html
    title: Apple, File System Events Programming Guide, Technology Overview
    locator: Technology Overview
  - url: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-readdirectorychangesw
    title: Microsoft Learn, ReadDirectoryChangesW function (winbase.h)
    locator: Return value, Remarks
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), sys/stat.h
    locator: DESCRIPTION, RATIONALE
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/WatchService.html
    title: Java SE 25 API, WatchService
    locator: Platform dependencies, Implementation Note
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filesystemwatcher?view=net-10.0
    title: .NET API reference, FileSystemWatcher Class
    locator: Remarks, Events and buffer sizes
  - url: https://nodejs.org/api/fs.html
    title: Node.js documentation, File system
    locator: fs.watch, Caveats, fs.watchFile
  - url: https://docs.python.org/3/library/os.html
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.stat_result
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Chtimes, type FileInfo
  - url: https://doc.rust-lang.org/std/fs/struct.Metadata.html
    title: The Rust Standard Library, std::fs::Metadata
    locator: modified
---

## Task

Tell when a file changes, so that a program can read it again. A server reads its configuration again after an edit, a build tool builds again after a save, and a log reader shows the new lines. The entry is a watcher of one path. Each poll of the watcher returns `created`, `modified`, `deleted` or `none`, for the time since the last poll.

The solutions poll. A watcher keeps the signature of the file: its size and its modification time (mtime). A poll takes a new signature with `stat` and compares the two. Linux says that the mtime "is changed by file modifications, for example, by mknod(2) , truncate(2) , utime(2) , and write(2) (of more than zero bytes)."[^2] A change to the content changes the mtime, so a poll sees it. A change of the owner or the mode does not: "The mtime timestamp is not changed for changes in owner, group, hard link count, or mode."[^2]

A watch loop calls the poll at an interval. A save often makes many changes in a short time, so the loop waits until the file is quiet for some time, and then calls its function once. A missing file is not an error: the watcher reports `created` when it comes. Report an error when the path is a directory. SQL, regular expressions, HTML and CSS do not apply, because they do not read files.

## Solutions

Each solution has a `FileWatcher` with `poll`, and a `watch` loop that calls a function once for each burst of changes. The operating systems also have event APIs, which tell a program about a change when it occurs:

- **Linux: inotify.** A program adds a watch for a file or a directory, and reads events such as `IN_MODIFY` ("File was modified (e.g., write(2) , truncate(2) )"[^1]) and `IN_CLOSE_WRITE` ("File opened for writing was closed"[^1]). Node.js `fs.watch` uses it on Linux[^9].
- **BSD and macOS: kqueue.** The filter `EVFILT_VNODE` "Takes a file descriptor as the identifier and the events to watch for in fflags"[^3], such as `NOTE_WRITE`, `NOTE_DELETE` and `NOTE_RENAME`[^3]. The program must keep the file open.
- **macOS: FSEvents.** It watches a directory tree. It "coalesces multiple notifications on a given directory if they occur in a short period of time. You will always receive at least one notification after the last change is made."[^4]
- **Windows: ReadDirectoryChangesW.** It watches a directory and fills a buffer with the changes[^5].

The languages:

- **Python.** The standard library has no watcher. `os.stat` gives `st_size` and `st_mtime_ns`, the "Time of most recent content modification expressed in nanoseconds as an integer."[^10] Use `st_mtime_ns`, not the float `st_mtime`: Python says that the float "cannot preserve all of it, and as such will be slightly inexact."[^10]
- **JavaScript and TypeScript (Node.js).** `fs.statSync(path, { bigint: true })` gives `mtimeNs`. Node.js also has `fs.watch`: "On Linux systems, this uses inotify(7) . On BSD systems, this uses kqueue(2) . On macOS, this uses kqueue(2) for files and FSEvents for directories."[^9] And `fs.watchFile` polls: an option tells "how often the target should be polled in milliseconds"[^9], and its default `interval` is 5007[^9].
- **Java.** `Files.readAttributes` gives `size()` and `lastModifiedTime()`. `WatchService` watches a directory. It uses "the native file event notification facility where available, or to use a primitive mechanism, such as polling, when a native facility is not available."[^7]
- **C#.** `FileInfo.Length` and `FileInfo.LastWriteTimeUtc`. `FileSystemWatcher` raises events for a directory[^8].
- **Go.** `os.Stat` "returns a FileInfo describing the named file"[^11], with `Size()` and `ModTime()`. Compare times with `Equal`, not `==`. The standard library has no watcher.
- **Rust.** `fs::metadata(path)?.modified()`. The value "corresponds to the mtime field of stat on Unix platforms and the ftLastWriteTime field on Windows platforms."[^12] It can fail: "This field might not be available on all platforms"[^12]. The standard library has no watcher.
- **C.** `stat` gives `st_size` and `st_mtim`, a `struct timespec` with nanoseconds[^6]. On macOS the field is `st_mtimespec`. For events, use inotify on Linux and kqueue on BSD and macOS.
- **C++.** `std::filesystem::file_size` and `std::filesystem::last_write_time`. The standard library has no watcher.
- **Bash.** `stat -c '%s %Y'` with GNU, and `stat -f '%z %m'` with macOS and BSD. These give the mtime in whole seconds (see Pitfalls).

In Python, a watcher is a few lines. The example writes, appends and deletes a file, and polls after each change:

```python run id=python-poll-watcher
import os
import tempfile


class FileWatcher:
    def __init__(self, path):
        self.path = path
        self.last = self.signature()

    def signature(self):
        try:
            st = os.stat(self.path)
        except FileNotFoundError:
            return None
        return (st.st_size, st.st_mtime_ns)

    def poll(self):
        now = self.signature()
        before, self.last = self.last, now
        if before is None and now is None:
            return "none"
        if before is None:
            return "created"
        if now is None:
            return "deleted"
        return "modified" if now != before else "none"


os.chdir(tempfile.mkdtemp())
watcher = FileWatcher("app.conf")
print("start:", watcher.poll())
with open("app.conf", "w") as f:
    f.write("port = 8080\n")
print("after the write:", watcher.poll())
print("no change:", watcher.poll())
with open("app.conf", "a") as f:
    f.write("debug = true\n")
print("after the append:", watcher.poll())
os.remove("app.conf")
print("after the delete:", watcher.poll())
```

```output
start: none
after the write: created
no change: none
after the append: modified
after the delete: deleted
```

In Go, a watcher of the path sees a save that renames a new file onto the old name, also when the size stays the same:

```go run id=go-poll-watcher
package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"time"
)

type signature struct {
	size  int64
	mtime time.Time
}

// stat gives the signature of the file, or ok false when it does not exist.
func stat(path string) (sig signature, ok bool) {
	info, err := os.Stat(path)
	if errors.Is(err, fs.ErrNotExist) {
		return signature{}, false
	}
	if err != nil {
		panic(err)
	}
	return signature{info.Size(), info.ModTime()}, true
}

func main() {
	work, err := os.MkdirTemp("", "watch-")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(work)
	path := filepath.Join(work, "app.conf")

	os.WriteFile(path, []byte("port = 8080\n"), 0o644)
	old := time.Now().Add(-time.Hour)
	os.Chtimes(path, old, old)
	before, _ := stat(path)

	// An editor saves: it writes a new file, then renames it onto the old name.
	os.WriteFile(path+".tmp", []byte("port = 9090\n"), 0o644)
	os.Rename(path+".tmp", path)

	after, ok := stat(path)
	fmt.Println("the file exists:", ok)
	fmt.Println("same size:", after.size == before.size)
	fmt.Println("modified:", after.size != before.size || !after.mtime.Equal(before.mtime))
}
```

```output
the file exists: true
same size: true
modified: true
```

The watch loop of the solutions keeps the time of the last change that it did not report. When a poll sees no change and that time is older than the quiet time, the loop calls its function and clears the time. Five writes 20 ms apart, with a quiet time of 500 ms, give one call.

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/errors.jsonl`. Each test runs in a new, empty directory, and the file is `watched.txt`. After each step except `none`, the entry sets the mtime of the file to a new second. So the cases do not depend on the clock of the file system.

- `create`, `create-then-write`, `create-then-delete` and `append`: the usual changes.
- `no-change`: a poll with no change in between gives `none`, also two times.
- `same-size`: `abc` becomes `xyz`. The size is the same, but the mtime changes, so the poll gives `modified`.
- `touch-only`: only the mtime changes. The poll gives `modified`, because the watcher cannot see the content.
- `replace-by-rename`: a new file goes onto the name, as [[write-a-file-atomically]] does. A watcher of the path sees a modified file, not a delete and a create.
- `delete-and-create`: the file goes away and comes back between two polls. The poll gives `modified`: a poll sees only the state at its time, not the steps in between.
- `never-created`: no file, no change.
- Errors: when `watched.txt` is a directory at a poll, the solution gives is-a-directory.

The native tests also check that a change that keeps the size and the mtime is not seen, and that the watch loop calls its function once for a burst of writes.

## Pitfalls

- **The clock resolution of the mtime.** Two saves in the same tick of the clock give the same mtime. Python says: "on Windows systems using the FAT32 file systems, st_mtime has 2-second resolution"[^10]. Linux says: "Nanosecond timestamps are not supported in ext2, ext3, and Reiserfs."[^2] POSIX lets an implementation keep only the resolution of the file system: "if the file system resolution is 1 microsecond, then a conforming stat () must always return an st_mtim.tv_nsec that is a multiple of 1000."[^6] A watcher that keeps the mtime in whole seconds does not see a second save of the same size in the same second:

```python wrong id=python-mtime-in-seconds
import os
import tempfile

os.chdir(tempfile.mkdtemp())


def signature(path):
    st = os.stat(path)
    return (st.st_size, int(st.st_mtime))  # the mtime in whole seconds


second = 1_700_000_000 * 1_000_000_000
with open("app.conf", "w") as f:
    f.write("port = 8080\n")
os.utime("app.conf", ns=(second + 100_000_000, second + 100_000_000))
before = signature("app.conf")
# A second save, 0.5 s later, with a value of the same length.
with open("app.conf", "w") as f:
    f.write("port = 9090\n")
os.utime("app.conf", ns=(second + 600_000_000, second + 600_000_000))
print("changed:", signature("app.conf") != before)
print("with st_mtime_ns it is seen:", os.stat("app.conf").st_mtime_ns != second + 100_000_000)
```

```output
changed: False
with st_mtime_ns it is seen: True
```

  Keep the mtime with all its digits, and also the size. When a change must not be lost, also compare a hash of the content, or read the file again after each event.

- **A change is not the end of a save.** A watcher can see a file in the middle of a write. Java says: "When an event is reported to indicate that a file in a watched directory has been modified then there is no guarantee that the program (or programs) that have modified the file have completed."[^7] Wait until the file is quiet, and read it then. A writer that renames a full file onto the name (see [[write-a-file-atomically]]) gives the reader no half file.
- **One save gives many events.** .NET says: "Common file system operations might raise more than one event. For example, when a file is moved from one directory to another, several OnChanged and some OnCreated and OnDeleted events might be raised."[^8] Java says that a change "may result in a single ENTRY_MODIFY event in some implementations but several events in other implementations."[^7] inotify joins identical events that are not yet read, so "an application can't use inotify to reliably count file events."[^1] Do not count events. Treat an event as a sign to look at the file again.
- **A watch of a file follows the inode, not the name.** "Inotify monitoring is inode-based"[^1]. Node.js says that `fs.watch` "resolves the path to an inode and watches the inode. If the watched path is deleted and recreated, it is assigned a new inode. The watch will emit an event for the delete but will continue watching the original inode. Events for the new inode will not be emitted."[^9] A program that saves with a rename makes a new inode, so a watch of the file stops after the first save. Watch the directory that holds the file, and filter by name, or add the watch again after a delete or a rename.
- **Event queues can overflow.** In inotify, "Events in excess of this limit are dropped, but an IN_Q_OVERFLOW event is always generated."[^1] "The JDK's WatchService implementations buffer up to 512 pending events for each registered watchable object."[^7] On Windows, `ReadDirectoryChangesW` "fails with ERROR_NOTIFY_ENUM_DIR when the system was unable to record all the changes to the directory. In this case, you should compute the changes by enumerating the directory or subtree."[^5] After an overflow, scan the directory again, as a poller does.
- **Network file systems and polls that are too slow.** inotify does "not catch remote events that occur on network filesystems. (Applications must fall back to polling the filesystem to catch such events.)"[^1] Java says that "it is not required that changes to files carried out on remote systems be detected."[^7] A poller works there, but it misses a short life: "Short-lived files (meaning files that are deleted very quickly after they are created) may not be detected by primitive implementations that periodically poll the file system to detect changes."[^7] The `delete-and-create` case shows the same limit.
- **A watch of a directory is not recursive.** In inotify, "to monitor subdirectories under a directory, additional watches must be created. This can take a significant amount time for large directory trees."[^1] Each watch also counts against a limit for each user (`/proc/sys/fs/inotify/max_user_watches`)[^1].
- **The name in an event can be old.** inotify says that "by the time an application processes an inotify event, the filename may already have been deleted or renamed."[^1] Node.js says that the file name argument "is not always guaranteed to be provided"[^9]. Look at the file again after an event, and accept that it is gone.

## Related tasks

- [[get-the-modification-time-of-a-file]]
- [[get-the-size-of-a-file]]
- [[file-metadata]]
- [[write-a-file-atomically]]
- [[read-a-file-line-by-line]]

## References
