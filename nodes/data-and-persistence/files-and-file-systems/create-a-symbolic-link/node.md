---
tag: AC2QN9
kind: task
slug: create-a-symbolic-link
name: Create a symbolic link
aliases: [make a symlink, create a symlink, ln -s, soft link, readlink]
area: data-and-persistence/files-and-file-systems
level: core
summary: Make a symbolic link that holds a target path, read the target back, and know that a relative target is read from the directory of the link and that a dangling link still exists.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/symlink.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), symlink, symlinkat
    locator: DESCRIPTION, ERRORS
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/readlink.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), readlink, readlinkat
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/ln.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), ln
    locator: DESCRIPTION
  - url: https://man7.org/linux/man-pages/man7/symlink.7.html
    title: Linux manual page, symlink(7)
    locator: DESCRIPTION
  - url: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createsymboliclinkw
    title: Windows API reference, CreateSymbolicLinkW function (winbase.h)
    locator: Parameters (dwFlags), Remarks
  - url: https://docs.python.org/3/library/os.html
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.symlink, os.readlink
  - url: https://docs.python.org/3/library/os.path.html
    title: The Python Standard Library, os.path, Common pathname manipulations
    locator: os.path.exists, os.path.lexists
  - url: https://nodejs.org/api/fs.html#fspromisessymlinktarget-path-type
    title: Node.js documentation, File system
    locator: fsPromises.symlink(target, path[, type])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: createSymbolicLink, readSymbolicLink
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.createsymboliclink?view=net-10.0
    title: .NET API reference, File.CreateSymbolicLink(String, String) Method
    locator: Parameters, Exceptions
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Symlink, func Readlink, type LinkError
  - url: https://doc.rust-lang.org/std/os/unix/fs/fn.symlink.html
    title: The Rust Standard Library, std::os::unix::fs::symlink
    locator: Description
  - url: https://en.cppreference.com/w/cpp/filesystem/create_symlink.html
    title: cppreference, std::filesystem::create_symlink, create_directory_symlink
    locator: Description
---

## Task

Make a symbolic link at the path `link` that holds the string `target`, and return the target that the link holds, read back from the link. The target is stored as it is. It need not exist, and a relative target is read from the directory of the link, not from the current directory. So a link `sub/link` with the target `../a.txt` points to `a.txt`. Report an error when a file, a directory or a link already has the path `link`, and when the directory of `link` does not exist.

POSIX says what the call stores: "The string pointed to by path1 shall be treated only as a string and shall not be validated as a pathname." Linux adds: "There is no requirement that the pathname referred to by a symbolic link should exist." A link to a path that does not exist "is said to be a dangling link". The page [[hard-links-and-symbolic-links]] tells how links work.

SQL, regular expressions, HTML and CSS do not apply, because they do not make files. The solutions are for Linux and macOS. On Windows, a link needs a privilege or Developer Mode, and a link to a directory is a different kind from a link to a file. See the pitfalls.

## Solutions

Each solution makes the link and then reads it back. The order of the two arguments is not the same in each language, so read each call with care.

