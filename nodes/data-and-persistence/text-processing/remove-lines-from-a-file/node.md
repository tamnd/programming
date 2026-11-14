---
tag: Z5ASD4
kind: task
slug: remove-lines-from-a-file
name: Remove lines from a file
aliases: [delete lines from a file, delete line N of a file, sed Nd, remove a range of lines from a file]
area: data-and-persistence/text-processing
level: core
summary: Remove count lines from a file, from line start, by writing the rest to a new file in the same directory and renaming it over the old one, so a reader never sees a part of a file.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Remove-lines-from-a-file/00-TASK.txt
  title: Rosetta Code, Remove lines from a file (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), rename, renameat"
  locator: DESCRIPTION; ERRORS (EXDEV)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fsync.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), fsync"
  locator: DESCRIPTION
- url: https://docs.python.org/3/library/shutil.html
  title: The Python Standard Library, shutil
  locator: copymode
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: os.replace
- url: https://nodejs.org/api/fs.html
  title: Node.js documentation, File system
  locator: File system flags; fs.renameSync
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
  title: Java SE 25 API, java.nio.file.Files
  locator: createTempFile; move (ATOMIC_MOVE)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.setunixfilemode
  title: .NET API reference, File.SetUnixFileMode
  locator: SetUnixFileMode(String, UnixFileMode)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.move
  title: .NET API reference, File.Move
  locator: Move(String, String, Boolean)
- url: https://pkg.go.dev/os
  title: Go standard library, package os
  locator: CreateTemp; File.Sync; Rename
- url: https://doc.rust-lang.org/std/fs/struct.OpenOptions.html
  title: The Rust Standard Library, std::fs::OpenOptions
  locator: create_new
- url: https://doc.rust-lang.org/std/fs/fn.rename.html
  title: The Rust Standard Library, std::fs::rename
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/mkdtemp.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), mkdtemp, mkostemp, mkstemp"
  locator: DESCRIPTION
- url: https://en.cppreference.com/w/cpp/io/ios_base/openmode
  title: cppreference.com, std::ios_base::openmode
  locator: noreplace
- url: https://en.cppreference.com/w/cpp/filesystem/rename
  title: cppreference.com, std::filesystem::rename
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tail.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), tail"
  locator: OPTIONS (-n)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Shell Command Language"
  locator: 2.7.2 Redirecting Output; 2.9.1 Simple Commands
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sed.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sed"
  locator: SYNOPSIS
- url: https://man.freebsd.org/cgi/man.cgi?query=sed&sektion=1
  title: FreeBSD manual page, sed(1)
  locator: SYNOPSIS; DESCRIPTION (-I, -i)
- url: https://docs.python.org/3/library/tempfile.html
  title: The Python Standard Library, tempfile
  locator: mkstemp
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
  title: MDN Web Docs, String.prototype.split()
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/head.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), head"
  locator: OPTIONS (-c, -n)
---

## Task

Remove lines from a file. Rosetta Code states the task like this: "Remove a specific line or a number of lines from a file." The function "takes three parameters (filename, starting line, and the number of lines to be removed)".[^1] Line 1 is the first line. The function removes the lines, writes the rest of the file back under the same name, and returns the number of lines that it removed.

The task needs rules for what a line is and for what to do at the end of the file. This page uses these rules:

- **A line ends at LF, and the LF goes with the line.** When a line goes, its LF goes too. A CR is a byte like the others, so a line that ends in CRLF loses its CR and its LF. The other bytes of the file do not change.
- **A last line with no LF is a line too.** If the function removes it, the file then ends with the LF of the line before it. If the function removes an earlier line, the last line still has no LF.
- **An empty line is a line.** Rosetta Code says: "Empty lines are considered and should still be counted, and if the specified line is empty, it should still be removed."[^1]
- **The file can end before the range ends.** Rosetta Code says: "An appropriate message should appear if an attempt is made to remove lines beyond the end of the file."[^1] Here the function returns the number of lines that it removed, and the caller shows the message if that number is less than the count. If the function removes no line, it does not write the file.
- **A bad range is an error.** A start less than 1 or a count less than 0 is an error, and the function checks the range before it opens the file. A count of 0 is not an error: the function removes no line.

If the file does not exist, the function reports an error.

## Solutions

The method is the same in each language:

