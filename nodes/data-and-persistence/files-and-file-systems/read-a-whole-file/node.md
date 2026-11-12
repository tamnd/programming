---
tag: GBTP51
kind: task
slug: read-a-whole-file
name: Read a whole file
aliases: [read a file into a string, read an entire file, slurp a file]
area: data-and-persistence/files-and-file-systems
level: intro
summary: Read all of a UTF-8 file in one call and return its text with no change to line ends, a byte order mark or final newlines. Report an error for a missing file or bad bytes.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/io.html#io.TextIOWrapper
    title: The Python Standard Library, io, Core tools for working with streams
    locator: class io.TextIOWrapper, the newline parameter
  - url: https://peps.python.org/pep-0686/
    title: PEP 686, Make UTF-8 mode default
  - url: https://nodejs.org/api/fs.html#fspromisesreadfilepath-options
    title: Node.js documentation, File system
    locator: fsPromises.readFile(path[, options])
  - url: https://nodejs.org/api/util.html#new-textdecoderencoding-options
    title: Node.js documentation, Util, class TextDecoder
    locator: the fatal and ignoreBOM options
  - url: https://nodejs.org/api/buffer.html#buffers-and-character-encodings
    title: Node.js documentation, Buffer
    locator: Buffers and character encodings, 'utf8'; buffer.constants.MAX_STRING_LENGTH
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#readString(java.nio.file.Path)
    title: Java SE 25 API, java.nio.file.Files
    locator: readString(Path), readAllBytes(Path)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.readalltext?view=net-10.0
    title: .NET API reference, File.ReadAllText
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.readallbytes?view=net-10.0
    title: .NET API reference, File.ReadAllBytes
  - url: https://pkg.go.dev/os#ReadFile
    title: Go standard library, package os
    locator: func ReadFile
  - url: https://doc.rust-lang.org/std/fs/fn.read_to_string.html
    title: The Rust Standard Library, std::fs::read_to_string
    locator: Errors
  - url: https://doc.rust-lang.org/std/fs/fn.read.html
    title: The Rust Standard Library, std::fs::read
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fread.html
    title: The Open Group, POSIX.1-2024, fread
    locator: Return Value
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fopen.html
    title: The Open Group, POSIX.1-2024, fopen
    locator: Errors, EISDIR
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
    title: The Open Group, POSIX.1-2024, sys/stat.h
    locator: st_size
  - url: https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator
    title: cppreference.com, std::istreambuf_iterator
  - url: https://en.cppreference.com/w/cpp/io/basic_ifstream
    title: cppreference.com, std::basic_ifstream
  - url: https://www.gnu.org/software/bash/manual/bash.html#Command-Substitution
    title: GNU Bash Reference Manual, Command Substitution
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.management/get-content?view=powershell-7.6
    title: PowerShell 7.6 reference, Get-Content
    locator: -Raw
  - url: https://www.rfc-editor.org/rfc/rfc3629.html
    title: F. Yergeau, RFC 3629, UTF-8, a transformation format of ISO 10646 (2003)
    locator: Section 3 and Section 10
---

## Task

Read all of a text file in one call, and return its text as one string.

The file is UTF-8. The solution returns the text with no change. A CRLF (`\r\n`) stays CRLF, a lone CR stays, a byte order mark stays as U+FEFF, and all final newlines stay. An empty file gives the empty string.

If the file does not exist, if the path names a directory, or if the bytes are not valid UTF-8, the solution reports an error and returns no text. It must not put U+FFFD in place of bad bytes and continue.

The whole file goes into memory, so use this only for a file that is small enough. For a large file, [[read-a-file-line-by-line]] keeps only one line in memory. To get the bytes with no decoding, see [[read-a-binary-file]].

SQL, regular expressions, HTML and CSS do not apply, because none of them can open a file.

## Solutions

Most languages have one call that reads all of a file. The differences are in what the call does to the text.

