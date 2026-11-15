---
tag: 09XWYW
kind: task
slug: tail-a-growing-file
name: Follow a growing file
aliases: [tail -f, tail -F, follow a log file, watch a log file for new lines, follow a file after log rotation]
area: data-and-persistence/text-processing
level: advanced
summary: Read each new line of a file as it grows, as tail -F does, and keep the lines when the file is truncated, renamed, deleted or made again under the same name.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tail.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), tail"
  locator: OPTIONS (-f)
- url: https://www.gnu.org/software/coreutils/manual/html_node/tail-invocation.html
  title: GNU Coreutils manual, tail invocation
  locator: -f, --follow; -F; --retry
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sys/stat.h"
  locator: DESCRIPTION; RATIONALE
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fstat.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), fstat"
  locator: DESCRIPTION
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pread.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), pread, read"
  locator: DESCRIPTION
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filesysteminfo
  title: .NET API reference, FileSystemInfo
  locator: Properties
- url: https://docs.python.org/3/library/os.path.html
  title: The Python Standard Library, os.path
  locator: samestat
- url: https://nodejs.org/api/fs.html
  title: Node.js documentation, File system
  locator: fs.statSync; fs.readSync; fs.Stats
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/channels/FileChannel.html
  title: Java SE 25 API, java.nio.channels.FileChannel
  locator: read(ByteBuffer, long); size
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/attribute/BasicFileAttributes.html
  title: Java SE 25 API, java.nio.file.attribute.BasicFileAttributes
  locator: fileKey
- url: https://pkg.go.dev/os
  title: Go standard library, package os
  locator: SameFile; File.ReadAt
- url: https://doc.rust-lang.org/std/os/unix/fs/trait.MetadataExt.html
  title: The Rust Standard Library, std::os::unix::fs::MetadataExt
  locator: dev; ino
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: GNU Bash manual, Bash Builtin Commands
  locator: read
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/ls.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), ls"
  locator: OPTIONS (-i); STDOUT
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), rename, renameat"
  locator: DESCRIPTION
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/open.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), open, openat"
  locator: DESCRIPTION (O_TRUNC)
- url: https://nodejs.org/api/string_decoder.html
  title: Node.js documentation, String decoder
  locator: Class StringDecoder
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/unlink.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), unlink, unlinkat"
  locator: DESCRIPTION
- url: https://man7.org/linux/man-pages/man7/inotify.7.html
  title: Linux manual page, inotify(7)
  locator: DESCRIPTION; NOTES (Limitations and caveats)
---

## Task

Follow a file that grows, and get each line that a program adds to it, as `tail -f` and `tail -F` do. For `tail -f`, POSIX says: "do not terminate after the last line of the input file has been copied, but read and copy further bytes from the input file when they become available."[^1] A log file also changes in other ways. A program can truncate it, or rename it and make a new file with the same name. GNU `tail -F` follows the name: "That is, tail will attempt to reopen a file when it is removed."[^2] This page follows the name, as `tail -F` does.

The follower is an object with a poll operation. Each poll returns the complete lines that the file got since the last poll. A loop that polls at an interval and stops on a condition is a small part on top of it. The entry of the tests is `follow_file(path, initial, steps)`. It writes `initial` to the file, makes a follower, does each step and returns the lines of each poll. A step can append text to the file or to `path.1`, truncate the file, rotate it (rename it to `path.1` and make a new empty file at `path`), delete it, make it again, or poll.

This page uses these rules:

- **The follower starts at the end of the file.** The text that is in the file at the start gives no lines, also when it ends in a part of a line.
- **A line ends at LF, and the LF is not in the line.** A CR stays at the end of the line. The bytes after the last LF wait for the next poll, because the program can still be writing that line.
- **A line is UTF-8 text.** The follower splits the bytes at each LF first, and decodes each line after.
- **A truncation starts the file again.** If the open file is now shorter than the read offset, the bytes that wait become a line, and the follower reads the file again from byte 0.
- **A rename or a delete ends the old file.** The follower reads the rest of the old file. Then the bytes that wait become a line, and the follower closes the old file. If a file is at the path, the follower opens it and reads it from byte 0.

If no file is at the path when the follower starts, the follower reports an error.

## Solutions

The follower keeps the file open, and keeps a read offset and the bytes after the last LF. Each poll does these steps:

