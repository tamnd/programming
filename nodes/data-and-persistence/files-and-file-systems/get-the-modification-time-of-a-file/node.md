---
tag: Z8YPVC
kind: task
slug: get-the-modification-time-of-a-file
name: Get the modification time of a file
aliases: [file modification time, last modified time, mtime, last write time, file timestamp]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Get the time of the last change to the data of a file as Unix time in whole seconds, and know how it differs from the change, access and creation times.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/os.html#os.stat_result
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: stat_result.st_mtime, st_mtime_ns, st_ctime, st_birthtime
  - url: https://nodejs.org/api/fs.html#class-fsstats
    title: Node.js documentation, File system
    locator: stats.mtimeNs, stats.birthtime, fsPromises.utimes(path, atime, mtime)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/BasicFileAttributes.html
    title: Java SE 25 API, BasicFileAttributes
    locator: lastModifiedTime(), creationTime()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/FileTime.html
    title: Java SE 25 API, FileTime
    locator: toInstant()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.getlastwritetimeutc?view=net-10.0
    title: .NET API reference, File.GetLastWriteTimeUtc Method
    locator: Remarks
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Stat, func Chtimes
  - url: https://doc.rust-lang.org/std/fs/struct.Metadata.html
    title: The Rust Standard Library, std::fs::Metadata
    locator: modified, created
  - url: https://doc.rust-lang.org/std/time/struct.SystemTime.html
    title: The Rust Standard Library, std::time::SystemTime
    locator: duration_since
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), sys/stat.h
    locator: DESCRIPTION (st_atim, st_mtim, st_ctim)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/futimens.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), futimens, utimensat
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/touch.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), touch
    locator: DESCRIPTION, OPTIONS (-d date_time), EXAMPLES
  - url: https://man7.org/linux/man-pages/man7/inode.7.html
    title: Linux manual page, inode(7)
    locator: File timestamps
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: -r, --reference
  - url: https://en.cppreference.com/w/cpp/filesystem/last_write_time
    title: cppreference, std::filesystem::last_write_time
    locator: Description
  - url: https://en.cppreference.com/w/cpp/chrono/file_clock
    title: cppreference, std::chrono::file_clock
    locator: Description
  - url: https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
    title: Windows API reference, FILETIME structure (minwinbase.h)
    locator: Description
---

## Task

Return the modification time of the file at `path`: the time of the last change to its data. Give it as Unix time, the count of whole seconds since 1970-01-01T00:00:00Z. A time with a part of a second gives the second before it (the floor), also for a time before 1970. So -1.5 seconds gives -2, not -1. Report an error when no file is at `path`. A directory has a modification time too, so a directory is not an error.

The Linux manual says what changes this time: "It is changed by file modifications, for example, by mknod(2) , truncate(2) , utime(2) , and write(2) (of more than zero bytes). Moreover, the mtime timestamp of a directory is changed by the creation or deletion of files in that directory. The mtime timestamp is not changed for changes in owner, group, hard link count, or mode."

A file has more times than one. POSIX gives three in `struct stat`: "struct timespec st_mtim Last data modification timestamp. struct timespec st_ctim Last file status change timestamp." The third, `st_atim`, is the time of the last access. Some systems also keep a creation time. The pitfalls show why each is not the modification time.

SQL, regular expressions, HTML and CSS do not apply, because they do not get information about files.

## Solutions

Each language gets the time from the status of the file, and each call follows a symbolic link, so a link gives the time of its target. The time is kept in a more exact unit than a second, so each solution must convert it to seconds with a floor.

