---
tag: X4W26P
kind: concept
slug: file-metadata
name: File metadata
aliases: [file attributes, stat, lstat, inode attributes, extended attributes, xattr, file status]
area: data-and-persistence/files-and-file-systems
level: core
summary: >-
  File metadata is the data about a file that is not in its bytes: the type, the size, the owner, the permissions, the link count and the times. A stat call reads it, with or without a link.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1974
  people: [D. M. Ritchie, K. Thompson]
  note: The Unix paper (1974) lists the fields of the i-node, which are still the core of the POSIX stat structure. Linux added statx in Linux 4.11.
see_also: [file-system-concept, get-the-size-of-a-file, get-the-modification-time-of-a-file]
same_as:
  wikidata: Q1144207
  wikipedia: File_attribute
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
  title: The Open Group, POSIX.1-2024, sys/stat.h
  locator: Description, the stat structure
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fstatat.html
  title: The Open Group, POSIX.1-2024, fstatat, lstat, stat
  locator: Description
- url: https://man7.org/linux/man-pages/man7/inode.7.html
  title: inode(7), Linux manual page
  locator: The timestamp fields
- url: https://man7.org/linux/man-pages/man2/statx.2.html
  title: statx(2), Linux manual page
  locator: Description; History
- url: https://man7.org/linux/man-pages/man7/xattr.7.html
  title: xattr(7), Linux manual page
  locator: Description; Filesystem differences; Standards
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/cacm.html
  title: D. M. Ritchie and K. Thompson, The UNIX Time-Sharing System (1974, the BSTJ version)
  locator: IV. Implementation of the file system
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: os.stat_result, os.scandir, os.access, Linux extended attributes
- url: https://nodejs.org/api/fs.html#class-fsstats
  title: Node.js, File system, Class fs.Stats
  locator: fs.Stats, Stat time values, fsPromises.lstat
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/BasicFileAttributes.html
  title: Java SE 25 API, java.nio.file.attribute.BasicFileAttributes
  locator: creationTime
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/PosixFileAttributes.html
  title: Java SE 25 API, java.nio.file.attribute.PosixFileAttributes
  locator: interface description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/UserDefinedFileAttributeView.html
  title: Java SE 25 API, java.nio.file.attribute.UserDefinedFileAttributeView
  locator: interface description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
  title: Java SE 25 API, java.nio.file.Files
  locator: readAttributes
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filesysteminfo
  title: FileSystemInfo Class (System.IO), .NET API, Microsoft Learn
  locator: Properties
- url: https://pkg.go.dev/io/fs
  title: Go standard library, package io/fs
  locator: type FileInfo
- url: https://pkg.go.dev/os
  title: Go standard library, package os
  locator: func Lstat
- url: https://doc.rust-lang.org/std/fs/struct.Metadata.html
  title: The Rust Standard Library, std::fs::Metadata
  locator: created
- url: https://doc.rust-lang.org/std/os/unix/fs/trait.MetadataExt.html
  title: The Rust Standard Library, std::os::unix::fs::MetadataExt
  locator: trait definition
- url: https://en.cppreference.com/w/cpp/filesystem/status.html
  title: std::filesystem::status, std::filesystem::symlink_status, cppreference.com
  locator: description
- url: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/ns-fileapi-by_handle_file_information
  title: BY_HANDLE_FILE_INFORMATION structure (fileapi.h), Win32 apps, Microsoft Learn
  locator: Members
---

## Idea

A file has its bytes, and it also has data about the bytes. The file system knows the type of the file, its size, its owner, its permissions, the number of names that it has, and the times of its last changes. This data is the metadata of the file. A program does not read it from the file. It asks the file system with one call, which POSIX names `stat`.

