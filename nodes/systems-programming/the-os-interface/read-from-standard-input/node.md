---
tag: NEW-read-from-standard-input
kind: task
slug: read-from-standard-input
name: Read from standard input
aliases: [read stdin, read input line by line, read all of standard input, read until end of input]
area: systems-programming/the-os-interface
level: core
summary: Read the standard input of the process to the end of input, line by line, and return each line without its newline or CRLF.
contract: contract.yaml
category: command-line-programs
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/stdin.html
    title: The Open Group, POSIX.1-2024, stderr, stdin, stdout
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/read.html
    title: The Open Group, POSIX.1-2024, pread, read
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getline.html
    title: The Open Group, POSIX.1-2024, getdelim, getline
    locator: Description, Return Value
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap11.html
    title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 11, General Terminal Interface
    locator: 11.1.6 Canonical Mode Input Processing, 11.1.9 Special Characters (EOF)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap12.html
    title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 12, Utility Conventions
    locator: 12.2 Utility Syntax Guidelines, Guideline 13
  - url: https://docs.python.org/3/library/sys.html#sys.stdin
    title: The Python Standard Library, sys, System-specific parameters and functions
    locator: sys.stdin
  - url: https://docs.python.org/3/library/functions.html#input
    title: The Python Standard Library, Built-in Functions
    locator: input
  - url: https://docs.python.org/3/library/io.html#io.TextIOWrapper.reconfigure
    title: The Python Standard Library, io, Core tools for working with streams
    locator: TextIOWrapper.reconfigure
  - url: https://nodejs.org/api/process.html#processstdin
    title: Node.js documentation, Process
    locator: process.stdin
  - url: https://nodejs.org/api/readline.html#readlinecreateinterfaceoptions
    title: Node.js documentation, Readline
    locator: readline.createInterface(options), crlfDelay; Event 'close'; rl[Symbol.asyncIterator]()
  - url: https://nodejs.org/api/stream.html#readablesetencodingencoding
    title: Node.js documentation, Stream
    locator: readable.setEncoding(encoding)
  - url: https://nodejs.org/api/webstreams.html#utility-consumers
    title: Node.js documentation, Web Streams API
    locator: Utility Consumers, streamConsumers.text(stream)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html#in
    title: Java SE 25 API, java.lang.System
    locator: in, setIn(InputStream), getProperties() (stdin.encoding)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/BufferedReader.html
    title: Java SE 25 API, java.io.BufferedReader
    locator: readLine(), lines()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Scanner.html
    title: Java SE 25 API, java.util.Scanner
    locator: nextLine(), nextInt()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.console.in?view=net-10.0
    title: .NET API reference, Console.In
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.console.readline?view=net-10.0
    title: .NET API reference, Console.ReadLine
    locator: Returns, Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.textreader.readline?view=net-10.0
    title: .NET API reference, TextReader.ReadLine
    locator: Returns, Remarks
  - url: https://pkg.go.dev/bufio#Scanner
    title: Go standard library, package bufio
    locator: Scanner, ScanLines, MaxScanTokenSize
  - url: https://pkg.go.dev/io#ReadAll
    title: Go standard library, package io
    locator: ReadAll, EOF
  - url: https://pkg.go.dev/fmt
    title: Go standard library, package fmt
    locator: Scanning, Scan
  - url: https://doc.rust-lang.org/std/io/struct.Stdin.html
    title: The Rust Standard Library, std::io::Stdin
    locator: lock, read_line, impl Read for Stdin (read_to_string)
  - url: https://doc.rust-lang.org/std/io/trait.BufRead.html
    title: The Rust Standard Library, std::io::BufRead
    locator: read_line, lines
  - url: https://en.cppreference.com/w/cpp/string/basic_string/getline.html
    title: cppreference.com, std::getline
  - url: https://www.gnu.org/software/bash/manual/bash.html#Bash-Builtins
    title: GNU Bash Reference Manual, Bash Builtin Commands
    locator: 4.2, read
  - url: https://www.gnu.org/software/bash/manual/bash.html#Command-Substitution
    title: GNU Bash Reference Manual, Command Substitution
    locator: 3.5.4
  - url: https://www.gnu.org/software/bash/manual/bash.html#Pipelines
    title: GNU Bash Reference Manual, Pipelines
    locator: 3.2.3
---

## Task

Read the standard input of the process to the end of input. Return the lines in order.

Each solution is a function `read_lines()` that returns a list of strings. A line ends with LF (`"\n"`) or CRLF (`"\r\n"`), and the line in the result has no terminator. A last line with no newline is also a line. An empty input gives an empty list.