1. Get the identity of the file at the path, if a file is there. A file is "uniquely identified by the combination of the st_ino and st_dev fields."[^3] Do this step first. Then the read in step 3 gets each byte that the old file got before a rename.
2. Get the size of the open file. `fstat` "shall obtain information about an open file associated with the file descriptor fildes".[^4] If the size is less than the offset, the file was truncated. Then the bytes that wait become a line, and the offset goes back to 0.
3. Read from the offset to the end of the open file. Split the bytes at each LF, and decode each complete line. Keep the bytes after the last LF.
4. If no file is at the path, or the file there is not the open file, the bytes that wait become a line. Close the open file.
5. If no file is open and a file is at the path, open it and read it from byte 0, as in step 3.

JavaScript, TypeScript, Java, Go, C and C++ read at the offset, with `pread` or a function like it. `pread` reads "from a given position in the file without changing the file offset."[^5] The poll does not wait, and does not loop. Each solution except JavaScript and TypeScript has also a follow function that polls at an interval until a stop condition is true. The tests call it only with lines that are in the file before it starts, and stop it after 2 lines or after a fixed number of polls.

There is no C# solution. .NET gives the attributes, times, name and Unix mode of a file through `FileSystemInfo`, but no device number and no inode number.[^6] So a C# solution cannot tell the file at the path from the open file after a rotation.

- **Python.** The file is open in binary mode. `os.stat` gets the path and `os.fstat` gets the open file, and `os.path.samestat` compares them: it returns "True if the stat tuples stat1 and stat2 refer to the same file."[^7] `bytes.split(b'\n')` splits the bytes, and the last part waits. `decode()` raises `UnicodeDecodeError` for bytes that are not UTF-8. `follow(path, interval, stop)` makes the follower at once and returns a generator. A missing file raises `FileNotFoundError`.
- **JavaScript and TypeScript (Node.js).** `fs.statSync(path, { bigint: true, throwIfNoEntry: false })` gets the path. The option `throwIfNoEntry` is "Whether an exception will be thrown if no file system entry exists, rather than returning undefined".[^8] `fstatSync` gets the open file, and the code compares `dev` and `ino` as `bigint` values. `readSync` reads at the offset: "If position is a non-negative integer, the current file position will remain unchanged."[^8] `buf.toString('utf8', start, end)` decodes each line. A missing file throws an error with the code `ENOENT`. The TypeScript file adds types only.
- **Java.** A `FileChannel` reads at the offset, with `read(buf, position)`, which "Reads a sequence of bytes from this channel into the given buffer, starting at the given file position."[^9] Its `size()` "Returns the current size of this channel's file."[^9] Java cannot get the attributes of an open channel. So the solution takes `BasicFileAttributes.fileKey()` of the path just after the open, and compares it with the key of the path at each poll. `fileKey()` "Returns an object that uniquely identifies the given file, or null if a file key is not available."[^10] A missing file throws `NoSuchFileException`.
- **Go.** `os.Stat` gets the path and `File.Stat` gets the open file. `os.SameFile` compares them: "on Unix this means that the device and inode fields of the two underlying structures are identical".[^11] `File.ReadAt` reads at the offset. `string(b)` keeps the bytes as they are, also bytes that are not UTF-8. `Follow(interval, stop, emit)` is a method of the follower, so the follower starts before the loop. A missing file gives an error that wraps `fs.ErrNotExist`.
- **Rust.** `fs::metadata` gets the path and `File::metadata` gets the open file. `MetadataExt` gives `dev`, which "Returns the ID of the device containing the file.", and `ino`, which "Returns the inode number."[^12] `seek` goes to the offset, `read_to_end` reads the new bytes, and `String::from_utf8_lossy` decodes each line. A missing file gives an error of kind `NotFound`.
- **C.** `follower_open` opens the file, and `follower_poll` calls a callback with each line and its length. `stat` gets the path, `fstat` gets the open file, and the code compares `st_dev` and `st_ino`. `pread` reads at the offset into a buffer that grows. The bytes stay as they are. A function returns -1 with `errno` set on an error, so a missing file gives `ENOENT`.
- **C++.** The class `Follower` uses the same POSIX calls as the C solution, and closes the file in its destructor. `poll` returns a `std::vector<std::string>`. An error throws `std::system_error` with the value of `errno`.
- **Bash.** The follower keeps the file open on file descriptor 9, and keeps its state in global variables. `read -r -u 9` reads each line. The Bash manual says that `read` will "Read one line from the standard input, or from the file descriptor fd supplied as an argument to the -u option".[^13] At the end of the file, `read` fails but puts the bytes after the last LF in the variable, so they wait. Bash cannot get the attributes of an open file, so the code takes the inode number of the path from `ls -di` just after the open. For `-i`, `ls` will "write the file's file serial number".[^14] `wc -c` on the path gives the size. Bash cannot seek, so after a truncation the code opens the file again. A NUL byte in the file is lost. `follow_poll` must run in the shell itself, not in `$(...)` or in a pipeline, because a subshell does not change the state of the follower. A missing file makes `follow_open` print a message and return 1.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `append-lines`: the file starts with `old`, and gets `a`, `b`, then `c`. The polls give `a`, `b` and then `c`. The line `old` does not come.
- `no-change`: two polls with no new bytes give no lines.
- `partial-line`: `ab` waits, then `c` LF `d` gives `abc`, then LF gives `d`.
- `start-mid-line`: the file starts with `abc` and no LF. The new bytes `d` LF `e` LF give `d` and `e`, because the follower starts at the end of the file.
- `empty-line` and `crlf`: LF LF `a` LF gives two empty lines and `a`. `a` CRLF `b` CRLF gives `a` CR and `b` CR.
- `non-ascii`: `café` and `naïve` come back as they are.
- `truncate`, `truncate-and-write` and `truncate-partial`: after a truncation, the follower reads the new text from byte 0. In `truncate-and-write`, the new text `b` LF is shorter than the old text `aaaa` LF, so the size shows the truncation. In `truncate-partial`, the bytes `abc` that waited come first, as a line.
- `rotate` and `rotate-twice`: after each rotation, the follower reads the new file from byte 0.
- `rotate-late-writes`: after the rotation, the old file gets `2` and the new file gets `3`. The poll gives `2` and then `3`, because the follower reads the rest of the old file before it opens the new file.
- `rotate-partial`: `a` waits, then the rotation makes it a line, and `b` comes from the new file.
- `delete`, `delete-partial`, `delete-and-create` and `delete-and-create-between-polls`: after a delete, the polls give no lines until a new file is at the path. The bytes that waited become a line. If the new file comes before the next poll, that poll gives its lines.

