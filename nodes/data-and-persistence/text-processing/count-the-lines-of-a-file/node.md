---
tag: 87Y514
kind: task
slug: count-the-lines-of-a-file
name: Count the lines of a file
aliases: [line count, count newlines in a file, wc -l, number of lines in a text file]
area: data-and-persistence/text-processing
level: intro
summary: Count the LF bytes of a file as wc -l does, and the lines with a last line that has no LF, by reading the bytes in chunks without decoding them.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/wc.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), wc"
  locator: DESCRIPTION; OPTIONS (-l); STDOUT
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3: Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://docs.python.org/3/library/functions.html#open
  title: The Python Standard Library, Built-in Functions
  locator: open(), the mode 'b' and the newline parameter
- url: https://docs.python.org/3/library/io.html#io.BufferedIOBase.read
  title: The Python Standard Library, io
  locator: BufferedIOBase.read
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: bytes.count; str.splitlines
- url: https://nodejs.org/api/fs.html#fscreatereadstreampath-options
  title: Node.js documentation, File system
  locator: fs.createReadStream(path[, options])
- url: https://nodejs.org/api/buffer.html#bufindexofvalue-byteoffset-encoding
  title: Node.js documentation, Buffer
  locator: buf.indexOf(value[, byteOffset][, encoding])
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/InputStream.html#read(byte%5B%5D)
  title: Java SE 25 API, java.io.InputStream
  locator: read(byte[])
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.stream.read?view=net-10.0
  title: .NET API reference, Stream.Read
  locator: Read(Span<Byte>), Returns
- url: https://learn.microsoft.com/en-us/dotnet/api/system.memoryextensions.count?view=net-10.0
  title: .NET API reference, MemoryExtensions.Count
  locator: Count<T>(Span<T>, T)
- url: https://pkg.go.dev/bytes#Count
  title: Go standard library, package bytes
  locator: func Count
- url: https://pkg.go.dev/io#Reader
  title: Go standard library, package io
  locator: type Reader
- url: https://doc.rust-lang.org/std/io/trait.Read.html#tymethod.read
  title: The Rust Standard Library, std::io::Read
  locator: Read::read, Errors
- url: https://en.cppreference.com/w/c/io/fread.html
  title: cppreference.com, fread
  locator: Return value
- url: https://en.cppreference.com/w/cpp/io/basic_istream/read.html
  title: cppreference.com, std::basic_istream::read
  locator: description
- url: https://en.cppreference.com/w/cpp/algorithm/count.html
  title: cppreference.com, std::count
  locator: description, overload (1)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tail.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), tail"
  locator: OPTIONS (-c)
---

## Task

Count the lines of a file. Each solution returns two numbers:

- **newlines**: the number of LF bytes (`\n`, 0x0A) in the file. This is the number that `wc -l` prints. POSIX says that `wc -l` shall "Write to the standard output the number of <newline> characters in each input file."[^1]
- **lines**: the number of lines, which also counts a last line that has no LF. It is newlines plus 1 if the file is not empty and its last byte is not LF.

The two numbers are different only when the file does not end with LF. POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^2] It defines an incomplete line as "A sequence of one or more non-<newline> characters at the end of the file."[^2] So newlines counts the lines, and lines counts the lines and the incomplete line. Use newlines when the result must agree with `wc -l`. Use lines when an incomplete line must also count, for example when you number the lines of a file.

The file can hold any bytes. The solution does not decode the file, so bytes that are not valid UTF-8 and NUL bytes are not errors. Only LF ends a line. CR alone, VT, FF, NEL (U+0085) and U+2028 do not. In a CRLF file, each CRLF holds one LF, so the count is correct. An empty file gives (0, 0). If the file does not exist, the solution reports an error.

The file can be larger than the memory. So the solution reads it in chunks of 64 KiB, counts the LF bytes in each chunk, and keeps the last byte that it read.

## Solutions

Each solution has the same loop:

