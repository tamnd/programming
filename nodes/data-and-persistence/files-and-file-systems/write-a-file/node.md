---
tag: NEW-write-a-file
kind: task
slug: write-a-file
name: Write a file
aliases: [write a string to a file, save text to a file, append to a file, replace a file atomically, overwrite a file]
area: data-and-persistence/files-and-file-systems
level: core
summary: Write a string to a file as UTF-8 with no change to its bytes, in one of three modes, truncate, append, or atomic replacement through a temporary file in the same directory.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
    title: The Open Group, POSIX.1-2024, rename
    locator: Description, Errors (EISDIR, EXDEV)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/mkstemp.html
    title: The Open Group, POSIX.1-2024, mkdtemp, mkstemp
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fsync.html
    title: The Open Group, POSIX.1-2024, fsync
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/mv.html
    title: The Open Group, POSIX.1-2024, mv
    locator: Description, second synopsis form
  - url: https://man7.org/linux/man-pages/man2/fsync.2.html
    title: Linux man-pages, fsync(2)
    locator: Description
  - url: https://man7.org/linux/man-pages/man2/close.2.html
    title: Linux man-pages, close(2)
    locator: Caveats, Dealing with error returns from close()
  - url: https://docs.python.org/3/library/functions.html#open
    title: The Python Standard Library, Built-in Functions
    locator: open(), mode, encoding, newline
  - url: https://docs.python.org/3/library/tempfile.html#tempfile.mkstemp
    title: The Python Standard Library, tempfile
    locator: mkstemp
  - url: https://docs.python.org/3/library/os.html#os.replace
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.replace, os.fsync
  - url: https://nodejs.org/api/fs.html#fspromiseswritefilefile-data-options
    title: Node.js documentation, File system
    locator: fsPromises.writeFile, filehandle.sync, fsPromises.rename, File system flags
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
    title: Java SE 25 API, java.nio.file.Files
    locator: writeString(Path, CharSequence, Charset, OpenOption...), move(Path, Path, CopyOption...), createTempFile(Path, String, String, FileAttribute...)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html#force(boolean)
    title: Java SE 25 API, java.nio.channels.FileChannel
    locator: force(boolean)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.writealltext?view=net-10.0
    title: .NET API reference, File.WriteAllText
    locator: WriteAllText(String, String), Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.encoding.utf8?view=net-10.0
    title: .NET API reference, Encoding.UTF8
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.move?view=net-10.0
    title: .NET API reference, File.Move
    locator: Move(String, String, Boolean), Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestream.flush?view=net-10.0
    title: .NET API reference, FileStream.Flush
    locator: Flush(Boolean), Remarks
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: WriteFile, OpenFile, CreateTemp, File.Sync, Rename
  - url: https://doc.rust-lang.org/std/fs/fn.write.html
    title: The Rust Standard Library, std::fs::write
  - url: https://doc.rust-lang.org/std/fs/struct.OpenOptions.html
    title: The Rust Standard Library, std::fs::OpenOptions
    locator: append, create, create_new
  - url: https://doc.rust-lang.org/std/fs/struct.File.html#method.sync_all
    title: The Rust Standard Library, std::fs::File
    locator: sync_all
  - url: https://doc.rust-lang.org/std/fs/fn.rename.html
    title: The Rust Standard Library, std::fs::rename
    locator: Platform-specific behavior, Errors
  - url: https://en.cppreference.com/w/c/io/fopen.html
    title: cppreference.com, fopen
    locator: File access flags
  - url: https://en.cppreference.com/w/cpp/filesystem/rename.html
    title: cppreference.com, std::filesystem::rename
  - url: https://www.gnu.org/software/bash/manual/bash.html#Bash-Builtins
    title: GNU Bash Reference Manual, Bash Builtin Commands
    locator: 4.2, echo, printf
  - url: https://www.gnu.org/software/bash/manual/bash.html#Redirecting-Output
    title: GNU Bash Reference Manual, Redirections
    locator: 3.6.2 Redirecting Output, 3.6.3 Appending Redirected Output
---

## Task

Write a string to a file. The file holds the UTF-8 bytes of the string and nothing else. There is no byte order mark, and each `\n`, `\r\n` and `\r` stays as it is.

Each solution is a function `write_text(path, text, mode)`. The mode is one of three:

- `truncate`: make the file if it does not exist, or cut an existing file to length zero, then write the text.
- `append`: make the file if it does not exist, then add the text at its end.
- `atomic`: replace the file as one step. Write the text to a new temporary file in the same directory, flush it to the disk, close it, and rename it over the path. On a failure, remove the temporary file.

The atomic mode uses POSIX `rename`. While the rename runs, a directory entry with the new name stays visible. It refers to the old file or to the new file. So a reader sees the whole old content or the whole new content, and never a part. A rename works only inside one file system, and POSIX gives the error `EXDEV` for two file systems. For this reason the temporary file goes in the same directory as the target, not in `/tmp`.

Two more steps make the new content survive a power failure. `fsync` on the file sends its data to the storage device before the rename. The Linux `fsync(2)` page adds that this does not put the directory entry on the disk. For that, an `fsync` on the directory is also needed. The solutions on this page do not sync the directory.

