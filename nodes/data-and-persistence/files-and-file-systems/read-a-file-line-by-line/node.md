---
tag: NEW-read-a-file-line-by-line
kind: task
slug: read-a-file-line-by-line
name: Read a file line by line
aliases: [iterate over the lines of a file, read lines from a text file, stream a text file]
area: data-and-persistence/files-and-file-systems
level: core
summary: Read a UTF-8 text file one line at a time, with only the current line in memory, and report an error for a missing file or bad bytes.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, kotlin, bash, powershell]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
    title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 3, Definitions
    locator: 3.185 Line, 3.172 Incomplete Line
  - url: https://www.rfc-editor.org/rfc/rfc3629.html
    title: F. Yergeau, RFC 3629, UTF-8, a transformation format of ISO 10646 (2003)
    locator: Section 3 and Section 10
  - url: https://docs.python.org/3/library/functions.html#open
    title: The Python Standard Library, Built-in Functions, open()
    locator: the encoding, errors and newline parameters
  - url: https://docs.python.org/3/tutorial/inputoutput.html#methods-of-file-objects
    title: The Python Tutorial, Input and Output
    locator: 7.2.1 Methods of File Objects
  - url: https://peps.python.org/pep-0686/
    title: PEP 686, Make UTF-8 mode default
  - url: https://nodejs.org/api/readline.html#example-read-file-stream-line-by-line
    title: Node.js documentation, Readline
    locator: Example, read file stream line-by-line; the crlfDelay option of createInterface
  - url: https://nodejs.org/api/util.html#new-textdecoderencoding-options
    title: Node.js documentation, Util, class TextDecoder
    locator: the fatal option; the stream option of decode
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#lines(java.nio.file.Path,java.nio.charset.Charset)
    title: Java SE 25 API, java.nio.file.Files
    locator: lines(Path, Charset), lines(Path), readAllLines(Path, Charset)
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.io.path/use-lines.html
    title: Kotlin standard library, kotlin.io.path, useLines
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.readlines?view=net-10.0
    title: .NET API reference, File.ReadLines
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.utf8encoding.-ctor?view=net-10.0
    title: .NET API reference, UTF8Encoding constructors
    locator: UTF8Encoding(Boolean, Boolean), Remarks
  - url: https://pkg.go.dev/bufio#Scanner
    title: Go standard library, package bufio
    locator: MaxScanTokenSize, Scanner, Scanner.Buffer, Scanner.Err, ScanLines
  - url: https://doc.rust-lang.org/std/io/trait.BufRead.html#method.lines
    title: The Rust Standard Library, std::io::BufRead
    locator: lines, read_line (Errors)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getline.html
    title: The Open Group, POSIX.1-2024, getdelim and getline
    locator: Return Value
  - url: https://en.cppreference.com/w/cpp/string/basic_string/getline
    title: cppreference.com, std::getline
  - url: https://c-faq.com/stdio/feof.html
    title: Steve Summit, comp.lang.c FAQ list
    locator: Question 12.2
  - url: https://www.gnu.org/software/bash/manual/bash.html#index-read
    title: GNU Bash Reference Manual, Bash Builtin Commands
    locator: read; wait
  - url: https://git.savannah.gnu.org/cgit/bash.git/tree/NEWS
    title: GNU Bash, NEWS
    locator: new features in bash-4.4, item u
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.management/get-content?view=powershell-7.6
    title: PowerShell 7.6 reference, Get-Content
    locator: -Encoding, -ReadCount
---

## Task

Read a text file one line at a time. Keep only the current line in memory, so the file can be larger than the memory.

So that the tests can check the result, each solution returns two numbers: the number of lines, and the number of Unicode code points in the longest line. The line terminator is not part of a line.

The file is UTF-8 with no byte order mark. A line ends with LF (`\n`) or CRLF (`\r\n`). A CR alone does not occur. The last line can have no terminator, and it is still a line. An empty file has no lines, so the result is (0, 0). A file that holds only `\n` has one empty line.

A solution must accept a line of 1 MiB or more. It can refuse a longer line, but then it reports an error.

If the file does not exist, or its bytes are not valid UTF-8, the solution reports an error and returns no result. It must not put U+FFFD in place of the bad bytes and continue.

SQL, regular expressions, HTML and CSS do not apply, because none of them can open a file and loop over it.

## Solutions

Each solution reads the file through a buffer and handles one line in each step of a loop.

