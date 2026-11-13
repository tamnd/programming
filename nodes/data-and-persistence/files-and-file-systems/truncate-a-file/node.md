---
tag: VHM159
kind: task
slug: truncate-a-file
name: Truncate a file
aliases: [resize a file, set the size of a file, shorten a file, extend a file with zero bytes, ftruncate]
area: data-and-persistence/files-and-file-systems
level: core
summary: Set the size of an existing file to a given number of bytes. Cut the bytes after the new end, or add zero bytes up to it, and do not make a file when none is there.
contract: contract.yaml
category: file-output
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/os.html#os.truncate
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.truncate
  - url: https://docs.python.org/3/library/io.html#io.IOBase.truncate
    title: The Python Standard Library, io, Core tools for working with streams
    locator: IOBase.truncate
  - url: https://docs.python.org/3/library/functions.html#open
    title: The Python Standard Library, Built-in Functions
    locator: open, mode
  - url: https://nodejs.org/api/fs.html#fspromisestruncatepath-len
    title: Node.js documentation, File system
    locator: fsPromises.truncate(path[, len]), filehandle.truncate(len)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html
    title: Java SE 25 API, FileChannel
    locator: truncate(long)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/RandomAccessFile.html
    title: Java SE 25 API, RandomAccessFile
    locator: RandomAccessFile(String, String), setLength(long)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestream.setlength?view=net-10.0
    title: .NET API reference, FileStream.SetLength(Int64) Method
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filemode?view=net-10.0
    title: .NET API reference, FileMode Enum
    locator: Fields (Append, Truncate)
  - url: https://pkg.go.dev/os
    title: Go standard library, package os
    locator: func Truncate
  - url: https://doc.rust-lang.org/std/fs/struct.File.html#method.set_len
    title: The Rust Standard Library, std::fs::File
    locator: set_len
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/truncate.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), truncate
    locator: DESCRIPTION, ERRORS
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/ftruncate.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), ftruncate
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/dd.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), dd
    locator: OPERANDS (seek=)
  - url: https://man7.org/linux/man-pages/man2/truncate.2.html
    title: Linux manual page, truncate(2)
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/filesystem/resize_file
    title: cppreference, std::filesystem::resize_file
    locator: Description
  - url: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setendoffile
    title: Windows API reference, SetEndOfFile function (fileapi.h)
    locator: Remarks
---

## Task

Set the size of the existing file at `path` to `length` bytes. If the file was longer, the bytes after `length` are gone. If the file was shorter, the file grows, and the new bytes are zero bytes. The bytes before `length` do not change.

Report an error, and change no file, when no file is at `path`, when `path` is a directory, and when `length` is less than 0. The first case is important: many ways to open a file for writing make a new file when there is none. A truncate must not do that.

POSIX gives the rule for the two directions: "If the file previously was larger than length , the extra data is discarded. If the file was previously shorter than length , its size is increased, and the extended area appears as if it were zero-filled." The Linux manual says the same, and adds: "The file offset is not changed." The pitfalls show why that last sentence is important.

SQL, regular expressions, HTML and CSS do not apply, because they do not change the size of a file.

## Solutions

Each language has a call that sets the size of a file by its path or through an open file. Most of them do the two directions, but not all.

- **Python.** `os.truncate(path, length)`. "Truncate the file corresponding to path , so that it is at most length bytes in size." But the call also makes a file longer: on Linux and macOS, a greater length adds zero bytes, and the tests check this. For a file that is open, `f.truncate(size)` does the same: "This resizing can extend or reduce the current file size. In case of extension, the contents of the new file area depend on the platform (on most systems, additional bytes are zero-filled)." Each error is an `OSError`.
- **JavaScript and TypeScript (Node.js).** `fsPromises.truncate(path, length)`. "Truncates (shortens or extends the length) of the content at path to len bytes." Node does not report an error for a negative length: "If len is negative then 0 will be used." So the code throws a `RangeError` first, because a negative length is a mistake of the caller, and Node would make the file empty.
- **Java.** `FileChannel.open(path, StandardOpenOption.WRITE)` opens the file with no `CREATE`, so a missing file gives `NoSuchFileException`. `truncate(length)` makes the file shorter, but it does not make it longer: "If the given size is greater than or equal to the file's current size then the file is not modified." So for a greater length, the code writes one zero byte at `length - 1`.
- **C#.** `new FileStream(path, FileMode.Open, FileAccess.Write, FileShare.ReadWrite)` and then `stream.SetLength(length)`. `FileMode.Open` does not make a file. "If the given value is less than the current length of the stream, the stream is truncated." The code throws an `IOException` for a directory, and `ArgumentOutOfRangeException.ThrowIfNegative` for a negative length.
- **Go.** `os.Truncate(path, length)`. "Truncate changes the size of the named file. If the file is a symbolic link, it changes the size of the link's target. If there is an error, it will be of type *PathError ." For an open `*os.File`, the method is `f.Truncate(size)`.
- **Rust.** `OpenOptions::new().write(true).open(path)` and then `file.set_len(length)`. "Truncates or extends the underlying file, updating the size of this file to become size . If the size is less than the current file’s size, then the file will be shrunk. If it is greater than the current file’s size, then the file will be extended to size and have all of the intermediate data filled in with 0s." The length is a `u64`, so a negative length is not possible.
- **C.** POSIX `truncate(path, length)`, with `ftruncate(fd, length)` for an open file. It returns -1 and sets `errno`, for example "[EISDIR] The named file is a directory." ISO C has no call that sets the size of a file.
- **C++.** `std::filesystem::resize_file(path, length, ec)`. It "Changes the size of the regular file named by p as if by POSIX truncate : if the file size was previously larger than new_size , the remainder of the file is discarded. If the file was previously smaller than new_size , the file size is increased and the new area appears as if zero-filled."
- **Bash.** POSIX has no `truncate` utility, so the code uses `dd if=/dev/null of="$path" bs=1 seek="$length"`. The input is empty, so dd writes nothing, and then: "If the input file is empty and either the size of the seek is greater than the previous size of the output file or the output file did not previously exist, the size of the output file shall be set to the file offset after the seek." With no `conv=notrunc`, dd also cuts the bytes after the seek. dd makes a missing file, so the function checks for a missing file, for a directory and for a length that is not a whole number first, and returns status 1.

