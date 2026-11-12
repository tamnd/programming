---
tag: N9ZE82
kind: concept
slug: file-system-concept
name: File system
aliases: [file system, filesystem, file tree, directory tree, volume, mount point, inode]
area: data-and-persistence/files-and-file-systems
level: intro
summary: >-
  A file system keeps named files in a tree of directories on a volume. A program sees names, file types, file serial numbers and mount points, and not the blocks on the disk.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1965
  people: [R. C. Daley, P. G. Neumann]
  note: The Multics file system of Daley and Neumann (1965) had a tree of directories and links. Unix (1974) added i-numbers and the mount of a volume on a directory.
see_also: [read-a-file-line-by-line, write-a-file, list-files-in-a-directory, path-traversal]
same_as:
  wikidata: Q174989
  wikipedia: File_system
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 3, Definitions
  locator: 3.103 Directory, 3.104 Directory Entry (or Hard Link), 3.139 File, 3.152 File Serial Number, 3.153 File System, 3.210 Mount Point, 3.310 Regular File
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html
  title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 4, General Concepts
  locator: 4.16 Pathname Resolution
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/cacm.html
  title: D. M. Ritchie and K. Thompson, The UNIX Time-Sharing System (1974, the BSTJ version)
  locator: Abstract; 3.4 Removable file systems; IV. Implementation of the file system
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
  title: The Open Group, POSIX.1-2024, sys/stat.h
  locator: Application Usage
- url: https://learn.microsoft.com/en-us/windows/win32/fileio/file-systems
  title: File Systems, Win32 apps, Microsoft Learn
  locator: Volume Management
- url: https://en.wikipedia.org/wiki/File_system
  title: File system, Wikipedia
  locator: Directories; Types of file systems
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/FileSystem.html
  title: Java SE 25 API, java.nio.file.FileSystem
  locator: class description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/FileStore.html
  title: Java SE 25 API, java.nio.file.FileStore
  locator: class description
- url: https://pkg.go.dev/io/fs
  title: Go standard library, package io/fs
  locator: Overview
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: GNU Bash Reference Manual
  locator: 6.4 Bash Conditional Expressions
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_filesystem_provider?view=powershell-7.6
  title: about_FileSystem_Provider, PowerShell 7.6, Microsoft Learn
  locator: Detailed description
- url: https://en.cppreference.com/w/cpp/filesystem.html
  title: Filesystem library, C++ reference, cppreference.com
  locator: introduction
- url: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
  title: Naming Files, Paths, and Namespaces, Win32 apps, Microsoft Learn
  locator: Naming Conventions
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: listdir, scandir, DirEntry.stat, statvfs_result
- url: https://doc.rust-lang.org/std/fs/fn.read_dir.html
  title: The Rust Standard Library, std::fs::read_dir
  locator: Platform-specific behavior
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
  title: The Open Group, POSIX.1-2024, rename
  locator: Errors, EXDEV
- url: https://multicians.org/fjcc4.html
  title: R. C. Daley and P. G. Neumann, A General-Purpose File System For Secondary Storage (FJCC 1965)
  locator: 2. The File Structure and Access Control
---

## Idea

A disk holds blocks of bytes, but a program asks for `docs/notes.txt`. The file system is the layer between the two. It keeps files, gives them names in a tree of directories, and stores the attributes of each file, such as its type, its size and its permissions. A program uses only names and attributes. The file system decides where the bytes go.

```python run id=idea
import os
import shutil
import stat
from pathlib import Path

# A directory holds names. Each name refers to a file.
os.mkdir("docs")
Path("docs/notes.txt").write_text("hello\n", encoding="utf-8")
# A hard link is a second name for the same file.
os.link("docs/notes.txt", "copy-of-notes.txt")

for name in ["docs", "docs/notes.txt", "copy-of-notes.txt"]:
    st = os.stat(name)
    kind = "directory" if stat.S_ISDIR(st.st_mode) else "regular file"
    print(f"{name:18} {kind:13} {st.st_size if kind != 'directory' else '-':>3}")

# The device ID and the file serial number (inode number) identify a file.
a = os.stat("docs/notes.txt")
b = os.stat("copy-of-notes.txt")
print("same file:", (a.st_dev, a.st_ino) == (b.st_dev, b.st_ino))
print("names of the file:", a.st_nlink)

# The root directory is a mount point, and each file system has its own space.
print("/ is a mount point:", os.path.ismount("/"))
usage = shutil.disk_usage(".")
print("free space is known:", 0 < usage.free <= usage.total)
```

```output for=idea
docs               directory       -
docs/notes.txt     regular file    6
copy-of-notes.txt  regular file    6
same file: True
names of the file: 2
/ is a mount point: True
free space is known: True
```

The program makes a directory, a file in it, and a second name for the file. `os.stat` gives the attributes of the file that a name refers to. The two names give the same device ID and the same file serial number, so they are one file with two names, and the link count is 2. The file system does not store the name in the file. It stores the name in the directory. The last two lines show that the root directory is a mount point, and that the file system reports its free space.