```python run id=idea
import os
import stat
from pathlib import Path

os.mkdir("box")
os.chdir("box")
Path("data.txt").write_text("hello", encoding="utf-8")
os.chmod("data.txt", 0o640)
os.utime("data.txt", (1700000000, 1700000000))
os.link("data.txt", "second-name.txt")
os.symlink("data.txt", "link")

st = os.stat("data.txt")
print("type:", "regular file" if stat.S_ISREG(st.st_mode) else "other")
print("size:", st.st_size)
print("permissions:", stat.filemode(st.st_mode)[1:])
print("links:", st.st_nlink)
print("owner is me:", st.st_uid == os.getuid())
print("mtime:", int(st.st_mtime))
print("ctime is later than mtime:", st.st_ctime > st.st_mtime)

# The link has metadata of its own. lstat reads it, stat follows the link.
lst = os.lstat("link")
print("link:", "symbolic link" if stat.S_ISLNK(lst.st_mode) else "other",
      "of size", lst.st_size)
print("stat of the link is the file:", os.stat("link") == st)
```

```output for=idea
type: regular file
size: 5
permissions: rw-r-----
links: 2
owner is me: True
mtime: 1700000000
ctime is later than mtime: True
link: symbolic link of size 8
stat of the link is the file: True
```

The program makes a file of 5 bytes, sets its permissions and its modification time, and gives it a second name and a symbolic link. One `os.stat` call returns all of the fields. The modification time is the value that the program set, but the change time is later, because the calls to `chmod`, `utime` and `link` changed the metadata. The link is a file of its own: `os.lstat` shows a symbolic link whose size is the length of the path `data.txt`, and `os.stat` follows the link to the file.

## How it works

**The stat structure.** POSIX defines `struct stat` in `<sys/stat.h>`, and the structure "shall include at least the following members"[^1]. They are the device ID and the file serial number, which together identify the file, the mode, the link count, the owner and the group, the size, three times and the block counts[^1]. The link count is the "Number of hard links to the file"[^1]. The size is "For regular files, the file size in bytes"[^1]. The three times are the "Last data access timestamp", the "Last data modification timestamp" and the "Last file status change timestamp"[^1]. These fields come from the Unix i-node. The paper of 1974 says that the i-node "contains the description of the file", which includes "the user and group-ID of its owner", "its protection bits", "its size" and "the number of links to the file"[^6].

**The mode.** One number, `st_mode`, holds two things. Python describes it as the "File mode: file type and file mode bits (permissions)"[^7]. The type bits tell a regular file from a directory, a symbolic link, a FIFO, a socket or a device, and macros such as `S_ISREG` and `S_ISLNK` test them. The low bits are the permissions of the owner, the group and the others. See [[file-permissions]].

**The times.** The modification time changes when the data changes. The change time is not a creation time. Linux says that it "is changed by writing or by setting inode information (i.e., owner, group, link count, mode, etc.)"[^3]. A creation time, or birth time, is not in the stat structure of POSIX. On Linux, it comes only from `statx`, and "The btime timestamp was not historically present on UNIX systems and is not currently supported by most Linux filesystems"[^3]. See [[get-the-modification-time-of-a-file]].

**Follow the link or not.** A symbolic link is a file that holds a path, so it has metadata of its own. POSIX has two calls. For `lstat`, "In that case lstat() shall return information about the link, while stat() shall return information about the file the link references"[^2]. For a link, the size is "the length in bytes of the pathname contained in the symbolic link"[^1]. Each language has the two forms:

- Python has `os.stat` and `os.lstat`, and most calls take `follow_symlinks`[^7].
- Node.js has `lstat`, where "the link itself is stat-ed, not the file that it refers to"[^8].
- In Java, "By default, symbolic links are followed", and `NOFOLLOW_LINKS` changes this[^12].
- Go's `os.Lstat` "makes no attempt to follow the link"[^15].
- Rust has `fs::metadata` and `fs::symlink_metadata`[^16].
- C++ has `status` and `symlink_status`, which works "as if the POSIX lstat is used (symlinks are not followed)"[^18].

