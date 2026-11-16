---
tag: 957C41
kind: feature
owner: python
slug: subprocess
name: The subprocess module
aliases: [subprocess.run, Popen, CompletedProcess, CalledProcessError, capture_output, shell=True, communicate, run a command from Python]
area: languages/python/io-and-os
level: core
summary: >-
  subprocess.run() starts a program, waits for it and returns its exit code and, if asked, its output. Popen gives lower-level control. A list of arguments avoids the shell and its quoting traps.
grammar: "subprocess.run(args, *, stdin=None, input=None, stdout=None, stderr=None, capture_output=False, shell=False, cwd=None, timeout=None, check=False, encoding=None, errors=None, text=None, env=None, universal_newlines=None, **other_popen_kwargs)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/subprocess.html", locator: "subprocess module"}
see_also: [python/operating-system-interface, python/environment-variables-from-python, python/error-output-redirection-and-program-termination, python/standard-streams, run-a-subprocess]
sources:
  - url: https://docs.python.org/3.15/library/subprocess.html
    title: subprocess, Python 3.15 documentation
    locator: Using the subprocess module; run(); CompletedProcess; TimeoutExpired; CalledProcessError; Popen constructor; Exceptions; Security considerations; Popen objects; Replacing shell pipeline
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/shlex.html#shlex.quote
    title: shlex, Python 3.15 documentation
    locator: shlex.quote()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os/exec
    title: Package exec, Go standard library
    locator: Overview; func Command; func (*Cmd) Output
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import subprocess

result = subprocess.run(['echo', 'hello', 'world'], capture_output=True, text=True)
print(result.returncode, repr(result.stdout))

result = subprocess.run(['sh', '-c', 'echo oops >&2; exit 3'],
                        capture_output=True, text=True)
print(result.returncode, repr(result.stderr))
```

```output for=synopsis
0 'hello world\n'
3 'oops\n'
```

The first child succeeds and the second fails with status 3. `run()` returns in both cases, with the status in `returncode`.

## Semantics

**r1.** "The subprocess module allows you to spawn new processes, connect to their input/output/error pipes, and obtain their return codes."[^1] "The recommended approach to invoking subprocesses is to use the run() function for all use cases it can handle. For more advanced use cases, the underlying Popen interface can be used directly."[^1]

**r2.** `run()` will "Run the command described by args. Wait for command to complete, then return a CompletedProcess instance."[^1] Its `returncode` is the "Exit status of the child process. Typically, an exit status of 0 indicates that it ran successfully. A negative value -N indicates that the child was terminated by signal N (POSIX only)."[^1]

**r3.** "args should be a sequence of program arguments or else a single string or path-like object."[^1] "Unless otherwise stated, it is recommended to pass args as a sequence."[^1] "On POSIX, if args is a string, the string is interpreted as the name or path of the program to execute. However, this can only be done if not passing arguments to the program."[^1]

**r4.** "If capture_output is true, stdout and stderr will be captured."[^1] The captured `stdout` is "A bytes sequence, or a string if run() was called with an encoding, errors, or text=True."[^1] For the stream arguments, "With the default settings of None, no redirection will occur."[^1] The child then writes to the parent's own streams.

**r5.** "If check is true, and the process exits with a non-zero exit code, a CalledProcessError exception will be raised. Attributes of that exception hold the arguments, the exit code, and stdout and stderr if they were captured."[^1] Example: [`check`](#check).

**r6.** The `input` argument goes to the child's stdin. "If used it must be a byte sequence, or a string if encoding or errors is specified or text is true."[^1] Example: [`input`](#input).

**r7.** With `timeout`, "If the timeout expires, the child process will be killed and waited for. The TimeoutExpired exception will be re-raised after the child process has terminated."[^1] Example: [`timeout`](#timeout).

**r8.** "The most common exception raised is OSError. This occurs, for example, when trying to execute a non-existent file."[^1]

**r9.** `Popen` will "Execute a child program in a new process."[^1] `communicate()` will "Send data to stdin. Read data from stdout and stderr, until end-of-file is reached. Wait for process to terminate and set the returncode attribute."[^1] `terminate()` "sends SIGTERM to the child" on POSIX.[^1] Example: [`pipeline`](#pipeline).

**r10.** "Unlike some other popen functions, this library will not implicitly choose to call a system shell. This means that all characters, including shell metacharacters, can safely be passed to child processes."[^1] With `shell=True`, "the specified command will be executed through the shell."[^1]

## Examples

```python run id=check
import subprocess