## How it works

**Files.** POSIX defines a file as "An object that can be written to, or read from, or both"[^1]. Each file has a type. "File types include regular file, character special file, block special file, FIFO special file, symbolic link, socket, and directory"[^1]. A regular file is "a randomly accessible sequence of bytes, with no further structure imposed by the system"[^1]. So lines, records and formats are the work of the program, not of the file system.

**Directories and names.** A directory is "A file that contains directory entries", and "No two directory entries in the same directory have the same name"[^1]. A directory entry, or hard link, is "An object that associates a filename with a file", and "Several directory entries can associate names with the same file"[^1]. Because a directory can hold other directories, the names make a tree. A path such as `a/b/c` names a route through the tree: "Each filename in the pathname is located in the directory specified by its predecessor"[^2].

**File serial numbers.** The name is not the identity of a file. POSIX gives each file a file serial number, "A per-file system unique identifier for a file"[^1]. Unix calls it the i-number. In the Unix paper, a directory entry "contains only a name for the associated file and a pointer to the file itself", and the pointer "is an integer called the i-number (for index number) of the file"[^3]. The i-number selects the i-node, which holds the owner, the protection bits, the disk addresses, the size, the times and the number of links[^3]. A file system is "A collection of files and certain of their attributes", and "It provides a name space for file serial numbers referring to those files"[^1]. So an inode number is unique only inside one file system. POSIX requires that files are "uniquely identified by the combination of the st_ino and st_dev fields"[^4], that is, the inode number and the device ID together.

