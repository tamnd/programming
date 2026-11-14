---
tag: A4JS2V
kind: task
slug: read-a-specific-line-from-a-file
name: Read a specific line from a file
aliases: [read line N of a file, get the nth line of a file, sed -n Np, print one line of a file]
area: data-and-persistence/text-processing
level: intro
summary: Read a UTF-8 file up to line N and return that line, or no line if the file has fewer lines, with line numbers that start at 1 and with only LF as the end of a line.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), awk"
  locator: OPERANDS; Variables and Special Variables (NR); Actions (exit)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3, Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://docs.python.org/3/library/functions.html#open
  title: The Python Standard Library, Built-in Functions
  locator: open (newline)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
  title: MDN Web Docs, String.prototype.split()
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.stream.readbyte
  title: .NET API reference, Stream.ReadByte
- url: https://pkg.go.dev/bufio
  title: Go standard library, package bufio
  locator: Reader.ReadString; ScanLines
- url: https://doc.rust-lang.org/std/io/trait.BufRead.html
  title: The Rust Standard Library, std::io::BufRead
  locator: read_line; lines
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getdelim.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), getdelim, getline"
  locator: DESCRIPTION; RETURN VALUE
- url: https://en.cppreference.com/w/cpp/string/basic_string/getline
  title: cppreference.com, std::getline
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/head.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), head"
  locator: OPTIONS (-n)
- url: https://docs.python.org/3/library/linecache.html
  title: The Python Standard Library, linecache
  locator: getline
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/BufferedReader.html
  title: Java SE 25 API, java.io.BufferedReader
  locator: readLine
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.streamreader.readline
  title: .NET API reference, StreamReader.ReadLine
  locator: Remarks
- url: https://nodejs.org/api/readline.html
  title: Node.js documentation, Readline
  locator: "Event: 'line'"
---

## Task

Read a text file and a line number N, and return line N of the file. Line 1 is the first line. If the file has fewer than N lines, or if N is less than 1, return no line. `sed -n 'Np'` and `awk 'NR == N'` do this job in a shell. In awk, `NR` is "The ordinal number of the current record from the start of input."[^1]

The task needs rules for what a line is. This page uses these rules:

- **A line ends at LF.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^2] The LF is not part of the line in the result. A CR before the LF stays in the line, and a CR that is not before an LF does not end a line.
- **A last line with no LF is a line too.** POSIX calls it an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file."[^2]
- **An empty line is a line.** It is different from no line. The result type tells the two apart: an empty string is an empty line, and null (or `None`, or an empty `Optional`) is no line.
- **The code stops at the line.** It does not read the rest of the file.

If the file does not exist, the solution reports an error.

## Solutions

The method is the same in each language. Read the file one line at a time and count the lines. When the count is N, return the line and stop. If the file ends first, return no line. A file has no index of its lines, so the code must read all the lines before line N to find where line N starts.

