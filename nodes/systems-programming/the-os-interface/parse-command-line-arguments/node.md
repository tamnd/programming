---
tag: 5V6AQQ
kind: task
slug: parse-command-line-arguments
name: Parse command-line arguments
aliases: [read command-line options, parse argv, read program arguments, getopt]
area: systems-programming/the-os-interface
level: core
summary: Turn the argument list of a program into options and file operands, with short and long options, a default value, the -- terminator and a usage error for bad input.
contract: contract.yaml
category: command-line-programs
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap12.html
    title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 12, Utility Conventions
    locator: 12.2 Utility Syntax Guidelines, Guidelines 5, 6, 9, 10 and 13
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getopt.html
    title: The Open Group, POSIX.1-2024, getopt
    locator: Description; Return Value
  - url: https://sourceware.org/glibc/manual/latest/html_node/Argument-Syntax.html
    title: The GNU C Library Reference Manual, Program Argument Syntax Conventions
    locator: 26.1.1
  - url: https://sourceware.org/glibc/manual/latest/html_node/Using-Getopt.html
    title: The GNU C Library Reference Manual, Using the getopt function
    locator: 26.2.1, the permutation of argv and POSIXLY_CORRECT
  - url: https://man7.org/linux/man-pages/man3/getopt.3.html
    title: Linux man-pages, getopt(3)
    locator: Description (optind); Notes (reset of optind to 0)
  - url: https://man.freebsd.org/cgi/man.cgi?query=getopt&sektion=3
    title: FreeBSD Library Functions Manual, getopt(3)
    locator: Description (optreset)
  - url: https://man.freebsd.org/cgi/man.cgi?query=getopt_long&sektion=3
    title: FreeBSD Library Functions Manual, getopt_long(3)
    locator: Description (the leading +); Bugs (optind set to 0)
  - url: https://en.cppreference.com/w/c/language/main_function
    title: cppreference.com, Main function (C)
    locator: Parameters, argv
  - url: https://en.cppreference.com/w/cpp/utility/from_chars
    title: cppreference.com, std::from_chars
  - url: https://docs.python.org/3/library/argparse.html
    title: The Python Standard Library, argparse
    locator: allow_abbrev; exit_on_error; Arguments containing -; ArgumentParser.exit; ArgumentParser.error
  - url: https://docs.python.org/3/library/sys.html#sys.argv
    title: The Python Standard Library, sys
    locator: sys.argv
  - url: https://nodejs.org/api/util.html#utilparseargsconfig
    title: Node.js documentation, Util
    locator: util.parseArgs([config])
  - url: https://nodejs.org/api/process.html#processargv
    title: Node.js documentation, Process
    locator: process.argv
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.1.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 12.1 Java Virtual Machine Startup; 12.1.4 Invoke a main Method
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html#parseInt(java.lang.String,int)
    title: Java SE 25 API, java.lang.Integer
    locator: parseInt(String, int)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/program-structure/main-command-line
    title: C# guide, Main() and command-line arguments
    locator: Overview; Command-line arguments
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.environment.getcommandlineargs?view=net-10.0
    title: .NET API reference, Environment.GetCommandLineArgs
    locator: Returns; Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.numberstyles?view=net-10.0
    title: .NET API reference, NumberStyles
    locator: the None field
  - url: https://pkg.go.dev/flag
    title: Go standard library, package flag
    locator: Command line flag syntax; ErrorHandling; FlagSet.SetOutput; FlagSet.Args
  - url: https://pkg.go.dev/os#pkg-variables
    title: Go standard library, package os
    locator: Args
  - url: https://doc.rust-lang.org/std/env/fn.args.html
    title: The Rust Standard Library, std::env::args
    locator: description; Panics
  - url: https://www.gnu.org/software/bash/manual/html_node/Bourne-Shell-Builtins.html
    title: GNU Bash Reference Manual, Bourne Shell Builtins
    locator: getopts
  - url: https://www.gnu.org/software/bash/manual/html_node/Special-Parameters.html
    title: GNU Bash Reference Manual, Special Parameters
    locator: "*, @, # and 0"
---

## Task

Parse the arguments of a small tool with this syntax:

```text
count [-v | --verbose] [-n N | --limit N | --limit=N] [--] [FILE...]
```

The function gets the arguments without the program name. It returns three values: `verbose`, `limit` and the list of files. `verbose` is true if `-v` or `--verbose` occurs. `limit` is 10 if no `-n` or `--limit` occurs. If the option occurs more than once, the last value applies. N is a decimal number of digits only. The files keep their order.

The first `--` ends the options, and it is not a file. Each argument after it is a file, also an argument that starts with `-` or a second `--`. POSIX Guideline 10 gives this rule. A lone `-` is a file. POSIX Guideline 13 gives it the meaning of standard input. An empty list of files is valid.

