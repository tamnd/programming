---
tag: KV4RMN
kind: feature
owner: python
slug: warnings
name: Warnings
aliases: [warnings module, warnings.warn, catch_warnings, filterwarnings, simplefilter, DeprecationWarning, UserWarning, python -W, PYTHONWARNINGS]
area: languages/python/errors
level: core
summary: >-
  warnings.warn reports a condition that does not stop the program. A filter list decides if each warning is ignored, printed or raised as an exception. catch_warnings changes the filter for a block.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/warnings.html", locator: "warnings, Warning control"}
see_also: [python/the-exception-hierarchy, python/raising-exceptions, python/assert-statement, python/context-managers]
sources:
  - url: https://docs.python.org/3.15/library/warnings.html
    title: warnings, Warning control
    locator: Warning Categories; The Warnings Filter; Default Warning Filter; Testing Warnings; Available Functions; Available Context Managers
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-W
    title: Command line and environment
    locator: -W arg
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#warnings
    title: Built-in Exceptions
    locator: Warnings
    accessed: 2026-11-16
  - url: https://go.dev/doc/comment#deprecations
    title: Go Doc Comments
    locator: Deprecations
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import warnings

def parse_port(text):
    if text.startswith('0'):
        warnings.warn('leading zero in port ' + repr(text))
    return int(text)

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    print(parse_port('080'))

for w in caught:
    print(w.category.__name__ + ':', w.message)
```

```output for=basics
80
UserWarning: leading zero in port '080'
```

A warning tells the user about a condition that does not stop the program. The function returns its result as usual. The warnings filter decides what happens to each warning: Python can ignore it, print it, or raise it as an exception.

## Semantics

**r1.** A program issues a warning with `warnings.warn(message, category=None, stacklevel=1)`. "Python programmers issue warnings by calling the warn() function defined in this module."[^1] The category "defaults to UserWarning."[^1]

**r2.** A warning category is a class. "A warning category must always be a subclass of the Warning class."[^1] Warning itself "is a subclass of Exception."[^1] Other built-in categories include DeprecationWarning, PendingDeprecationWarning, SyntaxWarning, RuntimeWarning, FutureWarning, ImportWarning, UnicodeWarning, EncodingWarning, BytesWarning and ResourceWarning.[^3] DeprecationWarning is for warnings "intended for other Python developers."[^3] FutureWarning is for warnings "intended for end users of applications that are written in Python."[^3]

**r3.** The warnings filter is an ordered list. "any specific warning is matched against each filter specification in the list in turn until a match is found; the filter determines the disposition of the match."[^1] Each entry has the form (action, message, category, module, lineno).[^1]

**r4.** The actions are `default`, `error`, `ignore`, `always`, `all`, `module` and `once`.[^1] The `default` action prints "the first occurrence of matching warnings for each location (module + line number) where the warning is issued".[^1] "If a warning is reported and doesn’t match any registered filter then the “default” action is applied (hence its name)."[^1] Example: [`once-per-location`](#once-per-location).

**r5.** The `error` action turns matching warnings into exceptions. warn() "raises an exception if the particular warning issued is changed into an error by the warnings filter."[^1] Example: [`error-action`](#error-action).

**r6.** "Warning messages are normally written to sys.stderr".[^1] The printed form is the file name, the line number, the category name and the message. Example: [`format`](#format).

**r7.** In a release build, the default filter has these entries, in order of precedence: `default::DeprecationWarning:__main__`, `ignore::DeprecationWarning`, `ignore::PendingDeprecationWarning`, `ignore::ImportWarning` and `ignore::ResourceWarning`.[^1] Thus DeprecationWarning is "ignored by default, unless triggered by code in __main__".[^1] Example: [`hidden-deprecation`](#hidden-deprecation).

**r8.** `filterwarnings(action, message='', category=Warning, module='', lineno=0, append=False)` adds a filter entry. "The entry is inserted at the front by default".[^1] "Entries closer to the front of the list override entries later in the list, if both match a particular warning."[^1] `simplefilter()` is the same, but it "always matches any message in any module as long as the category and line number match."[^1]

**r9.** In filterwarnings(), message is "a string containing a regular expression that the start of the warning message must match, case-insensitively."[^1] Example: [`message-match`](#message-match).

**r10.** The stacklevel argument selects the stack frame that the warning refers to. With stacklevel=2, the warning refers to the caller of the function that calls warn().[^1] Example: [`stacklevel`](#stacklevel).

**r11.** `catch_warnings` is "A context manager that copies and, upon exit, restores the warnings filter and the showwarning() function."[^1] With record=True, it returns a list that receives each warning, and it suppresses the output to sys.stderr.[^1] Each item has the attributes message, category, filename and lineno, and others.[^1] If the action argument is not None, the other arguments "are passed to simplefilter() as if it were called immediately on entering the context."[^1] Example: [`basics`](#basics).

**r12.** "The warnings filter is initialized by -W options passed to the Python interpreter command line and the PYTHONWARNINGS environment variable."[^1] For example, `-Werror` converts warnings to exceptions.[^2] Example: [`command-line`](#command-line).

**r13.** The `@warnings.deprecated(message)` decorator marks a class or function as deprecated. "For functions, that happens on calls; for classes, on instantiation and on creation of subclasses."[^1] The decorator keeps the message in the `__deprecated__` attribute.[^1] Example: [`deprecated`](#deprecated).

## Examples

Each location warns one time under the `default` action (rule r4):

```python run id=once-per-location
import warnings

