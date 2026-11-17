---
tag: 5HAWB7
kind: feature
owner: python
slug: executing-modules-as-scripts
name: Executing modules as scripts
aliases: [if __name__ == "__main__", __main__, python -m, main function, top-level code environment, runpy, script]
area: languages/python/modules-and-packages
level: intro
summary: >-
  A file that runs as the program gets the name "__main__", so the test if __name__ == "__main__" separates script code from code that runs on import. python -m runs a module found by its name.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/__main__.html", locator: "__name__ == '__main__'; Idiomatic Usage; Packaging Considerations"}
see_also: [python/modules, python/the-import-statement, python/the-module-search-path, python/command-line-arguments, python/running-a-python-program, python/error-output-redirection-and-program-termination]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html#executing-modules-as-scripts
  title: The Python Tutorial, 6. Modules
  locator: 6.1.1. Executing modules as scripts
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/__main__.html
  title: __main__, Top-level code environment
  locator: __name__ == '__main__'; Idiomatic Usage; Packaging Considerations
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/using/cmdline.html
  title: 1. Command line and environment
  locator: 1.1.1. Interface options, -c, -m and <script>
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/runpy.html
  title: runpy, Locating and executing Python modules
  locator: the module description
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/import.html#special-considerations-for-main
  title: The Python Language Reference, 5. The import system
  locator: 5.8. Special considerations for __main__; 5.8.1. __main__.__spec__
  accessed: 2026-11-16
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Program execution
  accessed: 2026-11-16
- url: https://doc.rust-lang.org/book/ch07-02-defining-modules-to-control-scope-and-privacy.html
  title: The Rust Programming Language, 7.2. Defining Modules to Control Scope and Privacy
  locator: Modules Cheat Sheet
  accessed: 2026-11-16
---

## Synopsis

```text
# fibo.py
def fib(n): ...

def main():
    fib(int(sys.argv[1]))
    return 0

if __name__ == "__main__":     # True only when fibo.py is the program
    sys.exit(main())

$ python fibo.py 50            # run the file; __name__ is "__main__"
$ python -m fibo 50            # find fibo on sys.path and run it as __main__
import fibo                    # __name__ is "fibo"; the if block does not run
```

## Semantics

