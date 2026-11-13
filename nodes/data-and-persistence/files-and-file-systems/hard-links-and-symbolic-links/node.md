---
tag: VQ38EM
kind: concept
slug: hard-links-and-symbolic-links
name: Hard links and symbolic links
aliases: [hard link, symbolic link, symlink, soft link, link count, dangling link, junction, reparse point, shortcut, ln, readlink, lstat]
area: data-and-persistence/files-and-file-systems
level: core
summary: >-
  A hard link is one more name for the same file, and the file stays while one name is left. A symbolic link is a small file that holds a path, which the system follows, so it can dangle or loop.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1974
  people: [D. M. Ritchie, K. Thompson]
  note: The Unix paper (1974) describes links, directory entries of equal status for one file. Symbolic links came in 4.1a BSD (1982), as the Wikipedia article on them reports.
see_also: [file-system-concept, delete-a-file, rename-a-file, check-whether-a-file-exists]
same_as:
  wikidata: Q646197
  wikipedia: Symbolic_link
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 3, Definitions
  locator: 3.168 Hard Link, 3.187 Link, 3.188 Link Count, 3.364 Symbolic Link
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/link.html
  title: The Open Group, POSIX.1-2024, link, linkat
  locator: Description, Errors
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/symlink.html
  title: The Open Group, POSIX.1-2024, symlink, symlinkat
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/readlink.html
  title: The Open Group, POSIX.1-2024, readlink, readlinkat
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
  title: The Open Group, POSIX.1-2024, sys/stat.h
  locator: Description, st_size
- url: https://man7.org/linux/man-pages/man7/inode.7.html
  title: inode(7), Linux manual page
  locator: Link count
- url: https://man7.org/linux/man-pages/man7/symlink.7.html
  title: symlink(7), Linux manual page
  locator: Description, Symbolic link ownership, permissions, and timestamps
- url: https://man7.org/linux/man-pages/man7/path_resolution.7.html
  title: path_resolution(7), Linux manual page
  locator: Step 2, symbolic links
- url: https://learn.microsoft.com/en-us/windows/win32/fileio/hard-links-and-junctions
  title: Hard links and junctions, Win32 apps, Microsoft Learn
  locator: Hard links, Junctions
- url: https://learn.microsoft.com/en-us/windows/win32/fileio/symbolic-links
  title: Symbolic Links, Win32 apps, Microsoft Learn
  locator: Overview, Supported Operating Systems
- url: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createsymboliclinkw
  title: CreateSymbolicLinkW function (winbase.h), Win32 apps, Microsoft Learn
  locator: Parameters (dwFlags), Remarks
- url: https://learn.microsoft.com/en-us/windows/win32/shell/links
  title: Shell Links, Win32 apps, Microsoft Learn
  locator: About Shell Links
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: os.link, os.symlink, os.readlink
- url: https://nodejs.org/api/fs.html#fspromisessymlinktarget-path-type
  title: Node.js, File system, fsPromises.symlink
  locator: type
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
  title: Java SE 25 API, java.nio.file.Files
  locator: createLink, createSymbolicLink
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.createsymboliclink?view=net-10.0
  title: File.CreateSymbolicLink(String, String) Method (System.IO), .NET API, Microsoft Learn
  locator: Definition
- url: https://pkg.go.dev/os
  title: Go standard library, package os
  locator: func Link, func Symlink, func Lstat, func SameFile
- url: https://doc.rust-lang.org/std/fs/fn.hard_link.html
  title: The Rust Standard Library, std::fs::hard_link
  locator: Description, Platform-specific behavior
- url: https://doc.rust-lang.org/std/os/unix/fs/fn.symlink.html
  title: The Rust Standard Library, std::os::unix::fs::symlink
  locator: Description
- url: https://en.cppreference.com/w/cpp/filesystem/create_symlink.html
  title: std::filesystem::create_symlink, cppreference.com
  locator: Description
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/cacm.html
  title: D. M. Ritchie and K. Thompson, The UNIX Time-Sharing System (1974, the BSTJ version)
  locator: 3.2 Directories
- url: https://en.wikipedia.org/wiki/Symbolic_link
  title: Symbolic link, Wikipedia
  locator: History
---

## Idea

A file and its name are two things. The name is an entry in a directory, and it points to the file. A hard link is one more entry that points to the same file, so the file has two names of equal status. A symbolic link is a different kind of file: it holds a path, and the system reads that path when a program opens the link. So a hard link names the file itself, and a symbolic link names a path, which can point to nothing.

```python run id=idea
import os

with open("data.txt", "w") as f:
    f.write("alpha\n")

os.link("data.txt", "hard.txt")     # a second name for the same file
os.symlink("data.txt", "soft.txt")  # a small file that holds a path

st, hard = os.stat("data.txt"), os.stat("hard.txt")
print("same inode:  ", st.st_ino == hard.st_ino)
print("link count:  ", st.st_nlink)
print("link target: ", os.readlink("soft.txt"))
print("soft is link:", os.path.islink("soft.txt"), os.path.islink("hard.txt"))

# Remove the first name. The file stays while a hard link names it.
os.remove("data.txt")
print("hard reads:  ", open("hard.txt").read().strip())
print("link count:  ", os.stat("hard.txt").st_nlink)
print("soft exists: ", os.path.exists("soft.txt"), "(dangling)")
print("soft lexists:", os.path.lexists("soft.txt"))
```