- **Python.** `os.stat(path).st_mtime_ns // 1_000_000_000`. `st_mtime_ns` is the "Time of most recent content modification expressed in nanoseconds as an integer." The `//` operator gives the floor, also for a negative count. `st_mtime` is a `float`, and a `float` does not keep all the nanoseconds of a time in this century. See the pitfalls.
- **JavaScript and TypeScript (Node.js).** `(await fsPromises.stat(path, { bigint: true })).mtimeNs`. `mtimeNs` is "Only present when bigint: true is passed into the method that generates the object." The division of two `bigint` values rounds toward zero, so the code subtracts 1 when the remainder is negative.
- **Java.** `Files.getLastModifiedTime(path).toInstant().getEpochSecond()`. `toInstant` "creates an Instant that represents the same point on the time-line as this FileTime", and `getEpochSecond` gives the floor.
- **C#.** `File.GetLastWriteTimeUtc(path)`, and then `new DateTimeOffset(time).ToUnixTimeSeconds()`. This call gives no error for a missing file: "If the file described in the path parameter does not exist, this method returns 12:00 midnight, January 1, 1601 A.D. (C.E.) Coordinated Universal Time (UTC)." So the code throws `FileNotFoundException` first. For a symbolic link, the call gives the time of the link itself. The native test shows this on macOS. So the code gets the final target with `ResolveLinkTarget(returnFinalTarget: true)` first, and reads `LastWriteTimeUtc` from it.
- **Go.** `os.Stat(path)`, and then `fi.ModTime().Unix()`. `Unix` gives the floor. The result is an `int64`.
- **Rust.** `fs::metadata(path)?.modified()?`. It "Returns the last modification time listed in this metadata. The returned value corresponds to the mtime field of stat on Unix platforms and the ftLastWriteTime field on Windows platforms." A `SystemTime` has no count of seconds. `duration_since(UNIX_EPOCH)` gives one, but it gives an error for a time before 1970. The error has the distance in the other direction, so the code makes it negative and takes the floor.
- **C.** POSIX `stat(path, &st)`, and then `st.st_mtime`, which is the seconds part of `st_mtim`. The nanoseconds part is never negative, so the seconds part is the floor. ISO C has no call that gets the time of a file.
- **C++.** `std::filesystem::last_write_time(path, ec)`. It "Returns the time of the last modification of p , determined as if by accessing the member st_mtime of the POSIX stat (symlinks are followed)." The result is a `file_clock` time, and for `file_clock`, "Its epoch is unspecified." So the code converts it with `file_clock::to_sys`, and then uses `std::chrono::floor<seconds>`. A `duration_cast` rounds toward zero.
- **Bash.** POSIX has no utility that prints the time of a file in seconds. `date -r "$path" +%s` works in GNU date ("-r, --reference=FILE display the last modification time of FILE") and in the date of macOS, but it is not POSIX. The date of macOS reads a number as a count of seconds when no file has that name, so the function returns status 1 for a missing file first. The native tests check this with a missing file named `1709296496`.

On Windows, a file time is a `FILETIME`, which has "a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC)." That is the reason for the year 1601 in C#.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each test makes the file in a new temporary directory, sets its modification time to `mtime_ns`, and checks the result. The tests set the time with the call of each language: `os.utime`, `utimes`, `Files.setLastModifiedTime`, `File.SetLastWriteTimeUtc`, `os.Chtimes`, `File::set_modified`, `utimensat`, `last_write_time`, and `touch -d`.

- `whole-second` and `sub-second`: 2024-03-01T12:34:56Z, and the same time and a half second. Both give 1709296496.
- `epoch`: 0.
- `before-1970`: one day before 1970 gives -86400. `before-1970-sub-second`: -1.5 seconds gives -2. A conversion that rounds toward zero gives -1.
- `after-2038`: 2100-01-01T00:00:00Z gives 4102444800. That is more than a signed 32-bit integer can hold.
- `directory`: a directory gives its time.
- `name-with-spaces`: `my notes.txt`. A shell solution must quote the name.
- Errors, in `cases/errors.jsonl`: a missing file gives not-found.

The native tests also check that a symbolic link gives the time of its target, and that a write to a file sets its time to the present time.

## Pitfalls

- **The change time is not the creation time.** `ctime` is the time of the last change to the status of the file, and a change to the mode is a change of status. Python says that `st_ctime` is the "Time of most recent metadata change expressed in seconds", and the Linux manual says that it "is changed by writing or by setting inode information (i.e., owner, group, link count, mode, etc.)." A `chmod` changes it, but it does not change the modification time:

```python wrong id=python-ctime-is-not-creation
import os
import tempfile
import time

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "a.txt")
    with open(path, "w") as f:
        f.write("alpha\n")
    made = os.stat(path).st_ctime_ns
    time.sleep(0.05)
    os.chmod(path, 0o600)  # no change to the data
    st = os.stat(path)
    print("ctime changed:", st.st_ctime_ns > made)
    print("mtime changed:", st.st_mtime_ns > made)
```

