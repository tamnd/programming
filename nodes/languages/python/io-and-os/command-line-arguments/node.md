---
tag: 8BJ9ES
kind: feature
owner: python
slug: command-line-arguments
name: Command Line Arguments
aliases: [argparse, sys.argv, ArgumentParser, add_argument, parse_args, subcommands, Python command-line options]
area: languages/python/io-and-os
level: intro
summary: >-
  A script finds its command-line arguments in sys.argv as a list of strings. The argparse module turns them into named values, checks them, writes the help text and handles subcommands.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/argparse.html", locator: "argparse module"}
see_also: [python/argument-passing, parse-command-line-arguments, python/standard-streams, python/input-and-output]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#command-line-arguments
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.3. Command-line arguments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.argv
    title: sys, Python 3.15 documentation
    locator: sys.argv
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/argparse.html
    title: argparse, Python 3.15 documentation
    locator: introduction; ArgumentParser objects; add_help; exit_on_error; nargs; type; parse_args(); argument abbreviations; Namespace; subcommands; error()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/flag
    title: Package flag, Go standard library
    locator: Overview; Command line flag syntax
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis args="--lines=5 alpha.txt beta.txt"
import argparse
import sys

print(sys.argv[1:])

parser = argparse.ArgumentParser(
    prog='top',
    description='Show top lines from each file')
parser.add_argument('filenames', nargs='+')
parser.add_argument('-l', '--lines', type=int, default=10)
args = parser.parse_args()
print(args.lines, args.filenames)
```

```output for=synopsis
['--lines=5', 'alpha.txt', 'beta.txt']
5 ['alpha.txt', 'beta.txt']
```

The program runs as `top.py --lines=5 alpha.txt beta.txt`. `sys.argv` holds the raw strings, and the parser turns them into an `int` and a list of names.

## Semantics

**r1.** Command-line arguments "are stored in the sys module's argv attribute as a list."[^1] `sys.argv` is "The list of command line arguments passed to a Python script. argv[0] is the script name (it is operating system dependent whether this is a full pathname or not)."[^2] The items are strings. See [argument passing](python/argument-passing) for the value of `argv[0]` in each start mode.

**r2.** "The argparse module provides a more sophisticated mechanism to process command-line arguments."[^1] "The program defines what arguments it requires, and argparse will figure out how to parse those out of sys.argv. The argparse module also automatically generates help and usage messages. The module will also issue errors when users give the program invalid arguments."[^3]

**r3.** `add_argument()` "supports positional arguments, options that accept values, and on/off flags".[^3] A name without a leading `-` is a positional argument. `action='store_true'` makes an on/off flag. Example: [`flags`](#flags).

**r4.** `nargs` sets how many strings an argument takes. With `'+'`, "Just like '*', all command-line arguments present are gathered into a list. Additionally, an error message will be generated if there wasn't at least one command-line argument present."[^3] `'?'` makes a positional argument optional.

**r5.** "By default, the parser reads command-line arguments in as simple strings."[^3] The `type` keyword "allows any necessary type-checking and type conversions to be performed."[^3] "If the type keyword is used with the default keyword, the type converter is only applied if the default is a string."[^3]

**r6.** `parse_args()` "Convert argument strings to objects and assign them as attributes of the namespace. Return the populated namespace."[^3] Its `args` is the "List of strings to parse. The default is taken from sys.argv."[^3] The `Namespace` class is "deliberately simple, just an object subclass with a readable string representation."[^3] `vars(args)` gives a dict.

**r7.** "If -h or --help is supplied at the command line, the ArgumentParser help will be printed."[^3] The `help` keyword "is a string containing a brief description of the argument."[^3] Example: [`help`](#help).

**r8.** On a bad argument list, `parse_args()` will "print a message to sys.stderr and exit with a status code of 2."[^3] With `exit_on_error=False`, it raises `argparse.ArgumentError` instead.[^3] Example: [`no-exit`](#no-exit).

**r9.** "The parse_args() method by default allows long options to be abbreviated to a prefix, if the abbreviation is unambiguous (the prefix matches a unique option)".[^3] `allow_abbrev=False` turns this off.[^3]

**r10.** `add_subparsers()` "returns a special action object. This object has a single method, add_parser(), which takes a command name and any ArgumentParser constructor arguments, and returns an ArgumentParser object that can be modified as usual."[^3] Example: [`subcommands`](#subcommands).

## Examples

```python run id=flags
import argparse