1. Open the file for reading as bytes. Set `last` to LF, so that an empty file gives (0, 0).
2. Read a chunk of up to 64 KiB. If the read gives no bytes, the file is at its end.
3. Add the number of LF bytes in the chunk to `newlines`. Set `last` to the last byte of the chunk. Go to step 2.
4. Return `newlines`, and `newlines + 1` if `last` is not LF, else `newlines`.

- **Python.** `open(path, 'rb')`. "Files opened in binary mode (including 'b' in the mode argument) return contents as bytes objects without any decoding."[^3] `f.read(CHUNK)` can return fewer bytes, and "An empty bytes object is returned if the stream is already at EOF."[^4] `chunk.count(b'\n')` counts the LF bytes.[^5] A missing file raises `FileNotFoundError`.
- **JavaScript and TypeScript (Node.js).** A `for await` loop over `fs.createReadStream(path)`. The default `encoding` is `null` and the default `highWaterMark` is 64 KiB, so each chunk is a `Buffer` of up to 64 KiB.[^6] `chunk.indexOf(0x0a, i)` finds the next LF. For a number, "value will be interpreted as an unsigned 8-bit integer value between 0 and 255", and the result is -1 "if buf does not contain value".[^7] A missing file rejects the loop with an error whose `code` is `ENOENT`. The TypeScript file adds types only.
- **Java.** `Files.newInputStream(path)` in try-with-resources, and `in.read(buf)` in a loop. It returns "the total number of bytes read into the buffer, or -1 if there is no more data because the end of the stream has been reached."[^8] A missing file throws `NoSuchFileException`.
- **C#.** `File.OpenRead(path)` and `stream.Read(buffer)`. The result "can be less than the size of the buffer if that many bytes are not currently available, or zero (0) if the buffer's length is zero or the end of the stream has been reached."[^9] `buffer.AsSpan(0, n).Count((byte)'\n')` "Counts the number of times the specified value occurs in the span."[^10] A missing file throws `FileNotFoundException`.
- **Go.** `Count(r io.Reader)` holds the loop, and `CountFile` opens the file. `bytes.Count(buf[:n], []byte{'\n'})` counts the LF bytes: "Count counts the number of non-overlapping instances of sep in s."[^11] A `Read` can return bytes and an error together. "Callers should always process the n > 0 bytes returned before considering the error err."[^12] So the loop counts the bytes first and then checks for `io.EOF`. A missing file gives an error for which `errors.Is(err, fs.ErrNotExist)` is true.
- **Rust.** `count<R: Read>(r)` holds the loop, and `count_lines` opens the file. `Ok(0)` from `read` is the end of the file. "An error of the ErrorKind::Interrupted kind is non-fatal and the read operation should be retried if there is nothing else to do."[^13] So the loop reads again on `Interrupted`. A missing file gives an error of kind `NotFound`.
- **C.** `fopen` with the mode `rb`, `fread` into a static buffer, and `memchr` to find each LF. "fread does not distinguish between end-of-file and error, and callers must use feof and ferror to determine which occurred."[^14] So after the loop, the code calls `ferror`. The counts are `uintmax_t`. On an error, `count_lines` returns -1 and `errno` tells why, for example `ENOENT`.
- **C++.** `std::ifstream` in binary mode, `in.read(buf.data(), buf.size())`, and `std::count`. The last read stops at the end of the file with fewer bytes, and "The number of successfully extracted characters can be queried using gcount()."[^15] `std::count` "Counts the elements that are equal to value".[^16] The function returns `std::expected` (C++23).
- **Bash.** `wc -l` with the file on standard input gives newlines, and "no name shall be written".[^1] `tail -c 1` on the file, piped to `wc -l`, prints 1 if the last byte is LF, and 0 if it is not or the file is empty. For `tail -c`, the number is "measured in bytes".[^17] A missing file gives a nonzero status and no output.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`. The result is (newlines, lines).

- `lf`, `blank-lines` and `crlf`: each line ends with LF or CRLF, so the two numbers are equal. An empty line is a line.
- `empty`: no bytes. The result is (0, 0), not (0, 1).
- `only-newline`: one empty line, so (1, 1).
- `no-final-newline`, `one-line-no-newline` and `crlf-no-final-newline`: the last line has no LF. `one\ntwo\nthree` gives (2, 3), and `x` gives (0, 1).
- `cr-only`: `one\rtwo\r` has no LF, so it gives (0, 1). The file has one incomplete line.
- `other-breaks`: VT, FF, NEL and U+2028 in the middle of a line do not end it, so (1, 1).
- `invalid-utf8` and `nul-bytes`: the bytes `FF FE` and NUL bytes are counted as any other byte.

Each test file also writes a file of 100000 lines and the line `end` with no LF. The file is larger than one chunk of 64 KiB, and the result must be (100000, 100001). The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds this case by name.

## Pitfalls

- **Splitting the text gives the wrong count.** `text.split('\n')` gives one part more than the number of LF bytes, so a file that ends with LF has one line too many. `str.splitlines()` agrees with the LF count when each line ends with LF, because "a terminal line break does not result in an extra line".[^5] But "the boundaries are a superset of universal newlines".[^5] They include FF, NEL and U+2028, so a form feed in the text adds a line:

```python run id=python-split
data = b"one\ntwo\n"
text = data.decode("utf-8")
print("split:", len(text.split("\n")))
print("splitlines:", len(text.splitlines()))
print("count:", data.count(b"\n"))