1. Read the file and count the lines. Copy each line that is not in the range to a new file in the same directory as the old file.
2. If no line was in the range, delete the new file and return 0. The old file does not change.
3. Give the new file the permissions of the old file, and write its data to the disk.
4. Rename the new file to the name of the old file.

The rename is the step that changes the file. POSIX says that during a rename, a directory entry named new "shall remain visible to other threads throughout the renaming operation and refer either to the file referred to by new or old before the operation began."[^2] So a program that opens the file gets the old file or the new file, and never a part of a file. If the program stops before the rename, the old file does not change. Before the rename, `fsync` "shall request that all data for the open file descriptor named by fildes is to be transferred to the storage device".[^3] Without this step, the new name can refer to data that is not yet on the disk.

Python, Go, Rust, C, C++ and Bash read the file one line at a time, so they need memory for one line only. JavaScript, Java and C# read the whole file into an array of bytes and keep the bytes before and after the range. This needs less code, but it needs memory for the whole file.

- **Python.** The file is open in binary mode, so only LF ends a line and the bytes do not change. `enumerate(src, start=1)` gives each line with its number. `tempfile.mkstemp(dir=...)` makes the new file in the same directory. `shutil.copymode` does this: "Copy the permission bits from src to dst".[^4] `os.fsync` writes the data to the disk, and `os.replace` renames the file: "If dst exists and is a file, it will be replaced silently if the user has permission."[^5] A bad range raises `ValueError`.
- **JavaScript and TypeScript (Node.js).** `readFileSync` gives a `Buffer`, and `indexOf(10, offset)` finds the next LF. The code skips start - 1 lines, then skips count lines, and keeps the bytes before and after the lines that it skipped the second time. `openSync(tmp, 'wx', 0o600)` makes the new file. The flag `'wx'` is "Like 'w' but fails if the path exists",[^6] so the code does not write over a file that has the same name. `fsyncSync` writes the data to the disk, `chmodSync` copies the permissions, and `renameSync` renames the file: "In the case that newPath already exists, it will be overwritten."[^6] A bad range throws a `RangeError`. The TypeScript file adds types only.
- **Java.** `Files.readAllBytes` reads the file, and a loop finds each LF. `Files.createTempFile(dir, prefix, suffix)` "Creates a new empty file in the specified directory, using the given prefix and suffix strings to generate its name."[^7] A `FileOutputStream` writes the bytes, and `getFD().sync()` writes them to the disk. If the file system has POSIX permissions, `setPosixFilePermissions` copies them. `Files.move` with `ATOMIC_MOVE` renames the file. A bad range throws an `IllegalArgumentException`.
- **C#.** `File.ReadAllBytes` reads the file, and `Array.IndexOf` finds each LF. A `FileStream` with `FileMode.CreateNew` makes the new file, and `Flush(true)` writes the data to the disk. On Unix, `File.SetUnixFileMode` "Sets the specified UnixFileMode of the file on the specified path."[^8] Then `File.Move(tmp, path, overwrite: true)` renames the file. The documentation describes the parameter `overwrite` set to true, "which replaces the file if it exists".[^9] A bad range throws an `ArgumentOutOfRangeException`.
- **Go.** `bufio.Reader.ReadBytes('\n')` reads each line with its LF, also a line that is longer than the buffer. `os.CreateTemp` "creates a new temporary file in the directory dir, opens the file for reading and writing, and returns the resulting file."[^10] A `bufio.Writer` writes the lines. `Chmod` copies the permissions, and `Sync` "commits the current contents of the file to stable storage."[^10] `os.Rename` renames the file: "If newpath already exists and is not a directory, Rename replaces it."[^10] A deferred function closes and removes the new file on each return before the rename. A bad range gives an error that wraps `ErrBadRange`.
- **Rust.** `BufRead::read_until(b'\n', ...)` reads each line with its LF into a `Vec<u8>`. The new file opens with `create_new`, which "Sets the option to create a new file, failing if it already exists."[^11] Its name has the process ID and a counter, and the code tries the next name if a file has that name. `set_permissions` copies the permissions, `sync_all` writes the data to the disk, and `fs::rename` renames the file, "replacing the original file if to already exists".[^12] Line numbers are `u64` values, so they do not overflow. A bad range gives an error of kind `InvalidInput`.
- **C.** `mkstemp` makes the new file from the path and the suffix `.XXXXXX`. POSIX says that it shall "create a regular file with a unique name derived from template and return a file descriptor for the file open for reading and writing."[^13] A `getc` loop copies the bytes that are not in the range, and counts the lines in a `long long`. `fchmod` copies the permissions, `fsync` writes the data to the disk, and `rename` renames the file. `remove_lines` returns the number of lines that it removed, or -1 with `errno` set. A bad range sets `errno` to `EINVAL`.
- **C++.** `std::getline` reads a binary `std::ifstream` one line at a time and removes the LF. The code writes each line that it keeps, and then an LF, except after a last line with no LF. The new file has a random suffix and opens with `std::ios::noreplace` (C++23), which means "open in exclusive mode".[^14] `fs::permissions` copies the permissions, and `fs::rename` renames the file "as if by the POSIX rename".[^15] This solution uses only the standard library, so it does not write the data to the disk before the rename. The result is a `std::expected<int, RemoveError>`.
- **Bash.** `awk 'END { print NR }'` counts the lines. In the tests, it also counts a last line with no LF. The function then calculates how many lines it can remove. If that number is 0, it prints 0 and does not write the file. If not, `mktemp` makes the new file from the template `$file.XXXXXX`, and `cp -p` copies the old file with its permissions to the new file. Then the redirection `>` truncates the copy, `head -n` writes the lines before the range, and `tail -n +K` writes the lines from line K, the first line after the range. For `tail`, "The origin for counting shall be 1; that is, -n +1 represents the first line of the file".[^16] `mv -f` renames the new file. The function prints the number of lines that it removed, and returns 2 with a message on an error. It does not write the data to the disk before the rename.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. For each case, `fixtures/NAME.txt` is the file before the call, and `fixtures/NAME.after.txt` is the file after the call.