**Extended attributes.** The fixed fields are not enough for some programs. "Extended attributes are name:value pairs associated permanently with files and directories, similar to the environment strings associated with a process"[^5]. On Linux, the name has a namespace, such as `user.mime_type`, `trusted.md5sum`, `system.posix_acl_access` or `security.selinux`[^5]. The file system keeps them, but they have limits: "The VFS-imposed limits on attribute names and values are 255 bytes and 64 kB, respectively"[^5]. Python has `os.getxattr`, `os.setxattr` and `os.listxattr`[^7]. In Java, "User-defined file attributes are used to store metadata with a file that is not meaningful to the file system"[^11].

**Windows.** Windows keeps a different set. `GetFileInformationByHandle` fills a `BY_HANDLE_FILE_INFORMATION` with the attribute flags, three times that include the creation time, the size, the number of links and a file index[^19]. "The identifier (low and high parts) and the volume serial number uniquely identify a file on a single computer"[^19], as the device ID and the inode number do on Unix.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `os.stat`, `os.lstat`, `os.stat_result`, the `stat` module; `os.DirEntry`; `os.getxattr` |
| JavaScript | stdlib | Node.js `node:fs`: `stat`, `lstat` and `fs.Stats`, with `bigint: true` for nanoseconds |
| TypeScript | stdlib | the `node:fs` module of Node.js, with the types of `@types/node` |
| Java | stdlib | `Files.readAttributes` with `BasicFileAttributes`, `PosixFileAttributes` or a view name; `NOFOLLOW_LINKS` |
| Kotlin | stdlib | `kotlin.io.path` extensions on `java.nio.file.Path`, so the Java attribute views do the work |
| C# | stdlib | `FileSystemInfo`: `Attributes`, the times, `UnixFileMode`, `LinkTarget`; no link count and no inode number |
| Go | stdlib | `os.Stat` and `os.Lstat` return an `fs.FileInfo`; `Sys()` gives `*syscall.Stat_t` on Unix |
| Rust | stdlib | `fs::metadata`, `fs::symlink_metadata`, `Metadata`; `std::os::unix::fs::MetadataExt` |
| C | library | ISO C has no metadata call; POSIX `stat`, `lstat`, `fstat` and `struct stat`; Linux `statx` |
| C++ | stdlib | `std::filesystem::status` and `symlink_status`, with `file_size`, `hard_link_count` and `last_write_time` |
| Bash | native | the tests `-e`, `-f`, `-d`, `-L`, `-s`, `-nt` and `-ot`; the `stat` command of the system |
| PowerShell | native | `Get-Item` returns a .NET `FileInfo` or `DirectoryInfo` object |
| SQL | n/a | a query works on rows; SQL has no operations on file metadata |
| Regex | n/a | a pattern matches text and does not read files |
| HTML | n/a | a page cannot stat a path; a `File` object from the user has a name, a size, a type and `lastModified` |
| CSS | n/a | style rules do not read files |

Each language reads the same stat structure on Unix, but each one shows a different part of it. Go's portable `FileInfo` has only six methods: `Name`, `Size`, `Mode`, `ModTime`, `IsDir` and `Sys`, which is the "underlying data source (can return nil)"[^14]. Rust puts the Unix fields in a trait, `MetadataExt`, with methods such as `dev`, `ino`, `mode`, `nlink`, `uid` and `gid`[^17]. Java has views: `BasicFileAttributes` for all file systems, and `PosixFileAttributes` for "File attributes associated with files on file systems used by operating systems that implement the Portable Operating System Interface (POSIX) family of standards"[^10]. In .NET, `FileSystemInfo` "Provides the base class for both FileInfo and DirectoryInfo objects"[^13], and its `LinkTarget` is `null` if the object "doesn't represent a link"[^13]. A C++ `file_status` holds only the type and the permissions[^18].

The example files show the two calls, `stat` and `lstat`, in each language. They also show the field that each language adds: Python's `os.DirEntry`, the `bigint` times of Node.js, the attribute views and the `fileKey` of Java, the `FileAttributes` flags of .NET, Go's `Sys`, Rust's `MetadataExt`, and the `file_type::not_found` status of C++.

