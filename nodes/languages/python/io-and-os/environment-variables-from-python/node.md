---
tag: V436P2
kind: feature
owner: python
slug: environment-variables-from-python
name: Environment variables
aliases: [os.environ, os.getenv, os.putenv, os.unsetenv, environment variable, env for a child process]
area: languages/python/io-and-os
level: intro
summary: >-
  os.environ is a mapping of the process environment, with str keys and values. os.getenv reads one variable with a default. Changes to os.environ, or an env mapping, set what a child process sees.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/os.html#os.environ", locator: "os.environ"}
see_also: [python/operating-system-interface, python/command-line-arguments, python/input-and-output]
sources:
  - url: https://docs.python.org/3.15/library/os.html#os.environ
    title: os, Python 3.15 documentation
    locator: os.environ; os.getenv(); os.putenv(); os.unsetenv()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/subprocess.html#subprocess.run
    title: subprocess, Python 3.15 documentation
    locator: subprocess.run(), env argument
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os#Getenv
    title: Package os, Go standard library
    locator: func Getenv; func LookupEnv; func Setenv; func Environ
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import subprocess

os.environ['APP_MODE'] = 'test'
print(os.environ['APP_MODE'])
print(os.getenv('APP_WORKERS', '4'))
print('APP_DEBUG' in os.environ, flush=True)

subprocess.run(['printenv', 'APP_MODE'], check=True)
```

```output for=synopsis
test
4
False
test
```

The program sets a variable, reads it back, reads a missing one with a default, and starts a child that sees the new value. The `flush=True` writes the parent's output before the child's output.

## Semantics

**r1.** `os.environ` is "A mapping object where keys and values are strings that represent the process environment."[^1] It supports the dict operations: `[]`, `get()`, `in`, iteration, `pop()`. A missing key raises `KeyError`.

**r2.** "This mapping is captured the first time the os module is imported, typically during Python startup as part of processing site.py. Changes to the environment made after this time are not reflected in os.environ, except for changes made by modifying os.environ directly."[^1]

**r3.** `os.getenv(key, default=None)` will "Return the value of the environment variable key as a string if it exists, or default if it doesn't."[^1] It uses `os.environ`, so it is "similarly also captured on import".[^1] The value is always a string; convert it with `int()` or a similar call. Example: [`convert`](#convert).

**r4.** "This mapping may be used to modify the environment as well as query the environment. putenv() will be called automatically when the mapping is modified."[^1] `putenv()` changes "affect subprocesses started with os.system(), popen() or fork() and execv()."[^1]

**r5.** "You can delete items in this mapping to unset environment variables. unsetenv() will be called automatically when an item is deleted from os.environ, and when one of the pop() or clear() methods is called."[^1]

**r6.** For a child process, the `env` argument of `subprocess.run()` "must be a mapping that defines the environment variables for the new process; these are used instead of the default behavior of inheriting the current process' environment."[^2] Without `env`, the child inherits the environment. Example: [`child-env`](#child-env).

**r7.** "On Windows, the keys are converted to uppercase. This also applies when getting, setting, or deleting an item."[^1] "On Unix, keys and values use sys.getfilesystemencoding() and 'surrogateescape' error handler. Use environb if you would like to use a different encoding."[^1]

## Examples

```python run id=convert
import os

os.environ['APP_PORT'] = '8080'
port = int(os.getenv('APP_PORT', '80'))
workers = int(os.getenv('APP_WORKERS', '4'))
print(port + 1, workers)

debug = os.getenv('APP_DEBUG', '').lower() in ('1', 'true', 'yes')
print(debug)
```

```output for=convert
8081 4
False
```

Every value is a string, so the program converts it and gives a string default (rule r3).

```python run id=child-env
import os
import subprocess

os.environ['APP_MODE'] = 'test'
extra = {**os.environ, 'APP_LEVEL': 'debug'}
subprocess.run(['printenv', 'APP_MODE', 'APP_LEVEL'], env=extra, check=True)

subprocess.run(['env'], env={'ONLY': '1'}, check=True)
print('APP_LEVEL' in os.environ)
```

```output for=child-env
test
debug
ONLY=1
False
```

A copy of `os.environ` with one more key adds a variable for one child only. A mapping of its own replaces the whole environment, so `env` sees one variable (rule r6). The parent's environment does not change.

```python run id=delete
import os

os.environ['APP_TMP'] = 'x'
del os.environ['APP_TMP']
print(os.environ.pop('APP_TMP', 'gone'))
print(os.environ.get('APP_TMP'))
```

```output for=delete
gone
None
```

`del` and `pop()` unset the variable (rule r5). `pop()` with a default and `get()` do not raise for a missing key.

## Pitfalls

The values must be strings:

```python fails id=int-value expect=exception:TypeError
import os

os.environ['APP_PORT'] = 8080
```

Convert with `str()` first (rule r1).

`os.putenv()` does not update `os.environ`. "Calling putenv() directly does not change os.environ, so it's better to modify os.environ."[^1]

```python run id=putenv
import os
import subprocess

os.putenv('APP_FLAG', 'on')
print(os.getenv('APP_FLAG'), flush=True)
subprocess.run(['printenv', 'APP_FLAG'], check=True)
```

```output for=putenv
None
on
```

The child sees the variable, but the program's own view does not (rules r2 and r4).

A value read at import time does not follow later changes. A module that does `MODE = os.getenv('APP_MODE')` keeps the old value after `os.environ` changes. Read the variable when you need it.

"On some platforms, including FreeBSD and macOS, setting environ may cause memory leaks."[^1] Prefer an `env` mapping for a child (rule r6) over many changes to the program's own environment.

## Comparison

Go's `os.Getenv` "retrieves the value of the environment variable named by the key. It returns the value, which will be empty if the variable is not present. To distinguish between an empty value and an unset value, use LookupEnv."[^3] Python's `os.getenv()` returns `None` for a missing variable, so the check is `is None` (rule r3). Go's `os.Setenv` "returns an error, if any."[^3] Go's `os.Environ` "returns a copy of strings representing the environment, in the form "key=value"."[^3]