- `first-two`, `middle` and `last`: from `a`, `b`, `c`, `d`, lines 1 and 2 go and `c`, `d` stay, or lines 2 and 3 go and `a`, `d` stay. From `a`, `b`, `c`, line 3 goes and `a`, `b` stay.
- `past-the-end`: line 3 of a file with two lines. The result is 0, and the file does not change.
- `partly-past-the-end`: 5 lines from line 2 of `a`, `b`, `c`. The result is 2, and `a` stays.
- `count-zero`: a count of 0. The result is 0, and the file does not change.
- `all-lines`: lines 1 and 2 of a file with two lines. The result is 2, and the file is empty.
- `empty`: line 1 of an empty file. The result is 0.
- `empty-line`: line 2 of `a`, empty, `b` is the empty line, and it goes.
- `no-final-newline` and `remove-incomplete-line`: the file is `a`, `b` with no LF at the end. If line 1 goes, `b` stays with no LF. If line 2 goes, the file is `a` and an LF.
- `crlf`: line 2 of `a` CRLF, `b` CRLF, `c` CRLF goes with its CR and its LF.
- `lone-cr`: line 1 of `a` CR `b` LF `c` LF is `a` CR `b`, because the CR does not end a line. After the call, the file is `c` and an LF.
- `non-ascii`: line 2 of `café`, `naïve`, `zoë` goes.
- `large-count`: 2147483647 lines from line 2 of a file with two lines. The result is 1.

The tests also write a file of 100000 lines with the numbers 0 to 99999, and remove 99998 lines from line 2. After the call, the file has the lines `0` and `99999`. Other tests check that a file with mode 640 keeps its mode, and that no new file stays in the directory. The cases in `cases/errors.jsonl` must give the error `not-found` or `bad-range`, and must not change a file. The toolchain does not load that file yet, so each test file holds the cases by name.

## Pitfalls

- **Redirecting the output to the input file empties the file.** In `sed '2d' notes.txt > notes.txt`, the shell does the redirection before it runs `sed`.[^17] For `>`, the shell opens an existing file as if `open` "was called with the O_TRUNC flag set".[^17] So the file is empty before `sed` reads it. Write the output to a new file, and then rename the new file:

```bash run id=bash-redirect
dir=$(mktemp -d)
printf 'a\nb\nc\n' >"$dir/notes.txt"
# Wrong: the shell empties notes.txt before sed reads it.
sed '2d' "$dir/notes.txt" >"$dir/notes.txt"
[ -s "$dir/notes.txt" ] || echo 'notes.txt is empty'
printf 'a\nb\nc\n' >"$dir/notes.txt"
# Right: write to a new file, then rename it.
sed '2d' "$dir/notes.txt" >"$dir/notes.new" && mv "$dir/notes.new" "$dir/notes.txt"
cat "$dir/notes.txt"
rm -r "$dir"
```