**r1.** When you run a file with `python fibo.py`, "the code in the module will be executed, just as if you imported it", and `__name__` is set to `'__main__'`.[^1] An import gives `__name__` the name of the module: "When a Python module or package is imported, __name__ is set to the module’s name."[^2] Example: [`names`](#names).

**r2.** The top-level code environment is "the first user-specified Python module that starts running."[^2] It can be a file argument, a module given with `-m`, code from standard input, code given with `-c`, or the interactive prompt. "In each of these situations, the top-level module’s __name__ is set to '__main__'."[^2] Example: [`names`](#names).

**r3.** The test `if __name__ == '__main__':` lets one file be a module and a script. "Code within this block won’t run unless the module is executed in the top-level environment."[^2] The tutorial says that this "is often used either to provide a convenient user interface to a module, or for testing purposes".[^1] Example: [`guard`](#guard).

**r4.** Keep the block short and put the work in a function. "Putting as few statements as possible in the block below if __name__ == '__main__' can improve code clarity and correctness. Most often, a function named main encapsulates the program’s primary behavior".[^2] The convention is `sys.exit(main())`, so `main()` returns an exit status. "sys.exit() will interpret a string argument as a failure message, so your program will have an exit code of 1, indicating failure, and the string will be written to sys.stderr."[^2] Example: [`exit-status`](#exit-status).

**r5.** `python -m name` uses the import system to find the module: "Locate the module using the standard import mechanism and execute its contents as the __main__ module. Since the argument is a module name, you must not give a file extension (.py)."[^3] For a package, "the interpreter will execute <pkg>.__main__ as the main module."[^3] The `runpy` module implements this: "Its main use is to implement the -m command line switch that allows scripts to be located using the Python module namespace rather than the filesystem."[^4] Example: [`names`](#names).

**r6.** The first item of `sys.argv` depends on the form. For a file, it is "the script name as given on the command line".[^3] For `-m`, it is "the full path to the module file".[^3] For `-c`, "the first element of sys.argv will be "-c"".[^3] Example: [`argv0`](#argv0).

**r7.** The spec of the main module is set only for `-m` and for a directory or zip file. For "running directly from a source or bytecode file", `__main__.__spec__` is `None`, and the reference says: "Use the -m switch if valid module metadata is desired in __main__."[^5] Example: [`argv0`](#argv0).

## Examples

The same file prints a different `__name__` when it is imported and when it is the program (rules r1, r2 and r5). The program runs `mod` as a child process with `-m`.

```python norun reason=fragment file=mod.py group=names
print("mod: __name__ is", __name__)
```

```python run file=main.py group=names id=names
import subprocess
import sys

print("main: __name__ is", __name__)
import mod

child = subprocess.run([sys.executable, "-m", "mod"], capture_output=True, text=True)
print(child.stdout, end="")
```

```output for=names
main: __name__ is __main__
mod: __name__ is mod
mod: __name__ is __main__
```

The module from the tutorial, with its guard (rule r3). This `fib` collects the numbers in a list and prints them on one line. The import does not run the block. The run as a script does.

```python norun reason=fragment file=fibo.py group=guard
def fib(n):
    a, b = 0, 1
    result = []
    while a < n:
        result.append(a)
        a, b = b, a + b
    print(*result)


if __name__ == "__main__":
    import sys
    fib(int(sys.argv[1]))
```

```python run file=main.py group=guard id=guard
import subprocess
import sys

import fibo

fibo.fib(10)
child = subprocess.run([sys.executable, "fibo.py", "50"], capture_output=True, text=True)
print(child.stdout, end="")
```

```output for=guard
0 1 1 2 3 5 8
0 1 1 2 3 5 8 13 21 34
```

A `main()` function that returns the exit status (rule r4). A string from `main()` goes to standard error, and the status is 1.

```python norun reason=fragment file=greet.py group=exit-status
import sys


def main():
    if len(sys.argv) < 2:
        return "usage: greet.py NAME"
    print("hello,", sys.argv[1])
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

```python run file=main.py group=exit-status id=exit-status
import subprocess
import sys

for args in (["greet.py", "Ada"], ["greet.py"]):
    child = subprocess.run([sys.executable, *args], capture_output=True, text=True)
    print(child.returncode, repr(child.stdout), repr(child.stderr))
```

```output for=exit-status
0 'hello, Ada\n' ''
1 '' 'usage: greet.py NAME\n'
```

`sys.argv[0]` and `__spec__` for a file, for `-m` and for `-c` (rules r6 and r7). The program prints only the last part of the path.

```python norun reason=fragment file=show.py group=argv0
import os
import sys

print(os.path.basename(sys.argv[0]), __spec__ is None)
```

```python run file=main.py group=argv0 id=argv0
import subprocess
import sys

code = "import sys; print(sys.argv[0], __spec__ is None)"
for args in (["show.py"], ["-m", "show"], ["-c", code]):
    child = subprocess.run([sys.executable, *args], capture_output=True, text=True)
    print(args[0], "->", child.stdout, end="")
```

```output for=argv0
show.py -> show.py True
-m -> show.py False
-c -> -c True
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### Script code without the guard

Code at the top level of a module runs on every import. The library page warns: "If a module like this was imported from a different module, for example to unit test it, the script code would unintentionally execute as well."[^2] Here a test imports `report` to use `total`, and the report prints.

```python norun reason=fragment file=report.py group=no-guard
def total(values):
    return sum(values)


print("report:", total([3, 4]))
```

```python wrong file=main.py group=no-guard id=no-guard
import report

assert report.total([1, 2]) == 3
print("test passed")
```

```output for=no-guard
report: 7
test passed
```

Put the last line in `if __name__ == '__main__':`, as in [`guard`](#guard).

### The main module imported by its own name

A file that runs as the program is the module `__main__`. An `import` of its file name makes a second module from the same file. The reference says that even when `__main__` matches an importable module, "they’re still considered distinct modules."[^5] Here `main.py` is the program, and `helper.py` imports `main`. The second copy has its own `score`.

```python norun reason=fragment file=helper.py group=two-copies
import main


def add_point():
    main.score += 1
```

```python wrong file=main.py group=two-copies id=two-copies
score = 0

if __name__ == "__main__":
    import helper

    helper.add_point()
    helper.add_point()
    print("score:", score)
```

```output for=two-copies
score: 0
```

Keep the state in a module that is never the program, and use a small script or `__main__.py` to start it.

### A string or no value from `main()`

`sys.exit(main())` treats a string as a failure (rule r4), and `None` as success. Return an integer, or `None` for success, as the library page says: "typically, an integer or None (which is implicitly returned if your function does not have a return statement)."[^2]

## Comparison

**Go** has one fixed entry point. "The main package must have package name main and declare a function main that takes no arguments and returns no value."[^6] A Go package cannot be both a library and a program, but a Python file can be both.

**Rust** separates the two kinds of crate. The compiler starts at "the crate root file (usually src/lib.rs for a library crate and src/main.rs for a binary crate)".[^7]

## Implementation notes

Console scripts that installers make call `main()` in the same way. "When this is done, pip inserts the function call into a template script, where the return value of main is passed into sys.exit()."[^2] Thus a `main()` that follows rule r4 behaves the same way when you run the file and when you run the installed command.

## References