try:
    subprocess.run(['sh', '-c', 'exit 4'], check=True)
except subprocess.CalledProcessError as err:
    print(err.returncode, err.cmd)
    print(err)
```

```output for=check
4 ['sh', '-c', 'exit 4']
Command '['sh', '-c', 'exit 4']' returned non-zero exit status 4.
```

`check=True` turns a failed command into an exception, so a script does not go on after it (rule r5).

```python run id=input
import subprocess

result = subprocess.run(['sort'], input='pear\napple\nfig\n',
                        capture_output=True, text=True, check=True)
print(result.stdout.split())
```

```output for=input
['apple', 'fig', 'pear']
```

```python run id=timeout
import subprocess

LIMIT = 0.5
try:
    subprocess.run(['sleep', '5'], timeout=LIMIT)
except subprocess.TimeoutExpired:
    print('timed out after', LIMIT, 'seconds')
```

```output for=timeout
timed out after 0.5 seconds
```

The child is killed when the time runs out (rule r7).

```python run id=pipeline
from subprocess import PIPE, Popen

p1 = Popen(['printf', 'b\\na\\nb\\n'], stdout=PIPE)
p2 = Popen(['sort', '-u'], stdin=p1.stdout, stdout=PIPE, text=True)
p1.stdout.close()
out, _ = p2.communicate()
print(out.split(), p1.wait(), p2.returncode)

p3 = Popen(['sleep', '10'])
p3.terminate()
print(p3.wait())
```

```output for=pipeline
['a', 'b'] 0 0
-15
```

The first child writes into a pipe that the second child reads, as `printf ... | sort -u` would in a shell. "The p1.stdout.close() call after starting the p2 is important in order for p1 to receive a SIGPIPE if p2 exits before p1."[^1] The last child ends by SIGTERM, which is signal 15, so its status is -15 (rules r2 and r9).

## Pitfalls

A string with arguments in it, without `shell=True`, names a program that does not exist on POSIX (rules r3 and r8):

```python fails id=string-args expect=exception:FileNotFoundError
import subprocess

subprocess.run('echo hello')
```

Pass `['echo', 'hello']`.

`shell=True` with outside text lets that text run commands:

```python run id=injection
import shlex
import subprocess

name = 'notes.txt; echo INJECTED'
subprocess.run('echo ' + name, shell=True)
subprocess.run(['echo', name])
subprocess.run('echo ' + shlex.quote(name), shell=True)
```

```output for=injection
notes.txt
INJECTED
notes.txt; echo INJECTED
notes.txt; echo INJECTED
```

The docs say: "If the shell is invoked explicitly, via shell=True, it is the application's responsibility to ensure that all whitespace and metacharacters are quoted appropriately to avoid shell injection vulnerabilities."[^1] A list needs no quoting (rule r10). `shlex.quote()` returns "a shell-escaped version of the string s", for "cases where you cannot use a list."[^2]

`wait()` with pipes can hang. "This will deadlock when using stdout=PIPE or stderr=PIPE and the child process generates enough output to a pipe such that it blocks waiting for the OS pipe buffer to accept more data. Use Popen.communicate() when using pipes to avoid that."[^1]

Without `check=True`, a failed command is silent. `run()` returns a nonzero `returncode` and raises nothing (rule r2).

## Comparison

Go's `os/exec` "runs external commands."[^3] Like `run()` with a list, it "intentionally does not invoke the system shell and does not expand any glob patterns or handle other expansions, pipelines, or redirections typically done by shells."[^3] `exec.Command` "returns the Cmd struct to execute the named program with the given arguments",[^3] and `Output` "runs the command and returns its standard output. Any returned error will usually be of type *ExitError."[^3] Go reports a nonzero status as an error value, where Python's `run()` needs `check=True` to raise (rule r5).
