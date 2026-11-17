---
tag: 3BD228
kind: feature
owner: python
slug: shell-true-injection
name: shell=True injection
aliases: [shell injection, command injection, subprocess shell=True, os.system injection, shlex.quote, CWE-78, S602, subprocess-popen-with-shell-equals-true]
area: languages/python/pitfalls
level: core
summary: >-
  With shell=True, a string from a user becomes part of a shell command, so a ; or a $( ) in it runs other commands. Pass a list of arguments and no shell, or quote each part with shlex.quote.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/subprocess.html#security-considerations", locator: "subprocess, Security Considerations"}
proposals: []
see_also: [python/subprocess, python/operating-system-interface, python/command-line-arguments, python/eval-of-untrusted-input, python/environment-variables-from-python, os-command-injection]
sources:
  - url: https://docs.python.org/3.15/library/subprocess.html#security-considerations
    title: subprocess, Subprocess management, Python 3.15 documentation
    locator: Frequently Used Arguments; Popen Constructor; Security Considerations
    accessed: 2026-11-18
  - url: https://cwe.mitre.org/data/definitions/78.html
    title: CWE-78, Improper Neutralization of Special Elements used in an OS Command
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/shlex.html#shlex.quote
    title: shlex, Simple lexical analysis, Python 3.15 documentation
    locator: shlex.quote()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/os.html#os.system
    title: os, Miscellaneous operating system interfaces, Python 3.15 documentation
    locator: os.system()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/subprocess-popen-with-shell-equals-true/
    title: subprocess-popen-with-shell-equals-true (S602), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=injected
import pathlib
import subprocess
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "notes.txt").write_text("hello\n", encoding="utf-8")
    name = "notes.txt; echo INJECTED"

    result = subprocess.run(
        f"cat {name}", shell=True, cwd=folder, capture_output=True, text=True
    )
    print(result.stdout, end="")
```

```output for=injected
hello
INJECTED
```

The shell read the `;` in the name as the end of one command and ran a second command. In a real attack, the second command can delete files or start a remote shell.

## Semantics

**r1.** `subprocess` does not use a shell unless you ask: "Unlike some other popen functions, this library will not implicitly choose to call a system shell. This means that all characters, including shell metacharacters, can safely be passed to child processes."[^1]

**r2.** With `shell=True`, the whole string goes to the shell. "On POSIX with shell=True, the shell defaults to /bin/sh. If args is a string, the string specifies the command to execute through the shell. This means that the string must be formatted exactly as it would be when typed at the shell prompt."[^1] So the shell reads `;`, `|`, `&`, `$( )`, quotes and spaces in any part of the string. Example: [`injected`](#injected).

**r3.** The responsibility then moves to the program: "If the shell is invoked explicitly, via shell=True, it is the application’s responsibility to ensure that all whitespace and metacharacters are quoted appropriately to avoid shell injection vulnerabilities."[^1]

**r4.** This is CWE-78: "The product constructs all or part of an OS command using externally-influenced input from an upstream component, but it does not neutralize or incorrectly neutralizes special elements that could modify the intended OS command before it is sent to a downstream component."[^2] For the general class of flaw, see [[os-command-injection]].

**r5.** Fix 1, pass a list and no shell. "Unless otherwise stated, it is recommended to pass args as a sequence."[^1] Each item reaches the program as one argument, so a `;` in a name is only a character. Example: [`list-args`](#list-args).

**r6.** Fix 2, when a shell is needed, quote each part from outside with `shlex.quote`. The documentation shows the hole and the fix: "quote() lets you plug the security hole".[^3] But "The shlex module is only designed for Unix shells."[^3] and "Executing commands quoted by this module on such shells can open up the possibility of a command injection vulnerability."[^3] It advises: "Consider using functions that pass command arguments with lists such as subprocess.run() with shell=False."[^3] Example: [`quoted`](#quoted).

**r7.** Other calls also use a shell. `os.system` will "Execute the command (a string) in a subshell."[^4] A list such as `["sh", "-c", text]` also gives the text to a shell. Example: [`sh-c`](#sh-c).

**r8.** Ruff S602 checks "for method calls that initiate a subprocess with a shell."[^5] Its reason: "Starting a subprocess with a shell can allow attackers to execute arbitrary shell commands."[^5]

## Examples

```python run id=list-args
import pathlib
import subprocess
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "notes.txt").write_text("hello\n", encoding="utf-8")

    for name in ["notes.txt", "notes.txt; echo INJECTED"]:
        result = subprocess.run(
            ["cat", name], cwd=folder, capture_output=True, text=True
        )
        print(result.returncode, repr(result.stdout))
```

```output for=list-args
0 'hello\n'
1 ''
```

`cat` got the second name as one argument and found no file with that name. No second command ran (rule r5).

```python run id=quoted
import shlex

name = "notes.txt; echo INJECTED"
command = f"cat {shlex.quote(name)}"
print(command)
print(shlex.split(command))
```

```output for=quoted
cat 'notes.txt; echo INJECTED'
['cat', 'notes.txt; echo INJECTED']
```

The quoted name is one word for a Unix shell (rule r6).

## Versions

The rules are the same in all Python 3 versions.

> [!since v="3.3"]
> `shlex.quote` is added to the `shlex` module.[^3]

## Pitfalls

```python wrong id=sh-c
import subprocess

name = "ann; echo INJECTED"
result = subprocess.run(
    ["sh", "-c", f"echo hello {name}"], capture_output=True, text=True
)
print(result.stdout, end="")
```

```output for=sh-c
hello ann
INJECTED
```

The code has no `shell=True`, but `sh -c` is a shell, and the name is part of its command text (rule r7).

```python wrong id=quote-is-not-enough
import shlex
import subprocess

for word in ["hello", "-n"]:
    result = subprocess.run(
        f"echo {shlex.quote(word)}", shell=True, capture_output=True, text=True
    )
    print(repr(result.stdout))
```

```output for=quote-is-not-enough
'hello\n'
''
```

The quoting kept `-n` as one word, but `echo` read the word as an option, because it starts with `-`, and printed nothing. Quoting stops shell injection, not option injection. Validate the value, or put `--` before it when the program supports that.

## Comparison

In Python, `subprocess` starts a program with a list of arguments and no shell, unless the code asks for a shell (rule r1). Shell injection needs a shell, such as `shell=True`, `os.system` or `sh -c`, with text from outside in the command. For the module, see [[python/subprocess]]. For the same kind of flaw in code, see [[python/eval-of-untrusted-input]].
