---
tag: PKH23E
kind: technique
slug: process-a-large-text-file-as-a-stream
name: Process a large text file as a stream
aliases: [stream a file, read a large file line by line, process a file larger than memory, streaming text processing, constant-memory file processing]
area: data-and-persistence/text-processing
level: core
summary: >-
  Read a text file one line at a time and keep only the state that the result needs, so that the memory of the program does not grow with the size of the file.
see_also: [read-a-file-line-by-line, read-a-whole-file, read-and-write-with-buffers, count-the-lines-of-a-file, compute-word-frequencies, sort-the-lines-of-a-file, tail-a-growing-file, line-ending]
sources:
- url: https://docs.python.org/3/tutorial/inputoutput.html
  title: The Python Tutorial, Input and Output
  locator: 7.2.1 Methods of File Objects
- url: https://doc.rust-lang.org/std/io/trait.BufRead.html
  title: The Rust Standard Library, std::io::BufRead
  locator: read_line; read_until
- url: https://docs.python.org/3/library/tracemalloc.html
  title: The Python Standard Library, tracemalloc, Trace memory allocations
  locator: get_traced_memory
- url: https://www.gnu.org/software/coreutils/manual/html_node/sort-invocation.html
  title: GNU Coreutils manual, sort invocation
  locator: -S, --buffer-size; -T, --temporary-directory
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html
  title: Java SE 25 API, java.nio.file.Files
  locator: lines(Path, Charset); readAllLines(Path, Charset); readAllBytes
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.readlines?view=net-10.0
  title: .NET API reference, File.ReadLines Method
  locator: Remarks
- url: https://pkg.go.dev/bufio
  title: Go standard library, package bufio
  locator: MaxScanTokenSize; ErrTooLong; type Scanner; func (*Scanner) Buffer
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getdelim.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), getdelim, getline"
  locator: DESCRIPTION
- url: https://nodejs.org/api/readline.html
  title: Node.js documentation, Readline
  locator: readline.createInterface (crlfDelay); Example, Read file stream line-by-Line
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), awk"
  locator: DESCRIPTION
---

## Purpose

A program that reads a whole file into memory needs memory for the whole file, and often for more than that: a list of lines holds one object for each line. When the file is larger than the memory, the program fails. When the file is only large, the program is slow to start, because it does no work until the read ends.

Many results do not need the whole file. A line count, a sum, a maximum, a filter or a word count needs one line at a time and a small state. The technique reads the file as a stream: it reads one line, updates the state, and then forgets the line. The memory of the program then depends on the length of the longest line and on the size of the state, not on the size of the file. The Python tutorial says about a loop over a file: "This is memory efficient, fast, and leads to simple code".[^1]

## Steps

1. **Open the file with a buffered reader.** The reader reads large blocks from the file and gives the program one line at a time. Rust says: "reading line-by-line is inefficient without using a buffer".[^2]
2. **Read one line in each iteration of a loop.** Use the line reader of the language, not a call that returns all the lines. Decide what a line is: whether the LF stays in the line, and what to do with a CR before it (see [[line-ending]]).
3. **Keep only the state that the result needs.** Keep a count, a sum, a maximum, or a map of counts. Do not add each line to a list. If a line must stay after the next read, copy it, because some readers use the same buffer again for the next line.
4. **Write each output line as soon as it is ready,** through a buffered writer. Then the output also does not wait for the end of the input.
5. **Check for an error after the loop.** Some readers stop the loop on an error and on the end of the file in the same way, and keep the error for a separate call.
6. **Close the file,** also when the loop ends early with an error. Use the scope construct of the language: `with`, `try` with resources, `defer`, or a value that closes the file when it goes out of scope.

## Example

In Python. The program writes a file of 200000 lines with the numbers 0 to 999, and then gets the number of lines, their sum and their maximum in two ways. `read_all` reads the whole file and makes a list. `stream` reads one line at a time and keeps three numbers. `tracemalloc.get_traced_memory` gives "the current size and peak size of memory blocks traced by the tracemalloc module".[^3] The two functions give the same result. The peak memory of `read_all` is more than 5 times the size of the file, and the peak memory of `stream` is less than a tenth of it:

```python run id=python-stream
import os
import tempfile
import tracemalloc

directory = tempfile.mkdtemp()
path = os.path.join(directory, 'numbers.txt')
with open(path, 'w', encoding='utf-8') as f:
    for i in range(200_000):
        f.write(f'{i % 1000}\n')
size = os.path.getsize(path)
print('file size:', size, 'bytes')


def read_all(path):
    with open(path, encoding='utf-8') as f:
        numbers = [int(line) for line in f.read().splitlines()]
    return len(numbers), sum(numbers), max(numbers)


def stream(path):
    count = total = 0
    largest = None
    with open(path, encoding='utf-8') as f:
        for line in f:  # one line at a time
            n = int(line)
            count += 1
            total += n
            largest = n if largest is None else max(largest, n)
    return count, total, largest


for function in (read_all, stream):
    tracemalloc.start()
    result = function(path)
    peak = tracemalloc.get_traced_memory()[1]
    tracemalloc.stop()
    print(f'{function.__name__}: {result}, peak above 5 times the file size: {peak > 5 * size}, peak below 1/10 of the file size: {peak < size / 10}')

os.remove(path)
os.rmdir(directory)
```

```output for=python-stream
file size: 778000 bytes
read_all: (200000, 99900000, 999), peak above 5 times the file size: True, peak below 1/10 of the file size: False
stream: (200000, 99900000, 999), peak above 5 times the file size: False, peak below 1/10 of the file size: True
```

The example files do the same in Go, Rust, JavaScript, Java, C and Bash. Each file writes the same file of numbers, and reads it again one line at a time.

## When to use

Use the technique when the result of each line depends only on the line and a small state: count, sum, filter, convert, extract fields, check a format, or count the words in a map. Use it also when the input does not end, as a pipe or a log file that grows (see [[tail-a-growing-file]]), and when the program must give the first output early.

Do not use it when the program needs all the lines at the same time. A sort needs all the lines before it can write the first line. For a file larger than the memory, a sort program writes sorted parts to temporary files and then merges them. GNU `sort` has the option `-S` to "Use a main-memory sort buffer of the given size", and it puts its temporary files in the directory of the variable `TMPDIR` or of the option `-T`.[^4] A stream also does not help when the state itself grows with the file. A map of the distinct lines of a file in which most lines are different has almost the size of the file. For a small file, reading the whole file is simpler, and it is correct.

## Pitfalls

- **A call that reads all the lines.** `f.readlines()` in Python, `Files.readAllLines` in Java and `File.ReadAllLines` in .NET return all the lines at once. Java says of `readAllLines`: "It is not intended for reading in large files."[^5] .NET says: "when you are working with very large files, ReadLines can be more efficient."[^6] Use the call that returns the lines one at a time: a loop over the file in Python, `Files.lines` in Java, `File.ReadLines` in .NET.
- **A line that is too long for the reader.** Go `bufio.Scanner` has a maximum line length. `MaxScanTokenSize` is `64 * 1024`, and a longer line stops the scan with `ErrTooLong`.[^7] The scan then ends early, and only a check of `Err` shows that the loop did not read the whole file. Call `Buffer` with a larger maximum, or use `bufio.Reader`. Go says: "Programs that need more control over error handling or large tokens, or must run sequential scans on a reader, should use bufio.Reader instead."[^7] In the block, the first scanner has the default maximum, and the second scanner has a maximum of 1 MiB:

```go run id=go-scanner-too-long
package main

import (
	"bufio"
	"fmt"
	"strings"
)

func count(sc *bufio.Scanner) {
	n := 0
	for sc.Scan() {
		n++
	}
	fmt.Println("lines:", n, "error:", sc.Err())
}

func main() {
	text := "short\n" + strings.Repeat("x", 100*1024) + "\nlast\n" // a line of 100 KiB

	count(bufio.NewScanner(strings.NewReader(text)))

	sc := bufio.NewScanner(strings.NewReader(text))
	sc.Buffer(make([]byte, 0, 64*1024), 1024*1024) // allow lines up to 1 MiB
	count(sc)
}
```