```output
ctime changed: True
mtime changed: False
```

On Windows, `st_ctime` is deprecated, and the documentation says: "Use st_birthtime for the file creation time." The creation time is not always present. `st_birthtime` "is not always available, and may raise AttributeError". On Linux, it comes from `statx`, and the Linux manual says that it "is not currently supported by most Linux filesystems." Rust gives it as `Metadata::created`, Node as `stats.birthtime`, and Java as `creationTime()`.

- **The access time is not exact.** The access time changes on a read, but a system can stop the update to save writes: "Some filesystem types allow mounting in such a way that file and/or directory accesses do not cause an update of the atime timestamp." Do not use it to find the last time a program read a file.

- **A float loses nanoseconds.** A floating-point number does not keep all the digits of a time in this century in nanoseconds. So `st_mtime` in Python does not keep the exact time:

```python wrong id=python-mtime-float
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "a.txt")
    open(path, "w").close()
    ns = 1709296496_123456789
    os.utime(path, ns=(ns, ns))
    st = os.stat(path)
    print("st_mtime_ns:   ", st.st_mtime_ns)
    print("from st_mtime: ", int(st.st_mtime * 1_000_000_000))
```

```output
st_mtime_ns:    1709296496123456789
from st_mtime:  1709296496123456768
```

To compare two times exactly, for example to see if a file changed since a build, use the integer forms: `st_mtime_ns`, `mtimeNs`, or the `timespec` of `stat`.

- **The resolution of the file system.** The time that a program sets is not always the time that it gets back. Go says: "The underlying filesystem may truncate or round the values to a less precise time unit." The Linux manual says: "Nanosecond timestamps are not supported in ext2, ext3, and Reiserfs." So two writes in the same second can give the same time, and a check that one file is newer than another must allow equal times.

- **Local time and UTC.** Unix time has no time zone. A conversion to a date and an hour needs one, and the default is the local time zone of the computer:

```python run id=python-mtime-local-and-utc
import os
import time
from datetime import datetime, timezone

os.environ["TZ"] = "Asia/Ho_Chi_Minh"
time.tzset()
mtime = 1709296496
print("local:", datetime.fromtimestamp(mtime))
print("UTC:  ", datetime.fromtimestamp(mtime, tz=timezone.utc))
```

```output
local: 2024-03-01 19:34:56
UTC:   2024-03-01 12:34:56+00:00
```

Keep and compare times as Unix time, or as dates with a time zone. The same is true for `touch -d`: the time zone "is either empty, signifying local time, or the letter 'Z' , signifying UTC."

- **A time before 1970 that Node does not set.** `utimes` in Node takes numbers of seconds, but it uses the present time for a negative number. The documentation does not say this. A `Date` works:

```javascript wrong id=javascript-utimes-negative
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

const dir = fs.mkdtempSync(path.join(os.tmpdir(), "mtime-"));
const file = path.join(dir, "a.txt");
fs.writeFileSync(file, "alpha\n");
fs.utimesSync(file, -86400, -86400); // 1969-12-31 as a number of seconds
console.log("year:", fs.statSync(file).mtime.getUTCFullYear() === 1969 ? 1969 : "not 1969");
fs.utimesSync(file, new Date(-86400000), new Date(-86400000));
console.log("year:", fs.statSync(file).mtime.getUTCFullYear());
fs.rmSync(dir, { recursive: true });
```

```output
year: not 1969
year: 1969
```

For `touch`, POSIX says: "If the resulting time precedes the Epoch, the behavior is implementation-defined."

- **A missing file with a time.** In C#, `File.GetLastWriteTimeUtc` gives 1601-01-01 for a missing file, with no error. A program that sorts files by time puts the missing file first. Check that the file exists, or use `FileInfo` and its `Exists` property.

## Related tasks

- [[get-the-size-of-a-file]]
- [[file-metadata]]
- [[check-whether-a-file-exists]]
- [[watch-a-file-for-changes]]
- [[hard-links-and-symbolic-links]]

## References
