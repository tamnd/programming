---
tag: 665PF5
kind: task
slug: set-file-permissions
name: Set file permissions
aliases: [chmod, change file mode, change file permissions, set file mode]
area: data-and-persistence/files-and-file-systems
level: core
summary: Set the mode of a file from three or four octal digits, as the chmod command does, and return the new permission bits, with the set-user-ID, set-group-ID and sticky bits.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/chmod.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), chmod, fchmodat
    locator: DESCRIPTION, ERRORS
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/chmod.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), chmod utility
    locator: OPERANDS, APPLICATION USAGE
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), sys/stat.h
    locator: DESCRIPTION, the file mode bits
  - url: https://man7.org/linux/man-pages/man2/chmod.2.html
    title: Linux manual page, chmod(2)
    locator: DESCRIPTION
  - url: https://docs.python.org/3/library/os.html
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.chmod
  - url: https://docs.python.org/3/library/stat.html
    title: The Python Standard Library, stat, Interpreting stat() results
    locator: stat.S_IMODE
  - url: https://nodejs.org/api/fs.html#fschmodpath-mode-callback
    title: Node.js documentation, File system
    locator: fs.chmod(path, mode, callback), File modes
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: setPosixFilePermissions, getAttribute
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/PosixFilePermission.html
    title: Java SE 25 API, PosixFilePermission
    locator: Enum description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.setunixfilemode?view=net-10.0
    title: .NET API reference, File.SetUnixFileMode Method
    locator: SetUnixFileMode(String, UnixFileMode), Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.unixfilemode
    title: .NET API reference, UnixFileMode Enum
    locator: Fields
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Chmod
  - url: https://pkg.go.dev/io/fs#FileMode
    title: Go standard library, package io/fs, type FileMode
    locator: type FileMode
  - url: https://doc.rust-lang.org/std/fs/fn.set_permissions.html
    title: The Rust Standard Library, std::fs::set_permissions
    locator: Platform-specific behavior, Symlinks
  - url: https://doc.rust-lang.org/std/os/unix/fs/trait.PermissionsExt.html
    title: The Rust Standard Library, std::os::unix::fs::PermissionsExt
    locator: from_mode
  - url: https://en.cppreference.com/w/cpp/filesystem/permissions.html
    title: cppreference, std::filesystem::permissions
    locator: Description
---

## Task

Set the mode of the file at `path` to `mode`, and return the new permission bits of the file as an integer. `mode` is a string of three or four octal digits, as for the `chmod` command: `640` is read and write for the owner, read for the group, and nothing for others. The fourth digit, at the left, holds the set-user-ID (4), set-group-ID (2) and sticky (1) bits, so `4755` sets the set-user-ID bit. The result is `st_mode & 07777`, so `640` gives 416, that is `0o640`. Report an error for a mode that is not three or four octal digits, and for a missing file.

An octal mode replaces the whole mode. The POSIX `chmod` utility says: "For an octal integer mode operand, the file mode bits shall be set absolutely. For each bit set in the octal number, the corresponding file permission bit shown in the following table shall be set; all other file permission bits shall be cleared." The values of the bits are in `sys/stat.h`: "S_ISUID 04000 Set-user-ID on execution. S_ISGID 02000 Set-group-ID on execution."

The `chmod` command also takes symbolic modes, such as `u+x`. This task takes only octal digits, and a symbolic mode is an error. The page [[file-permissions]] tells what each bit means.

SQL, regular expressions, HTML and CSS do not apply, because they do not change files. On Windows, a file has no Unix mode. The solutions are for Linux and macOS.

## Solutions

Each solution checks the mode first, then converts it from base 8, sets it, and reads the mode back. The check must match the whole string, so `640` followed by a newline is an error. Each call follows a symbolic link, so a link changes the mode of its target.

