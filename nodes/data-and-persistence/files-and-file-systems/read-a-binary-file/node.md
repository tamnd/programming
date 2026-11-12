---
tag: 7S862T
kind: task
slug: read-a-binary-file
name: Read a binary file
aliases: [read the bytes of a file, read a file into a byte array, read a file in binary mode, read exactly n bytes]
area: data-and-persistence/files-and-file-systems
level: core
summary: Read the bytes of a file with no text decoding and no change to line ends or 0 bytes, and read exactly the first n bytes, with an error if the file ends first.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/functions.html#open
    title: The Python Standard Library, Built-in Functions
    locator: open(), the mode argument
  - url: https://docs.python.org/3/library/io.html#io.BufferedIOBase.read
    title: The Python Standard Library, io, Core tools for working with streams
    locator: BufferedIOBase.read, RawIOBase.read
  - url: https://nodejs.org/api/fs.html#fspromisesreadfilepath-options
    title: Node.js documentation, File system
    locator: fsPromises.readFile(path[, options]), filehandle.read(buffer, offset, length, position)
  - url: https://nodejs.org/api/buffer.html#buffers-and-character-encodings
    title: Node.js documentation, Buffer
    locator: Buffers and character encodings, 'utf8'
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#readAllBytes(java.nio.file.Path)
    title: Java SE 25 API, java.nio.file.Files
    locator: readAllBytes(Path)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/InputStream.html#readNBytes(int)
    title: Java SE 25 API, java.io.InputStream
    locator: read(byte[]), readNBytes(int)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.readallbytes?view=net-10.0
    title: .NET API reference, File.ReadAllBytes
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.stream.read?view=net-10.0
    title: .NET API reference, Stream.Read
    locator: Read(Byte[], Int32, Int32), Returns
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.stream.readexactly?view=net-10.0
    title: .NET API reference, Stream.ReadExactly
    locator: Exceptions
  - url: https://pkg.go.dev/os#ReadFile
    title: Go standard library, package os
    locator: func ReadFile
  - url: https://pkg.go.dev/io#ReadFull
    title: Go standard library, package io
    locator: func ReadFull
  - url: https://doc.rust-lang.org/std/fs/fn.read.html
    title: The Rust Standard Library, std::fs::read
  - url: https://doc.rust-lang.org/std/io/trait.Read.html#method.read_exact
    title: The Rust Standard Library, std::io::Read
    locator: read_exact, Errors
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fopen.html
    title: The Open Group, POSIX.1-2024, fopen
    locator: Description, the mode character b
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fread.html
    title: The Open Group, POSIX.1-2024, fread
    locator: Return value
  - url: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170
    title: Microsoft C runtime library reference, fopen, _wfopen
    locator: the translation modes t and b
  - url: https://en.cppreference.com/w/cpp/io/basic_istream/read
    title: cppreference, std::basic_istream::read
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/od.html
    title: The Open Group, POSIX.1-2024, od
    locator: Options, -A, -N, -t, -v
  - url: http://www.libpng.org/pub/png/spec/1.2/PNG-Rationale.html#R.PNG-file-signature
    title: PNG (Portable Network Graphics) Specification, Version 1.2, Rationale
    locator: 12.12 PNG file signature
---

## Task

Read a file as bytes. Give back each byte of the file in its order, with no text decoding. A CR LF stays CR LF, a 0 byte stays a 0 byte, and bytes that are not valid UTF-8 stay as they are.

Then read a fixed number of bytes: give back exactly the first `n` bytes of the file. If the file ends before byte `n`, report an error and give back no bytes. Programs do this to read the header of a file format, for example the 8-byte signature at the start of a PNG file.

If the file does not exist, or if the path names a directory, report an error.

The file must be small enough to keep in memory. To read a large file part by part, see [[read-and-write-with-buffers]]. To read text, see [[read-a-whole-file]]. SQL, regular expressions, HTML and CSS do not apply, because none of them can open a file.

## Solutions

Each solution has two functions: `read_bytes(path)` reads all of the file, and `read_exact(path, n)` reads the first `n` bytes. Each one returns a byte array, not a string.

