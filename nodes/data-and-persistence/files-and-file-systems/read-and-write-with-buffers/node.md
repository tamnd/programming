---
tag: 2ADZBJ
kind: task
slug: read-and-write-with-buffers
name: Read and write with buffers
aliases: [buffered input and output, buffered reader and writer, copy a file through a buffer, flush a buffer]
area: data-and-persistence/files-and-file-systems
level: core
summary: Copy a file through a buffered reader and a buffered writer of a given size, then flush and close the output and report each error, also the error that comes only at the flush.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/io.html#io.BufferedWriter
    title: The Python Standard Library, io, Core tools for working with streams
    locator: BufferedReader, BufferedWriter, BufferedIOBase.read1
  - url: https://nodejs.org/api/stream.html#buffering
    title: Node.js documentation, Stream
    locator: Buffering; stream.pipeline(source[, ...transforms], destination[, options])
  - url: https://nodejs.org/api/fs.html#filehandlecreatewritestreamoptions
    title: Node.js documentation, File system
    locator: filehandle.createReadStream([options]), filehandle.createWriteStream([options])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/BufferedOutputStream.html
    title: Java SE 25 API, BufferedOutputStream
    locator: BufferedOutputStream(OutputStream out, int size), write(byte[] b, int off, int len)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/BufferedInputStream.html
    title: Java SE 25 API, BufferedInputStream
    locator: BufferedInputStream(InputStream in, int size)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestream.-ctor?view=net-11.0
    title: .NET API reference, FileStream Constructors
    locator: FileStream(String, FileMode, FileAccess, FileShare, Int32), the bufferSize parameter
  - url: https://pkg.go.dev/bufio
    title: Go standard library, package bufio
    locator: type Writer, NewReaderSize, NewWriterSize, Writer.Flush
  - url: https://doc.rust-lang.org/std/io/struct.BufWriter.html
    title: The Rust Standard Library, std::io::BufWriter
    locator: the struct documentation, into_inner
  - url: https://doc.rust-lang.org/std/io/struct.BufReader.html
    title: The Rust Standard Library, std::io::BufReader
    locator: with_capacity
  - url: https://doc.rust-lang.org/std/fs/struct.File.html
    title: The Rust Standard Library, std::fs::File
    locator: the struct documentation, sync_all
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/setvbuf.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), setvbuf
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fclose.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), fclose
    locator: DESCRIPTION
  - url: https://man7.org/linux/man-pages/man2/close.2.html
    title: Linux manual page, close(2)
    locator: NOTES, Dealing with error returns from close()
  - url: https://man7.org/linux/man-pages/man4/full.4.html
    title: Linux manual page, full(4)
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/io/basic_filebuf/setbuf
    title: cppreference, std::basic_filebuf::setbuf
    locator: Notes
  - url: https://en.cppreference.com/w/cpp/io/basic_filebuf/close
    title: cppreference, std::basic_filebuf::close
    locator: Return value
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/dd.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), dd
    locator: DESCRIPTION, OPERANDS (bs=expr)
---

## Task

Copy a file through a buffered reader and a buffered writer. The function gets the path of the source file, the path of the output file and a buffer size in bytes. It returns the number of bytes copied. After the call, the output file holds exactly the bytes of the source file.

A buffer keeps bytes in memory, so that the program asks the operating system for a large block at a time and not for each small piece. Each call to the operating system costs time, and a disk or a network is fastest with large blocks. A buffered writer sends its bytes to the file only when the buffer is full, at a flush, or at the close. Thus the last bytes go to the file only at the flush or the close, and an error from that last write, such as a full disk, also comes only then. The solution must flush and close the output and report that error.

Report an error if the source file does not exist, if the directory of the output file does not exist, or if the size is below 1. If the output file is there, replace it.

To read a small file in one call, see [[read-a-whole-file]] and [[read-a-binary-file]]. SQL, regular expressions, HTML and CSS do not apply, because they do not open files.

## Solutions

Each solution reads the source in a loop. Each read gives at most `size` bytes, and the loop writes those bytes to the buffered writer and stops at the end of the file.

