---
tag: 78QPZ3
kind: concept
slug: file-permissions
name: File permissions
aliases: [permission bits, file mode, mode bits, chmod, umask, setuid, sticky bit, access control list, ACL, read-only attribute]
area: data-and-persistence/files-and-file-systems
level: core
summary: >-
  File permissions say who can read, write or execute a file. Unix keeps nine mode bits for the owner, the group and the others, with a umask for new files. Windows uses ACLs.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1974
  people: [D. M. Ritchie, K. Thompson]
  note: The Unix paper (1974) describes ten protection bits, nine for read, write and execute by the owner, the group and the others, and one for set-user-ID.
see_also: [file-metadata, file-system-concept, get-the-modification-time-of-a-file]
same_as:
  wikidata: Q1172314
  wikipedia: File-system_permissions
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
  title: The Open Group, POSIX.1-2024, sys/stat.h
  locator: Description, the file mode bits
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html
  title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 4, General Concepts
  locator: 4.7 File Access Permissions
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/chmod.html
  title: The Open Group, POSIX.1-2024, chmod
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/umask.html
  title: The Open Group, POSIX.1-2024, umask
  locator: Description
- url: https://man7.org/linux/man-pages/man7/inode.7.html
  title: inode(7), Linux manual page
  locator: The file type and mode
- url: https://man7.org/linux/man-pages/man2/chmod.2.html
  title: chmod(2), Linux manual page
  locator: Description
- url: https://man7.org/linux/man-pages/man5/acl.5.html
  title: acl(5), Linux manual page
  locator: Description; ACL entries; Standards
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/cacm.html
  title: D. M. Ritchie and K. Thompson, The UNIX Time-Sharing System (1974, the BSTJ version)
  locator: 3.5 Protection
- url: https://learn.microsoft.com/en-us/windows/win32/secauthz/access-control-lists
  title: Access Control Lists, Win32 apps, Microsoft Learn
  locator: DACLs and SACLs
- url: https://learn.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
  title: File Attribute Constants, Win32 apps, Microsoft Learn
  locator: FILE_ATTRIBUTE_READONLY
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: os.chmod, os.access
- url: https://docs.python.org/3/library/stat.html
  title: The Python Standard Library, stat
  locator: S_IMODE
- url: https://nodejs.org/api/fs.html#fschmodpath-mode-callback
  title: Node.js, File system, fs.chmod
  locator: File modes
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/PosixFilePermission.html
  title: Java SE 25 API, java.nio.file.attribute.PosixFilePermission
  locator: enum description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/AclFileAttributeView.html
  title: Java SE 25 API, java.nio.file.attribute.AclFileAttributeView
  locator: interface description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.unixfilemode
  title: UnixFileMode Enum (System.IO), .NET API, Microsoft Learn
  locator: Fields
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.fileattributes
  title: FileAttributes Enum (System.IO), .NET API, Microsoft Learn
  locator: Fields, ReadOnly
- url: https://pkg.go.dev/io/fs#FileMode
  title: Go standard library, package io/fs, type FileMode
  locator: type FileMode
- url: https://doc.rust-lang.org/std/fs/struct.Permissions.html
  title: The Rust Standard Library, std::fs::Permissions
  locator: readonly, set_readonly
- url: https://en.cppreference.com/w/cpp/filesystem/perms.html
  title: std::filesystem::perms, cppreference.com
  locator: description
---

## Idea

A system with many users must decide who can use each file. Unix gives each file an owner and a group, and nine bits: read, write and execute for the owner, for the members of the group, and for all other users. A process gets the access of the first class that it belongs to. A mask, the umask, removes bits from each new file, so a program can ask for wide permissions and the user can make them narrower.

```python run id=idea
import os
import stat

def show(path):
    m = os.stat(path).st_mode
    print(f"{path:8} {stat.S_IMODE(m):04o} {stat.filemode(m)}")

# The umask removes bits from the mode that the program asks for.
old = os.umask(0o027)
fd = os.open("new.txt", os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o666)
os.close(fd)
os.mkdir("dir", 0o777)
os.umask(old)
show("new.txt")
show("dir")

# chmod sets the bits as they are; the umask does not apply.
os.chmod("new.txt", 0o755)
show("new.txt")

# Three more bits: set-user-ID, set-group-ID and sticky.
os.chmod("new.txt", 0o4755)
os.chmod("dir", 0o1777)
show("new.txt")
show("dir")
```

```output for=idea
new.txt  0640 -rw-r-----
dir      0750 drwxr-x---
new.txt  0755 -rwxr-xr-x
new.txt  4755 -rwsr-xr-x
dir      1777 drwxrwxrwt
```