- **Python.** `open(path, encoding="utf-8", newline="").read()`. Give `encoding`, because before Python 3.15 the default encoding comes from the locale. Give `newline=""`, because the default `newline=None` changes `\r\n` and `\r` to `\n`. The codec `"utf-8"` keeps a byte order mark. `pathlib.Path.read_bytes` gives the bytes.
- **JavaScript and TypeScript (Node.js).** `fsPromises.readFile(path)` gives a `Buffer`. A `TextDecoder` with `fatal: true` and `ignoreBOM: true` decodes it. With no options, `TextDecoder` removes a byte order mark and puts U+FFFD in place of bad bytes. `readFile(path, "utf8")` also puts U+FFFD in place of bad bytes. The TypeScript file adds types only.
- **Java.** `Files.readString(path)`, from Java 11, decodes UTF-8. It throws `MalformedInputException` on bad bytes, and it keeps a byte order mark. `Files.readAllBytes(path)` gives the bytes.
- **C#.** `File.ReadAllBytes(path)`, then `new UTF8Encoding(false, true).GetString(bytes)`. The second argument `true` makes bad bytes throw `DecoderFallbackException`. Do not use `File.ReadAllText(path)` here: it uses a byte order mark to find the encoding and removes it, and it puts U+FFFD in place of bad bytes.
- **Go.** `os.ReadFile(path)` gives a `[]byte`. It does not decode, so the code calls `utf8.Valid` before it converts the bytes to a `string`.
- **Rust.** `fs::read_to_string(path)` returns `io::Result<String>`. Bytes that are not valid UTF-8 give an error of kind `InvalidData`. `fs::read(path)` gives a `Vec<u8>`.
- **C.** ISO C has no call for a whole file. The code opens the file with `fopen(path, "rb")`, uses `fstat` to refuse a directory and to guess the size, and calls `fread` until it gives less than it was asked for. Then `ferror` tells an error from the end of the file. The code checks the UTF-8 itself, and returns a buffer with a 0 byte after the text.
- **C++.** A binary `std::ifstream` and two `std::istreambuf_iterator<char>` put all the bytes into a `std::string`. The code checks for a directory first, checks the UTF-8 itself, and returns `std::expected` (C++23).
- **Bash.** `text=$(iconv -f UTF-8 -t UTF-8 -- "$path" && printf x)`, then `text=${text%x}`. `iconv` stops with an error on bad bytes, and the `x` keeps the final newlines. A Bash variable cannot hold a 0 byte.

PowerShell `Get-Content -Raw` returns all of a file as one string. Without `-Raw`, it returns an array of lines.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`. The cases that must give an error are in `cases/errors.jsonl`. The native tests also read a file of 1 MiB.

- `empty`: no bytes. The result is the empty string, not an error.
- `no-final-newline`: the text ends with no `\n`. Nothing is added.
- `trailing-newlines`: `end\n\n\n`. Bash `$(...)` removes all three newlines.
- `crlf` and `mixed-endings`: `\r\n` and a lone `\r` stay. Python with the default `newline` changes them to `\n`.
- `bom`: the bytes `EF BB BF` at the start become U+FEFF. C# `File.ReadAllText` and a default `TextDecoder` remove it.
- `whitespace`: leading and trailing blanks and a tab stay.
- `non-ascii`: characters of 2, 3 and 4 bytes.
- Errors: a missing file, a directory, the bytes `FF FE`, a sequence cut by the end of the file, an encoded surrogate (`ED A0 80`) and an overlong form (`C0 AF`). RFC 3629 does not allow these bytes.

## Pitfalls

- **Line ends that change in Python.** With the default `newline=None`, Python changes `\r\n` to `\n` when it reads text:

```python wrong id=python-newline
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "crlf.txt")
with open(path, "wb") as f:
    f.write(b"one\r\ntwo\r\n")

with open(path, encoding="utf-8") as f:
    print(repr(f.read()))
```

```output
'one\ntwo\n'
```

Give `newline=""` to get the text as it is in the file:

```python run id=python-newline-kept
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "crlf.txt")
with open(path, "wb") as f:
    f.write(b"one\r\ntwo\r\n")

with open(path, encoding="utf-8", newline="") as f:
    print(repr(f.read()))