other = "page\fone\x85two\n"
print("splitlines:", len(other.splitlines()), "count:", other.count("\n"))
```

```output for=python-split
split: 3
splitlines: 2
count: 2
splitlines: 3 count: 1
```

Both methods also need the whole file in memory as decoded text. Count the LF bytes in chunks, as the solutions do.

- **A loop over a text file sees CR as a line end.** In text mode with the default `newline=None`, "universal newlines mode is enabled", and a line can end with `\r`, `\n` or `\r\n`.[^3] So the lines that a loop gives are not the lines that `wc -l` counts:

```python run id=python-universal
import io

data = b"one\rtwo\r"
text_mode = io.TextIOWrapper(io.BytesIO(data), encoding="utf-8")
print("text mode:", sum(1 for _ in text_mode))
print("LF bytes:", data.count(b"\n"))
```

```output for=python-universal
text mode: 2
LF bytes: 0
```

Text mode also decodes the bytes, so a byte that is not valid UTF-8 raises `UnicodeDecodeError`. Open the file in binary mode.

- **`wc -l` does not count a last line with no LF.** It counts LF characters, not lines:[^1]

```bash run id=bash-wc-last-line
printf 'one\ntwo\n' | wc -l
printf 'one\ntwo' | wc -l
```

```output for=bash-wc-last-line
2
1
```

If you need the incomplete line too, check the last byte, as the Bash solution does.

- **The end of the read is not the end of the file.** A read can return fewer bytes than the buffer holds, before the end of the file.[^4][^9] Loop until the read gives no bytes (Python, C#, Rust, C), -1 (Java) or `io.EOF` (Go). Do not stop when a chunk is shorter than the buffer.
- **Only the bytes that the read gave are counted.** Count in `buf[:n]`, not in the whole buffer. After a short read, the end of the buffer still holds bytes of the chunk before it.
- **Reading the whole file in one call.** `f.read()`, `fs.readFileSync`, `Files.readAllBytes` and `File.ReadAllBytes` put the whole file in memory. They are fine for small files, but they fail on a file that is larger than the memory.

## Related tasks

- [[read-a-file-line-by-line]]: read each line as text, with the line terminator removed.
- [[line-ending]]: LF, CRLF and CR, and how programs change them.
- [[read-and-write-with-buffers]]: read a file in chunks through a buffer.
- [[read-a-binary-file]]: read the bytes of a file without decoding them.
- [[get-the-size-of-a-file]]: get the number of bytes without reading them.

## References