- **Python.** `open(path, encoding='utf-8', newline='\n')` opens the file as text. With a value such as `'\n'`, "input lines are only terminated by the given string, and the line ending is returned to the caller untranslated",[^3] so a CR stays in the line. `enumerate(f, start=1)` gives each line with its number, and `removesuffix('\n')` removes the LF. The function returns `None` if there is no line.
- **JavaScript and TypeScript (Node.js).** `readFileSync(path, 'utf8')` reads the file, and `split('\n')` gives the lines. If the last part is empty, the code removes it: MDN says that if the separator is at the end of the string, there is "an empty (i.e., zero length) string appearing at the first (or last) position of the returned array."[^4] The function returns `lines[number - 1]`, or `null` if `number` is not an integer from 1 to the number of lines. This solution reads the whole file, because the code is short. The TypeScript file adds types only.
- **Java.** A `BufferedInputStream` gives the bytes one at a time. The code counts the LF bytes, keeps the bytes of line N in a `ByteArrayOutputStream`, and decodes them as UTF-8 at the LF. The result is an `Optional<String>`. The code does not use `BufferedReader.readLine`, because it also ends a line at a CR (see Pitfalls).
- **C#.** A `BufferedStream` gives the bytes. `ReadByte` "Reads a byte from the stream and advances the position within the stream by one byte, or returns -1 if at the end of the stream."[^5] The code keeps the bytes of line N in a `MemoryStream` and decodes them with `Encoding.UTF8.GetString`. The result is a `string?`. The code does not use `StreamReader.ReadLine`, for the same reason as in Java.
- **Go.** `bufio.Reader.ReadString('\n')` reads each line: it "reads until the first occurrence of delim in the input, returning a string containing the data up to and including the delimiter."[^6] `strings.TrimSuffix` removes the LF. `ReadLine` returns the line, a `bool` that is false if there is no line, and an `error`. The code does not use `bufio.Scanner`, because its `ScanLines` removes a CR before the LF: "The end-of-line marker is one optional carriage return followed by one mandatory newline."[^6] A `Scanner` also has a maximum line length.
- **Rust.** `BufRead::read_line` "Reads all bytes until a newline (the 0xA byte) is reached, and append them to the provided String buffer."[^7] It returns 0 at the end of the file. The code calls it N times, and then `pop` removes the LF. The result is an `io::Result<Option<String>>`. The code does not use `lines`, because "Each string returned will not have a newline byte (the 0xA byte) or CRLF",[^7] so a CR before the LF goes away.
- **C.** `getline` reads each line into a buffer that it grows as necessary. POSIX says that `getdelim` "shall read from stream until it encounters a character matching the delimiter character",[^8] and that `getline` is `getdelim` with an LF as the delimiter. The return value is "the number of bytes written into the buffer, including the delimiter character if one was encountered before EOF",[^8] so the code knows the length of the line and can remove the LF. `read_line` returns 1 and gives the line and its length, which the caller frees. It returns 0 if there is no line, or -1 with `errno` set on an error.
- **C++.** `std::getline` reads a binary `std::ifstream` one line at a time and removes the LF. At the end of the file, it fails only "If no characters were extracted for whatever reason (not even the discarded delimiter)",[^9] so a last line with no LF is a line too. The result is a `std::expected` (C++23) that holds a `std::optional<std::string>`, and the errors are `cannot_open` and `read_failed`.
- **Bash.** The awk program `NR == n { print; found = 1; exit } END { exit !found }` prints line N, and `-v` sets the awk variable `n` to N. "The exit statement shall invoke all END actions in the order in which they occur in the program source and then terminate the program without reading further input."[^1] So awk stops at the line, and the `END` action gives the status: 0 if it found the line, 1 if not. The file comes on the standard input, not as an operand. For awk, an operand that starts with a letter or an underscore, followed by letters, digits or underscores and then `=`, is an assignment, not a file.[^1] So a file with the name `x=1` would not be read. The function returns 2 if the file cannot be read.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`.

- `first`, `middle` and `last`: lines 1, 2 and 3 of `a`, `b`, `c` are `a`, `b` and `c`.
- `past-the-end`: line 3 of a file with two lines is no line.
- `zero` and `negative`: line 0 and line -1 are no line. Line numbers start at 1.
- `empty`: line 1 of an empty file is no line.
- `empty-line` and `only-newline`: line 2 of `a`, empty, `b` is an empty line, and line 1 of a file with one LF is an empty line.
- `no-final-newline`: line 2 of `a`, `b` with no LF at the end is `b`.
- `crlf`: line 1 of `a` CRLF, `b` CRLF is `a` CR.
- `lone-cr`: line 2 of `a` CR `b` LF `c` LF is `c`, because the CR does not end a line.
- `non-ascii`: line 2 of `café`, `zoë` is `zoë`.

The tests also write a file of 100000 lines with the numbers 0 to 99999. Line 100000 must be `99999`, and line 100001 must be no line. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **Line numbers start at 1, and indexes start at 0.** Line N is at index N - 1 in a list of lines. If the code does not subtract 1, it returns the next line, and for the last line it reads past the end of the list.
- **`head | tail` gives the last line of a short file.** `head -n N FILE | tail -n 1` is a common way to get line N. But for `head`, "When a file contains less than number bytes or lines, it shall be copied to standard output in its entirety. This shall not be an error."[^10] So if the file has fewer than N lines, `tail -n 1` gives the last line of the file, not no line. `sed -n 'Np'` and `awk 'NR == N'` give no output:

```bash run id=bash-head-tail
f=$(mktemp)
printf 'a\nb\nc\n' >"$f"
echo "head -n 5 | tail -n 1:"
head -n 5 "$f" | tail -n 1
echo "sed -n 5p:"
sed -n 5p "$f"
echo "awk 'NR == 5':"
awk 'NR == 5' "$f"
rm "$f"
```

```output for=bash-head-tail
head -n 5 | tail -n 1:
c
sed -n 5p:
awk 'NR == 5':
```

- **`linecache.getline` hides the difference between an empty line and no line.** The function "will return '' on errors (the terminating newline character will be included for lines that are found)."[^11] So an empty line is `'\n'` and no line is `''`. If the code removes the LF first, the two values are the same:

```python run id=python-linecache
import linecache, os, tempfile

fd, path = tempfile.mkstemp()
with os.fdopen(fd, "w") as f:
    f.write("a\n\nb\n")
for number in [2, 9]:
    line = linecache.getline(path, number)
    print(number, repr(line), repr(line.rstrip("\n")))
os.remove(path)
```

```output for=python-linecache
2 '\n' ''
9 '' ''
```

- **Some line readers also end a line at a CR.** Java `BufferedReader.readLine` says: "A line is considered to be terminated by any one of a line feed ('\n'), a carriage return ('\r'), a carriage return followed immediately by a line feed, or by reaching the end-of-file (EOF)."[^12] .NET `StreamReader.ReadLine` says: "A line is defined as a sequence of characters followed by a line feed",[^13] a carriage return, or a CRLF. In Node.js, "The 'line' event is emitted whenever the input stream receives an end-of-line input",[^14] and an end-of-line input is LF, CR or CRLF. In a file that has a CR alone, these readers count more lines than `awk` or `sed`, so line N is a different line:

```javascript run id=javascript-readline-cr
import { createInterface } from "node:readline";
import { Readable } from "node:stream";

const lines = [];
for await (const line of createInterface({ input: Readable.from(["a\rb\nc\n"]) })) {
  lines.push(line);
}
console.log(lines);
console.log("a\rb\nc\n".split("\n"));
```

```output for=javascript-readline-cr
[ 'a', 'b', 'c' ]
[ 'a\rb', 'c', '' ]
```

- **Reading the whole file for one line.** A solution that reads all the lines into memory needs memory for the whole file, also when N is 1. For a large file, read one line at a time and stop at line N, as all the solutions here do except the JavaScript solution.

## Related tasks

- [[count-the-lines-of-a-file]]: count the lines of a file, with the same rules for a line.
- [[print-the-first-and-last-lines-of-a-file]]: the first N lines and the last N lines of a file.
- [[filter-lines-that-match-a-pattern]]: keep the lines in which a regular expression matches.
- [[line-ending]]: LF, CRLF and CR, and why a CR stays in the line.