Standard input is file descriptor 0. It is open for reading when the program starts. The shell connects it to a terminal, a file (`prog < file`) or a pipe (`a | prog`). The program reads it in the same way in each case. The end of input is a read that returns zero bytes. For a file, this occurs at the end of the file. For a pipe, it occurs when the pipe is empty and no process has it open for writing.

The tests control the input. Each test writes the bytes of a case to a file or a pipe and makes it the standard input of the call. The cases are UTF-8 text.

## Solutions

There are two ways to read standard input. Read all of it at once, and then split it. Or read one line at a time until the end of input. The first way keeps all of the input in memory. The second way keeps only one line, so it also works on input that is larger than memory, and on input that never ends. The solutions use the second way.

The two examples read the same input:

```input id=names
alpha
beta
gamma
```

All of the input at once:

```python run id=python-read-all stdin=names
import sys

text = sys.stdin.read()
print(repr(text))
print(text.splitlines())
```

```output
'alpha\nbeta\ngamma\n'
['alpha', 'beta', 'gamma']
```

One line at a time, until the end of input:

```python run id=python-line-by-line stdin=names
import sys

for number, line in enumerate(sys.stdin, start=1):
    print(number, line.removesuffix("\n"))
```

```output
1 alpha
2 beta
3 gamma
```

- **Python.** `for line in sys.stdin` reads one line at a time and stops at the end of input. Each line keeps its `"\n"`, so the solution removes it, and also a `"\r"` before it. `sys.stdin` is a text file, and its default encoding depends on the platform. The solution calls `sys.stdin.reconfigure(encoding="utf-8")`, which must come before the first read. `sys.stdin.read()` reads all of the input.
- **JavaScript and TypeScript (Node.js).** `readline.createInterface({ input: process.stdin, crlfDelay: Infinity })` and a `for await` loop. With `crlfDelay: Infinity`, `"\r\n"` is always one line break. The loop ends when the stream ends. `setEncoding("utf8")` decodes a character correctly also when two chunks split its bytes. To read all of the input, use `text(process.stdin)` from `node:stream/consumers`. The TypeScript file adds types only.
- **Java.** `new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8))`. `readLine` returns the line without `"\n"`, `"\r"` or `"\r\n"`, and returns `null` at the end of the stream. The documentation of `System.in` tells you to decode with the encoding in the `stdin.encoding` property. The contract says UTF-8, so the solution names UTF-8. The test replaces `System.in` with `System.setIn`.
- **C#.** `Console.In` is a `TextReader` for standard input. `ReadLine` returns `null` when all of the characters are read. A final newline does not give an extra empty line. The test replaces the reader with `Console.SetIn`.
- **Go.** A `bufio.Scanner` on `os.Stdin`. `Scan` returns false at the end of input, and `Text` gives the line without `"\n"` or `"\r\n"`. After the loop, `Err` tells an error apart from the end of input. The solution raises the line limit of the scanner, as [[read-a-file-line-by-line]] explains. `io.ReadAll(os.Stdin)` reads all of the input, and returns a nil error at the end of input, not `io.EOF`.
- **Rust.** `io::stdin().lock()` gives a reader that implements `BufRead`, and `lines()` gives each line without `"\n"` or `"\r\n"`. A line that is not valid UTF-8 gives an error. `read_line` keeps the newline, and returns `Ok(0)` at the end of input. `read_to_string` reads all of the input.
- **C.** `getline(&line, &size, stdin)` reads one line into a buffer that it allocates. It keeps the `"\n"`, so the solution removes it and a `"\r"`. It returns -1 at the end of input and on an error, so `ferror` tells the two apart. `getline` is POSIX, not ISO C, so the file defines `_POSIX_C_SOURCE`.
- **C++.** `std::getline(std::cin, line)` in a `while` loop. It removes the `'\n'`, but it does not remove a `'\r'`, so the solution removes it. The loop stops when `getline` extracts no character.
- **Bash.** `while IFS= read -r line || [[ -n $line ]]`. `IFS=` keeps blanks at the start and end of a line, and `-r` keeps backslashes. `read` returns a status that is not zero at the end of file. The test `[[ -n $line ]]` keeps a last line with no newline, as [[read-a-file-line-by-line]] explains. The solution removes a `"\r"` at the end of a line, and puts the lines in the global array `lines`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the bytes on standard input (`stdin`) and the lines that the solution must return.

