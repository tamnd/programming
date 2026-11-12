---
tag: WZE7MN
kind: task
slug: append-to-a-file
name: Append to a file
aliases: [append a line to a file, add text to the end of a file, append to a log file, O_APPEND]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Add one line to the end of a file in append mode, with one write call, so that many threads or processes can add lines to the same file and no line overwrites another.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/write.html
    title: The Open Group, POSIX.1-2024, write
    locator: Description, the paragraph on O_APPEND
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/open.html
    title: The Open Group, POSIX.1-2024, open
    locator: O_APPEND, O_CREAT
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html
    title: The Open Group, POSIX.1-2024, System Interfaces, General Information
    locator: 2.9.7 Thread Interactions with File Operations
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fopen.html
    title: The Open Group, POSIX.1-2024, fopen
    locator: the mode a
  - url: https://man7.org/linux/man-pages/man2/open.2.html
    title: Linux manual page, open(2)
    locator: O_APPEND
  - url: https://docs.python.org/3/library/functions.html#open
    title: The Python Standard Library, Built-in Functions, open()
    locator: the mode 'a'
  - url: https://docs.python.org/3/library/os.html#os.open
    title: The Python Standard Library, os
    locator: os.open, os.write, os.O_APPEND
  - url: https://nodejs.org/api/fs.html#file-system-flags
    title: Node.js documentation, File system
    locator: File system flags, 'a'; filehandle.write
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/StandardOpenOption.html
    title: Java SE 25 API, java.nio.file.StandardOpenOption
    locator: APPEND
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html
    title: Java SE 25 API, java.nio.channels.FileChannel
    locator: the paragraph on append mode
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#writeString(java.nio.file.Path,java.lang.CharSequence,java.nio.file.OpenOption...)
    title: Java SE 25 API, java.nio.file.Files
    locator: write, writeString
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filemode?view=net-10.0
    title: .NET API reference, FileMode
    locator: Append
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.fileshare?view=net-10.0
    title: .NET API reference, FileShare
    locator: None
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.appendalltext?view=net-10.0
    title: .NET API reference, File.AppendAllText
  - url: https://github.com/dotnet/runtime/issues/53432
    title: dotnet/runtime issue 53432, Allow for atomic appends to end of file (2021)
  - url: https://github.com/dotnet/runtime/issues/76408
    title: dotnet/runtime issue 76408, Allow for atomic appends to end of file, Round 2 (2022)
  - url: https://pkg.go.dev/os#OpenFile
    title: Go standard library, package os
    locator: OpenFile, O_APPEND, O_CREATE
  - url: https://doc.rust-lang.org/std/fs/struct.OpenOptions.html#method.append
    title: The Rust Standard Library, std::fs::OpenOptions
    locator: append
  - url: https://en.cppreference.com/w/cpp/io/ios_base/openmode
    title: cppreference.com, std::ios_base::openmode
    locator: app
  - url: https://www.gnu.org/software/bash/manual/bash.html#Appending-Redirected-Output
    title: GNU Bash Reference Manual, Redirections
    locator: Appending Redirected Output
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.management/add-content?view=powershell-7.6
    title: PowerShell 7.6 reference, Add-Content
---

## Task

Add one line to the end of a file. If the file does not exist, make it.

The solution writes the UTF-8 bytes of the line and one `\n` after the old content, with nothing between them. It does not read the file, and it does not change the old content. If the old content does not end with `\n`, the new line joins the last old line.

Many writers can use the same file at the same time: threads of one program, or many programs that write to one log. So the solution opens the file in append mode, and it sends the line and its `\n` in one write call. In append mode, POSIX moves the file offset to the end of the file before each write, and "no intervening file modification operation shall occur between changing the file offset and the write operation". A line from another writer then cannot go over the line.

If the parent directory does not exist, or if the line holds `\n` or `\r`, the solution reports an error and writes nothing.

To replace all of a file, or to write it atomically, see [[write-a-file]]. SQL, regular expressions, HTML and CSS do not apply, because none of them can open a file.

## Solutions

Each solution opens the file in append mode with "create" on, makes one buffer that holds the line and `\n`, writes it once and closes the file.

- **Python.** `os.open(path, os.O_WRONLY | os.O_APPEND | os.O_CREAT, 0o666)` and one `os.write`. `open(path, "a")` also appends, but it writes through a buffer, so the code does not control the number of write calls.
- **JavaScript and TypeScript (Node.js).** `fsPromises.open(path, "a")` and one `filehandle.write(buffer)`. The flag `"a"` means "Open file for appending. The file is created if it does not exist." The code compares `bytesWritten` with the length of the buffer.
- **Java.** `FileChannel.open(path, CREATE, WRITE, APPEND)` and one `write`. The code checks that the buffer has no bytes left. `Files.writeString(path, text, CREATE, APPEND)` is shorter, but without `CREATE` it fails on a new file.
- **C#.** .NET has no option for `O_APPEND`. `FileMode.Append` "seeks to the end of the file" when the file opens, and later writes go to that position. So the solution opens the file with `FileShare.None`, which makes a second open fail "until the file is closed", and it tries again after 1 ms while another writer holds the file. After 10 seconds it gives up and throws.
- **Go.** `os.OpenFile(path, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0o666)` and one `f.Write`. The code returns the error of `Close` too.
- **Rust.** `OpenOptions::new().append(true).create(true).open(path)` and one `write`. The code does not use `write_all`, because it can split the data into more than one write. It reports an error if `write` wrote less.
- **C.** POSIX `open` with `O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC`, then one `write` of a buffer that holds the line and `'\n'`. The code tries the write again after `EINTR`, reports a short write as `EIO` and checks `close`.
- **C++.** `std::ofstream(path, std::ios::binary | std::ios::app)`, one `write` of a `std::string` that holds the line and `'\n'`, then `close`. The flag `app` seeks "to the end of stream before each write". The code returns `std::expected<void, AppendError>` (C++23).
- **Bash.** `printf '%s\n' "$line" >>"$path"`. The redirection `>>` opens the file for appending, and `printf` sends the line and the newline together.

