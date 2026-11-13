---
tag: GT2DXC
kind: task
slug: create-a-temporary-file
name: Create a temporary file
aliases: [temporary file, temp file, tmp file, mkstemp, mktemp, tmpfile, secure temporary file]
area: data-and-persistence/files-and-file-systems
level: core
summary: Make a new temporary file with a random name and the mode 0600 in one call, so that no other process can put a file or a link at the name first, then use it and remove it.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/mkdtemp.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), mkdtemp, mkostemp, mkstemp
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/tmpfile.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), tmpfile
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/tmpnam.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), tmpnam
    locator: APPLICATION USAGE
  - url: https://man7.org/linux/man-pages/man3/mkstemp.3.html
    title: Linux manual page, mkstemp(3)
    locator: DESCRIPTION
  - url: https://man.freebsd.org/cgi/man.cgi?query=mktemp&sektion=1
    title: FreeBSD manual page, mktemp(1)
    locator: DESCRIPTION
  - url: https://docs.python.org/3/library/tempfile.html
    title: The Python Standard Library, tempfile, Generate temporary files and directories
    locator: tempfile.mkstemp, tempfile.TemporaryFile, tempfile.mktemp, tempfile.tempdir
  - url: https://nodejs.org/api/fs.html#fspromisesmkdtempprefix-options
    title: Node.js documentation, File system
    locator: fsPromises.mkdtemp, File system flags
  - url: https://nodejs.org/api/os.html#ostmpdir
    title: Node.js documentation, OS
    locator: os.tmpdir()
  - url: https://nodejs.org/api/crypto.html#cryptorandombytessize-callback
    title: Node.js documentation, Crypto
    locator: crypto.randomBytes(size[, callback])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, Files
    locator: createTempFile(Path, String, String, FileAttribute...)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.path.gettempfilename?view=net-10.0
    title: .NET API reference, Path.GetTempFileName Method
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestreamoptions.unixcreatemode?view=net-10.0
    title: .NET API reference, FileStreamOptions.UnixCreateMode Property
    locator: Definition
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.security.cryptography.randomnumbergenerator.getstring?view=net-10.0
    title: .NET API reference, RandomNumberGenerator.GetString Method
    locator: Definition
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func CreateTemp, func MkdirTemp, func TempDir
  - url: https://doc.rust-lang.org/std/env/fn.temp_dir.html
    title: The Rust Standard Library, std::env::temp_dir
    locator: Description
  - url: https://doc.rust-lang.org/std/fs/struct.OpenOptions.html
    title: The Rust Standard Library, std::fs::OpenOptions
    locator: create_new
  - url: https://doc.rust-lang.org/std/os/unix/fs/trait.OpenOptionsExt.html
    title: The Rust Standard Library, std::os::unix::fs::OpenOptionsExt
    locator: mode
  - url: https://doc.rust-lang.org/std/hash/struct.RandomState.html
    title: The Rust Standard Library, std::hash::RandomState
    locator: Description, new
  - url: https://en.cppreference.com/w/cpp/filesystem/temp_directory_path.html
    title: cppreference, std::filesystem::temp_directory_path
    locator: Description, Return value
---

## Task

Make a new, empty file with a random name in a directory, use it, and remove it. The task has two functions:

- `make_temp_file(dir, prefix)` makes a new regular file in `dir`. Its name is `prefix` and then at least six random characters. Its mode is 0600: only the owner can read and write it. The function returns the path.
- `use_temp_file(dir, prefix, data)` makes a file with `make_temp_file`, writes `data` to it, reads it back from the start, removes the file and returns the text. The name is random, so the cases test this function.

The important rule: make the file and check the name in one call. A temporary directory such as `/tmp` is shared by all users. When a program first picks a free name and then opens it, another process can put a file or a symbolic link at that name between the two steps. POSIX says that `mkstemp` acts "as if the O_EXCL flag for open () is set", and so "the function prevents any possible race condition between testing whether the file exists and opening it for use."

Report an error when `dir` does not exist, and when `prefix` has a slash, because then the file would go in another directory. SQL, regular expressions, HTML and CSS do not apply, because they do not make files. The solutions are for Linux and macOS.

## Solutions