```output for=idea
same inode:   True
link count:   2
link target:  data.txt
soft is link: True False
hard reads:   alpha
link count:   1
soft exists:  False (dangling)
soft lexists: True
```

The two names `data.txt` and `hard.txt` have the same inode, so they are one file with a link count of 2. `soft.txt` holds the string `data.txt`. When the program removes `data.txt`, the file stays, because `hard.txt` still names it, and the count goes down to 1. But `soft.txt` now holds a path that names nothing, so it is a dangling link: `exists` follows it and gives `False`, and `lexists` looks at the link itself and gives `True`.

## How it works

**Hard links.** POSIX calls a directory entry a hard link: "A file can have multiple hard links as a result of an execution of the ln utility (without the -s option) or the link () function"[^1]. The link count is "The number of directory entries that refer to a particular file"[^1]. `link` "shall atomically create a new hard link for the existing file and the link count of the file shall be incremented by one"[^2]. Linux keeps the count in `st_nlink`, which "contains the number of hard links to the file"[^6]. So `unlink` and `remove` remove one name, and the file stays while another name is left, as the idea shows.

A hard link has two limits. Linux says: "Hard links may not refer to directories (to prevent the possibility of loops within the filesystem tree, which would confuse many programs) and may not refer to files on different filesystems (because inode numbers are not unique across filesystems)"[^7]. POSIX gives the error `EXDEV` when "the implementation does not support hard links between file systems"[^2].

**Symbolic links.** POSIX defines a symbolic link as "A type of file with the property that when the file is encountered during pathname resolution, a string stored by the file is used to modify the pathname resolution"[^1]. `symlink` stores the string as it is: "The string pointed to by path1 shall be treated only as a string and shall not be validated as a pathname"[^3]. Linux says: "a symbolic link is a pointer to another name, and not to an underlying object. For this reason, symbolic links may refer to directories and may cross filesystem boundaries. There is no requirement that the pathname referred to by a symbolic link should exist"[^7]. A link to a path that does not exist "is said to be a dangling link"[^7].

`readlink` reads the string. It does not add a NUL byte, and "If the number of bytes in the symbolic link is less than bufsize , the contents of the remainder of buf are unspecified"[^4]. `lstat` gives the status of the link itself, and for a link, `st_size` is "the length in bytes of the pathname contained in the symbolic link"[^5].

**Which calls follow a link.** Most calls follow a symbolic link, so `open` and `stat` act on the target. Some calls act on the link itself: "lchown(2) , lgetxattr(2) , llistxattr(2) , lremovexattr(2) , lsetxattr(2) , lstat(2) , readlink(2) , rename(2) , rmdir(2) , and unlink(2)"[^7]. So `unlink` removes the link, not its target, and `rename` moves the link.

**Loops.** A link can point to itself, or to a link that points back. The system counts the links that it follows: "As currently implemented on Linux, the maximum number of symbolic links that will be followed while resolving a pathname is 40"[^8]. "An ELOOP error is returned when the maximum is exceeded"[^8].

**Windows.** NTFS "supports three types of file links: hard links, junctions, and symbolic links"[^9]. A junction "can also link directories located on different local volumes on the same computer"[^9], and junctions "are implemented through reparse points"[^9]. Symbolic links "are designed to aid in migration and application compatibility with UNIX operating systems"[^10]. A process that is not elevated can make one only with the flag `SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE`, and "Developer Mode must first be enabled on the machine before this option will function"[^11]. A shortcut (a `.lnk` file) is not a link of the file system. It is "a data object that contains information used to access another object in the Shell's namespace"[^12], and only the Shell reads it.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `os.link`, `os.symlink`, `os.readlink`, `os.lstat`; `os.path.islink` and `os.path.lexists`; `pathlib.Path.hardlink_to` and `symlink_to` |
| JavaScript | stdlib | Node.js `fs.link`, `fs.symlink` with a `type` for Windows, `fs.readlink`, `fs.lstat` |
| TypeScript | stdlib | the `node:fs` module of Node.js, with the types of `@types/node` |
| Java | stdlib | `Files.createLink`, `Files.createSymbolicLink`, `Files.readSymbolicLink`, `LinkOption.NOFOLLOW_LINKS` |
| Kotlin | stdlib | `kotlin.io.path` extensions such as `createSymbolicLinkPointingTo` on `java.nio.file.Path` |
| C# | stdlib | `File.CreateSymbolicLink`, `FileSystemInfo.LinkTarget` and `ResolveLinkTarget`; no hard link call |
| Go | stdlib | `os.Link`, `os.Symlink`, `os.Readlink`, `os.Lstat`, `os.SameFile` |
| Rust | stdlib | `fs::hard_link`, `fs::read_link`, `fs::symlink_metadata`; `std::os::unix::fs::symlink` and the Windows `symlink_file` and `symlink_dir` |
| C | library | ISO C has no links; POSIX `link`, `symlink`, `readlink`, `lstat`, `unlink` |
| C++ | stdlib | `std::filesystem::create_hard_link`, `create_symlink`, `read_symlink`, `hard_link_count`, `symlink_status` |
| Bash | native | the `ln` and `ln -s` commands; the test `-L`; `readlink` on GNU and BSD systems |
| PowerShell | native | `New-Item -ItemType HardLink`, `SymbolicLink` or `Junction`; the `LinkType` and `Target` properties |
| SQL | n/a | a database has no links between files |
| Regex | n/a | a pattern matches text and does not read files |
| HTML | n/a | a page cannot make a link in a file system |
| CSS | n/a | style rules do not read files |