- **Python.** `open(path, "rb").read()`. "Files opened in binary mode (including 'b' in the mode argument) return contents as bytes objects without any decoding." For `read_exact`, the code calls `read` again until it has `n` bytes, because `BufferedIOBase.read` says "Fewer bytes may be returned than requested". An empty result means the end of the file, and the code raises `EOFError`.
- **JavaScript and TypeScript (Node.js).** `fsPromises.readFile(path)` with no encoding. "If no encoding is specified (using options.encoding), the data is returned as a <Buffer> object." For `read_exact`, the code calls `filehandle.read` in a loop. A `bytesRead` of 0 is the end of the file, and the code throws `UnexpectedEofError`.
- **Java.** `Files.readAllBytes(path)`. For `read_exact`, `Files.newInputStream(path).readNBytes(n)`, which "blocks until the requested number of bytes has been read, end of stream is detected, or an exception is thrown". If the result is shorter than `n`, the code throws `EOFException`.
- **C#.** `File.ReadAllBytes(path)`. For `read_exact`, a `FileStream` and `Stream.ReadExactly(buffer)`, which throws `EndOfStreamException` when "The end of the stream is reached before filling the buffer".
- **Go.** `os.ReadFile(path)`. For `read_exact`, `os.Open` and `io.ReadFull(f, buf)`. `io.ReadFull` gives `io.EOF` when it reads no byte at all, so the code changes `io.EOF` to `io.ErrUnexpectedEOF`. Then each short file gives the same error.
- **Rust.** `std::fs::read(path)` gives a `Vec<u8>`. For `read_exact`, `File::open(path)?.read_exact(&mut buf)`. If the file ends first, the error is of kind `ErrorKind::UnexpectedEof`.
- **C.** `fopen(path, "rb")`, `fstat` to reject a directory with `EISDIR`, then `fread` into a buffer that grows. The function returns the buffer and its length, because the bytes can hold 0 bytes and `strlen` cannot count them. `read_exact` calls `fread` once: POSIX says that `fread` returns fewer elements "only if an error or end-of-file is encountered", and `ferror` tells which. It returns `READ_EXACT_EOF` for a short file.
- **C++.** `std::ifstream(path, std::ios::binary)` and `read` into a `std::vector<std::byte>`. `gcount()` gives the number of bytes that `read` got. The code returns `std::expected<std::vector<std::byte>, ReadError>` (C++23), with `ReadError::unexpected_eof` for a short file.
- **Bash.** A Bash variable cannot hold a 0 byte, so the Bash functions store the bytes as hexadecimal text, two digits for each byte. `od -An -v -tx1` writes each byte in hexadecimal with no offsets, and `-N n` stops after `n` bytes. The function `read_exact` returns status 2 when `od` gives fewer than `2n` digits.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with the files in `fixtures/`. Each expected result is the bytes in lowercase hexadecimal. The cases of `read_exact` are in `cases/exact.jsonl`, and the cases that must give an error are in `cases/errors.jsonl`. The file `.gitattributes` marks the fixtures as `-text`, so Git does not change their line ends.

- `signature`: the first 16 bytes of a PNG file. The signature holds `\r\n`, the byte `0x1A` and a lone `\n`, so that a text transfer changes it. The PNG specification says: "The CR-LF sequence catches bad file transfers that alter newline sequences."
- `empty`: the file has no bytes. `read_bytes` gives an empty array, not an error.
- `all-bytes`: the 256 byte values from `0x00` to `0xFF`, in order.
- `zeros`: 8 bytes of `0x00`.
- `crlf`: `a\r\nb\r\n`. Each `\r` stays.
- `ctrl-z`: `a`, the byte `0x1A`, then `b`. In text mode on Windows, the C runtime reads `0x1A` as the end of the file.
- `not-utf8`: bytes that are not valid UTF-8, such as `0xFF` and `0xFE`. No decoder may see them.
- `read_exact` cases: the first 8 bytes of the signature, the first 4 bytes, exactly all of the file, `n = 0` on an empty file, and 3 bytes across `0x1A`.
- Errors: a missing file, a directory, `n = 17` on the 16-byte signature file, and `n = 1` on an empty file.

The native tests also read a file of 1 MiB. The Python, Go and C tests read 8 bytes from a named pipe that gets `abc` first and `defgh` 0.2 seconds later. One read call gets only `abc`, so a `read_exact` with no loop fails this test.

## Pitfalls

- **Text mode.** A file that is open in text mode goes through a decoder and, in Python, through newline translation. Latin-1 accepts each byte, so there is no decode error here, but the `\r` of the PNG signature is lost:

```python wrong id=python-text-mode
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "image.png")
data = bytes.fromhex("89504e470d0a1a0a0000000d49484452")
with open(path, "wb") as out:
    out.write(data)

# Text mode with an encoding that accepts each byte: no decode error,
# but "\r\n" comes back as "\n".
with open(path, encoding="latin-1") as file:
    text = file.read()
print(len(data), len(text))
print(text.encode("latin-1")[:8].hex())
```

```output
16 15
89504e470a1a0a00
```

With UTF-8, the first byte `0x89` gives `UnicodeDecodeError`. Open the file with `"rb"`:

```python run id=python-binary-mode
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "image.png")
data = bytes.fromhex("89504e470d0a1a0a0000000d49484452")
with open(path, "wb") as out:
    out.write(data)

with open(path, "rb") as file:
    got = file.read()
print(len(data), len(got))
print(got[:8].hex())
```

```output
16 16
89504e470d0a1a0a
```

