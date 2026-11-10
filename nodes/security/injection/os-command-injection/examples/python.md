```python wrong id=shell-true
import subprocess


def look_up(name):
    subprocess.run("echo looking up " + name, shell=True)


look_up("report.txt")
look_up("report.txt; echo INJECTED")
```

```output for=shell-true
looking up report.txt
looking up report.txt
INJECTED
```

The shell splits the second command line at `;` and runs two commands. The second command is harmless here, but it could be any command that the account of the program may run.

```python run id=argv
import subprocess
import sys


def look_up(name):
    subprocess.run(["echo", "looking up", name])


look_up("report.txt")
look_up("report.txt; echo INJECTED")

# What the child process gets: one argument per list item.
child = [sys.executable, "-c", "import sys; print(sys.argv[1:])"]
subprocess.run(child + ["report.txt; echo INJECTED", "$HOME", "a b"])
```

```output for=argv
looking up report.txt
looking up report.txt; echo INJECTED
['report.txt; echo INJECTED', '$HOME', 'a b']
```

```python run id=quote
import shlex
import subprocess


def look_up(name):
    subprocess.run("echo looking up " + shlex.quote(name), shell=True)


look_up("report.txt; echo INJECTED")
print(shlex.quote("report.txt; echo INJECTED"))
```

```output for=quote
looking up report.txt; echo INJECTED
'report.txt; echo INJECTED'
```

- Verdict: stdlib. `subprocess` takes the program and its arguments as a list and does not start a shell unless the call has `shell=True`. With a list, no character in an argument has a meaning, because no shell parses it.
- With `shell=True`, the program must quote every value itself. `shlex.quote` puts a value in single quotes for a POSIX shell, as in the third example. The Python documentation says that this works only on some platforms. On Windows, `cmd.exe` has other quoting rules.
- `os.system` and `os.popen` always start a shell. Prefer `subprocess.run` with a list.
- A list stops a second command. It does not stop argument injection: a value that starts with `-` can still be an option of the program. Put `--` before the value when the program supports it, or check that the value does not start with `-`.
- Ruff rules S602 and S605 report `subprocess` calls with `shell=True` and functions that start a shell. CodeQL query `py/command-line-injection` follows input to them.