The cases in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds that case by name. Each test file also appends 100000 lines with the numbers 0 to 99999, and checks that one poll gives all of them. Each test file except Python appends the first byte of `é`, polls, then appends the second byte and an LF, and checks that the next poll gives `café`. Each test file except JavaScript and TypeScript, which have no follow function, writes 2 lines after the follower starts and stops the follow loop after 2 lines or after a fixed number of polls.

## Pitfalls

- **`tail -f` does not follow a rotation.** GNU `tail -f` follows the descriptor, and "When following a descriptor, tail does not detect that the file has been unlinked or renamed and issues no message".[^2] After a log rotation, it waits for bytes in the old file, and the new file gets the lines. Use `tail -F`, which is "the same as --follow=name --retry".[^2] When a follower opens the file again, it must compare the device and the inode number, not only the name. A rename keeps the old file. POSIX says that the new name shall "refer either to the file referred to by new or old before the operation began".[^15] The inode number shows the difference:

```bash run id=bash-inode
dir=$(mktemp -d)
cd "$dir"
inode() { ls -di "$1" | awk '{ print $1 }'; }

printf 'a\n' > app.log
before=$(inode app.log)
: > app.log # truncate
if [ "$(inode app.log)" = "$before" ]; then echo 'truncate: same file'; else echo 'truncate: new file'; fi

before=$(inode app.log)
mv app.log app.log.1 && : > app.log # rotate
if [ "$(inode app.log)" = "$before" ]; then echo 'rotate: same file'; else echo 'rotate: new file'; fi
if [ "$(inode app.log.1)" = "$before" ]; then echo 'app.log.1 is the old file'; fi

cd / && rm -rf "${dir:?}"
```

```output for=bash-inode
truncate: same file
rotate: new file
app.log.1 is the old file
```

- **A truncation is not always visible in the size.** The follower sees a truncation only when the size is less than its offset. A truncation keeps the file: with `O_TRUNC`, "its length shall be truncated to 0, and the mode and owner shall be unchanged."[^16] GNU `tail` has the same limit. When the file has shrunk, it "resumes tracking from the start of the file, assuming it has been truncated to 0".[^2] If the program then writes more bytes than the offset before the next poll, the size is not less, and the follower reads from the old offset, in the middle of the new text. Then the follower loses lines:

```python run id=python-truncate
import os
import tempfile

with tempfile.TemporaryDirectory() as directory:
    path = os.path.join(directory, 'app.log')
    with open(path, 'w') as f:
        f.write('old 1\nold 2\n')
    offset = os.path.getsize(path)  # the follower has read up to here
    with open(path, 'w') as f:  # O_TRUNC: the same file, now empty
        f.write('new line 1\nnew line 2\n')
    size = os.path.getsize(path)
    print('offset:', offset, 'size:', size)
    print('truncation seen:', size < offset)
    with open(path, 'rb') as f:
        f.seek(offset)
        print('read from the offset:', f.read())
```

```output for=python-truncate
offset: 12 size: 22
truncation seen: False
read from the offset: b'ew line 2\n'
```

- **Decode each line, not each read.** A read can stop in the middle of a character. If the code decodes each read, the two parts of `é` become two replacement characters. Split the bytes at each LF first, as all the solutions here do. In Node.js, a `StringDecoder` also works. Its documentation says that "an internal buffer is used to ensure that the decoded string does not contain any incomplete multibyte characters."[^17] The JavaScript block shows the problem:

```javascript run id=javascript-decode
const show = (s) => s.replace(/[^\x20-\x7e]/g, (c) => "\\u" + c.charCodeAt(0).toString(16).padStart(4, "0"));

const bytes = Buffer.from("café\n");
const first = bytes.subarray(0, 4); // c, a, f and the first byte of é
const second = bytes.subarray(4);
console.log("decode each read:", show(first.toString("utf8") + second.toString("utf8")));
console.log("decode the line: ", show(Buffer.concat([first, second]).toString("utf8")));
```

```output for=javascript-decode
decode each read: caf\ufffd\ufffd\u000a
decode the line:  caf\u00e9\u000a
```

- **The last line can be a part of a line.** When the read gets to the end of the file, the program can be in the middle of a line. Keep those bytes for the next poll. Return them as a line only when the file is truncated, renamed or deleted, because no more bytes come for them then.
- **Look at the path before the read.** If the poll reads first and looks at the path after, a program can append to the file and rename it between the two steps. Then the poll sees the new file, closes the old file and loses the bytes that it did not read.
- **Keep the old file open.** POSIX `unlink` says that if a process has the file open, "the removal of the file contents shall be postponed until there are no such references to the file."[^18] So the follower can read the rest of a file after a rename or a delete. POSIX makes the device and the inode number unique for the files that are visible in the file tree.[^3] If the follower closes the file after each poll and keeps only the inode number, the old file can go, a new file can get the same number, and the follower takes the new file for the old file.
- **Java takes the file key from the path.** The key comes from the path just after the open, so a rename between the open and that step gives the key of the wrong file. On a system with no file key, `fileKey()` returns null, and the Java solution takes the file as deleted at each poll, so it reads the file again and gives the lines again. On Linux and macOS, the key holds the device and the inode number.
- **A Python generator starts late.** The body of a generator runs only at the first `next`. If the generator opens the file, it starts at the end of the file at that time, and it loses the lines that came between the call and the first `next`. The Python `follow` opens the file before it makes the generator.
- **Polling is not an event.** A poll at an interval adds a delay and costs a system call when the file does not change. On Linux, `inotify` gives events. But inotify "does not catch remote events that occur on network filesystems. (Applications must fall back to polling the filesystem to catch such events.)"[^19] Also "Inotify monitoring is inode-based", so a watch on the file stays on the old file after a rotation.[^19] A program that uses events must watch the directory, and must still read and compare as this page does.

## Related tasks

- [[watch-a-file-for-changes]]: get an event when a file changes, with inotify and the other system APIs.
- [[read-a-file-line-by-line]]: read the lines of a file that does not change.
- [[print-the-first-and-last-lines-of-a-file]]: `head` and `tail` with no `-f`.
- [[rename-a-file]]: rename a file, which keeps the old file under the new name.
- [[delete-a-file]]: delete a file, and what happens to a process that has it open.
- [[line-ending]]: LF, CRLF and CR, and why a CR stays in the line.
- [[utf-8]]: the encoding of the lines, and how a character can take more than one byte.