- **Python.** `tempfile.mkstemp(prefix=prefix, dir=dir)` returns an open descriptor and the path. It "Creates a temporary file in the most secure manner possible. There are no race conditions in the file’s creation, assuming that the platform properly implements the os.O_EXCL flag for os.open() . The file is readable and writable only by the creating user ID." The caller closes the descriptor and removes the file. When the program does not need the name, `tempfile.TemporaryFile()` is simpler: "It will be destroyed as soon as it is closed".
- **JavaScript and TypeScript (Node.js).** Node.js has `mkdtemp` for a directory, but no call that makes a temporary file. So the solution makes the name with `crypto.randomBytes(6)`, which "Generates cryptographically strong pseudorandom data", and opens it with `open(path, "wx", 0o600)`. The flag `'wx'` is "Like 'w' but fails if the path exists". When the name is taken, it tries a new name.
- **Java.** `Files.createTempFile(dir, prefix, "", attrs)`. Without file attributes, the mode is not given: "the resulting file may have more restrictive access permissions than files created by the File.createTempFile(String,String,File) method." So the solution gives `PosixFilePermissions.asFileAttribute(PosixFilePermissions.fromString("rw-------"))`. The suffix is `""`, because the documentation says that for a `null` suffix, `.tmp` is used.
- **C#.** `Path.GetTempFileName()` "creates a temporary file with a .tmp file extension", and only in the folder that `GetTempPath` gives. It takes no directory and no prefix. So the solution makes the name with `RandomNumberGenerator.GetString`, which "Creates a string populated with characters chosen at random from choices", and opens it with `FileMode.CreateNew` and `FileStreamOptions.UnixCreateMode`, which "Gets or sets the Unix file mode used when a new file is created." When the name is taken, it tries a new name.
- **Go.** `os.CreateTemp(dir, prefix+"*")`. It "creates a new temporary file in the directory dir, opens the file for reading and writing, and returns the resulting file." Also: "The file is created with mode 0o600 (before umask)." The `*` at the end is important. See the pitfalls.
- **Rust.** The standard library has `std::env::temp_dir`, but no call that makes a temporary file. Its documentation says: "Consider using a crate that securely creates temporary files or directories." The solution uses only the standard library. It opens the name with `OpenOptions::create_new`, which fails when the file exists, "also no (dangling) symlink. In this way, if the call succeeds, the file returned is guaranteed to be new." It sets the mode with `OpenOptionsExt::mode(0o600)`. The random part of the name is a hash from `RandomState::new()`, which "Constructs a new RandomState that is initialized with random keys". This hash is not a source of secure random numbers, but `create_new` makes the solution safe: the worst case is a name that is taken, and then the solution tries again.
- **C.** POSIX `mkstemp` with the template `dir/prefixXXXXXX`. "The last six characters of template must be "XXXXXX" and these are replaced with a string that makes the filename unique." The template is changed in place, so it must be an array, not a string constant. The descriptor is open for reading and writing, so `use_temp_file` writes and reads through it, and does not open the name again.
- **C++.** `std::filesystem::temp_directory_path()` gives "A directory suitable for temporary files", but the library has no call that makes a file. So the solution calls POSIX `mkstemp`, as in C, and returns a `std::filesystem::path`.
- **Bash.** `mktemp "$dir/${prefix}XXXXXX"` makes the file and prints its name. The FreeBSD manual says: "If mktemp can successfully generate a unique file name, the file is created with mode 0600 (unless the -u flag is given) and the filename is printed to standard output." The script removes the file with `rm -f` on each path.

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/errors.jsonl`. Each test runs in a new temporary directory that holds only the empty directory `sub`.

- `one-line`, `many-lines` and `in-subdirectory`: the data comes back, and after the call, the directory holds only `sub` again.
- `empty-prefix`: the name is only random characters.
- `empty-data`: the file stays empty, and the result is the empty string.
- `non-ascii-data`, `prefix-with-spaces` and `non-ascii-prefix`: `café 日本`, `my temp ` and `café-`. A shell solution must quote the name.
- Errors: the directory `nodir` gives not-found, and the prefix `sub/tmp-` gives invalid-prefix, and no file is made in `sub`.

The native tests also check `make_temp_file`: the name starts with the prefix and has at least six more characters, the file is a regular file and not a link, its permission bits are 0600, it is empty, and two calls give two different files.

A prefix that ends with `X` is not in the cases. POSIX says that `mkstemp` replaces "six or more 'X' characters at the end of the pathname", and the Node.js documentation says: "Some platforms, notably the BSDs, can return more than six random characters, and replace trailing X characters in prefix with random characters." So with `mkstemp` and `mktemp`, a prefix such as `tmpX` can lose its last `X`.

## Pitfalls

- **A free name is not a file.** `tempfile.mktemp`, `tmpnam` and the like give a name that is free now. POSIX says of `tmpnam`: "Between the time a pathname is created and the file is opened, it is possible for some other process to create a file with the same name." Here, another user puts a link at the name first, and the program writes through the link into a file of the user:

```python wrong id=python-mktemp-race
import os
import tempfile

