---
tag: 37PAFF
kind: feature
owner: python
slug: standard-modules
name: Standard modules
aliases: [standard library, built-in modules, sys module, sys.builtin_module_names, sys.stdlib_module_names, builtins module, platform-specific modules]
area: languages/python/modules-and-packages
level: intro
summary: >-
  Python comes with a library of standard modules. Some are built into the interpreter, and some exist only on some platforms. sys is in every interpreter.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/modules.html#standard-modules", locator: "6.2. Standard Modules"}
see_also: [python/modules, python/the-module-search-path, python/the-import-statement, python/the-builtins-namespace, python/interactive-mode, python/the-dir-function]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html#standard-modules
  title: The Python Tutorial, 6. Modules
  locator: 6.2. Standard Modules; 6.3. The dir() Function
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/intro.html
  title: The Python Standard Library, Introduction
  locator: Introduction; WebAssembly platforms
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/sys.html
  title: sys, System-specific parameters and functions
  locator: sys.builtin_module_names; sys.stdlib_module_names; sys.ps1
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/datamodel.html#module.__file__
  title: The Python Language Reference, 3. Data model
  locator: 3.2.9.1. Import-related attributes on module objects, module.__file__
  accessed: 2026-11-16
- url: https://doc.rust-lang.org/book/ch07-04-bringing-paths-into-scope-with-the-use-keyword.html
  title: The Rust Programming Language, 7.4. Bringing Paths into Scope with the use Keyword
  locator: Using External Packages
  accessed: 2026-11-16
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 7. Packages and Modules
  locator: 7.3. Compilation Units
  accessed: 2026-11-16
---

## Synopsis

```text
import sys                    # built into every interpreter
sys.builtin_module_names      # tuple: modules compiled into this interpreter
sys.stdlib_module_names       # frozenset: names of all standard modules
import builtins               # the names you can use without an import

try:
    import winreg             # Windows only
except ModuleNotFoundError:
    winreg = None
```

## Semantics

**r1.** "Python comes with a library of standard modules, described in a separate document, the Python Library Reference".[^1] The library documentation says: "Most modules are written in Python, but some are written in C. All can be imported into your program to add functionality."[^2]

**r2.** Some modules are part of the interpreter: "Some modules are built into the interpreter; these provide access to operations that are not part of the core of the language but are nevertheless built in, either for efficiency or to provide access to operating system primitives such as system calls."[^1] `sys.builtin_module_names` is "A tuple of strings containing the names of all modules that are compiled into this Python interpreter."[^3] Example: [`names`](#names).

**r3.** `sys.stdlib_module_names` is "A frozenset of strings containing the names of standard library modules. It is the same on all platforms."[^3] It lists only the top names: "For packages, only the main package is listed: sub-packages and sub-modules are not listed."[^3] Example: [`names`](#names).

**r4.** The set of modules depends on the platform and the build. "The set of such modules is a configuration option which also depends on the underlying platform. For example, the winreg module is only provided on Windows systems."[^1] The library introduction says that some modules "are only available when the underlying system supports or requires them; yet others are available only when a particular configuration option was chosen at the time when Python was compiled and installed."[^2] Example: [`platform`](#platform).

**r5.** One module is always there: "sys, which is built into every Python interpreter."[^1] Some of its attributes depend on the mode. `sys.ps1` and `sys.ps2` "are only defined if the interpreter is in interactive mode."[^3] Example: [`ps1`](#ps1).

**r6.** A built-in module has no source file. `__file__` "might be missing for certain types of modules, such as C modules that are statically linked into the interpreter".[^4] Example: [`names`](#names).

**r7.** The built-in functions are in a standard module too. "dir() does not list the names of built-in functions and variables. If you want a list of those, they are defined in the standard module builtins".[^1] Example: [`builtins`](#builtins).

## Examples

`sys` is built in, and `json` is a standard module that is a package of Python files (rules r2, r3 and r6). `email.mime` is a sub-package, so `sys.stdlib_module_names` does not list it.

```python run id=names
import json
import sys

print("sys" in sys.builtin_module_names, "json" in sys.builtin_module_names)
print("json" in sys.stdlib_module_names)
print("email" in sys.stdlib_module_names, "email.mime" in sys.stdlib_module_names)
print(hasattr(sys, "__file__"), hasattr(json, "__file__"))
```

```output for=names
True False
True
True False
False True
```

A program that also runs on Windows can import `winreg` in a `try` statement (rule r4). The examples run on Linux, so the import fails, and the program goes on without the module.

```python run id=platform
import sys

try:
    import winreg
except ModuleNotFoundError:
    winreg = None

print(sys.platform)
print("winreg" in sys.stdlib_module_names, winreg is None)
```

```output for=platform
linux
True True
```

A script is not interactive, so `sys.ps1` does not exist (rule r5).

```python run id=ps1
import sys

print(hasattr(sys, "ps1"), hasattr(sys, "path"))
```

```output for=ps1
False True
```

The built-in function `len` is an attribute of the module `builtins` (rule r7).

```python run id=builtins
import builtins

print("len" in dir(), "len" in dir(builtins))
print(builtins.len is len)
```

```output for=builtins
False True
True
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### A module that the platform does not have

A name in `sys.stdlib_module_names` does not mean that the import works on this machine (rules r3 and r4). On WebAssembly, the library introduction warns: "Any Python standard library module that uses processes, threading, networking, signals, or other forms of inter-process communication (IPC), is either not available or may not work as on other Unix-like systems."[^2] Import a platform module in a `try` statement, as in [`platform`](#platform), or test `sys.platform` first.

### Interactive-only attributes in a script

Code that reads `sys.ps1` works in the REPL and fails in a script (rule r5).

```python fails id=ps1-in-script expect=exception:AttributeError
import sys

print(sys.ps1)
```

Use `hasattr(sys, "ps1")` to test for interactive mode.

## Comparison

**Rust** also ships its standard library with the language, as a crate: "Because the standard library is shipped with the Rust language, we don’t need to change Cargo.toml to include std. But we do need to refer to it with use to bring items from there into our package’s scope."[^5]

**Java** imports one package of its library in every file: each compilation unit "implicitly imports every public class or interface declared in the predefined package java.lang".[^6] In Python, only the names of `builtins` are available without an import (rule r7).

## Implementation notes

Which modules are built into the interpreter depends on the build (rule r4). On the Linux builds that run these examples, `math` and `itertools` are built in, but `json` is a package of Python files. The tutorial gives `winreg` as an example of a module that exists on Windows only.[^1]

## References