- **Python.** `for line in file` reads one line per step. Give `encoding="utf-8"`, because before Python 3.15 the default encoding comes from the locale. The default `newline=None` changes `\r\n` to `\n`, so the code removes only `\n`.
- **JavaScript and TypeScript (Node.js).** A `for await` loop over `readline.createInterface` on `fs.createReadStream`, with `crlfDelay: Infinity`. The bytes go through a `TextDecoder` with `fatal: true` first, because readline puts U+FFFD in place of bad bytes. The TypeScript file adds types only. It uses async iteration.
- **Java.** `Files.lines(path)` decodes UTF-8 and fills the stream lazily. Put it in try-with-resources, so the file closes. A read error comes out of the stream as `UncheckedIOException`, and the solution throws its cause.
- **Kotlin.** `Path.useLines` gives a lazy sequence of lines and closes the file at the end. It reads with `Files.newBufferedReader`, so bad bytes cause a `MalformedInputException`.
- **C#.** `File.ReadLines` returns the lines one by one. Give it `new UTF8Encoding(false, true)`, because the default decoder replaces bad bytes. `EnumerateRunes` counts code points, not UTF-16 units.
- **Go.** `bufio.Scanner` with `ScanLines` removes `\n` and one `\r` before it. Call `Buffer` to raise the limit, and call `Err` after the loop. The scanner does not decode, so the code calls `utf8.Valid` on each line.
- **Rust.** `BufReader::new(File::open(path)?).lines()` gives one `io::Result<String>` per line, without `\n` or `\r\n`. Bytes that are not UTF-8 give an error of kind `InvalidData`.
- **C.** POSIX `getline` grows one buffer to the longest line and returns -1 at the end or on an error. `ferror` tells the two apart. The code removes `\n` and `\r`, and checks the UTF-8 itself.
- **C++.** `while (std::getline(in, line))` on a binary `std::ifstream`. The code removes a final `\r`, checks the UTF-8 itself, and returns `std::expected` (C++23).
- **Bash (4.4 or later).** `while IFS= read -r line || [[ -n $line ]]` reads from `iconv -f UTF-8 -t UTF-8`, which stops on bad bytes. `wait $!` gets the status of `iconv`. In a UTF-8 locale, `${#line}` counts characters.
- **PowerShell.** `foreach` over `[System.IO.File]::ReadLines` with a strict `UTF8Encoding`. `Get-Content` also sends one line at a time, but it replaces bad bytes. .NET resolves a relative path from the process directory, so the code resolves it from the current location first.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`. The cases that must give an error are in `cases/errors.jsonl`.

- `empty`: no bytes. The result is (0, 0), not (1, 0).
- `only-newline`: one empty line, so the result is (1, 0).
- `no-final-newline` and `crlf-no-final-newline`: the last line has no terminator. Many loops lose it.
- `crlf` and `mixed-endings`: if the `\r` stays, each length is one too large.
- `whitespace`: leading and trailing blanks and a tab are part of the line. Bash `read` without `IFS=` removes them.
- `backslashes`: Bash `read` without `-r` removes the backslashes.
- `long-line`: one line of 1 MiB, more than the default limit of the Go scanner.
- `non-ascii`: characters of 2, 3 and 4 bytes. The emoji is 4 bytes in UTF-8 and 2 units in UTF-16, but 1 code point.
- Errors: a missing file, the bytes `FF FE`, a sequence cut by the end of the file, an encoded surrogate (`ED A0 80`) and an overlong form (`C0 AF`). RFC 3629 does not allow these bytes.

## Pitfalls

- **The lost last line in Bash.** At the end of the file, `read` returns a nonzero status, but it still sets the variable. So `while read` drops a last line that has no `\n`:

```bash wrong id=bash-lost-line
printf 'one\ntwo' | while IFS= read -r line; do echo "got: $line"; done
```

```output
got: one
```

Add `|| [[ -n $line ]]` to the condition:

```bash run id=bash-last-line
printf 'one\ntwo' | while IFS= read -r line || [[ -n $line ]]; do echo "got: $line"; done
```

```output
got: one
got: two
```

- **The 64 KiB limit of the Go scanner.** By default a token can be at most `bufio.MaxScanTokenSize` bytes. On a longer line `Scan` returns false, as at the end of the file. Only `Err` tells you that the scanner stopped early:

```go run id=go-scanner-limit
package main

import (
	"bufio"
	"fmt"
	"strings"
)

func main() {
	text := "short\n" + strings.Repeat("x", 70000) + "\nlast\n"
	s := bufio.NewScanner(strings.NewReader(text))
	lines := 0
	for s.Scan() {
		lines++
	}
	fmt.Println("lines:", lines)
	fmt.Println("error:", s.Err())
}
```

```output
lines: 1
error: bufio.Scanner: token too long
```

- **Methods that read the whole file.** Python `file.readlines()` and `list(file)`, Java `Files.readAllLines` and C# `File.ReadAllLines` make a list of all the lines. Loop over the lines instead.
- **A trailing `\r` with CRLF.** The Go scanner, Rust `lines`, Python, Java and .NET remove it. C `getline`, C++ `std::getline` and Bash `read` keep it, so the code must remove it. Node.js readline can see `\r` and `\n` as two line ends if they arrive more than `crlfDelay` milliseconds apart.
- **Silent replacement of bad bytes.** Node.js readline, PowerShell `Get-Content`, C# `File.ReadLines(path)` and Kotlin `File.bufferedReader` put U+FFFD in place of bad bytes and give no error. Python before 3.15 uses the locale encoding, so a UTF-8 file can decode with the wrong encoding and give no error.
- **`feof` loops in C.** `while (!feof(f))` tests for the end before the read. `feof` becomes true only after a read fails, so the loop body runs once more with the old line. Test the return value of `getline` or `fgets`.

## References
