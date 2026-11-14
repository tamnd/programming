---
tag: G9C785
kind: task
slug: replace-text-in-many-files
name: Replace text in many files
aliases: [globally replace text in several files, search and replace in many files, find and replace in files, sed s/old/new/g on many files]
area: data-and-persistence/text-processing
level: core
summary: Replace each instance of a text with another text in a list of files, and write each changed file to a new file that a rename puts in the place of the old one.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Globally-replace-text-in-several-files/00-TASK.txt
  title: Rosetta Code, Globally replace text in several files (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rename.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), rename, renameat"
  locator: DESCRIPTION
- url: https://docs.python.org/3/builtins/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: bytes.count; bytes.replace
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: os.replace
- url: https://nodejs.org/api/buffer.html
  title: Node.js documentation, Buffer
  locator: buf.indexOf
- url: https://nodejs.org/api/fs.html
  title: Node.js documentation, File system
  locator: File system flags; fs.renameSync
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
  title: Java SE 25 API, java.nio.file.Files
  locator: createTempFile; move (ATOMIC_MOVE)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.memoryextensions.indexof
  title: .NET API reference, MemoryExtensions.IndexOf
  locator: IndexOf<T>(ReadOnlySpan<T>, ReadOnlySpan<T>)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.move
  title: .NET API reference, File.Move
  locator: Move(String, String, Boolean)
- url: https://pkg.go.dev/bytes
  title: Go standard library, package bytes
  locator: Count; ReplaceAll
- url: https://pkg.go.dev/os
  title: Go standard library, package os
  locator: CreateTemp; File.Sync; Rename
- url: https://doc.rust-lang.org/std/fs/fn.rename.html
  title: The Rust Standard Library, std::fs::rename
  locator: Description
- url: https://en.cppreference.com/cpp/string/basic_string/find
  title: cppreference.com, std::basic_string::find
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), awk"
  locator: OPERANDS (assignment); Variables and Special Variables (ENVIRON); String Functions (index)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sed.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sed"
  locator: Regular Expressions in sed; Editing Commands in sed (s)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace
  title: MDN Web Docs, String.prototype.replace()
  locator: Specifying a string as the replacement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replaceAll
  title: MDN Web Docs, String.prototype.replaceAll()
  locator: Parameters
- url: https://docs.python.org/3/library/re.html
  title: The Python Standard Library, re
  locator: re.sub
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: GNU Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion; 4.3.2 The Shopt Builtin (patsub_replacement)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/xargs.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), xargs"
  locator: DESCRIPTION; OPTIONS (-0)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/find.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), find"
  locator: OPERANDS (-exec, -print0)
---

## Task

Replace a text with another text in a list of files. Rosetta Code states the task like this: "Replace every occurring instance of a piece of text in a group of text files with another one."[^1] In its example, the old text is `Goodbye London!` and the new text is `Hello New York!`. The function on this page takes the list of files, the old text and the new text. It returns the number of instances that it replaced in each file.

The task needs rules for what an instance is and for what to do with each file. This page uses these rules:

- **The old text and the new text are plain text, not patterns.** A `.` in the old text matches only a `.`, and a `$` or a `&` in the new text goes into the file as it is. The search is case-sensitive.
- **The code compares bytes.** The old text and the new text are UTF-8 text, and the code compares the bytes of the old text with the bytes of the file. The code does not decode the file, so the bytes that it does not replace do not change. An instance can contain LF bytes, so the old text can go across lines. CRLF is two bytes like the others.
- **Instances do not overlap.** The search starts at the start of the file and finds the first instance. It then goes on after the end of that instance. So in `aaa`, the old text `aa` has one instance, and the file becomes `ba` if the new text is `b`. The code does not search the new text, so a new text that contains the old text does not cause a loop.
- **A file with no instance is not written.** Its bytes, its modification time and its permissions do not change.
- **A changed file is replaced in one step.** The code writes the new text to a new file in the same directory, gives it the permissions of the old file, and renames it to the name of the old file.
- **An empty old text is an error.** An empty text has no clear instances, and the function checks it before it opens a file.
- **The files are done in order, and the first error stops the function.** If a file does not exist or cannot be written, the function reports the error. The files before it have the new text, and the files after it do not change.

## Solutions

The method is the same in each language:

1. Check that the old text is not empty.
2. For each file, read the file as bytes. Find each instance from the left, and copy the bytes between the instances and the new text to a buffer. Count the instances.
3. If the count is 0, go on to the next file.
4. Otherwise, write the buffer to a new file in the same directory, give it the permissions of the old file, write its data to the disk, and rename it to the name of the old file.