The function reports a usage error for an unknown option, for an option with no value, for a value that is not a decimal number, and for a value after `--verbose`. It returns the error as a value or an exception. It must not exit the process, so that a test can check the error. The `main` of a real tool prints the message on standard error and exits with status 2, as `argparse` and Go `flag` do.

Parsers do not agree on some input. So the contract has four preconditions. No option follows a file. Short options are not grouped, as in `-vn3`, and the value of `-n` is a separate argument. A long option is not abbreviated. N has no sign and no leading zero.

## Solutions

Each solution uses the parser of the standard library where one exists. The other solutions loop over the arguments and stop at the first file, as POSIX Guideline 9 expects.

- **Python.** `argparse.ArgumentParser` with `nargs="*"` for the files and a type function for N. On an error, `ArgumentParser.error` ends the program with status 2. The argparse documentation shows an override of `exit` that raises an exception for a nonzero status, and the solution uses one. `allow_abbrev=False` turns off abbreviations. `sys.argv[0]` is the script name, so the arguments are `sys.argv[1:]`.
- **JavaScript and TypeScript (Node.js).** `util.parseArgs` with `strict: true` and `allowPositionals: true`. It throws for an unknown option and for a value of the wrong type. An option has the type `boolean` or `string`, so the code converts N. `process.argv[0]` is the path of `node` and `process.argv[1]` is the script. The TypeScript file adds types only.
- **Java.** `main` gets a `String[]` without the class name (JLS 12.1). A `switch` over the strings handles each option. `Integer.parseInt` also accepts a sign and the digits of other scripts, so a check for ASCII digits comes first.
- **C#.** `Main` gets a `string[]` without the program name. `Environment.GetCommandLineArgs` returns the executable file name first. A loop handles each option, and `int.TryParse` with `NumberStyles.None` accepts digits only. The C# guide suggests the System.CommandLine library for complex parsing.
- **Go.** `flag.NewFlagSet` with `flag.ContinueOnError` returns an error and does not exit. `SetOutput(io.Discard)` stops the messages. The short and the long name point to one variable. `os.Args` starts with the program name, so the tool passes `os.Args[1:]`.
- **Rust.** A loop over `std::env::args().skip(1)`. The first item is the path of the executable by tradition, but it can be any text.
- **C.** `getopt_long` with the option string `":vn:"`. The leading `:` makes a missing value return `':'`. `argv[0]` is the program name, and `getopt_long` starts at `argv[1]`. The solution sets `opterr = 0` and returns an error code, and the caller prints the message.
- **C++.** The caller copies `argv + 1` to `argv + argc` into a vector of `std::string_view`, and `parse_args` loops over it as a `std::span`. `getopt` is a POSIX function and not part of C++. The loop keeps no global state. `std::from_chars` converts N, and `parse_args` returns `std::expected` (C++23).
- **Bash.** `getopts` knows only one-letter options. A first `case` loop changes `--verbose` and `--limit` to `-v` and `-n`, then `getopts ":vn:"` reads them. The function declares `local OPTIND=1`, because the shell does not reset `OPTIND` between calls. `"$@"` gives each argument as one word.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The cases that must give a usage error are in `cases/errors.jsonl`.

- `no-args`: no arguments at all. The result is the defaults and no files.
- `long-equals`: `--limit=7` holds the value in the same argument.
- `repeated`: `-n 3 --limit 5` gives 5, because the last value applies.
- `zero`: `-n 0` is valid. A check such as `if (!limit)` treats it as a missing value.
- `end-of-options`: after `--`, the strings `-n` and `--limit=4` are files. `second-terminator`: in `-- --`, the second `--` is a file.
- `dash`: a lone `-` is a file and not an option.
- `empty-arg`, `space` and `non-ascii`: an empty argument, a name with a space and names with non-ASCII letters are files. Each one is a single argument.
- Errors: `-x`, `--color`, `-n` or `--limit` at the end, `-n ten`, `--limit=ten`, `--limit=` and `--verbose=yes`.

## Pitfalls

- **The first element of the raw list.** In C, Python, Go and Rust, element 0 is the program or the script. In Node.js, the arguments start at index 2. Java and C# do not give the program name to `main`. These programs get the arguments `-v --limit 3 a.txt`:

```python run id=python-argv args="-v --limit 3 a.txt"
import os
import sys

print(len(sys.argv), os.path.basename(sys.argv[0]))
print(sys.argv[1:])
```

```output
5 main.py
['-v', '--limit', '3', 'a.txt']
```

```javascript run id=node-argv args="-v --limit 3 a.txt"
console.log(process.argv.length);
console.log(process.argv.slice(2));
```

```output
6
[ '-v', '--limit', '3', 'a.txt' ]
```

```java run id=java-args args="-v --limit 3 a.txt"
public class Main {
    public static void main(String[] args) {
        System.out.println(args.length);
        System.out.println(String.join(" ", args));
    }
}
```