- **Python.** `io.BufferedReader(io.FileIO(src, "r"), buffer_size=size)` and `io.BufferedWriter(io.FileIO(dst, "w"), buffer_size=size)` in one `with` statement. "When writing to this object, data is normally placed into an internal buffer." The `with` statement closes the writer, and the close writes the buffer, so an error from that write comes out of the `with` statement. `read1(size)` makes at most one read from the file and gives `b""` at the end.
- **JavaScript and TypeScript (Node.js).** `fsPromises.open` for each file, then `stream.pipeline` from `filehandle.createReadStream({ highWaterMark: size })`, through a generator that counts the bytes, to `filehandle.createWriteStream({ highWaterMark: size })`. "The highWaterMark option is a threshold, not a limit: it dictates the amount of data that a stream buffers before it stops asking for more data." The promise of `pipeline` resolves when the output stream is closed, and it is rejected if a step fails. The code opens the source first, so that a missing source does not make an empty output file.
- **Java.** `new BufferedInputStream(Files.newInputStream(src), size)` and `new BufferedOutputStream(Files.newOutputStream(dst), size)` in a try-with-resources statement. The close flushes the buffer, and an error from that write comes out of the `try` as an `IOException`. A write of a large array does not go through the buffer: "If the requested length is at least as large as this stream's buffer, however, then this method will flush the buffer and write the bytes directly to the underlying output stream."
- **C#.** Two `FileStream` objects with the `bufferSize` argument, and an explicit `Flush` before the method returns. Then an error from the last write comes out of `Copy` and not out of `Dispose`. The documentation of `bufferSize` says: "A value of 0 or 1 disables buffering."
- **Go.** `bufio.NewReaderSize(in, size)` and `bufio.NewWriterSize(out, size)`, a loop of `Read` and `Write`, then `w.Flush()`. "After all data has been written, the client should call the Writer.Flush method to guarantee all data has been forwarded to the underlying io.Writer." A deferred function returns the error of `out.Close()` if there is no other error. A `bufio.Reader` uses at least 16 bytes: "NewReaderSize returns a new Reader whose buffer has at least the specified size."
- **Rust.** `BufReader::with_capacity(size, File::open(src)?)` and `BufWriter::with_capacity(size, File::create(dst)?)`, a loop of `read` and `write_all`, then `writer.into_inner()`, which flushes the buffer and gives back the error of that write. A `size` of type `usize` cannot be negative, so only 0 is an invalid size there.
- **C.** `fopen` for each file, then `setvbuf(stream, buf, _IOFBF, size)` with a buffer that the code allocates. "{_IOFBF} shall cause input/output to be fully buffered." The buffers must live until `fclose`, so the code frees them after it. The function checks the result of `fclose`, because `fclose` writes the bytes that are still in the buffer: "Any unwritten buffered data for the stream shall be written to the file".
- **C++.** Two `std::filebuf` objects. `pubsetbuf(buf, size)` comes before `open`, because the effect of `setbuf` on a file that is open is not the same in each library. The loop uses `sgetn` and `sputn`, and the code checks the result of `close()`: "If any of the function calls made, including the call to std::fclose(), fails, returns a null pointer". The code returns `std::expected<std::uintmax_t, CopyError>` (C++23).
- **Bash.** `dd if="$src" of="$dst" bs="$size"`. With `bs`, dd reads one block and writes it before it reads the next block: "the data returned from each input block shall be written as a separate output block". dd writes the count to standard error, and the last line starts with the number of bytes, with GNU dd and with BSD dd. The function stores that number in a variable that the caller names.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with the files in `fixtures/`. Each case gives the source, the output path and the buffer size, and the expected result is the number of bytes copied. The output file must then hold the same bytes as the source.

- `text`: 17 bytes of text with a buffer of 4096 bytes. All of the bytes stay in the buffer until the close.
- `several-buffers`: the same 17 bytes with a buffer of 4 bytes, so the buffer fills 4 times before the end.
- `empty`: an empty source gives an empty output file and a count of 0, not an error.
- `smaller-than-buffer`, `exact-buffer`, `one-past-buffer`: 5, 8 and 9 bytes with a buffer of 8 bytes. These cases test the end of the loop at the edge of the buffer: a read that fills the buffer exactly, and a last read of 1 byte.
- `size-one`: a buffer of 1 byte. The copy is slow but correct.
- `binary`: the 256 byte values, so that a text mode or a decoder changes the bytes.
- Errors, in `cases/errors.jsonl`: a missing source, a missing directory for the output, and a size of 0 or -1. A missing source must not make an output file.

The native tests also copy 1 MiB with buffers of 4096 and 4093 bytes, and they replace an output file that is longer than the source. On Linux, they copy 17 bytes and 1 MiB to `/dev/full`. "Writes to the /dev/full device fail with an ENOSPC error." The 17 bytes stay in the buffer, so the error comes only at the flush or the close, and each solution must report it.