with open("victim.txt", "w") as f:
    f.write("important data\n")

name = tempfile.mktemp(dir=".")
# Another user makes a link at the free name before the program opens it.
os.symlink("victim.txt", name)

with open(name, "w") as f:
    f.write("temporary data\n")
with open("victim.txt") as f:
    print("victim.txt holds:", f.read(), end="")
```

```output
victim.txt holds: temporary data
```

Python says that `mktemp` "is not secure, because a different process may create a file with this name in the time between the call to mktemp() and the subsequent attempt to create the file by the first process. The solution is to combine the two steps and create the file immediately." With `mkstemp`, the file exists when the call returns, so no link can be put at its name:

```python run id=python-mkstemp-owns-the-name
import os
import stat
import tempfile

fd, path = tempfile.mkstemp(prefix="tmp-", dir=".")
print("mode:", oct(stat.S_IMODE(os.fstat(fd).st_mode)))
try:
    os.symlink("victim.txt", path)
except FileExistsError:
    print("the name is taken, so no link can go there")
os.close(fd)
os.remove(path)
```

```output
mode: 0o600
the name is taken, so no link can go there
```

- **A `*` in a Go prefix.** In the pattern of `os.CreateTemp`, "If pattern includes a "*", the random string replaces the last "*"." So a prefix that has a `*` loses it, and the random part goes in the middle of the name. Add a `*` at the end of the prefix:

```go wrong id=go-star-in-the-prefix
package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

func main() {
	prefix := "data*backup-"
	f, err := os.CreateTemp(".", prefix)
	if err != nil {
		panic(err)
	}
	defer os.Remove(f.Name())
	defer f.Close()
	fmt.Println("with the pattern prefix:", strings.HasPrefix(filepath.Base(f.Name()), prefix))

	g, err := os.CreateTemp(".", prefix+"*")
	if err != nil {
		panic(err)
	}
	defer os.Remove(g.Name())
	defer g.Close()
	fmt.Println("with the pattern prefix+\"*\":", strings.HasPrefix(filepath.Base(g.Name()), prefix))
}
```

```output
with the pattern prefix: false
with the pattern prefix+"*": true
```

- **The file is not removed for you.** Only some calls remove the file: Python `TemporaryFile`, and POSIX `tmpfile`, whose file "shall be automatically deleted when all references to the file are closed". With `mkstemp`, Python says: "the user of mkstemp() is responsible for closing the file descriptor (for example, using os.close() ) and deleting the temporary file". Go says: "It is the caller's responsibility to remove the file when it is no longer needed." Remove the file in a `finally` block, a `defer` or a destructor, so that an error does not leave it in the directory.

- **The mode of the file.** A file made with a plain `open` gets the mode 0666, less the umask. Rust says: "If no mode is set, the default of 0o666 will be used. The operating system masks out bits with the system’s umask , to produce the final permissions." With a umask of 022, the mode is 0644, and all users can read the file. Ask for 0600 in the same call that makes the file, as all the solutions do. A `chmod` after the file is made leaves a short time when other users can open it.

- **Where the temporary directory is.** Each language reads environment variables. Go says that `TempDir` "returns $TMPDIR if non-empty, else /tmp" on Unix, and "The directory is neither guaranteed to exist nor have accessible permissions." Python looks at `TMPDIR`, `TEMP` and `TMP`, and then at a list of directories. Rust says: "Note that the returned value may be a symbolic link, not a directory." So a program that makes many files, or large files, can take a directory as an argument, as the solutions do.

## Related tasks

- [[write-a-file]]
- [[write-a-file-atomically]]
- [[delete-a-file]]
- [[file-permissions]]
- [[hard-links-and-symbolic-links]]

## References