```

```output
'one\r\ntwo\r\n'
```

- **Final newlines that Bash removes.** Command substitution removes all newlines at the end of the output, and `$(<file)` is a command substitution:

```bash wrong id=bash-lost-newlines
file=$(mktemp)
printf 'end\n\n\n' >"$file"
text=$(<"$file")
printf '%d characters: %q\n' "${#text}" "$text"
rm -f -- "$file"
```

```output
3 characters: end
```

Print one more character after the file, and then remove it:

```bash run id=bash-kept-newlines
file=$(mktemp)
printf 'end\n\n\n' >"$file"
text=$(cat -- "$file" && printf x)
text=${text%x}
printf '%d characters: %q\n' "${#text}" "$text"
rm -f -- "$file"
```

```output
6 characters: $'end\n\n\n'
```

- **Silent replacement of bad bytes.** Node.js `readFile(path, "utf8")`, C# `File.ReadAllText` and a default `TextDecoder` put U+FFFD in place of bad bytes and give no error. Go `os.ReadFile` does not check the bytes at all. Later code then works on text that is not the text of the file:

```javascript wrong id=node-replacement
import { mkdtempSync, readFileSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";

const path = join(mkdtempSync(join(tmpdir(), "bad-")), "bad.txt");
writeFileSync(path, Uint8Array.from([0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a]));

const text = readFileSync(path, "utf8");
console.log(JSON.stringify(text));
```

```output
"ok\n��\n"
```

A `TextDecoder` with `fatal: true` throws a `TypeError`:

```javascript run id=node-fatal-decoder
import { mkdtempSync, readFileSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";

const path = join(mkdtempSync(join(tmpdir(), "bad-")), "bad.txt");
writeFileSync(path, Uint8Array.from([0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a]));

const decoder = new TextDecoder("utf-8", { fatal: true, ignoreBOM: true });
try {
  decoder.decode(readFileSync(path));
} catch (err) {
  console.log(err.name + ": " + err.code);
}
```

```output
TypeError: ERR_ENCODING_INVALID_ENCODED_DATA
```

- **A byte order mark that C# removes.** `File.ReadAllText(path)` uses a byte order mark to find the encoding, and the mark is not in the result. Here it also changes the bad byte `FF` to U+FFFD:

```csharp wrong id=csharp-readalltext
using System;
using System.IO;

string path = Path.Combine(Directory.CreateTempSubdirectory().FullName, "bom.txt");
File.WriteAllBytes(path, [0xEF, 0xBB, 0xBF, (byte)'h', (byte)'i', 0xFF]);

string text = File.ReadAllText(path);
foreach (char c in text) Console.Write($"U+{(int)c:X4} ");
Console.WriteLine();
```

```output
U+0068 U+0069 U+FFFD
```

A strict `UTF8Encoding` keeps the mark. With the byte `FF`, it would throw:

```csharp run id=csharp-strict-utf8
using System;
using System.IO;
using System.Text;

string path = Path.Combine(Directory.CreateTempSubdirectory().FullName, "bom.txt");
File.WriteAllBytes(path, [0xEF, 0xBB, 0xBF, (byte)'h', (byte)'i', (byte)'\n']);

var strict = new UTF8Encoding(false, true);
string text = strict.GetString(File.ReadAllBytes(path));
foreach (char c in text) Console.Write($"U+{(int)c:X4} ");
Console.WriteLine();
```

```output
U+FEFF U+0068 U+0069 U+000A
```

- **Files that are too large.** The whole file must fit in memory, and some languages have a lower limit. Java `Files.readString` and `Files.readAllBytes` throw `OutOfMemoryError` if the file is "extremely large, for example larger than 2GB". In Node.js, a string can have at most `buffer.constants.MAX_STRING_LENGTH` UTF-16 code units. Read a large file line by line or in blocks.
- **A size from `stat` that is wrong.** Code that calls `fstat`, allocates `st_size` bytes and calls `fread` once can lose data. The file can grow before the read, and POSIX does not say what `st_size` means for most types of file other than a regular file. `fread` can also give fewer bytes than it was asked for. Read in a loop until `fread` gives less, and then call `ferror`.
- **A directory.** POSIX `fopen` gives the error `EISDIR` for a directory only when the mode "requires write access". So `fopen(path, "rb")` on a directory can succeed. The C and C++ solutions check the type of the file before they read.

## Related tasks

- [[read-a-file-line-by-line]]
- [[read-a-binary-file]]
- [[write-a-file]]
- [[append-to-a-file]]

## References