Check the result of `close` too. The Linux `close(2)` page says that the error of an earlier `write` can come only at the final `close`.

## Solutions

- **Python.** `open(path, "w")` truncates and `open(path, "a")` appends. The solution gives `encoding="utf-8"`, because the default encoding comes from the locale when UTF-8 Mode is off. It also gives `newline=""`. With the default `newline=None`, Python changes each `\n` to `os.linesep` on output. The atomic mode uses `tempfile.mkstemp(dir=...)`, `f.flush()`, `os.fsync` and `os.replace`. `os.replace` replaces an existing file, and the documentation says that a successful rename is atomic.
- **JavaScript and TypeScript (Node.js).** `fsPromises.writeFile` with the flag `"w"` (the default) or `"a"`. The atomic mode opens the temporary file with `"wx"`, which fails if the path exists. Then it calls `filehandle.writeFile`, `filehandle.sync` and `fsPromises.rename`. The TypeScript file adds types only.
- **Java.** `Files.writeString(path, text, UTF_8)`. With no options, it works as if `CREATE`, `TRUNCATE_EXISTING` and `WRITE` are given. Append mode gives `CREATE` and `APPEND`. The atomic mode uses `Files.createTempFile(dir, ...)`, a `FileChannel` with `force(true)`, and `Files.move` with `ATOMIC_MOVE`. The documentation says that, when the target exists, it is specific to the implementation whether `ATOMIC_MOVE` replaces it or throws an `IOException`. On macOS, where the tests ran, it replaces the file.
- **C#.** `File.WriteAllText` and `File.AppendAllText` with `new UTF8Encoding(false, true)`. The `false` turns off the byte order mark, and the `true` makes invalid text throw an exception. The atomic mode opens the temporary file with `FileMode.CreateNew`, then calls `Flush(flushToDisk: true)` and `File.Move(tmp, path, overwrite: true)`. The `File.Move` documentation says that a move to another volume is a copy and a delete. So the temporary file must be on the volume of the target.
- **Go.** `os.WriteFile(path, data, 0o666)` truncates. Append mode uses `os.OpenFile` with `O_WRONLY|O_APPEND|O_CREATE`, and it returns the error of `Close` too. The atomic mode uses `os.CreateTemp(dir, pattern)`, `File.Sync`, `Close` and `os.Rename`.
- **Rust.** `fs::write(path, text)` creates or replaces the file. Append mode uses `OpenOptions::new().append(true).create(true)`. The atomic mode makes the temporary file with `create_new(true)`, which fails with `AlreadyExists` if the name exists. Then it calls `write_all`, `sync_all` and `fs::rename`. The `sync_all` documentation says that a drop of a `File` ignores all errors, and `sync_all` reports them.
- **C.** `fopen(path, "wb")` or `fopen(path, "ab")`, then `fwrite` and `fclose`. The `b` has no effect on POSIX systems. On Windows it turns off the special handling of `'\n'`. `fclose` flushes the buffered data to the operating system, so the solution checks its result. The atomic mode uses POSIX `mkstemp`, a loop of `write`, `fsync`, `close` and `rename`.
- **C++.** `std::ofstream` with `std::ios::binary` and `trunc` or `app`, and with exceptions on `failbit` and `badbit`. The atomic mode uses POSIX `mkstemp`, `write` and `fsync`, then `std::filesystem::rename`, which works as if by POSIX `rename`.
- **Bash.** `printf '%s' "$text" >"$path"` truncates, and `>>` appends. `printf '%s'` writes the bytes of the argument with no change. The atomic mode makes the temporary file with `mktemp "$path.XXXXXX"` and renames it with `mv -f`. It first refuses a path that is a directory (see the pitfalls). The Bash builtins include no command that syncs a file, so this mode has no `fsync`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the text, the mode, and the content of the file before the call (`null` when there is no file). The expected value is the content after the call. Each test also checks that the directory holds only `out.txt`, so no temporary file stays behind.

- `new-file`, `append-missing` and `atomic-new`: no file before the call. Each mode makes the file.
- `truncate-longer`: the old content is longer than the new text. No part of the old content stays.
- `empty-truncate`, `empty-new` and `atomic-empty`: an empty text gives a file of length zero, and the file exists.
- `append-existing` and `append-no-newline`: append mode adds no newline. `one` and `two` give `onetwo`.
- `non-ascii` and `atomic-non-ascii`: characters of 2, 3 and 4 bytes in UTF-8.
- `crlf` and `mixed-endings`: `\r\n` and `\r` stay as they are.
- `backslashes` and `format-chars`: the text `C:\temp\new` and the text `-n 100% %s`. These catch a Bash solution that uses `echo`, or a `printf` with the text as the format.

The cases in `cases/errors.jsonl` are failures. The toolchain does not load this file yet, so the test files hold these cases by name:

- `missing-dir-truncate`, `missing-dir-append` and `missing-dir-atomic`: the parent directory does not exist. Each mode fails with a "not found" error and makes no file.
- `atomic-onto-directory`: the path names an empty directory. POSIX `rename` fails with `EISDIR` when the new name is a directory and the old name is not. The directory stays, and no temporary file stays.