- **Python.** `os.symlink(target, link)`, and then `os.readlink(link)`. `os.symlink` will "Create a symbolic link pointing to src named dst", so the target comes first, as in `ln -s`. A link that exists gives `FileExistsError`.
- **JavaScript and TypeScript (Node.js).** `await symlink(target, link)` from `node:fs/promises`, and then `await readlink(link)`. The third argument "is only used on Windows platforms and can be one of 'dir' , 'file' , or 'junction'". The error codes are `EEXIST` and `ENOENT`.
- **Java.** `Files.createSymbolicLink(link, Path.of(target))`, and then `Files.readSymbolicLink(link)`. Here the new link comes first. The target "may be an absolute or relative path and may not exist. When the target is a relative path then file system operations on the resulting link are relative to the path of the link." A link that exists gives `FileAlreadyExistsException`.
- **C#.** `File.CreateSymbolicLink(link, target)`, and then the `LinkTarget` of the `FileInfo` that it returns. The new link comes first, and `pathToTarget` is "The path of the target to which the symbolic link points." A link that exists gives `IOException`: "A file or directory already exists in the location of path".
- **Go.** `os.Symlink(target, link)`, and then `os.Readlink(link)`. The target comes first. An error is a `*LinkError`, which "records an error during a link or symlink or rename system call and the paths that caused it." It matches `fs.ErrExist` or `fs.ErrNotExist` with `errors.Is`.
- **Rust.** `std::os::unix::fs::symlink(target, link)`, and then `fs::read_link(link)`. The call "Creates a new symbolic link on the filesystem. The link path will be a symbolic link pointing to the original path." It is in the Unix module, because Windows has two calls, `symlink_file` and `symlink_dir`. The error kinds are `AlreadyExists` and `NotFound`.
- **C.** POSIX `symlink(target, link)`, and then `readlink(link, buf, size)`. ISO C has no call for links. `readlink` gives a count of bytes and does not add a NUL byte: "If the number of bytes in the symbolic link is less than bufsize , the contents of the remainder of buf are unspecified." So the solution adds the NUL byte, and it gives `ERANGE` when the target fills the buffer, because the target can be longer.
- **C++.** `std::filesystem::create_symlink(target, link, ec)`, and then `read_symlink(link, ec)`. It "Creates a symbolic link link with its target set to target as if by POSIX symlink() : the pathname target may be invalid or non-existing."
- **Bash.** `ln -s -- "$target" "$link"`, and then `readlink -- "$link"`. When `link` names a directory, `ln` makes the link inside it. See the pitfalls. So the function first checks that no file has the name, with `[[ -e $link || -L $link ]]`. Another process can make the name between the check and `ln`. The C, Python and other solutions do not have this race, because `symlink` checks and makes the link in one call.

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/errors.jsonl`. Each test runs in a new temporary directory that holds the file `a.txt` and the directory `sub`, which holds `b.txt`. Each test checks the result and that the new link is a symbolic link, with `lstat` or the call of the language.

- `file-target`, `directory-target` and `link-in-subdirectory`: links to `a.txt`, to `sub`, and from `sub/link` to `b.txt`.
- `dangling-target`: the target `missing.txt` does not exist. The link is made, and reading through it gives no such file.
- `relative-target`: `sub/link` with the target `a.txt` points to `sub/a.txt`, which does not exist. The result is still `a.txt`.
- `parent-target`: `sub/link` with the target `../a.txt` points to `a.txt`. The native tests read `alpha` through it.
- `name-with-spaces` and `non-ascii-name`: `my notes.txt` and `café 日本.txt`. A shell solution must quote both names.
- Errors: a file (`a.txt`) and a directory (`sub`) at `link` give exists, and `nodir/link` gives not-found.

The native tests also check that a dangling link at `link` gives exists and stays as it was, that an absolute target is stored as it is, and, in Bash, that a directory at `link` does not get a link inside it.

## Pitfalls

- **ln -s into a directory.** When the last operand of `ln` is a directory, POSIX says: "The second synopsis form shall be assumed when the final operand names an existing directory." In that form, `ln` "shall create a new directory entry for each source_file operand, at a destination path in the existing directory". So `ln -s a.txt sub` gives no error. It makes `sub/a.txt`, and that link points to itself:

```bash wrong id=bash-ln-into-directory
cd "$(mktemp -d)"
printf 'alpha\n' >a.txt
mkdir sub
ln -s a.txt sub
if [ -L sub ]; then echo "sub is a link"; else echo "sub is not a link"; fi
echo "sub/a.txt holds: $(readlink sub/a.txt)"
cat sub/a.txt 2>/dev/null || echo "cat sub/a.txt: fails (a link to itself)"
```

```output
sub is not a link
sub/a.txt holds: a.txt
cat sub/a.txt: fails (a link to itself)
```

- **A dangling link exists.** `os.path.exists` follows a link: it "Returns False for broken symbolic links." So a check with `exists` does not see a dangling link, and `symlink` then fails. `os.path.lexists` "Return True if path refers to an existing path, including broken symbolic links":

```python wrong id=python-exists-misses-dangling-link
import os

os.symlink("old-target.txt", "link")  # a dangling link from an earlier run

if not os.path.exists("link"):
    try:
        os.symlink("a.txt", "link")
    except FileExistsError:
        print("exists said no, but symlink gave FileExistsError")
print("lexists:", os.path.lexists("link"))
```

```output
exists said no, but symlink gave FileExistsError
lexists: True
```

Do not check first. Call `symlink` and handle the error, as the solutions do.

- **A relative target is read from the link.** `os.readlink` gives the string as it is. Python says: "The result may be either an absolute or relative pathname; if it is relative, it may be converted to an absolute pathname using os.path.join(os.path.dirname(path), result)":

```python run id=python-resolve-relative-target
import os

os.makedirs("sub")
open("a.txt", "w").close()
os.symlink("../a.txt", "sub/link")

target = os.readlink("sub/link")
print("readlink:", target)
print("from the current directory:", os.path.exists(target))
full = os.path.join(os.path.dirname("sub/link"), target)
print("from the link:", full, os.path.exists(full))
```

```output
readlink: ../a.txt
from the current directory: False
from the link: sub/../a.txt True
```

- **The order of the arguments.** Python, Node.js, Go, Rust, C, C++ and `ln -s` take the target first. Java and C# take the new link first. With the wrong order in Java, the call tries to make a link with the name of the target:

```java wrong id=java-arguments-in-the-wrong-order
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Main {
    public static void main(String[] args) throws Exception {
        Files.writeString(Path.of("a.txt"), "alpha\n");
        try {
            // The order of ln -s: the target, then the link. Java wants the link first.
            Files.createSymbolicLink(Path.of("a.txt"), Path.of("link"));
        } catch (FileAlreadyExistsException e) {
            System.out.println("FileAlreadyExistsException: " + e.getFile());
        }
    }
}
```

```output
FileAlreadyExistsException: a.txt
```

- **Windows needs a privilege.** Python says: "When Developer Mode is not available/enabled, the SeCreateSymbolicLinkPrivilege privilege is required, or the process must be run as an administrator." The Windows call has the flag `SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE`, and "Developer Mode must first be enabled on the machine before this option will function." A program that makes links must handle this error on Windows.

- **A link to a directory on Windows.** A Windows link is either for a file or for a directory. C++ says: "Portable code should use (3,4) to create directory symlinks rather than (1,2) , even though there is no distinction on POSIX systems." Rust has `symlink_dir`, Node.js has the type `'dir'`, and Python has `target_is_directory`.

## Related tasks

- [[hard-links-and-symbolic-links]]
- [[check-whether-a-file-exists]]
- [[delete-a-file]]
- [[rename-a-file]]
- [[file-system-concept]]

## References
