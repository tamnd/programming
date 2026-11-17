---
tag: 5H8HS4
kind: feature
owner: python
slug: application-distribution
name: Application Distribution
aliases: [zipapp, .pyz, Python zip application, entry points, console_scripts, entry_points.txt, freezing, PyInstaller, standalone executable, shebang]
area: languages/python/tooling
level: advanced
summary: >-
  Ways to give a program to users: a zip application run by an installed Python, a console_scripts entry point that an installer makes into a command, or a frozen bundle with its own interpreter.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/zipapp.html", locator: "The Python Zip Application Archive Format"}
see_also: [package-a-program-for-release, python-packaging-tools, python/executing-modules-as-scripts, python/the-main-module, python/import-hooks, python/the-module-search-path, python/linters-and-formatters]
sources:
  - url: https://docs.python.org/3.15/library/zipapp.html
    title: zipapp, Python 3.15 documentation
    locator: module overview; Command-Line Interface; Creating Standalone Applications with zipapp; Caveats; The Python Zip Application Archive Format
    accessed: 2026-11-18
  - url: https://packaging.python.org/en/latest/specifications/entry-points/
    title: Entry points specification, Python Packaging User Guide
    locator: Data model; File format; Use for scripts
    accessed: 2026-11-18
  - url: https://packaging.python.org/en/latest/overview/
    title: An Overview of Packaging for Python, Python Packaging User Guide
    locator: Python source distributions; Bringing your own Python executable
    accessed: 2026-11-18
  - url: https://pyinstaller.org/en/stable/operating-mode.html
    title: What PyInstaller Does and How It Does It, PyInstaller documentation
    locator: introduction; Bundling to One Folder; Bundling to One File
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Compile packages and dependencies
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile
import zipfile

with tempfile.TemporaryDirectory() as tmp:
    tmp = os.path.realpath(tmp)
    package = pathlib.Path(tmp, 'src', 'greet')
    package.mkdir(parents=True)
    (package / '__init__.py').write_text('')
    (package / 'cli.py').write_text(
        'import sys\n\n\ndef main():\n'
        '    print("hello,", *sys.argv[1:])\n'
        '    print("loaded from", __file__)\n')
    subprocess.run([sys.executable, '-m', 'zipapp', 'src', '-m', 'greet.cli:main',
                    '-p', '/usr/bin/env python3', '-o', 'greet.pyz'], cwd=tmp, check=True)
    archive = pathlib.Path(tmp, 'greet.pyz')
    print('first line:', archive.read_bytes().split(b'\n')[0])
    print('executable:', os.access(archive, os.X_OK))
    print('members:', sorted(zipfile.ZipFile(archive).namelist()))
    print(zipfile.ZipFile(archive).read('__main__.py').decode().strip())
    for command in ([sys.executable, 'greet.pyz', 'Ada'], ['./greet.pyz', 'Grace']):
        result = subprocess.run(command, cwd=tmp, capture_output=True, text=True)
        print('$', *command[-2:], '->', result.stdout.replace(tmp, '<tmp>').splitlines())
```

```output for=synopsis
first line: b'#!/usr/bin/env python3'
executable: True
members: ['__main__.py', 'greet/', 'greet/__init__.py', 'greet/cli.py']
# -*- coding: utf-8 -*-
import greet.cli
greet.cli.main()
$ greet.pyz Ada -> ['hello, Ada', 'loaded from <tmp>/greet.pyz/greet/cli.py']
$ ./greet.pyz Grace -> ['hello, Grace', 'loaded from <tmp>/./greet.pyz/greet/cli.py']
```

`python -m zipapp` packs the directory `src` into one file, `greet.pyz`. `-m greet.cli:main` makes it write a `__main__.py` that imports the module and calls the function, and `-p` puts a `#!` line at the start and sets the executable bit. The interpreter runs the archive when it is given as the script, and on POSIX the archive also runs as a command by itself. The module's `__file__` is a path inside the archive, because the code is imported from the zip file and not from a folder.