The rename is the step that changes the file. POSIX says that during a rename, a directory entry named new "shall remain visible to other threads throughout the renaming operation and refer either to the file referred to by new or old before the operation began."[^2] So a program that opens the file gets the old text or the new text, and never a part of a file. If the function stops before the rename, the old file does not change.

All the solutions except Bash read the whole file into memory. The Bash solution reads the file one line at a time.

- **Python.** The file opens in binary mode. `bytes.count` gives the count: "Return the number of non-overlapping occurrences of subsequence sub in the range [ start , end ]."[^3] Then `bytes.replace` makes the new bytes: "Return a copy of the sequence with all occurrences of subsequence old replaced by new ."[^3] Both find the instances from the left, so the count agrees with the replacement. `tempfile.mkstemp` makes the new file, `shutil.copymode` copies the permissions, `os.fsync` writes the data to the disk, and `os.replace` renames the file. If the new name exists, the file "will be replaced silently if the user has permission."[^4] An empty old text raises `ValueError`.
- **JavaScript and TypeScript (Node.js).** `readFileSync` gives a `Buffer`, and `indexOf(oldBytes, from)` returns "The index of the first occurrence of value in buf , or -1 if buf does not contain value".[^5] The loop keeps the parts of the buffer and the new bytes in an array, and `Buffer.concat` joins them. The new file opens with the flag `'wx'`, which is "Like 'w' but fails if the path exists".[^6] `fsyncSync` writes the data to the disk, `chmodSync` copies the permissions, and `renameSync` renames the file. An empty old text throws a `RangeError`. The TypeScript file adds types only.
- **Java.** `Files.readAllBytes` reads the file, and a loop compares the bytes of the old text with the bytes at each position. `Files.createTempFile(dir, prefix, suffix)` "Creates a new empty file in the specified directory, using the given prefix and suffix strings to generate its name."[^7] A `FileOutputStream` writes the bytes, and `getFD().sync()` writes them to the disk. If the file system has POSIX permissions, `setPosixFilePermissions` copies them. `Files.move` with `ATOMIC_MOVE` renames the file. An empty old text throws an `IllegalArgumentException`.
- **C#.** `File.ReadAllBytes` reads the file. `data.AsSpan(from).IndexOf(oldBytes)` "Searches for the specified sequence and returns the index of its first occurrence."[^8] A `FileStream` with `FileMode.CreateNew` makes the new file, and `Flush(true)` writes the data to the disk. On Unix, `File.SetUnixFileMode` copies the permissions. `File.Move(tmp, path, overwrite: true)` renames the file, with the parameter overwrite set to true, "which replaces the file if it exists".[^9] An empty old text throws an `ArgumentException`.
- **Go.** `os.ReadFile` reads the file. `bytes.Count` "counts the number of non-overlapping instances of sep in s".[^10] `bytes.ReplaceAll` "returns a copy of the slice s with all non-overlapping instances of old replaced by new."[^10] `os.CreateTemp` makes the new file, `Chmod` copies the permissions, and `Sync` "commits the current contents of the file to stable storage."[^11] `os.Rename` renames the file: "If newpath already exists and is not a directory, Rename replaces it."[^11] A deferred function removes the new file if a step fails. An empty old text gives `ErrEmptyOld`.
- **Rust.** `fs::read` reads the file. `windows(old.len()).position(...)` finds the next instance, and the code copies the bytes before it and the new bytes to a `Vec<u8>`. The new file opens with `create_new`, so the code does not write over a file that has the same name. `set_permissions` copies the permissions, `sync_all` writes the data to the disk, and `fs::rename` renames the file, "replacing the original file if to already exists".[^12] An empty old text gives an error of kind `InvalidInput`.
- **C.** `read_file` reads the file into a buffer that `realloc` makes larger. A loop with `memcmp` finds each instance. `mkstemp` makes the new file from the path and the suffix `.XXXXXX`, `fchmod` copies the permissions, `fsync` writes the data to the disk, and `rename` renames the file. `replace_in_files` puts the counts in an array that the caller gives, and returns 0, or -1 with `errno` set. An empty old text sets `errno` to `EINVAL`.
- **C++.** A binary `std::ifstream` reads the file into a `std::string`. `std::string::find(old_text, from)` "Finds the first substring equal to the given character sequence. Search begins at pos".[^13] The new file has a random suffix and opens with `std::ios::noreplace` (C++23). `fs::permissions` copies the permissions, and `fs::rename` renames the file. This solution uses only the standard library, so it does not write the data to the disk before the rename. The result is a `std::expected<std::vector<std::size_t>, ReplaceError>`.
- **Bash.** `replace_in_files OLD NEW FILE...` prints one count on each line. An `awk` program reads the file one line at a time and keeps the text that it did not write in a variable. `index` finds plain text: it returns "the position, in characters, numbering from 1, in string s where string t first occurs, or zero if it does not occur at all."[^14] The program writes the text before each instance and the new text, and keeps only the last bytes, fewer than the length of the old text, because an instance can start there and go on in the next line. `LC_ALL=C` makes `awk` count bytes. The texts go to `awk` in environment variables and `ENVIRON`, not with `-v` (see the pitfalls). `awk` does not tell if the last line ends with LF, so the function looks at the last byte with `tail -c 1`. `mktemp` makes the new file from the template `$file.XXXXXX`, `cp -p` gives it the permissions of the old file, and `mv -f` renames it. On an error, the function returns 2 with a message. It does not write the data to the disk before the rename, and it does not support a NUL byte in a file.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. For each case, the directory `fixtures/NAME` holds the files before the call, and `fixtures/NAME.after` holds the files after the call.