```output
4
-v --limit 3 a.txt
```

```c run id=c-argv args="-v --limit 3 a.txt"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char *slash = strrchr(argv[0], '/');
    printf("%d %s\n", argc, slash != NULL ? slash + 1 : argv[0]);
    for (int i = 1; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
```

```output
5 main
argv[1] = -v
argv[2] = --limit
argv[3] = 3
argv[4] = a.txt
```

- **`$*` in Bash.** `"$@"` gives each argument as a separate word. `"$*"` joins all the arguments into one word with the first character of `IFS` between them:

```bash run id=bash-args args="-v --limit 3 a.txt"
echo "$# $(basename "$0")"
printf '<%s> ' "$@"
echo
IFS=,
printf '<%s> ' "$*"
echo
```

```output
4 main.sh
<-v> <--limit> <3> <a.txt>
<-v,--limit,3,a.txt>
```

- **Options after a file.** POSIX `getopt` and Go `flag` stop at the first operand. So Go reads `a.txt -v` as two files:

```go run id=go-stops args="a.txt -v"
package main

import (
	"flag"
	"fmt"
)

func main() {
	verbose := flag.Bool("v", false, "print more")
	flag.Parse()
	fmt.Println("verbose:", *verbose)
	fmt.Println("files:", flag.Args())
}
```

```output
verbose: false
files: [a.txt -v]
```

`argparse` finds the option after the file:

```python run id=python-intermixed args="a.txt -v"
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-v", action="store_true")
parser.add_argument("files", nargs="*")
args = parser.parse_args()
print("verbose:", args.v)
print("files:", args.files)
```

```output
verbose: True
files: ['a.txt']
```

GNU `getopt` permutes `argv` by default, so the files move to the end and the options apply. `POSIXLY_CORRECT` or a leading `+` in the option string turns this off. On FreeBSD, `getopt` stops at the first operand, but `getopt_long` permutes. So the same C program can give a different result on two systems.

- **A value in the same argument.** Go reads `-n=3` as the value 3. POSIX `getopt` takes the rest of the argument after the option letter, so the value is `=3`. `util.parseArgs` does the same:

```javascript run id=node-attached args="-n=3 a.txt"
import { parseArgs } from "node:util";

const { values } = parseArgs({
  options: { limit: { type: "string", short: "n" } },
  allowPositionals: true,
});
console.log(values.limit);
```

```output
=3
```

- **Abbreviated long options.** `argparse` accepts a prefix of a long option if the prefix is unique. GNU `getopt_long` does the same. So `--lim 3` sets the limit:

```python run id=python-abbrev args="--lim 3"
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--limit", type=int, default=10)
print(parser.parse_args().limit)
```

```output
3
```

- **Number parsers accept more than digits.** Go `flag` integer flags accept `0664`, `0x1234` and negative values. `Integer.parseInt` in Java accepts a `+` or a `-` sign. `std::from_chars` accepts a `-` for a signed type. Each solution checks for ASCII digits first:

```go run id=go-int-forms args="-n 0x10"
package main

import (
	"flag"
	"fmt"
)

func main() {
	n := flag.Int("n", 10, "the limit")
	flag.Parse()
	fmt.Println(*n)
}
```

```output
16
```

- **A parser that exits.** By default `argparse` prints the usage and exits with status 2 on an error. The flag set of Go `flag.Parse` uses `ExitOnError`, which also exits with status 2. A test cannot check an error after an exit, so the solutions use `ContinueOnError` in Go and override `exit` in Python:

```python fails expect=exit:2 id=python-exit args="-n ten"
import argparse

parser = argparse.ArgumentParser(prog="count")
parser.add_argument("-n", type=int, default=10)
parser.parse_args()
print("not reached")
```

```output stream=stderr
usage: count [-h] [-n N]
count: error: argument -n: invalid int value: 'ten'
```

- **The global state of `getopt`.** `getopt` keeps its position in the global `optind`. A second parse needs a reset, and the reset is not portable. POSIX leaves `optind = 0` unspecified. glibc restarts with `optind = 1`, but it needs `optind = 0` to read `POSIXLY_CORRECT` and the `+` or `-` prefix again. FreeBSD `getopt` needs `optreset = 1` and a reset of `optind`. FreeBSD `getopt_long` resets when `optind` is 0. The C solution uses `getopt_long` and sets `optind = 0`. In Bash, `OPTIND` has the same problem, so the function declares it `local`.
- **Arguments that are not Unicode.** On Unix the system passes each argument as bytes. Python decodes them with the `surrogateescape` handler. Rust `std::env::args` panics on an argument that is not valid Unicode. Use `std::env::args_os` to read such an argument.

## Related tasks

- [[read-an-environment-variable]]: the other input that a process gets at its start.
- [[split-a-string]]: split one string into fields.

## References