## Semantics

**r1.** A zip application is a zip file with a main module. zipapp "provides tools to manage the creation of zip files containing Python code, which can be executed directly by the Python interpreter".[^1] "In order to be executed by Python, an application archive simply has to be a standard zip file containing a __main__.py file which will be run as the entry point for the application. As usual for any Python script, the parent of the script (in this case the zip file) will be placed on sys.path and thus further modules can be imported from the zip file."[^1]

**r2.** The command line options set the start. `-m` will "Write a __main__.py file to the archive that executes mainfn . The mainfn argument should have the form “pkg.mod:fn”, where “pkg.mod” is a package/module in the archive, and “fn” is a callable in the given module."[^1] `-p` will "Add a #! line to the archive specifying interpreter as the command to run. Also, on POSIX, make the archive executable. The default is to write no #! line, and not make the file executable."[^1]

**r3.** The archive needs an interpreter on the user's machine. With zipapp, "it is possible to create self-contained Python programs, which can be distributed to end users who only need to have a suitable version of Python installed on their system. The key to doing this is to bundle all of the application’s dependencies into the archive, along with the application code."[^1] The docs install the dependencies into the source folder first with `python -m pip install -r requirements.txt --target myapp`.[^1]

**r4.** An installed package can declare commands. "Entry points are a mechanism for an installed distribution to advertise components it provides to be discovered and used by other code."[^2] "Distributions can specify console_scripts entry points, each referring to a function. When pip (or another console_scripts aware installer) installs the distribution, it will create a command-line wrapper for each entry point."[^2] The function "will be called with no arguments when this command is run. The function may return an integer to be used as a process exit code, and returning None is equivalent to returning 0 ."[^2]

**r5.** Entry points are data in the installed package. "Entry points are defined in a file called entry_points.txt in the *.dist-info directory of the distribution."[^2] "The file contents are in INI format, as read by Python’s configparser module."[^2] Each entry point has a group: "the group console_scripts is for entry points referring to functions which can be used as a command".[^2] Groups are not only for commands: "Applications can use entry points to load plugins".[^2]

**r6.** A frozen program brings its own interpreter. The packaging overview calls it freezing: "embedding the Python interpreter and any other dependencies into a single executable file. This approach, called freezing , offers wide compatibility and seamless user experience, though often requires multiple technologies, and a good amount of effort."[^3] PyInstaller is one freezer: "It analyzes your code to discover every other module and library your script needs in order to execute. Then it collects copies of all those files – including the active Python interpreter! – and puts them with your script in a single folder, or optionally in a single executable file."[^4]

## Examples

### Entry points from a dist-info folder

```python run id=entry-points
import importlib.metadata
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as tmp:
    root = pathlib.Path(tmp)
    (root / 'tally.py').write_text('def main():\n    return "tally main ran"\n\n\ndef csv_reader():\n    return "csv plugin"\n')
    info = root / 'tally-1.0.dist-info'
    info.mkdir()
    (info / 'METADATA').write_text('Metadata-Version: 2.1\nName: tally\nVersion: 1.0\n')
    (info / 'entry_points.txt').write_text(
        '[console_scripts]\ntally = tally:main\n\n[tally.readers]\ncsv = tally:csv_reader\n')
    sys.path.insert(0, tmp)
    dist = importlib.metadata.distribution('tally')
    for ep in dist.entry_points:
        print(f'{ep.group:16} {ep.name:6} {ep.value:18} module={ep.module} attr={ep.attr}')
    (script,) = importlib.metadata.entry_points(group='console_scripts', name='tally')
    print(script.load()())
    for ep in importlib.metadata.entry_points(group='tally.readers'):
        print(ep.name, '->', ep.load()())
    sys.path.remove(tmp)
```