The program asks for the mode 0666 for a new file and 0777 for a new directory, but the umask 027 removes the write bit of the group and all of the bits of the others. So the file gets 0640 and the directory gets 0750. `chmod` then sets the mode as it is. Each octal digit is one class: 7 is read, write and execute, 5 is read and execute, and 4 is read only. The fourth digit holds the set-user-ID bit, shown as `s`, and the sticky bit, shown as `t`.

## How it works

**The nine bits.** POSIX names each bit in `<sys/stat.h>`, from `S_IRUSR` to `S_IXOTH`[^1]. For the three classes, "The file permission bits of a file contain read, write, and execute/search permissions for the file owner class, file group class, and file other class"[^2]. Access "shall be granted if an alternate access control mechanism is not enabled and the requested access permission bit is set for the class (file owner class, file group class, or file other class) to which the process belongs"[^2]. A process is in only one class. If it is the owner, the owner bits apply, even when the group bits give more.

**Directories.** On a directory, the bits mean other things. Read lets a process list the names. Write lets it make, rename and remove entries. Execute is "search", which "applies for directories, and means that entries within the directory can be accessed"[^6]. So a file in a directory without the search bit cannot be opened, whatever its own bits are.

**The three special bits.** The mode has three more bits. `S_ISUID` is "Set-user-ID on execution", `S_ISGID` is "Set-group-ID on execution", and `S_ISVTX` is, "On directories, restricted deletion flag"[^1]. A program with the set-user-ID bit runs with the user ID of the owner of the file, not of the user who starts it. The sticky bit on a directory "means that a file in that directory can be renamed or deleted only by the owner of the file, by the owner of the directory, and by a privileged process"[^5].

**The umask.** Each process has a file mode creation mask. POSIX says that "The file mode creation mask of the process is used to turn off permission bits in the mode argument supplied during calls to the following functions"[^4], which include `open`, `creat` and `mkdir`. A program asks for 0666 for a data file and 0777 for a directory or a program, and the umask of the user makes them narrower. The umask does not change existing files.

**chmod.** `chmod` changes the special bits "and the file permission bits of the file named by the pathname pointed to by the path argument to the corresponding bits in the mode argument"[^3]. Only the owner or a privileged process can do this: "If the effective user ID of the process does not match the owner of the file and the process does not have appropriate privileges, the chmod () function shall fail"[^3].

**Privileged processes.** The root user does not need the bits. POSIX says that for a process with appropriate privileges, "If read, write, or directory search permission is requested, access shall be granted"[^2]. Execute is the exception: it is granted only "if execute permission is granted to at least one user by the file permission bits or by an alternate access control mechanism"[^2].

**Access control lists.** Nine bits cannot give access to one more user. POSIX ACLs "are used to define more fine-grained discretionary access rights for files and directories"[^7]. "An ACL entry specifies the access permissions on the associated object for an individual user or a group of users as a combination of read, write and search/execute permissions"[^7]. A directory can also have a default ACL for the files that are made in it[^7].

**Windows.** Windows does not use mode bits. A file has a security descriptor, and its discretionary ACL "identifies the trustees that are allowed or denied access to a securable object"[^9]. "If the object doesn't have a DACL, the system grants full access to everyone"[^9]. Windows also has the read-only attribute: "Applications can read the file, but cannot write to it or delete it. This attribute is not honored on directories"[^10].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `os.chmod`, `os.umask`, `os.access`, the `stat` constants; `shutil.copymode`; on Windows only the read-only flag |
| JavaScript | stdlib | Node.js `fs.chmod`, `fs.access`, `process.umask`, `fs.constants` |
| TypeScript | stdlib | the `node:fs` module of Node.js, with the types of `@types/node` |
| Java | stdlib | `PosixFilePermission` and `PosixFilePermissions`; `AclFileAttributeView` for Windows ACLs |
| Kotlin | stdlib | `kotlin.io.path` extensions such as `setPosixFilePermissions` on `java.nio.file.Path` |
| C# | stdlib | `UnixFileMode` with `File.SetUnixFileMode`; `FileAttributes.ReadOnly`; `FileStreamOptions.UnixCreateMode` |
| Go | stdlib | `os.Chmod` with `fs.FileMode`; `fs.ModeSetuid`, `fs.ModeSticky`; `syscall.Umask` on Unix |
| Rust | stdlib | `fs::Permissions` with `readonly`; `std::os::unix::fs::PermissionsExt` for the mode bits |
| C | library | ISO C has no permissions; POSIX `chmod`, `fchmod`, `umask`, `access` and the mode argument of `open` |
| C++ | stdlib | `std::filesystem::perms`, `permissions` with `perm_options::replace`, `add` and `remove` |
| Bash | native | the `chmod` and `umask` commands; the tests `-r`, `-w` and `-x` |
| PowerShell | native | `Get-Acl` and `Set-Acl` on Windows; the `IsReadOnly` property of `FileInfo` |
| SQL | n/a | `GRANT` and `REVOKE` work on database objects, not on files |
| Regex | n/a | a pattern matches text and does not read files |
| HTML | n/a | a page cannot read or set the permissions of a local file |
| CSS | n/a | style rules do not read files |