- **Python.** `os.chmod(path, int(mode, 8))`. It does: "Change the mode of path to the numeric mode." The code checks the mode with `re.fullmatch`. The result is `stat.S_IMODE(os.stat(path).st_mode)`. `S_IMODE` returns "the portion of the file's mode that can be set by os.chmod()".
- **JavaScript and TypeScript (Node.js).** `await chmod(path, parseInt(mode, 8))` from `node:fs/promises`, and then `(await stat(path)).mode & 0o7777`. The function "Asynchronously changes the permissions of a file." The Node documentation says: "An easier method of constructing the mode is to use a sequence of three octal digits (e.g. 765 )."
- **Java.** `PosixFilePermission` "Defines the bits for use with the permissions attribute", but it has only the nine bits of read, write and execute. It has no set-user-ID bit. So the code uses `Files.setAttribute(path, "unix:mode", Integer.parseInt(mode, 8))`. The `Files` documentation shows the `unix` view only in an example, `Files.getAttribute(path, "unix:uid")`, and it does not list the attributes of the view. The native tests show that `unix:mode` works on Linux and macOS.
- **C#.** `File.SetUnixFileMode(path, (UnixFileMode)Convert.ToInt32(mode, 8))`. It "Sets the specified UnixFileMode of the file on the specified path." The values of `UnixFileMode` are the Unix bits, so the number casts to the enum. The call is not for Windows, so the class has `[UnsupportedOSPlatform("windows")]`. In .NET, the pattern `^[0-7]{3,4}$` matches `640` with a newline after it, so the code checks each character. A missing file gives `FileNotFoundException`: "The file cannot be found."
- **Go.** `os.Chmod(path, m)`. "Chmod changes the mode of the named file to mode. If the file is a symbolic link, it changes the mode of the link's target." A Go `FileMode` does not use the Unix values for the three special bits. See the pitfalls. So the code maps `0o4000`, `0o2000` and `0o1000` to `fs.ModeSetuid`, `fs.ModeSetgid` and `fs.ModeSticky`, and back.
- **Rust.** `fs::set_permissions(path, Permissions::from_mode(m))`. `from_mode` "Creates a new instance from the given mode permission bits." It is in the trait `std::os::unix::fs::PermissionsExt`, which is "Available on Unix only." `set_permissions` "currently corresponds to the chmod function on Unix".
- **C.** POSIX `chmod(path, m)`. ISO C has no call that changes the mode of a file. The code reads each digit itself, because `strtol` also takes a sign and spaces. A missing file gives `ENOENT`: "A component of path does not name an existing file or path is an empty string."
- **C++.** `std::filesystem::permissions(path, perms(m), perm_options::replace, ec)`. It "Changes access permissions of the file to which p resolves, as if by POSIX fchmodat." With `replace`, "file permissions are set to exactly prms & std :: filesystem :: perms :: mask".
- **Bash.** `chmod -- "$mode" "$path"`. The `chmod` command takes symbolic modes too, so the function checks the mode first with `[[ $mode =~ ^[0-7]{3,4}$ ]]`. Then it reads the mode back with `stat -L -c %a` (GNU), or with `stat -L -f %Mp%Lp` (macOS).

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/errors.jsonl`. Each test makes the file in a new temporary directory with the mode `0644`, sets the mode, and checks the result and the mode of the file.

- `owner-read-write-group-read`, `owner-only`, `executable` and `same-mode`: `640`, `600`, `755` and `644` give 416, 384, 493 and 420.
- `no-permissions`: `000` gives 0. The owner can still change the mode again, but cannot read the file. The tests reset the modes before they delete the directory.
- `leading-zero`: `0640` gives 416, the same as `640`.
- `set-user-id`: `4755` gives 2541, that is `0o4755`.
- `name-with-spaces` and `non-ascii-name`: `my notes.txt` and `café 日本.txt`. A shell solution must quote the name.
- Errors: a missing file gives not-found. `648` (8 is not an octal digit), `64` and `10640` (two and five digits), `u+x` (a symbolic mode), the empty string, and `640` with a newline give invalid-mode.

The native tests also check that a symbolic link changes the mode of its target, that a directory takes the mode `700`, that an invalid mode does not change the file, and that the umask does not change the result: with the umask `077`, the mode `666` gives `0o666`.

## Pitfalls

- **A decimal number is not a mode.** In Python, C and Go, a mode is an integer, and `400` without the `0o` or `0` prefix is a decimal number. Decimal 400 is `0o620`, so the owner cannot read the file, and the group can write to it:

```python wrong id=python-chmod-decimal
import os
import stat
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "a.txt")
    open(path, "w").close()
    os.chmod(path, 400)  # decimal, not 0o400
    print(oct(stat.S_IMODE(os.stat(path).st_mode)))