On Windows, the system call that sets the size of a file is `SetEndOfFile`. It "Sets the physical file size for the specified file to the current position of the file pointer", so a program moves the file pointer to the new size first.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each test writes the file in a new temporary directory, calls the function, and checks all the bytes of the file.

- `shrink`: `alpha beta\n` to 5 bytes gives `alpha`.
- `extend`: `alpha` to 8 bytes gives `alpha` and three zero bytes.
- `same-length`: a length equal to the size changes nothing.
- `to-zero`: a length of 0 gives an empty file.
- `empty-file`: an empty file to 3 bytes gives three zero bytes.
- `name-with-spaces`: `my notes.txt`. A shell solution must quote the name.
- Errors, in `cases/errors.jsonl`: a missing file gives not-found, and the test checks that no file is made. A directory gives is-a-directory, and a length of -1 gives invalid-length, in the languages where the type of the length lets the caller give one.

The native tests also check what happens to a stream that has the file open. A stream in append mode writes its next line at the new end. A stream at a position after the new end writes at that position, and a hole of zero bytes comes before the new data.

## Pitfalls

- **A truncate that makes a new file.** `open(path, "w")` in Python makes the file empty, but it also makes a file when there is none: "'w' open for writing, truncating the file first". So a typing mistake in the path gives a new empty file, and no error:

```python wrong id=python-truncate-with-open-w
import os
import tempfile


def truncate_with_w(path):
    with open(path, "w"):  # "w" makes a file when there is none
        pass


with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "missing.txt")
    truncate_with_w(path)
    print("exists:", os.path.exists(path))
```

```output
exists: True
```

The same is true for `: > "$path"` in a shell, for `RandomAccessFile` in Java with the mode `"rw"` ("If the file does not already exist then an attempt will be made to create it."), and for `FileMode.Create` in C#. Use a call that takes a path and does not open the file for writing, such as `os.truncate`, or open with no create flag. Also, `open` with `"w"` can only give a length of 0.

- **A writer that keeps its old position.** A truncate does not move the position of a stream that has the file open. POSIX says for `ftruncate`: "The value of the seek pointer shall not be modified by a call to ftruncate ()." So when one program empties a log file and the program that writes the log has it open with no append mode, the next write goes to the old position. The bytes before it read as zero bytes, and the file is as large as before:

```python run id=python-truncate-open-writer
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "app.log")
    log = open(path, "w")  # no O_APPEND
    log.write("old line 1\nold line 2\n")
    log.flush()
    os.truncate(path, 0)  # another program empties the log
    log.write("new\n")
    log.close()
    with open(path, "rb") as f:
        data = f.read()
    print("size:", len(data))
    print("zero bytes:", data.count(0))
```

```output
size: 26
zero bytes: 22
```

Open a log file in append mode. Python says that `"a"` is "for appending (which on some Unix systems, means that all writes append to the end of the file regardless of the current seek position)". With `"a"` in place of `"w"`, the same program gives a file of 4 bytes:

```python run id=python-truncate-append-writer
import os
import tempfile

with tempfile.TemporaryDirectory() as dir:
    path = os.path.join(dir, "app.log")
    log = open(path, "a")  # O_APPEND: each write goes to the end
    log.write("old line 1\nold line 2\n")
    log.flush()
    os.truncate(path, 0)
    log.write("new\n")
    log.close()
    with open(path, "rb") as f:
        print(f.read())
```

```output
b'new\n'
```

- **Append mode in .NET is not the same.** `FileMode.Append` "Opens the file if it exists and seeks to the end of the file, or creates a new file." The stream goes to the end one time, when it opens the file, and then it writes at its own position. On Linux and macOS, the C# native test shows that a write after a truncate to 0 gives a hole of zero bytes, as in the `"w"` example above.

- **A truncate that cannot make a file longer.** Java `FileChannel.truncate` does nothing when the size is greater than the size of the file. `RandomAccessFile.setLength` does make the file longer, but "The contents of the extended portion of the file are not defined." Test the two directions.

- **A negative length.** Most calls report an error. Node uses 0, so a mistake in a calculation of the length makes the file empty. Check the length before the call.

- **A truncate in the middle of a character.** The length is in bytes, not in characters. A truncate of `café` (5 bytes in UTF-8) to 4 bytes cuts the `é` into one byte, and the file is not valid UTF-8 after it. To cut text, find the byte offset of a character boundary first.

## Related tasks

- [[get-the-size-of-a-file]]
- [[append-to-a-file]]
- [[copy-a-file]]
- [[delete-a-file]]
- [[check-whether-a-file-exists]]
- [[read-and-write-with-buffers]]

## References