## Variations and related ideas

- **statx.** The Linux call `statx` takes a mask of the fields that the program wants, and it can return the birth time[^4]. The answer can differ from the request: "the kernel may return fields that weren't requested and may fail to return fields that were requested, depending on what the backing filesystem supports"[^4].
- **Access control lists.** An ACL gives permissions to more users than the owner and the group. On Linux, "additional security features such as Access Control Lists (ACLs) may be implemented using extended attributes"[^5].
- **Windows attribute flags.** Windows keeps a set of attribute flags in `dwFileAttributes`[^19]. .NET shows them in `FileSystemInfo.Attributes` on all systems[^13].
- **The file identity.** The device ID and the inode number together identify a file, and a program uses them to see that two names are hard links to one file. See [[file-system-concept]].

## Pitfalls

- **The change time is not the creation time.** `st_ctime` changes on each change of the metadata[^3]. Python's `st_ctime` is the "Time of most recent metadata change expressed in seconds", and on Windows, where it is deprecated, the documentation says: "Use st_birthtime for the file creation time"[^7].
- **The creation time can be missing or false.** Python's `st_birthtime` "is not always available, and may raise AttributeError"[^7]. Rust's `created` returns an error "on platforms or filesystems where it is not available"[^16]. Node.js does not fail: where a birth time is not available, `birthtime` "may instead hold either the ctime or 1970-01-01T00:00Z"[^8]. Java returns "an implementation specific default value, typically the last-modified-time or a FileTime representing the epoch (1970-01-01T00:00:00Z)"[^9]. Do not trust a creation time that you did not check.
- **The access time is often not updated.** "Some filesystem types allow mounting in such a way that file and/or directory accesses do not cause an update of the atime timestamp"[^3]. Do not use the access time to find files that nobody reads.
- **stat follows the link.** A program that walks a tree with `stat` follows the links out of the tree, and a link to a parent directory makes a loop. A dangling link has no target, so `stat` fails while `lstat` works. Use the form that does not follow links when the link itself matters.
- **The permission bits of a link have no meaning.** POSIX says that for symbolic links, "The file mode bits in st_mode are unspecified"[^2]. The values can differ between systems, so do not test them.
- **The metadata can change after the call.** The result describes the file at one time. Python warns that a check before an open "creates a security hole, because the user might exploit the short time interval between checking and opening the file to manipulate it"[^7]. Open the file, and then use `fstat` on the open descriptor.
- **Extended attributes are not portable.** "Extended attributes are not specified in POSIX.1, but some other systems (e.g., the BSDs and Solaris) provide a similar feature"[^5]. The kernel and the file system can also set limits on the number and the size of the attributes[^5]. Test for the support before you use them.

## Cost

A `stat` call is one system call. It reads the inode, which the system often has in its cache, and it does not read the data of the file. A program that lists a directory and calls `stat` for each name makes one more system call for each file. Python says that `scandir` instead of `listdir` "can significantly increase the performance of code that also needs file type or file attribute information, because os.DirEntry objects expose this information if the operating system provides it when scanning a directory"[^7]. On a network file system, each call can wait for the server. Linux `statx` has a flag for this case: `AT_STATX_DONT_SYNC` takes "whatever the system has cached if possible", and "This may mean that the information returned is approximate"[^4].

## History

Ritchie and Thompson described the i-node in their Unix paper of 1974. It holds the owner, the "protection bits", the "physical disk or tape addresses for the file contents", the size, the times, the link count, and "a code indicating whether the file is a directory, an ordinary file, or a special file"[^6]. The `stat` call returned these fields to programs, and POSIX keeps them in `struct stat`[^1], with `lstat` for symbolic links[^2]. The i-node of 1974 listed a "time of creation"[^6], but the POSIX structure has no creation time[^1]. Linux added `statx`, with a mask and a birth time, in "Linux 4.11, glibc 2.28"[^4]. Extended attributes are not in POSIX[^5].