Each language with files has the calls of POSIX: Python's `os.link` will "Create a hard link pointing to src named dst"[^13], and `os.symlink` will "Create a symbolic link pointing to src named dst"[^13]. Go's `Link` "creates newname as a hard link to the oldname file"[^17], and Rust's `hard_link` "currently corresponds to the CreateHardLink function on Windows. On most Unix systems, it corresponds to the linkat function with no flags"[^18]. In Java, both calls are an "optional operation"[^15], so a file system without links throws an exception.

Windows makes the symbolic link calls differ. A Windows link is for a file or for a directory, so Node.js has a `type` argument that "is only available on Windows and ignored on other platforms. It can be set to 'dir' , 'file' , or 'junction'"[^14]. Rust has `symlink` only in the Unix module, where it "Creates a new symbolic link on the filesystem"[^19], and C++ says: "Portable code should use (3,4) to create directory symlinks rather than (1,2) , even though there is no distinction on POSIX systems"[^20]. .NET has `File.CreateSymbolicLink`, which "Creates a file symbolic link identified by path that points to pathToTarget"[^16].

The example files show more: the relative target and the loop in Python, `stat` and `lstat` in Node.js, `NOFOLLOW_LINKS` in Java, a chain of links in C#, `SameFile` in Go, the link count in Rust, the missing NUL of `readlink` in C, and `symlink_status` in C++.

## Variations and related ideas

- **Relative and absolute targets.** A relative target is read from the directory of the link, not from the current directory. A link with a relative target keeps working when the directory that holds both moves, and an absolute target keeps working when only the link moves. Linux warns that with a relative path, "moving it to another directory may very well cause it to stop working, since the path may no longer be correct"[^7].
- **Rename and delete.** A rename moves one name, and the other hard links do not change. See [[rename-a-file]] and [[delete-a-file]].
- **Exists or not.** A dangling link exists as a link, but not as a file. The answer depends on whether the call follows the link. See [[check-whether-a-file-exists]].

## Pitfalls

- **A dangling link looks like a missing file.** `open` and `stat` follow the link, so a link to a removed file gives "no such file". Check with `lstat` or `lexists` when the difference matters.
- **A relative target is read from the link.** `os.symlink("data.txt", "sub/link")` makes a link to `sub/data.txt`. The Python example shows this. Make the target relative to the link, or make it absolute.
- **readlink does not end the string.** In C, `readlink` gives a count of bytes, and the rest of the buffer is unspecified[^4]. Add the NUL byte yourself, and use a buffer of the size that `lstat` gives plus one.
- **A walk of a tree can loop.** A link to a parent directory makes a tree with no end. A walk that follows links must keep the device and inode of each directory that it visits, or not follow links.
- **A hard link changes the file for all names.** A write through one name changes the data that the other names show. A program that edits a file in place changes each link, and a program that writes a new file and renames it over the old name breaks the hard link.
- **A link can let another user choose a file.** A program that follows a link in a directory that other users can write to can open or change a file that the other user picks. In such directories, use the calls that act on the link itself[^7], and check what each name is before the program uses it.

## Cost

A hard link is one directory entry, and it uses no new space for the data. A symbolic link is a small file, and each path lookup that meets it reads it and starts again from its target, so each link adds one more lookup. The system stops after 40 links on Linux[^8].

## History

Ritchie and Thompson described links in 1974: "a directory entry for a file is sometimes called a link. The Unix system differs from other systems in which linking is permitted in that all links to a file have equal status. That is, a file does not exist within a particular directory; the directory entry for a file consists merely of its name and a pointer to the information actually describing the file"[^21]. They also wrote: "Thus a file exists independently of any directory entry, although in practice a file is made to disappear along with the last link to it"[^21]. The Wikipedia article on symbolic links reports that they "were introduced in 1982 in 4.1a BSD Unix"[^22]. POSIX now has both kinds[^1], and Microsoft says that "Symbolic links are available in NTFS starting with Windows Vista"[^10].
