---
tag: A2G6A2
kind: feature
owner: python
slug: shadowing-names
name: Shadowing names
aliases: [shadowing a builtin, shadowing a module, list is not callable, script named like a standard module, random.py, A001, A005, builtin-variable-shadowing, stdlib-module-shadowing, PYTHONSAFEPATH]
area: languages/python/pitfalls
level: intro
summary: >-
  A variable named list or str hides the builtin, and a file named random.py hides the standard module. Later code then gets the wrong object. Use other names, and run scripts with -P.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names", locator: "4.2.2 Resolution of names"}
proposals: []
see_also: [python/name-resolution, python/built-in-functions, python/the-module-search-path, python/naming-conventions, python/invoking-the-interpreter, python/standard-modules, python/circular-imports]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#python-scopes-and-namespaces
    title: The Python Tutorial, Python 3.15 documentation
    locator: 9.2 Python Scopes and Namespaces
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, Python 3.15 documentation
    locator: 4.2.2 Resolution of names; 4.2.4 Builtins and restricted execution
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/builtin-variable-shadowing/
    title: builtin-variable-shadowing (A001), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/modules.html#the-module-search-path
    title: The Python Tutorial, Python 3.15 documentation
    locator: 6.1.2 The Module Search Path
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/stdlib-module-shadowing/
    title: stdlib-module-shadowing (A005), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-P
    title: Command line and environment, Python 3.15 documentation
    locator: -P
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.11.html#other-language-changes
    title: What’s New In Python 3.11
    locator: Other Language Changes
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.13.html#improved-error-messages
    title: What’s New In Python 3.13
    locator: Improved error messages
    accessed: 2026-11-18
---

## Synopsis

```python fails id=list-shadowed expect=exception:TypeError
list = [3, 1, 2]
print(sorted(list))

letters = list("abc")
```

After the first line, the name `list` in this module is a list object. The call `list("abc")` calls that object, not the built-in type.

## Semantics

**r1.** Python looks up a name in scopes from the inside out, and "the outermost scope (searched last) is the namespace containing built-in names".[^1] The reference says the same for a global name: "If the names are not found there, the builtins namespace is searched next."[^2]

**r2.** So a global or local name with the same name as a builtin wins. The builtin is not changed. It is only hidden in that scope. Example: [`list-shadowed`](#list-shadowed). A `del` of the global name shows the builtin again. Example: [`del-restores`](#del-restores).

**r3.** Ruff rule A001 "Checks for variable (and function) assignments that use the same names as builtins."[^3] Its reason: "Reusing a builtin name for the name of a variable increases the difficulty of reading and maintaining the code, and can cause non-obvious errors, as readers may mistake the variable for the builtin and vice versa."[^3]

**r4.** A file can hide a module. "The directory containing the script being run is placed at the beginning of the search path, ahead of the standard library path. This means that scripts in that directory will be loaded instead of modules of the same name in the library directory. This is an error unless the replacement is intended."[^4] Example: [`script-named-random`](#script-named-random).

**r5.** Ruff rule A005 "Checks for modules that use the same names as Python standard-library modules."[^5] Its reason: "Reusing a standard-library module name for the name of a module increases the difficulty of reading and maintaining the code, and can cause non-obvious errors."[^5]

**r6.** The option `-P` removes the script directory from the search path: "Don’t prepend the potentially unsafe path sys._path0 to sys.path."[^6] The environment variable `PYTHONSAFEPATH` does the same.[^7] Example: [`script-named-random`](#script-named-random).

**r7.** A variable can also hide a module that the code imported. After `json = json.dumps(data)`, the name `json` is a string, and the next `json.loads` fails. Example: [`module-shadowed`](#module-shadowed).

## Examples

```python run id=del-restores
list = [3, 1, 2]
print(type(list).__name__)

del list
print(list("ab"))
```

```output for=del-restores
list
['a', 'b']
```

After `del list`, the global name is gone, so the lookup goes on to the builtins again (rules r1 and r2).

```python run id=script-named-random
import pathlib
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    script = pathlib.Path(folder, "random.py")
    script.write_text("import random\nprint(random.randint(7, 7))\n")

    plain = subprocess.run(
        [sys.executable, str(script)], capture_output=True, text=True
    )
    print(plain.returncode != 0)
    print("same name as the standard library module named 'random'" in plain.stderr)

    safe = subprocess.run(
        [sys.executable, "-P", str(script)], capture_output=True, text=True
    )
    print(safe.stdout.strip())
```

```output for=script-named-random
True
True
7
```

With no option, the line `import random` in `random.py` imports the file itself, which has no `randint` (rule r4). With `-P`, the import finds the standard module (rule r6).

## Versions

> [!since v="3.11"]
> "Added a -P command line option and a PYTHONSAFEPATH environment variable, which disable the automatic prepending to sys.path of the script’s directory when running a script, or the current directory when using -c and -m."[^7]

> [!since v="3.13"]
> The error message names the cause. "A common mistake is to write a script with the same name as a standard library module. When this results in errors, we now display a more helpful error message".[^8] The message adds "(consider renaming '/home/me/random.py' since it has the same name as the standard library module named 'random' and prevents importing that standard library module)".[^8]

## Pitfalls

```python fails id=module-shadowed expect=exception:AttributeError
import json

data = {"a": 1}
json = json.dumps(data)
print(json)

back = json.loads('{"b": 2}')
```

The fourth line makes `json` a string, so the module is hidden in the rest of the module (rule r7). Use a name such as `text`.

```python fails id=builtin-in-function expect=exception:TypeError
def largest(rows):
    max = 0
    for row in rows:
        for value in row:
            max = max(max, value)
    return max


print(largest([[1, 5], [3]]))
```

Inside `largest`, `max` is a local integer, so `max(max, value)` calls an integer (rule r2). Ruff A001 reports this assignment (rule r3). Use a name such as `best`.

## Comparison

In Python, the builtins are not keywords. They are names in the last scope of the lookup, so any assignment can hide them (rule r1). The keywords, such as `if` and `class`, cannot be names at all. For the lookup rules, see [[python/name-resolution]]. For the search path, see [[python/the-module-search-path]]. A module that imports itself through a hidden name is also a kind of [[python/circular-imports]] problem.