- `rosetta`: `Goodbye London!` becomes `Hello New York!` in two files. The second file has two instances, and a line with `Goodbye` only, which does not change. The result is 1 and 2.
- `some-files-match`: `dog` becomes `fox`. The first file has no `dog`, so it is not written. The result is 0 and 2.
- `one-file`: `1` becomes `2` in a file with two lines. The result is 2.
- `no-files`: an empty list. The result is an empty list.
- `no-match` and `empty-file`: no instance. The result is 0, and the file does not change.
- `delete` and `whole-file`: the new text is empty. `-` goes from `a-b-c`, and `abc` goes from a file that holds only `abc`, which is then empty.
- `new-contains-old`: `a` becomes `aa` in `a a`. The result is 2, and the file is `aa aa`.
- `overlapping` and `adjacent`: `aa` becomes `b`. `aaa` has one instance and becomes `ba`. `aaaa` has two instances and becomes `bb`.
- `special-chars`: `1.5` becomes the text `\&$1 $& [x]`, with each character as it is. `125` does not change, because `.` is not a pattern.
- `glob-chars`: `*.txt` becomes `?`. Only the text `*.txt` changes, and `a.txt` stays.
- `across-lines`: `end`, LF, `start` becomes `end start`. The old text goes across two lines.
- `lf-to-crlf`: LF becomes CRLF. The result is 2.
- `crlf`: `a` becomes `c` in a file with CRLF line ends. The CR bytes do not change.
- `no-final-newline`: the file has no LF at the end, and does not get one.
- `non-ascii`: `é` becomes `e` in `café, cafe`. The old text is two bytes in UTF-8.
- `case-sensitive`: `hello` becomes `bye` in `Hello hello`. `Hello` does not change.

The tests also write a file of 100000 lines, each line `line ` and a number, and replace `line ` with `L`. The result is 100000. Other tests check that a file with mode 640 keeps its mode, that no new file stays in the directory, and that a file with no instance is not written. The cases in `cases/errors.jsonl` must give the error `not-found` or `bad-old`. In the case `stop`, the second file of three does not exist: the first file has the new text, and the third file does not change. The toolchain does not load `cases/errors.jsonl` yet, so each test file holds these cases by name.

## Pitfalls

- **In `sed`, the old text is a regular expression and `&` in the new text is the match.** POSIX says that an `&` "appearing in the replacement shall be replaced by the string matching the RE."[^15] So `s/1.5/[&]/g` also changes `125`, and the new text is the match in brackets. To use `sed` for plain text, put a backslash before each special character of the old text and before each `&`, `\` and delimiter of the new text:

```bash run id=bash-sed-special
# Wrong: in sed, . matches any character and & is the text that matched.
printf '1.5 and 125\n' | sed 's/1.5/[&]/g'
# Right: a backslash before . and & makes each one a plain character.
printf '1.5 and 125\n' | sed 's/1\.5/[\&]/g'
```

```output for=bash-sed-special
[1.5] and [125]
[&] and 125
```

- **In JavaScript, `$` patterns in the new text have a meaning.** For `replace`, "The replacement string can include the following special replacement patterns".[^16] `$&` "Inserts the matched substring.",[^16] and `$$` inserts one `$`. `replaceAll` is the same: "The replacement has the same semantics as that of String.prototype.replace() ."[^17] A function as the replacement does not have this problem, because the code uses its result as it is:

```javascript run id=javascript-replaceall
const text = 'echo PID';
const newText = '$$';
// Wrong: in the replacement string, $$ inserts one $.
console.log(text.replaceAll('PID', newText));
// Right: replaceAll uses the result of a function as it is.
console.log(text.replaceAll('PID', () => newText));
```

```output for=javascript-replaceall
echo $
echo $$
```

- **In Python, `re.sub` changes backslashes in the new text.** If the replacement is a string, "any backslash escapes in it are processed. That is, \n is converted to a single newline character, \r is converted to a carriage return, and so forth."[^18] So a Windows path such as `C:\new` gets an LF. Use `str.replace` or `bytes.replace` for plain text, or give `re.sub` a function:

```python run id=python-re-sub
import re