- `empty`: no bytes. The result is `[]`.
- `only-newline`: one `"\n"` is one empty line, so the result is `[""]`.
- `trailing-blank-line`: `"last\n\n"` gives `["last", ""]`. The final newline does not add a line, but the empty line before it is a line.
- `no-final-newline` and `crlf-no-final-newline`: the last line has no terminator, and it is still a line.
- `crlf` and `mixed-endings`: `"\r\n"` ends a line, and the `"\r"` is not part of the line.
- `whitespace` and `backslashes`: the line comes back with no change. In Bash, this needs `IFS=` and `-r`.
- `non-ascii`: UTF-8 characters of 2, 3 and 4 bytes.
- `long-line`: only in the native tests. It is a line of 1 MiB. The default buffer of a Go `bufio.Scanner` stops at 64 KiB.

## Pitfalls

- **`input()` at the end of input.** In Python, `input()` raises `EOFError` at the end of input. A loop that calls it without a stop condition fails after the last line:

```python fails expect=exception:EOFError id=python-input-eof stdin=names
while True:
    line = input()
    print(line.upper())
```

```output
ALPHA
BETA
GAMMA
```

Use `for line in sys.stdin`, or catch `EOFError`.

- **Lost trailing newlines in Bash.** Command substitution deletes the trailing newlines of the output. So `$(cat)` changes the input:

```bash wrong id=bash-cat stdin=names
text=$(cat)
printf '%q\n' "$text"
```

```output
$'alpha\nbeta\ngamma'
```

The difference is important when the program writes the text again, or counts its bytes. To read the lines, use the `read` loop of the solution.

- **A loop on the right side of a pipe.** Each command of a pipeline runs in its own subshell. A variable that the loop sets is lost when the loop ends. Redirect the input into the loop instead:

```bash run id=bash-pipe-subshell
count=0
printf 'a\nb\nc\n' | while IFS= read -r line; do
  count=$((count + 1))
done
echo "after the pipe: $count"
count=0
while IFS= read -r line; do
  count=$((count + 1))
done < <(printf 'a\nb\nc\n')
echo "after the redirection: $count"
```

```output
after the pipe: 0
after the redirection: 3
```

- **An extra empty line after a split.** Text that ends with a newline gives an empty string at the end when you split it on `"\n"`:

```javascript wrong id=js-split stdin=names
import { text } from "node:stream/consumers";

const data = await text(process.stdin);
console.log(JSON.stringify(data.split("\n")));
```

```output
["alpha","beta","gamma",""]
```

A split on `"\n"` also keeps the `"\r"` of CRLF input. Use the readline loop of the solution.

The next two pitfalls read this input:

```input id=count-and-name
2
Ada Lovelace
```

- **`nextLine` after `nextInt` in Java.** `nextInt` reads the number and stops before the newline. `nextLine` then returns the rest of that line, which is empty:

```java wrong id=java-scanner stdin=count-and-name
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int count = in.nextInt();
        String name = in.nextLine();
        System.out.println(count + " [" + name + "]");
    }
}
```

```output
2 []
```

Read each line with `nextLine`, and then parse the number with `Integer.parseInt`.

- **`fmt.Scan` reads words, not lines.** In Go, `fmt.Scan` treats newlines as spaces, and a string ends at the first space:

```go wrong id=go-scan stdin=count-and-name
package main

import "fmt"

func main() {
	var count int
	var name string
	fmt.Scan(&count, &name)
	fmt.Printf("%d [%s]\n", count, name)
}
```

```output
2 [Ada]
```

Use a `bufio.Scanner` to read lines.

- **The end of input on a terminal.** When standard input is a terminal, a read waits until the user types a whole line. A program that reads to the end of input waits until the user types the EOF character. The Node.js documentation gives Ctrl+D for it. If the line has bytes, the EOF character sends them to the program without a newline, and the input does not end. At the start of a line, the read returns zero bytes, and this is the end of input. So type Ctrl+D at the start of a line, or type it two times. On Windows, the .NET documentation gives Ctrl+Z, then Enter. A program that waits with no prompt can look as if it stopped, so show a prompt, or read from a file or a pipe.

## Related tasks

- [[read-a-file-line-by-line]]: the same loop on a file, with the checks for UTF-8.
- [[parse-command-line-arguments]]: by the POSIX guidelines, the operand `-` means standard input.
- [[run-a-subprocess]]: give a child process its standard input.
- [[read-an-environment-variable]]: another input that a program gets from its parent.
- [[write-a-file]]: the output side.

## References