```output for=bash-redirect
notes.txt is empty
a
c
```

- **`sed -i` is not portable.** The POSIX synopsis of `sed` is `sed [-En] script [file...]`, with no `-i` option.[^18] In FreeBSD `sed`, `-i` takes an argument, `extension`. The `-I` option edits files in place, "saving backups with the specified extension", and `-i` works "similarly to -I".[^19] So FreeBSD `sed -i '2d' notes.txt` takes `2d` as the extension and `notes.txt` as the script. A portable script writes to a new file and renames it, as the Bash solution does.
- **The new file does not have the permissions of the old file.** POSIX `mkstemp` opens the file as if with `open(pathname, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR)`,[^13] and in Python, the file from `mkstemp` "is readable and writable only by the creating user ID."[^20] If the code renames the new file and does not copy the permissions, a file with mode 644 has mode 600 after the call:

```python run id=python-mode
import os
import stat
import tempfile

with tempfile.TemporaryDirectory() as directory:
    path = os.path.join(directory, 'notes.txt')
    with open(path, 'w') as f:
        f.write('a\nb\nc\n')
    os.chmod(path, 0o644)
    print('before:', oct(stat.S_IMODE(os.stat(path).st_mode)))
    fd, tmp = tempfile.mkstemp(dir=directory)
    with os.fdopen(fd, 'w') as f:
        f.write('a\nc\n')
    # No shutil.copymode(path, tmp) here.
    os.replace(tmp, path)
    print('after: ', oct(stat.S_IMODE(os.stat(path).st_mode)))
```

```output for=python-mode
before: 0o644
after:  0o600
```

- **The new file must be on the same file system.** A temp directory such as `/tmp` can be on a different file system from the old file. Then POSIX `rename` can fail with `[EXDEV]`, because the two files "are on different file systems and the implementation does not support hard links between file systems."[^2] Rust says about `fs::rename`: "This will not work if the new name is on a different mount point."[^12] Make the new file in the directory of the old file, as all the solutions here do.
- **`split('\n')` counts one line too many.** If the text ends with an LF, `split` gives an empty string as the last part: "an empty (i.e., zero length) string appearing at the first (or last) position of the returned array."[^21] Code that removes lines with `splice` and puts the rest back together with `join` thus sees one more line than the file has. If the range starts just after the last line, the code removes the empty string, reports 1 line, and the file loses its last LF:

```javascript run id=javascript-split
// Removes count lines from text, from line start, with split and join.
function removeLines(text, start, count) {
  const lines = text.split('\n');
  const removed = lines.splice(start - 1, count);
  return { text: lines.join('\n'), removed: removed.length };
}

// Two lines, and line 2 goes: the result is right.
console.log(JSON.stringify(removeLines('a\nb\n', 2, 1)));
// Two lines, and line 3 does not exist: the LF at the end goes.
console.log(JSON.stringify(removeLines('a\nb\n', 3, 1)));
```

```output for=javascript-split
{"text":"a\n","removed":1}
{"text":"a\nb","removed":1}
```

- **`start + count` can be too large for an `int`.** In the `large-count` case, start is 2 and count is 2147483647, so `start + count` does not fit in a 32-bit signed integer. The solutions in languages with fixed-size integers do not calculate `start + count`. They compare `n - start < count`, or they count the lines that they removed.
- **`head -n 0` is not portable.** For `head`, "The application shall ensure that the number option-argument is a positive decimal integer."[^22] So when start is 1, the Bash solution does not run `head`.
- **An atomic move can fail if the target exists.** For `ATOMIC_MOVE`, Java says: "If the target file exists then it is implementation specific if the existing file is replaced or this method fails by throwing an IOException".[^7] On such a system, the Java solution throws the exception, deletes the new file and does not change the old file.

## Related tasks

- [[write-a-file-atomically]]: write a new file and rename it over the old one, the method that this page uses.
- [[rename-a-file]]: rename a file, and what happens if the new name exists.
- [[read-a-specific-line-from-a-file]]: read line N of a file, with the same rules for a line.
- [[remove-duplicate-lines]]: remove each line that is the same as a line before it.
- [[filter-lines-that-match-a-pattern]]: keep the lines in which a regular expression matches.
- [[line-ending]]: LF, CRLF and CR, and why a CR stays in the line.