The languages split into two groups. Python, Node.js, C, Go and Rust's `PermissionsExt` use the Unix mode as a number. Java, .NET and C++ give the bits names: Java's enum "Defines the bits for use with the permissions attribute"[^14], .NET's `UnixFileMode` "Represents the Unix filesystem permissions"[^16], and C++'s `perms` "represents file access permissions"[^20]. Go keeps its own layout: "The bits have the same definition on all systems, so that information about files can be moved from one system to another portably"[^18]. Python's `stat.S_IMODE` returns "the file's permission bits, plus the sticky bit, set-group-id, and set-user-id bits"[^12].

The portable forms are small, because Windows has no mode bits. On Windows, Python's `chmod` can "only set the file's read-only flag"[^11], and in Node.js, "on Windows only the write permission can be changed"[^13]. In .NET, "ReadOnly is supported on Windows, Linux, and macOS. On Linux and macOS, changing the ReadOnly flag is a permissions operation"[^17]. For a full Windows ACL, Java has `AclFileAttributeView`, "A file attribute view that supports reading or updating a file's Access Control Lists (ACL) or file owner attributes"[^15].

The example files show the bits in each language: the `stat` constants and `copymode` of Python, the umask and `access` of Node.js, the `PosixFilePermission` set of Java, the `UnixFileMode` flags and `UnixCreateMode` of .NET, Go's own setuid and sticky bits, the `set_readonly` trap of Rust, the `s` and `S` of a setuid bit in C, and the `add` and `remove` options of C++.

## Variations and related ideas

- **Octal and symbolic modes.** `chmod 640` sets all of the bits, and `chmod u+x` or `g-w` changes some bits. A library call that takes a number works like the octal form, so a program that adds one bit must read the mode first. C++ has `perm_options::add` and `perm_options::remove` for the symbolic form.
- **Set-group-ID on a directory.** On Linux, a directory with the set-group-ID bit makes "files created there inherit their group ID from the directory, not from the effective group ID of the creating process"[^5].
- **ACLs on Unix.** POSIX ACLs come from a draft that was not finished: "While the work on 1003.1e has been abandoned, many UNIX style systems implement parts of POSIX.1e draft 17, or of earlier drafts"[^7].
- **Owner and group.** The permissions depend on the owner and the group of the file, which `chown` changes. See [[file-metadata]].

## Pitfalls

- **chmod replaces the whole mode.** `os.chmod(path, stat.S_IXUSR)` does not add one bit. It removes all of the other bits. Read the mode, change the bits, and then write it.
- **The umask applies to the mode of a new file.** A program that gives 0666 to `open` does not get 0666 if the umask is 022. A file for secrets must get the mode 0600 when it is made, not with a later `chmod`, because another process can open it between the two calls.
- **Rust's set_readonly(false) opens the file to all users.** The Rust documentation says: "set_readonly(false) makes the file world-writable on Unix"[^19]. Use `PermissionsExt::set_mode` to change one bit.
- **The bits do not tell you that an operation will work.** Rust warns that `readonly` "does not take Access Control Lists (ACLs), Unix group membership and other nuances into account"[^19], and that it ignores "That root user can write to files that do not have any write bits set"[^19]. A test that a process can write a file, followed by a write, also has a race. Try the operation and handle the error.
- **Root can read a file with the mode 0000.** For a privileged process, read and write access "shall be granted"[^2]. Tests that check a "permission denied" error fail when they run as root, for example in a container.
- **Windows ignores most of the bits.** Code that sets 0700 to keep a file private does nothing on Windows, where only the read-only flag changes[^11]. Use the ACL calls of the system, or keep the file in a directory of the user.

## Cost

A permission check is part of each `open`, so it has no extra cost for the program. The kernel checks the search bit of each directory in the path, and the bits of the file. `chmod` and `umask` are one system call each.

## History

Ritchie and Thompson described the protection of Unix in 1974: "When a file is created, it is marked with the user ID of its owner. Also given for new files is a set of ten protection bits. Nine of these specify independently read, write, and execute permission for the owner of the file, for other members of his group, and for all remaining users"[^8]. With the tenth bit, "the system will temporarily change the user identification (hereafter, user ID) of the current user to that of the creator of the file whenever the file is executed as a program"[^8]. The paper explains why: "The set-user-ID feature provides for privileged programs that may use files inaccessible to other users"[^8]. POSIX has the same nine bits and the set-user-ID bit, and also the set-group-ID bit and the restricted deletion flag[^1]. Many Unix systems have ACLs from the POSIX.1e drafts[^7], and Windows uses ACLs[^9].