parser = argparse.ArgumentParser(prog='copy')
parser.add_argument('src')
parser.add_argument('dst', nargs='?', default='out.txt')
parser.add_argument('-v', '--verbose', action='store_true')
parser.add_argument('-n', '--count', type=int, default=1)

print(vars(parser.parse_args(['a.txt'])))
print(vars(parser.parse_args(['-v', '-n', '3', 'a.txt', 'b.txt'])))
print(parser.parse_args(['--count=2', 'a.txt']).count)
```

```output for=flags
{'src': 'a.txt', 'dst': 'out.txt', 'verbose': False, 'count': 1}
{'src': 'a.txt', 'dst': 'b.txt', 'verbose': True, 'count': 3}
2
```

Passing a list to `parse_args()` instead of reading `sys.argv` is handy in tests (rule r6). The option `--count` gives the attribute name `count`.

```python run id=help args="-h"
import argparse

parser = argparse.ArgumentParser(
    prog='top',
    description='Show top lines from each file')
parser.add_argument('filenames', nargs='+', help='files to read')
parser.add_argument('--lines', type=int, default=10, help='lines per file')
parser.parse_args()
print('not reached')
```

```output for=help
usage: top [-h] [--lines LINES] filenames [filenames ...]

Show top lines from each file

positional arguments:
  filenames      files to read

options:
  -h, --help     show this help message and exit
  --lines LINES  lines per file
```

The parser writes the help text and ends the program with status 0, so the last line never runs (rule r7).

```python run id=subcommands
import argparse

def add(args):
    print('adding', args.name)

def remove(args):
    print('removing', args.name, args.force)

parser = argparse.ArgumentParser(prog='pkg')
sub = parser.add_subparsers(dest='command', required=True)
add_parser = sub.add_parser('add')
add_parser.add_argument('name')
add_parser.set_defaults(func=add)
remove_parser = sub.add_parser('remove')
remove_parser.add_argument('name')
remove_parser.add_argument('--force', action='store_true')
remove_parser.set_defaults(func=remove)

for argv in (['add', 'numpy'], ['remove', '--force', 'six']):
    args = parser.parse_args(argv)
    args.func(args)
print(parser.parse_args(['add', 'x']).command)
```

```output for=subcommands
adding numpy
removing six True
add
```

Each subcommand has its own parser and its own options. `set_defaults(func=...)` stores the handler, so the program can "let parse_args() do the job of calling the appropriate function after argument parsing is complete."[^3] `dest='command'` keeps the name of the subcommand (rule r10).

```python run id=no-exit
import argparse

parser = argparse.ArgumentParser(exit_on_error=False)
parser.add_argument('--port', type=int)
try:
    parser.parse_args(['--port', 'http'])
except argparse.ArgumentError as err:
    print('bad:', err)
```

```output for=no-exit
bad: argument --port: invalid int value: 'http'
```

## Pitfalls

A bad value ends the program. The parser prints the usage and the error to standard error and exits with status 2 (rule r8):

```python fails id=bad-int args="--lines ten a.txt" expect=exit:2
import argparse

parser = argparse.ArgumentParser(prog='top')
parser.add_argument('filenames', nargs='+')
parser.add_argument('--lines', type=int, default=10)
parser.parse_args()
print('not reached')
```

```output for=bad-int stream=stderr
usage: top [-h] [--lines LINES] filenames [filenames ...]
top: error: argument --lines: invalid int value: 'ten'
```

`ArgumentParser.error()` does the same for checks of your own: it "prints a usage message, including the message, to sys.stderr and terminates the program with a status code of 2."[^3]

`type=bool` does not parse `False`. "The bool() function is not recommended as a type converter. All it does is convert empty strings to False and non-empty strings to True."[^3]

```python run id=bool-type
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--verbose', type=bool)
print(parser.parse_args(['--verbose', 'False']))
```

```output for=bool-type
Namespace(verbose=True)
```

Use `action='store_true'` for a flag (rule r3).

Prefix matching accepts a short form of a long option, so `--line 5` sets `lines` (rule r9). A new option that shares the prefix can make an old command line ambiguous. Pass `allow_abbrev=False` to require full names.

## Comparison

Go's `flag` package "implements command-line flag parsing."[^4] In Go, "One or two dashes may be used; they are equivalent."[^4] In argparse, `-l` and `--lines` are two separate spellings set up in `add_argument()` (rule r3). Go's "Flag parsing stops just before the first non-flag argument" or after the terminator `--`.[^4] The leftover arguments "are available as the slice flag.Args".[^4] argparse instead declares positional arguments and returns them in the namespace (rule r6).