## Pitfalls

- **Truncate mode empties the file first.** If the program fails after the open and before the write, the old content is lost. Here the conversion fails after `open` has cut the file:

```python wrong id=python-truncate-first
from pathlib import Path

path = Path("config.txt")
path.write_text("port = 80\n", encoding="utf-8")
try:
    with open(path, "w", encoding="utf-8") as f:
        f.write("port = %d\n" % int("eighty"))
except ValueError as e:
    print("error:", e)
print(repr(path.read_text(encoding="utf-8")))
```

```output for=python-truncate-first
error: invalid literal for int() with base 10: 'eighty'
''
```

Make the new content before the open, or use the atomic mode.

- **`echo` changes the text in Bash.** `echo` adds a newline at the end. It reads a first argument `-n` as an option, and `echo` does not take `--` as the end of the options. `printf '%s'` writes the argument as it is:

```bash wrong id=bash-echo
for text in 'line' '-n'; do
  echo "$text" >echo.txt
  printf '%s' "$text" >printf.txt
  IFS= read -r -d '' a <echo.txt
  IFS= read -r -d '' b <printf.txt
  printf '%s: echo wrote %d bytes, printf wrote %d bytes\n' "$text" "${#a}" "${#b}"
done
```

```output for=bash-echo
line: echo wrote 5 bytes, printf wrote 4 bytes
-n: echo wrote 0 bytes, printf wrote 2 bytes
```

Do not give the text as the format of `printf` either. In a format, `%` starts a format specification and `\` starts an escape sequence.

- **`mv` onto a directory moves the file into it.** POSIX `mv` has a second form: when the last operand is an existing directory, `mv` moves the file into that directory. So the target does not change, and the command reports no error:

```bash wrong id=bash-mv-dir
mkdir out.txt
printf 'new\n' >out.txt.tmp
mv -f out.txt.tmp out.txt
echo "status: $?"
ls out.txt
```

```output for=bash-mv-dir
status: 0
out.txt.tmp
```

The Bash solution tests `[ -d "$path" ]` before the move.

- **The atomic mode can change the permissions.** POSIX `mkstemp` makes the file with read and write permission for the owner only. Python `tempfile.mkstemp` and Go `os.CreateTemp` do the same. After the rename, the file has the mode of the temporary file, not the old mode:

```python wrong id=python-mode
import os
import stat
import tempfile

os.umask(0o022)
with open("config.txt", "w", encoding="utf-8") as f:
    f.write("old\n")
print("before:", oct(stat.S_IMODE(os.stat("config.txt").st_mode)))
fd, tmp = tempfile.mkstemp(dir=".")
with open(fd, "w", encoding="utf-8") as f:
    f.write("new\n")
os.replace(tmp, "config.txt")
print("after:", oct(stat.S_IMODE(os.stat("config.txt").st_mode)))
```

```output for=python-mode
before: 0o644
after: 0o600
```

If the mode matters, read it from the old file and set it on the temporary file before the rename, for example with `os.chmod`. The solutions on this page do not copy the mode, and the cases do not check it.

- **`Encoding.UTF8` writes a byte order mark in C#.** `File.WriteAllText(path, text)` writes UTF-8 with no byte order mark. With the argument `Encoding.UTF8`, the file starts with the bytes `EF BB BF`. `new UTF8Encoding(false)` gives an encoding with no mark:

```csharp wrong id=csharp-bom
using System.Text;

File.WriteAllText("a.txt", "hi", Encoding.UTF8);
File.WriteAllText("b.txt", "hi", new UTF8Encoding(false));
Console.WriteLine(Convert.ToHexString(File.ReadAllBytes("a.txt")));
Console.WriteLine(Convert.ToHexString(File.ReadAllBytes("b.txt")));
```

```output for=csharp-bom
EFBBBF6869
6869
```

- **`APPEND` alone does not make the file in Java.** When `Files.writeString` gets options, it uses only those options. `APPEND` without `CREATE` fails when the file does not exist:

```java run id=java-append
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public class Main {
    public static void main(String[] args) throws IOException {
        Path path = Path.of("log.txt");
        try {
            Files.writeString(path, "first\n", StandardOpenOption.APPEND);
        } catch (NoSuchFileException e) {
            System.out.println("APPEND: " + e.getClass().getSimpleName());
        }
        Files.writeString(path, "first\n", StandardOpenOption.CREATE, StandardOpenOption.APPEND);
        System.out.print("CREATE, APPEND: " + Files.readString(path));
    }
}
```

```output for=java-append
APPEND: NoSuchFileException
CREATE, APPEND: first
```

## Related tasks

- [[read-a-file-line-by-line]]: read the file back.
- [[read-from-standard-input]]: get the text to write from the standard input.
- [[read-an-environment-variable]]: get the path of the output file from the environment.
- [[parse-command-line-arguments]]: get the path of the output file from the command line.
- [[run-a-subprocess]]: a child process can write to a file through a redirection of its output.
- [[os-command-injection]]: do not build a shell command such as `echo ... > file` from text that a user controls.

## References