```output for=entry-points
console_scripts  tally  tally:main         module=tally attr=main
tally.readers    csv    tally:csv_reader   module=tally attr=csv_reader
tally main ran
csv -> csv plugin
```

The example writes by hand the two files that an installer puts in a `.dist-info` folder: `METADATA` with the name and version, and `entry_points.txt` with two groups (r5). With the folder on `sys.path`, `importlib.metadata` finds the distribution. The `console_scripts` entry is the one that pip would turn into a `tally` command (r4); here `load()` imports `tally` and returns `main`, and the program calls it. The group `tally.readers` is a plugin group: the program asks for all entry points in the group and loads each one, so another installed package could add a reader without a change to `tally`.

### A C extension in a zip file

```python run id=extension
import importlib.util
import os
import shutil
import subprocess
import sys
import tempfile
import zipfile

origin = importlib.util.find_spec('ujson').origin
name = os.path.basename(origin)
print('extension module:', os.path.splitext(name)[1])
code = 'import sys; sys.path.insert(0, sys.argv[1]); import ujson; print(ujson.dumps([1, 2]))'
with tempfile.TemporaryDirectory() as tmp:
    folder = os.path.join(tmp, 'deps')
    os.mkdir(folder)
    shutil.copy(origin, folder)
    with zipfile.ZipFile(os.path.join(tmp, 'deps.zip'), 'w') as archive:
        archive.write(origin, name)
    for place in ('deps', 'deps.zip'):
        result = subprocess.run([sys.executable, '-I', '-S', '-c', code, place],
                                cwd=tmp, capture_output=True, text=True)
        print(f'{place:8}', result.stdout.strip() or result.stderr.strip().splitlines()[-1])
```

```output for=extension
extension module: .so
deps     [1,2]
deps.zip ModuleNotFoundError: No module named 'ujson'
```

`ujson` is a third-party JSON library with its code in a compiled extension module; it is in the check environment as a dependency of another tool. The same file is copied into a folder and into a zip file. From the folder it imports; from the zip file the import system does not find it. The zipapp docs explain: "If your application depends on a package that includes a C extension, that package cannot be run from a zip file (this is an OS limitation, as executable code must be present in the filesystem for the OS loader to load it)."[^1] `-S` keeps the installed copy of `ujson` off the path, so only the copy in `deps` or `deps.zip` can be found.

## Pitfalls

- A `.pyz` file is not a standalone executable. The user needs "a suitable version of Python installed" (r3), and the `#!` line names the interpreter to use.
- Packages with C extensions do not import from the archive (see the last example). The docs suggest that you "exclude that dependency from the zipfile, and either require your users to have it installed, or ship it alongside your zipfile".[^1]
- In a zip application, `__file__` is a path inside the archive, such as `<tmp>/greet.pyz/greet/cli.py` in the synopsis. No folder has that path, so code that builds file paths from `__file__` must be checked in the archive form too.
- A one-file frozen program "is a little slower to start up than the one-folder bundle. Before you attempt to bundle to one file, make sure your app works correctly when bundled to one folder."[^4]
- A console_scripts command is made by the installer (r4). A `.dist-info` folder that is not installed, as in the entry points example, gives no command on the shell's path.

## Comparison

Go builds a program into an executable file. "Build compiles the packages named by the import paths, along with their dependencies, but it does not install the results."[^5] "When compiling a single main package, build writes the resulting executable to an output file named after the last non-major-version component of the package import path. The '.exe' suffix is added when writing a Windows executable."[^5] Of the Python forms, a frozen bundle is the nearest to this, because it is an executable file (r6). A zip application needs an installed Python (r3).

[^1]: zipapp, Python 3.15 documentation.
[^2]: Entry points specification, Python Packaging User Guide.
[^3]: An Overview of Packaging for Python, Python Packaging User Guide.
[^4]: What PyInstaller Does and How It Does It, PyInstaller documentation.
[^5]: Command go, Go documentation.