- **Bytes in a string.** In Node.js, `readFile(path, "utf8")` decodes the bytes. The Buffer documentation says that for bytes that are not valid UTF-8, "the Unicode replacement character U+FFFD � will be used to represent those errors". The string then does not give the old bytes back:

```javascript wrong id=javascript-utf8-round-trip
import { mkdtempSync, readFileSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";

const path = join(mkdtempSync(join(tmpdir(), "png-")), "image.png");
const data = Buffer.from("89504e470d0a1a0a0000000d49484452", "hex");
writeFileSync(path, data);

// The bytes go through a string and back.
const text = readFileSync(path, "utf8");
const back = Buffer.from(text, "utf8");
console.log(data.length, back.length);
console.log(back.subarray(0, 8).toString("hex"));
```

```output
16 18
efbfbd504e470d0a
```

The byte `0x89` became the 3 bytes `ef bf bd` of U+FFFD. Keep the bytes in a `Buffer`, a `byte[]` or a `Vec<u8>`, and do not put them in a string.

- **`strlen` on bytes.** In C, a byte buffer is not a string. `strlen` and `printf("%s")` stop at the first 0 byte:

```c wrong id=c-strlen
#include <stdio.h>
#include <string.h>

int main(void) {
    const unsigned char data[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
                                  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52};
    FILE *out = tmpfile();
    if (out == NULL) return 1;
    fwrite(data, 1, sizeof data, out);
    rewind(out);

    char buf[64] = {0};
    size_t n = fread(buf, 1, sizeof buf, out);
    fclose(out);
    // strlen stops at the first 0 byte.
    printf("%zu %zu\n", n, strlen(buf));
    return 0;
}
```

```output
16 8
```

Keep the count that `fread` returns, and pass it with the buffer.

- **Text mode in C on Windows.** On POSIX systems, the `b` in `"rb"` "shall have no effect". On Windows it matters. The Microsoft C runtime says that in text mode, "Carriage return-line feed (CR-LF) combinations are translated into single line feeds (LF) on input", and "CTRL+Z is interpreted as an end-of-file character on input". So `fopen(path, "r")` can stop at the byte `0x1A` and drop each `\r`. Code that works on Linux can then fail on Windows. Always give `"rb"`, and `std::ios::binary` in C++.
- **One read call is not enough.** A read call can give fewer bytes than it asks for, before the end of the file. This is usual for pipes, sockets and terminals:

```python wrong id=python-one-read
import os

r, w = os.pipe()
os.write(w, b"abc")  # the first part of 8 bytes
data = os.read(r, 8)
print(len(data), data)
```

```output
3 b'abc'
```

Java `InputStream.read(byte[])` "Reads some number of bytes". .NET `Stream.Read` returns a count that "can be less than the number of bytes requested". Use the call that reads again: `readNBytes`, `ReadExactly`, `io.ReadFull`, `read_exact`, or a loop that stops only when a read gives 0 bytes.

- **`io.EOF` from `io.ReadFull`.** In Go, `io.ReadFull` gives `io.ErrUnexpectedEOF` for a short input, but "The error is EOF only if no bytes were read". Code that checks only for `io.ErrUnexpectedEOF` misses an empty file:

```go wrong id=go-readfull-eof
package main

import (
	"errors"
	"fmt"
	"io"
	"strings"
)

func main() {
	// An empty input: ReadFull reads no byte, so the error is io.EOF.
	_, err := io.ReadFull(strings.NewReader(""), make([]byte, 4))
	fmt.Println(errors.Is(err, io.ErrUnexpectedEOF), err)
	// Two of four bytes: now the error is io.ErrUnexpectedEOF.
	_, err = io.ReadFull(strings.NewReader("ab"), make([]byte, 4))
	fmt.Println(errors.Is(err, io.ErrUnexpectedEOF), err)
}
```

```output
false EOF
true unexpected EOF
```

- **0 bytes in a Bash variable.** Bash drops each 0 byte when it stores the output of a command in a variable:

```bash wrong id=bash-nul-byte
file=$(mktemp)
printf 'a\0b' >"$file"
# Some versions of Bash also print a warning here.
{ text=$(<"$file"); } 2>/dev/null
printf '%s %s\n' "$(wc -c <"$file" | tr -d ' ')" "${#text}"
rm -f -- "$file"
```

```output
3 2
```

The file has 3 bytes, but the variable has 2 characters. Keep binary data in files and pipes, or change it to hexadecimal with `od` first.

- **A very large file.** `readAllBytes` and the other functions here keep all of the file in memory. Java `Files.readAllBytes` throws `OutOfMemoryError` "if an array of the required size cannot be allocated, for example the file is larger that 2GB". For a large file, read a part at a time, as in [[read-and-write-with-buffers]].

## Related tasks

- [[read-a-whole-file]]
- [[write-a-file]]
- [[base64-encode-and-decode]]
- [[bits-and-bytes]]

## References