def check(n):
    if n < 0:
        warnings.warn('negative value')

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('default')
    for n in [-1, -2, -3]:
        check(n)
print(len(caught))
```

```output for=once-per-location
1
```

The loop calls check() three times, but the warn() call is at one location. Use the `always` action to see every warning.

The `error` action turns a warning into an exception (rule r5). The code can catch it like any other exception:

```python run id=error-action
import warnings

with warnings.catch_warnings():
    warnings.simplefilter('error')
    try:
        warnings.warn('disk almost full', ResourceWarning)
    except ResourceWarning as err:
        print('caught', type(err).__name__ + ':', err)
```

```output for=error-action
caught ResourceWarning: disk almost full
```

The -W option sets the filter for the whole process (rule r12):

```python run id=command-line
import subprocess
import sys

code = "import warnings; warnings.warn('old setting')"
result = subprocess.run([sys.executable, '-Werror', '-c', code],
                        capture_output=True, text=True)
print(result.returncode)
print(result.stderr.splitlines()[-1])
```

```output for=command-line
1
UserWarning: old setting
```

A function that warns about its own use gives stacklevel=2, so that the warning points to the caller (rule r10):

```python run id=stacklevel
import inspect
import warnings

def old_api():
    warnings.warn('old_api is deprecated', DeprecationWarning, stacklevel=2)

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    old_api()
    call_line = inspect.currentframe().f_lineno - 1
print(caught[0].lineno == call_line)
```

```output for=stacklevel
True
```

formatwarning() shows the form that Python prints to sys.stderr (rule r6):

```python run id=format
import warnings

text = warnings.formatwarning('disk almost full', UserWarning, 'app.py', 12, line='')
print(repr(text))
```

```output for=format
'app.py:12: UserWarning: disk almost full\n'
```

The deprecated decorator issues a DeprecationWarning on each call (rule r13):

> [!since v="3.13"]
> `warnings.deprecated` is in CPython 3.13 and later.[^1]

```python run id=deprecated since=3.13
import warnings

@warnings.deprecated('use connect() instead')
def open_connection(host):
    return 'connected to ' + host

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    print(open_connection('db1'))
print(caught[0].category.__name__ + ':', caught[0].message)
print(open_connection.__deprecated__)
```

```output for=deprecated
connected to db1
DeprecationWarning: use connect() instead
use connect() instead
```

## Pitfalls

A DeprecationWarning from library code is hidden by default (rule r7). The same warning is shown when the code runs in `__main__`:

```python run id=hidden-deprecation
import warnings

code = "import warnings\nwarnings.warn('old api', DeprecationWarning)\n"
with warnings.catch_warnings(record=True) as caught:
    exec(code, {'__name__': 'library'})
    exec(code, {'__name__': '__main__'})
print(len(caught))
```

```output for=hidden-deprecation
1
```

Only the run in `__main__` gives a warning. Thus a program does not show the deprecation warnings of the libraries that it calls. The documentation advises developers to "test their code with typically ignored warnings made visible".[^1] Run the tests with -Wd or -Werror, or set PYTHONWARNINGS.

The message argument of filterwarnings() matches the start of the message, and it ignores case (rule r9):

```python run id=message-match
import warnings

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    warnings.filterwarnings('ignore', message='port')
    warnings.warn('leading zero in port')
    warnings.warn('Port 80 needs root')
print([str(w.message) for w in caught])
```

```output for=message-match
['leading zero in port']
```

The filter does not ignore the first warning, because 'port' is not at the start of the message. It ignores the second, because the match ignores case.

A filter set with simplefilter() or filterwarnings() outside catch_warnings stays in force for the rest of the process. In tests, change the filter inside catch_warnings, so that the old filter comes back at the end of the block (rule r11).

catch_warnings is not always safe with threads. "If the context_aware_warnings flag is false, then catch_warnings will modify the global attributes of the warnings module."[^1] Then "if two or more threads use the catch_warnings class at the same time, the behavior is undefined."[^1] "The flag defaults to true for free-threaded builds and false otherwise."[^1]

## Comparison

For deprecation, Go uses a doc comment convention: "Paragraphs starting with Deprecated: are treated as deprecation notices."[^4] "Some tools will warn when deprecated identifiers are used."[^4] The Go notice is a comment in the source code. A Python DeprecationWarning is issued by a call to warn() when the program runs (rule r1), and the warnings filter controls it (rule r3).

[^1]: [warnings, Warning control](https://docs.python.org/3.15/library/warnings.html)
[^2]: [Command line and environment, -W](https://docs.python.org/3.15/using/cmdline.html#cmdoption-W)
[^3]: [Built-in Exceptions, Warnings](https://docs.python.org/3.15/builtins/exceptions.html#warnings)
[^4]: [Go Doc Comments, Deprecations](https://go.dev/doc/comment#deprecations)