PowerShell `Add-Content` adds text to the end of a file, and it can make a new file.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The `file` argument gives the content of the file before the call, and `null` means that there is no file. The cases that must give an error are in `cases/errors.jsonl`.

- `new-file` and `new-file-empty-line`: there is no file. The solution makes it.
- `empty-line`: the line is empty, so the solution adds only `\n`.
- `empty-file`: the file exists and has no bytes.
- `no-final-newline`: the old content is `one`. The result is `onetwo\n`, because the solution does not read the file.
- `crlf-file`: the old lines end with `\r\n`. The new line ends with `\n`, and the old content stays.
- `whitespace` and `non-ascii`: blanks, a tab and characters of 2, 3 and 4 bytes stay as they are.
- Errors: a missing parent directory, and a line that holds `\n` or `\r`.

The native tests also start 8 writers, threads or processes, that each add 250 lines to one file at the same time. Each of the 2000 lines must be in the file once and whole. A solution that seeks to the end and then writes fails this test. In one run on macOS, `File.AppendAllText` from 8 .NET threads kept 1631 of the 2000 lines.

## Pitfalls

- **Seek to the end, then write.** A writer that moves to the end of the file once, and then writes, can write over the lines of another writer. The other writer adds bytes between the seek and the write:

```python wrong id=python-seek-then-write
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "log.txt")
open(path, "wb").close()

# Two writers that each go to the end of the file once, when they open it.
a = os.open(path, os.O_WRONLY)
b = os.open(path, os.O_WRONLY)
os.lseek(a, 0, os.SEEK_END)
os.lseek(b, 0, os.SEEK_END)
os.write(a, b"A1\n")
os.write(b, b"B1\n")
os.write(a, b"A2\n")
os.close(a)
os.close(b)

print(open(path).read(), end="")
```

```output
B1
A2
```

The line `A1` is lost. With `O_APPEND`, each write goes to the end of the file as it is at the time of the write:

```python run id=python-o-append
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "log.txt")
open(path, "wb").close()

# With O_APPEND, each write goes to the end of the file as it is then.
a = os.open(path, os.O_WRONLY | os.O_APPEND)
b = os.open(path, os.O_WRONLY | os.O_APPEND)
os.write(a, b"A1\n")
os.write(b, b"B1\n")
os.write(a, b"A2\n")
os.close(a)
os.close(b)

print(open(path).read(), end="")
```

```output
A1
B1
A2
```

- **`FileMode.Append` in .NET.** The name says "append", but .NET does not open the file with `O_APPEND`. dotnet/runtime issue 53432 says that .NET mimics "the append by setting the file offset to the end of the file". Issue 76408, which asks for real appends, is open. So two .NET writers act as the two writers above:

```csharp wrong id=csharp-filemode-append
using System;
using System.IO;
using System.Text;

string path = Path.Combine(Directory.CreateTempSubdirectory().FullName, "log.txt");
File.WriteAllText(path, "");

using (var a = new FileStream(path, FileMode.Append, FileAccess.Write))
using (var b = new FileStream(path, FileMode.Append, FileAccess.Write))
{
    a.Write(Encoding.UTF8.GetBytes("A1\n"));
    a.Flush();
    b.Write(Encoding.UTF8.GetBytes("B1\n"));
    b.Flush();
    a.Write(Encoding.UTF8.GetBytes("A2\n"));
    a.Flush();
}
Console.Write(File.ReadAllText(path));
```

```output
B1
A2
```

`File.AppendAllText` and `new StreamWriter(path, append: true)` open the file in the same way. Use a lock, as the C# solution does with `FileShare.None`.

- **A line in more than one write.** Append mode puts each write at the end, but two writes of one line can have a line of another writer between them. A buffered stream can split a long line, and Rust `write_all` calls `write` again after a short write. The Rust documentation says: "make sure that all data that belongs together is written in one operation". It also says that append mode "does not necessarily guarantee that data appended by different processes or threads does not interleave". So keep each line short, and write it in one call.
- **No final newline.** If the last old line has no `\n`, the new line joins it:

```bash wrong id=bash-join-last-line
file=$(mktemp)
printf 'one' >"$file"
printf '%s\n' two >>"$file"
cat -- "$file"
rm -f -- "$file"
```

```output
onetwo
```

If the file can come from another program, check its last byte before the first append.

- **`APPEND` without `CREATE` in Java.** `Files.writeString(path, text, StandardOpenOption.APPEND)` throws `NoSuchFileException` if the file does not exist. With no options, `Files.write` "works as if the CREATE, TRUNCATE_EXISTING, and WRITE options are present". Give `CREATE` and `APPEND` together.
- **Append on NFS.** The Linux manual says that `O_APPEND` "may lead to corrupted files on NFS filesystems if more than one process appends data to a file at once", because the client must simulate the append. On a network file system, use a lock or one writer process.
- **Append and read on one handle.** If a file is open to read and to append, the position for reading can move to the end of the file after each write. The Rust documentation says to save the position before the write and restore it before the next read.

## Related tasks

- [[write-a-file]]
- [[read-a-whole-file]]
- [[read-a-file-line-by-line]]

## References