```

```output
0o620
```

- **A mode replaces all the bits.** To add one bit, read the mode first. `os.chmod(path, stat.S_IXUSR)` does not add the execute bit for the owner. It removes all the other bits:

```python wrong id=python-chmod-replaces
import os
import stat
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "run.sh")
    open(path, "w").close()
    os.chmod(path, 0o644)
    os.chmod(path, stat.S_IXUSR)
    print(oct(stat.S_IMODE(os.stat(path).st_mode)))
```

```output
0o100
```

Use the bitwise OR of the old mode and the new bit:

```python run id=python-chmod-add-bit
import os
import stat
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "run.sh")
    open(path, "w").close()
    os.chmod(path, 0o644)
    old = stat.S_IMODE(os.stat(path).st_mode)
    os.chmod(path, old | stat.S_IXUSR)
    print(oct(stat.S_IMODE(os.stat(path).st_mode)))
```

```output
0o744
```

- **The special bits of Go are not the Unix bits.** Go says: "A FileMode represents a file's mode and permission bits. The bits have the same definition on all systems". So `ModeSetuid` is a high bit of the `uint32`, not `0o4000`, and `os.Chmod` drops `0o4000` with no error:

```go wrong id=go-chmod-unix-setuid
package main

import (
	"fmt"
	"os"
	"path/filepath"
	"syscall"
)

func main() {
	dir, err := os.MkdirTemp("", "chmod")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "tool")
	if err := os.WriteFile(path, []byte("#!/bin/sh\n"), 0o644); err != nil {
		panic(err)
	}
	if err := os.Chmod(path, 0o4755); err != nil { // the Unix value of set-user-ID
		panic(err)
	}
	var st syscall.Stat_t
	if err := syscall.Stat(path, &st); err != nil {
		panic(err)
	}
	fmt.Printf("asked for 4755, got %o\n", st.Mode&0o7777)
}
```

```output
asked for 4755, got 755
```

Use `0o755 | fs.ModeSetuid`, as the Go solution does.

- **The system can ignore the special bits.** POSIX says that "Additional implementation-defined restrictions may cause the S_ISUID and S_ISGID bits in mode to be ignored". For a file whose group is not a group of the process, it clears the set-group-ID bit: "bit S_ISGID (set-group-ID on execution) in the file's mode shall be cleared upon successful return from chmod ()." Linux adds: "the S_ISGID bit will be turned off, but this will not cause an error to be returned." So read the mode back, as each solution does, and do not assume that the call set all the bits.

- **A symbolic link changes its target.** Each call in the solutions follows a link. Rust warns: "Note that this behavior can lead to privilege escalation vulnerabilities". A program that changes the mode of files in a directory that other users can write to must not follow links.

- **The umask does not apply to an octal mode.** The umask clears bits when a program makes a file, but an octal mode sets the bits "absolutely". The `chmod` utility uses the umask only for a symbolic mode with no `who` part: systems "do so when who is not specified."

## Related tasks

- [[file-permissions]]
- [[file-metadata]]
- [[get-the-modification-time-of-a-file]]
- [[check-whether-a-file-exists]]
- [[create-a-temporary-file]]

## References