**Mounts and volumes.** A system has many file systems, but on Unix the program sees one tree. The mount operation puts the root of one file system at a directory of another. The Unix paper says that "mount replaces a leaf of the hierarchy tree (the ordinary file) by a whole new subtree (the hierarchy stored on the removable volume)"[^3]. POSIX defines a mount point as "Either the system root directory or a directory for which the st_dev field of structure stat differs from that of its parent directory"[^1]. Windows shows each volume with its own root, such as `C:\`. Microsoft says that "The highest level of organization in the file system is the volume", and "A file system resides on a volume"[^5].

**File system types.** Each volume is formatted with one file system type, such as FAT, exFAT, NTFS, APFS or ext3[^6]. The type decides the rules that a program can see: the longest name, the characters that a name can hold, case sensitivity, the resolution of the times, and the support for links and permissions. The site keeps the types as cards, not as pages.

**File systems in a library.** A language library gives one interface to many kinds of storage. In Java, a `FileSystem` "Provides an interface to a file system and is the factory for objects to access files and other objects in the file system"[^7], and a `FileStore` "represents a storage pool, device, partition, volume, concrete file system or other implementation specific means of file storage"[^8]. Go says that "A file system can be provided by the host operating system but also by other packages"[^9]. So a zip archive, a tree in memory and the files in a program can use the same calls as a disk.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `os.stat`, `os.scandir`, `os.link`, `os.statvfs`, `os.path.ismount`, `shutil.disk_usage`; `pathlib.Path` |
| JavaScript | stdlib | Node.js `node:fs`: `stat` with `dev`, `ino` and `nlink`, `readdir` with `withFileTypes`, `link`, `statfs` |
| TypeScript | stdlib | the `node:fs` module of Node.js, with the types of `@types/node` |
| Java | stdlib | `java.nio.file`: `FileSystem`, `FileStore`, `Files.isSameFile`, the `unix` attribute view; the zip file system provider |
| Kotlin | stdlib | `kotlin.io.path` extensions on `java.nio.file.Path`, so the Java classes do the work |
| C# | stdlib | `System.IO`: `DirectoryInfo`, `FileInfo`, `FileSystemInfo.LinkTarget`, `DriveInfo`; .NET 10 has no method that makes a hard link |
| Go | stdlib | `os.Stat`, `os.SameFile`, `os.Link`; the `io/fs` interfaces with `os.DirFS` and `testing/fstest.MapFS` |
| Rust | stdlib | `std::fs`: `metadata`, `read_dir`, `hard_link`; `std::os::unix::fs::MetadataExt` for `dev`, `ino` and `nlink` |
| C | library | ISO C has `fopen`, `remove`, `rename` and `tmpfile`, and no directories; POSIX adds `stat`, `link`, `statvfs` and `<dirent.h>` |
| C++ | stdlib | `std::filesystem`: `directory_iterator`, `equivalent`, `hard_link_count`, `space` |
| Bash | native | the tests `-d`, `-f` and `-ef`; the commands `ls -i`, `ln`, `df` and `mount` |
| PowerShell | native | the FileSystem provider with drives such as `C:` and `Temp:`; `Get-Item`, `Get-ChildItem`, `Get-PSDrive` |
| SQL | n/a | a query works on tables; the database server keeps them in files, but SQL has no file operations |
| Regex | n/a | a pattern matches text, and it does not name or open files |
| HTML | n/a | a page cannot see the file system; `<input type="file">` lets the user choose files and give them to the page |
| CSS | n/a | style rules do not read or name files, except the URLs of resources |

Each general-purpose language reaches the file system through its standard library, which calls the operating system. C is the exception, because the ISO C library has only a few operations on files and none on directories, so C programs use POSIX or the Win32 API. In Bash, `[[ a -ef b ]]` is "True if file1 and file2 refer to the same device and inode numbers"[^10]. In PowerShell, "The FileSystem drives are a hierarchical namespace containing the directories and files on your computer"[^11].

The example files show the same ideas in each language. Python and Rust show that a rename keeps the inode number, and Python and JavaScript read the free space of the file system with `statvfs` and `statfs`. Java writes a file into a zip archive with the same `Files` calls that it uses for the disk. Go walks a directory on the disk and a tree in memory with one function, because both are an `fs.FS`. C# makes a symbolic link, because .NET 10 has no method that makes a hard link. C uses `stat`, `link` and `statvfs`, and C++ uses `std::filesystem`, which "provides facilities for performing operations on file systems and their components, such as paths, regular files, and directories"[^12].

## Variations and related ideas

- **Hard links and symbolic links.** A hard link is one more directory entry for the same file. A symbolic link is a small file that holds a path. See the page about links.
- **Virtual file systems.** Some file systems have no disk behind them. A virtual file system gives files that are "computed on request", for example the files of procfs and sysfs[^6], and procfs "exposes process information as special files"[^6]. A zip archive, a tree in memory and the files built into a program can also be file systems in a library, as in Java and Go.
- **Network file systems.** With a network file system, "Programs using local interfaces can transparently create, manage and access hierarchical directories and files in remote network-connected computers"[^6]. NFS and SMB are examples[^6]. POSIX includes "parts of the tree that may be remotely mounted from other machines on the network"[^4] in the rule for `st_ino` and `st_dev`.
- **Databases.** A database also keeps data on a disk, but it gives records and queries instead of byte streams and names.

## Pitfalls

- **Case rules differ.** Microsoft says: "Do not assume case sensitivity"[^13]. Windows treats `OSCAR`, `Oscar` and `oscar` as the same name by default, but "some file systems (such as a POSIX-compliant file system) may consider them as different"[^13]. Code that works on one system can overwrite a file, or not find it, on the other.
- **Directory order is not defined.** Python returns the entries of `listdir` "in arbitrary order"[^14]. In Rust, "The order in which read_dir returns entries can change between calls"[^15]. Sort the names when the order matters, for example in tests and in build outputs.
- **A rename does not cross file systems.** POSIX `rename` fails with `EXDEV` when the two names are "on different file systems and the implementation does not support hard links between file systems"[^16]. A move to another mount point must copy the data and then remove the old file.
- **An inode number alone is not an identity.** Two files on two file systems can have the same inode number. Compare the device ID and the inode number together. Python warns that on Windows, the `st_ino`, `st_dev` and `st_nlink` of a cached `DirEntry.stat()` result "are always set to zero"[^14], so call `os.stat()` for them.
- **Free space has two numbers.** `statvfs` gives `f_bfree`, the "Number of free blocks", and `f_bavail`, the "Number of free blocks for unprivileged users"[^14]. A normal program can use only the second number.
- **The block unit is not fixed.** The unit of `st_blocks` "is not defined within POSIX.1-2024", and "In some implementations it is 512 bytes"[^4]. Do not use `st_blocks` as a size in bytes without a check.

## Cost

Each call to the file system of the operating system is a system call, which costs much more than a call to a function in the program. The name lookup is done one component at a time, so a long path needs a lookup in each directory. A program that lists a directory and then calls `stat` on each name makes one more system call per file. Python's documentation says that `scandir` "can significantly increase the performance of code that also needs file type or file attribute information, because os.DirEntry objects expose this information if the operating system provides it when scanning a directory"[^14]. On a network file system, each call can also wait for the network.

## History

Daley and Neumann described the file system of Multics in 1965. They wrote that for on-line work, "user owned off-line detachable storage media such as cards and tape become highly undesirable"[^17]. In it, "The file structure consists of a basic tree hierarchy of files, across which links may be added to facilitate simple access to files elsewhere in the hierarchy"[^17]. Wikipedia says that "The first file system to support arbitrary hierarchies of directories was used in the Multics operating system"[^6].

Unix took the tree and made it simpler. The paper of Ritchie and Thompson (1974) lists "A hierarchical file system incorporating demountable volumes" as the first feature of the system[^3]. It describes the i-number, the i-node and the mount of a volume on a directory[^3], and these ideas are still in POSIX. Windows keeps a separate root for each volume, and the file systems of MS-DOS, Windows and the Apple systems also have directory trees[^6].