## Pitfalls

- **No buffer for small writes.** Each write to an unbuffered stream is one call to the operating system. This raw stream counts the calls and keeps no bytes:

```python wrong id=python-no-buffer
import io


class Counter(io.RawIOBase):
    """A raw stream that counts the calls to write and keeps no bytes."""

    calls = 0

    def writable(self):
        return True

    def write(self, b):
        self.calls += 1
        return len(b)


raw = Counter()
for _ in range(1000):
    raw.write(b"x")  # no buffer: one call to the file for each byte
print("calls:", raw.calls)
```

```output
calls: 1000
```

With a `BufferedWriter` in front of it, the 1000 bytes go to the raw stream in one call, at the close:

```python run id=python-buffer
import io


class Counter(io.RawIOBase):
    """A raw stream that counts the calls to write and keeps no bytes."""

    calls = 0

    def writable(self):
        return True

    def write(self, b):
        self.calls += 1
        return len(b)


raw = Counter()
with io.BufferedWriter(raw, buffer_size=4096) as out:
    for _ in range(1000):
        out.write(b"x")  # the bytes stay in the buffer
    print("calls before the close:", raw.calls)
print("calls after the close:", raw.calls)
```

```output
calls before the close: 0
calls after the close: 1
```

- **No flush.** The bytes in a buffered writer go to the file only at a flush. In Go, the close of the file does not flush the `bufio.Writer`, so the bytes are lost with no error:

```go wrong id=go-no-flush
package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	dir, _ := os.MkdirTemp("", "noflush")
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "out.txt")
	f, _ := os.Create(path)
	w := bufio.NewWriter(f)
	fmt.Fprintln(w, "alpha")
	f.Close() // no w.Flush(): the 6 bytes are still in the buffer
	info, _ := os.Stat(path)
	fmt.Println(info.Size(), "bytes in the file")
}
```

```output
0 bytes in the file
```

- **The error comes after the write.** A write to a buffered writer only puts the bytes in the buffer, so it succeeds also when the file cannot take them. The error comes at the flush or the close. Here the output is a pipe with no reader, so each write to the pipe fails:

```python run id=python-error-at-close
import io
import os

r, w = os.pipe()
os.close(r)  # no reader, so each write to the pipe fails
out = io.BufferedWriter(io.FileIO(w, "w"), buffer_size=4096)
print("write gives", out.write(b"alpha\nbeta\ngamma\n"))
try:
    out.close()  # the 17 bytes go to the pipe only now
except BrokenPipeError as err:
    print("close gives", type(err).__name__)
```

```output
write gives 17
close gives BrokenPipeError
```

- **An error in a destructor is lost.** In Rust, a drop of a `BufWriter` flushes the buffer, but it ignores an error. "It is critical to call flush before BufWriter<W> is dropped. Though dropping will attempt to flush the contents of the buffer, any errors that happen in the process of dropping will be ignored." Call `flush` or `into_inner` and check the result:

```rust wrong id=rust-drop
use std::io::{BufWriter, Write};

fn main() -> std::io::Result<()> {
    let (reader, writer) = std::io::pipe()?;
    drop(reader); // no reader, so each write to the pipe fails
    let mut out = BufWriter::new(writer);
    out.write_all(b"alpha\nbeta\ngamma\n")?; // the bytes stay in the buffer
    println!("write_all gives Ok");
    drop(out); // the flush in drop fails, and drop ignores the error
    println!("drop gives no error");
    Ok(())
}
```

```output
write_all gives Ok
drop gives no error
```

The same is true for the close of a Rust `File`: "Errors detected on closing are ignored by the implementation of Drop." The C++ destructor of `std::filebuf` also closes the file with no way to report an error, so the solution calls `close()` and checks it. In C, check the result of `fclose`. The Linux manual says: "A careful programmer will check the return value of close(), since it is quite possible that errors on a previous write(2) operation are reported only on the final close() that releases the open file description."

- **A buffer that does not live long enough.** In C, the buffer that `setvbuf` gets must exist until the stream is closed. A buffer in a local array of a function that returns before `fclose` gives undefined behavior. In C++, `pubsetbuf` before `open` has an effect, but after `open` the result is not the same in each library.

## Related tasks

- [[read-a-binary-file]]
- [[read-a-whole-file]]
- [[write-a-file]]
- [[append-to-a-file]]
- [[copy-a-file]]
- [[file-system-concept]]

## References