```output for=go-scanner-too-long
lines: 1 error: bufio.Scanner: token too long
lines: 3 error: <nil>
```

- **A line with no limit.** A reader with no maximum line length needs memory for the longest line. A file with no LF is one line, so a stream reads all of it into memory. Rust says of `read_line`: "it is possible for an attacker to continuously send bytes without ever sending a newline or EOF".[^2] For input that the program does not trust, limit the length of a line, for example with `take` in Rust or with the maximum of `Scanner.Buffer` in Go.
- **A line buffer that is not cleared.** Rust `read_line` adds to the string: "Previous content of the buffer will be preserved. To avoid appending to the buffer, you need to clear it first."[^2] A loop that does not clear the string gets all the lines before it again, and the memory grows with the file:

```rust run id=rust-read-line-clear
use std::io::{BufRead, Cursor};

fn main() {
    let text = "one\ntwo\nthree\n";

    let mut reader = Cursor::new(text);
    let mut line = String::new();
    while reader.read_line(&mut line).unwrap() > 0 {
        println!("no clear: {:?}", line); // the old lines stay in the buffer
    }

    let mut reader = Cursor::new(text);
    let mut line = String::new();
    loop {
        line.clear();
        if reader.read_line(&mut line).unwrap() == 0 {
            break;
        }
        println!("clear:    {:?}", line);
    }
}
```

```output for=rust-read-line-clear
no clear: "one\n"
no clear: "one\ntwo\n"
no clear: "one\ntwo\nthree\n"
clear:    "one\n"
clear:    "two\n"
clear:    "three\n"
```

- **A buffer that the next read changes.** Go `Scanner.Bytes` says: "The underlying array may point to data that will be overwritten by a subsequent call to Scan."[^7] C `getline` uses one buffer for all the lines, and it can move the buffer when a line needs more space.[^8] Copy the line if the program keeps it after the next read.
- **A file that is not closed.** Java `Files.lines` keeps the file open until the stream closes: "The file is closed by closing the stream."[^5] Use it in a `try` with resources.
- **A CR at the end of each line.** Line readers do different things with a CR before an LF. In Go, the end of a line for `bufio.Scanner` "is one optional carriage return followed by one mandatory newline", so the CR goes.[^7] Rust `read_line` and C `getline` stop at the LF and keep all the bytes before it, so the CR stays.[^2][^8] In Node.js, `readline` takes a CR and an LF as one end of line only when they come close together in time. With `crlfDelay` set to `Infinity`, "\r followed by \n will always be considered a single newline".[^9] Test the reader with a file that has CRLF line endings.
- **A character split between two blocks.** A code that reads fixed blocks and decodes each block can cut a UTF-8 character in two. Split the bytes at each LF first, and decode each line after, as the line readers of the languages do.

## Tools that do it

| Language | Read one line at a time | Reads all the lines at once |
|---|---|---|
| Python | `for line in f` | `f.readlines()`, `f.read().splitlines()` |
| Go | `bufio.Scanner`, `bufio.Reader.ReadString` | `os.ReadFile` and `strings.Split` |
| Rust | `BufRead::read_line`, `BufRead::lines` | `fs::read_to_string` |
| JavaScript (Node.js) | `readline.createInterface` with `for await` | `fs.readFileSync` and `split` |
| Java | `Files.lines`, `BufferedReader.readLine` | `Files.readAllLines` |
| C# | `File.ReadLines`, `StreamReader.ReadLine` | `File.ReadAllLines` |
| C (POSIX) | `getline`, `fgets` | `fread` of the whole file |
| Shell | `awk`, `sed`, `grep`, `while read` | none |

POSIX `getline` reads "until it encounters a character matching the delimiter character", and makes the buffer larger when a line needs more space.[^8] The Node.js `readline` module reads data from a stream "one line at a time".[^9] In `awk`, "Input shall be interpreted as a sequence of records. By default, a record is a line".[^10] So an `awk` program is a stream in which the state is in variables and the `END` action writes the result.

## References