text = 'path = DIR\n'
new = r'C:\new'
# Wrong: re.sub changes the two characters \n in new to an LF.
print(re.sub('DIR', new, text), end='')
# Right: str.replace uses new as it is.
print(text.replace('DIR', new), end='')
# Also right: re.sub uses the result of a function as it is.
print(re.sub('DIR', lambda match: new, text), end='')
```

```output for=python-re-sub
path = C:
ew
path = C:\new
path = C:\new
```

- **In Bash, `&` in the new text can be the match.** In Bash 5.2 and later, with `patsub_replacement`, an unquoted `&` in the replacement of `${text//old/new}` is the text that matched. The manual says about the option: "This option is enabled by default."[^19] It also says: "Quoting any part of string inhibits replacement in the expansion of the quoted portion, including replacement strings stored in shell variables."[^19] So write `${text//"$old"/"$new"}` with quotes. This expansion is also slow on a large text. In a test on macOS with Bash 3.2, two expansions of this type took 9.5 seconds for a text of 2000 short lines, and 73 seconds for 4000 lines. This is why the Bash solution uses `awk`.
- **`awk -v` changes backslashes.** For an assignment, POSIX says that the characters after the `=` "shall be interpreted as if they appeared in the awk program preceded and followed by a double-quote".[^14] So with `-v new='C:\new'`, the variable holds an LF. The Bash solution puts the texts in environment variables, and reads them from `ENVIRON`, which gives them as they are.
- **`xargs` splits file names at blanks.** If no `-0` option is given, `xargs` splits its input at blanks and newlines, and it gives quotes a meaning.[^20] So `find . -name '*.txt' | xargs sed ...` breaks a file named `my notes.txt` into two names. Use `find ... -exec cmd {} +`, for which "the pathnames for which the primary is evaluated shall be aggregated into sets",[^21] or use `find ... -print0 | xargs -0`. The option `-0` makes `xargs` "Use a null byte as the input argument delimiter and do not treat any other input bytes as special."[^20]
- **A count that steps by one byte counts overlapping instances.** A loop that finds an instance and then searches again from the next byte counts two instances of `aa` in `aaa`, but the replacement changes only one. Continue the search after the end of the instance, as `bytes.Count` in Go does: it "counts the number of non-overlapping instances of sep in s."[^10]
- **An empty old text gives strange results.** In Go, "If sep is an empty slice, Count returns 1 + the number of UTF-8-encoded code points in s."[^10] A replacement with an empty old text puts the new text between each pair of characters. The solutions here check the old text first and report an error.
- **The rename replaces a symbolic link, not its target.** POSIX says: "If either the old or new argument names a symbolic link, rename () shall operate on the symbolic link itself, and shall not resolve the last component of the argument."[^2] If a path in the list is a symbolic link, the solutions read the target, but after the rename the path is a regular file with the new text, and the target does not change. To change the target, resolve the path first, for example with `os.path.realpath` in Python.
- **The new file must be on the same file system.** A temp directory such as `/tmp` can be on a different file system from the old file. Rust says about `fs::rename`: "This will not work if the new name is on a different mount point."[^12] Make the new file in the directory of the old file, as all the solutions here do.
- **An atomic move can fail if the target exists.** For `ATOMIC_MOVE`, Java says: "If the target file exists then it is implementation specific if the existing file is replaced or this method fails by throwing an IOException".[^7] On such a system, the Java solution throws the exception, deletes the new file and does not change the old file.
- **Writing each file changes files that have no instance.** If the code writes each file in the list, a file with no instance gets a new modification time, and a program that looks at the times sees a change that is not there. The solutions here write a file only if its count is more than 0.

## Related tasks

- [[write-a-file-atomically]]: write a new file and rename it over the old one, the method that this page uses.
- [[rename-a-file]]: rename a file, and what happens if the new name exists.
- [[replace-a-substring]]: replace a text with another text in a string in memory.
- [[remove-lines-from-a-file]]: remove a range of lines from a file with the same write and rename steps.
- [[filter-lines-that-match-a-pattern]]: keep the lines in which a regular expression matches.
