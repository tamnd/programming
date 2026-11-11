---
tag: J39M4D
kind: task
slug: run-a-subprocess
name: Run a subprocess
aliases: [run an external command, run a program and capture its output, spawn a child process, exec a command]
area: systems-programming/processes-and-signals
level: core
summary: Start a program with a list of arguments and no shell, read its standard output and standard error, and return its exit status. A missing program is an error.
contract: contract.yaml
category: processes-and-the-os
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/posix_spawn.html
    title: The Open Group, POSIX.1-2024, posix_spawn, posix_spawnp
    locator: Description, Errors
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/waitpid.html
    title: The Open Group, POSIX.1-2024, wait, waitpid
    locator: Description, WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html
    title: The Open Group, POSIX.1-2024, Shell Command Language
    locator: 2.6.3 Command Substitution, 2.8.2 Exit Status for Commands
  - url: https://docs.python.org/3/library/subprocess.html#subprocess.run
    title: The Python Standard Library, subprocess, Subprocess management
    locator: subprocess.run, subprocess.DEVNULL, Exceptions, Popen.wait, Popen.returncode, Security Considerations
  - url: https://docs.python.org/3/library/exceptions.html#FileNotFoundError
    title: The Python Standard Library, Built-in Exceptions
    locator: OS exceptions, FileNotFoundError
  - url: https://nodejs.org/api/child_process.html#child_processspawnsynccommand-args-options
    title: Node.js documentation, Child process
    locator: child_process.spawnSync, options.stdio, child_process.spawn, child_process.exec, child_process.execFile
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Process.html
    title: Java SE 25 API, java.lang.Process
    locator: class description, getOutputStream, waitFor
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ProcessBuilder.html
    title: Java SE 25 API, java.lang.ProcessBuilder
    locator: class description, start, environment, directory
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.processstartinfo.argumentlist?view=net-10.0
    title: .NET API reference, ProcessStartInfo.ArgumentList
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.process.standardoutput?view=net-10.0
    title: .NET API reference, Process.StandardOutput
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.process.start?view=net-10.0
    title: .NET API reference, Process.Start
    locator: Start(ProcessStartInfo), Exceptions
  - url: https://pkg.go.dev/os/exec
    title: Go standard library, package os/exec
    locator: Overview, Command, Cmd, Cmd.Run, ErrNotFound, ExitError
  - url: https://pkg.go.dev/os#ProcessState.ExitCode
    title: Go standard library, package os
    locator: ProcessState.ExitCode
  - url: https://doc.rust-lang.org/std/process/struct.Command.html
    title: The Rust Standard Library, std::process::Command
    locator: new, arg, output, spawn
  - url: https://doc.rust-lang.org/std/process/struct.ExitStatus.html
    title: The Rust Standard Library, std::process::ExitStatus
    locator: code
  - url: https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html
    title: GNU Bash Reference Manual, Exit Status
    locator: 3.7.5
  - url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
    title: GNU Bash Reference Manual, Bash Builtin Commands
    locator: 4.2, type
---

## Task

Run a program, wait for it to exit, and return what it did. The result has three parts: the exit status, the text on standard output and the text on standard error.

Each solution is a function `run(argv)`. The first element of `argv` is the program, and the other elements are its arguments. The solution finds the program through `PATH` and starts it with no shell, so each argument reaches the program as one string with no change. The standard input of the program is empty, so a read gets end of file at once. The program gets the environment and the working directory of the caller.

A status that is not 0 is a result, not an error. A program that is not in `PATH` is an error, because no status exists. A program that a signal stops is outside the contract. Each solution reports it in its own way.

The program can write to both streams at the same time. A pipe holds a limited amount of data. So the solution must read both pipes while the program runs. If it reads one pipe to its end first, the program can block on the other pipe, and the two processes wait for ever.

## Solutions

Each solution passes a list of arguments, captures the two streams, and reads them at the same time.

- **Python.** `subprocess.run(argv, stdin=subprocess.DEVNULL, capture_output=True, encoding="utf-8")`. `run` waits for the program and returns all of its output. `shell` is false by default. A missing program raises `FileNotFoundError`, a subclass of `OSError`. A status that is not 0 raises an exception only with `check=True`.
- **JavaScript and TypeScript (Node.js).** `spawnSync(file, args, {stdio: ["ignore", "pipe", "pipe"], encoding: "utf8", maxBuffer: Infinity})`. `'ignore'` attaches `/dev/null` to the standard input of the child. A missing program sets `result.error` with the code `ENOENT`, and the solution throws it. `result.status` is `null` when a signal stopped the child. The TypeScript file adds types only.
- **Java.** `new ProcessBuilder(argv).start()`. The solution closes `getOutputStream()`, the standard input of the child. It reads standard error on a second thread and standard output on the calling thread, and then calls `waitFor`. A missing program throws an `IOException`.
- **C#.** A `ProcessStartInfo` with `UseShellExecute = false` and the three `Redirect` properties set to true. Each argument goes into `ArgumentList`, which does the quoting. The solution reads standard error with `ReadToEndAsync` and standard output with `ReadToEnd`. A missing program throws a `Win32Exception`.
- **Go.** `exec.Command(argv[0], argv[1:]...)` with two `strings.Builder` values as `Stdout` and `Stderr`. A writer that is not a file makes the package copy the pipe on its own goroutine. A nil `Stdin` reads from the null device. `Run` returns an `*exec.ExitError` for a status that is not 0, and the solution takes the status from `cmd.ProcessState.ExitCode()`. A missing program gives an error that wraps `exec.ErrNotFound`.
- **Rust.** `Command::new(program).args(args).output()`. `output` collects both streams, waits for the child, and closes its standard input. It returns `Ok` also for a status that is not 0. `status.code()` is `None` when a signal stopped the child. A missing program gives an `io::Error` of the kind `NotFound`.
- **C.** The solution uses POSIX. `posix_spawnp` searches `PATH` and starts the program. Its file actions open `/dev/null` as file descriptor 0 and connect 1 and 2 to two pipes. A `poll` loop reads both pipes until each gives end of file. Then `waitpid`, `WIFEXITED` and `WEXITSTATUS` give the status.
- **C++.** The same POSIX calls, with `std::string` buffers and a class that closes each file descriptor. A missing program throws `std::system_error` with `std::errc::no_such_file_or_directory`.
- **Bash.** `run_program STATUS OUT ERR program args...` stores the three results in the variables that the caller names. `type -P` searches `PATH` for a file, so the solution runs the `printf` program in `PATH` and not the builtin `printf`. The program writes to two temporary files, and the solution reads them back. The function name is not `run`, because `bats` uses that name.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The case that must give an error is in `cases/errors.jsonl`.

- `metacharacters`: the arguments `a b`, `$HOME`, `*`, `x;y`, `"q"` and `'s'` reach `printf` with no change. With no shell, nothing expands them.
- `empty-argument`: an empty string is one argument. A command line in one string loses it.
- `newline-argument`: an argument can hold a newline.
- `trailing-newlines`: the output `a\n\n` comes back with both newlines. Shell command substitution removes them.
- `non-ascii`: the program writes UTF-8 text with characters of 2, 3 and 4 bytes.
- `stderr-only` and `both-streams`: the two streams stay apart.
- `false` and `status-255`: a status that is not 0 is a result. A status has 8 bits, so 255 is the largest value.
- `empty-stdin`: `read` in the child gets end of file at once.
- `missing-program`: no program has the name. Each solution reports the error, not a status. POSIX lets `posix_spawnp` start the child and then fail in it, and the child then exits with the status 127. So the C and C++ tests accept the status 127 or the error `ENOENT`.
- Large output: JSON cannot hold it, so each test also runs a program that writes 1 MiB to standard output and then 1 MiB to standard error. A solution that reads standard output to its end before it reads standard error blocks on this program.

## Pitfalls

- **One string through a shell.** A command line in one string goes through a shell. The shell splits words, expands variables and removes empty arguments. The list form passes each argument as one string:

```python run id=shell-split
import subprocess

arg = "two  spaces $NO_SUCH_VAR_7F3A end"
listed = subprocess.run(["printf", "[%s]\n", arg], capture_output=True, text=True)
print("list: ", listed.stdout, end="")
shelled = subprocess.run(f"printf '[%s]\\n' {arg}", shell=True, capture_output=True, text=True)
print("shell:", " ".join(shelled.stdout.split()))
```

```output
list:  [two  spaces $NO_SUCH_VAR_7F3A end]
shell: [two] [spaces] [end]
```

When the string holds input from a user, this is also [[os-command-injection]]. Python `shell=True`, Node.js `exec` and `bash -c` with a joined string all start a shell. Node.js `execFile` and `spawn` do not start a shell by default.

- **A status that is not 0 passes in silence.** Most of the functions return a result for a failed program and do not throw. The caller must check the status. Python raises `CalledProcessError` only with `check=True`:

```python run id=check
import subprocess

result = subprocess.run(["sh", "-c", "echo partial; exit 1"], capture_output=True, text=True)
print(repr(result.stdout), result.returncode)
try:
    subprocess.run(["sh", "-c", "exit 1"], check=True)
except subprocess.CalledProcessError as e:
    print("CalledProcessError", e.returncode)
```

```output
'partial\n' 1
CalledProcessError 1
```

Rust `output` returns `Ok`, Go `Run` returns an `*ExitError`, and Node.js `spawnSync` sets `status`. In Java and C#, read `waitFor()` or `ExitCode`.

- **A wait before the read.** A child that fills a pipe blocks until the parent reads. A parent that waits for the child before it reads waits for ever. The Python documentation gives this warning for `Popen.wait` and says to use `communicate`. The Java and .NET documentation give the same warning:

```python fails expect=timeout timeout=3 id=wait-deadlock
import subprocess

proc = subprocess.Popen(["sh", "-c", "head -c 1048576 /dev/zero"], stdout=subprocess.PIPE)
proc.wait()
print(len(proc.stdout.read()))
```

Two reads in sequence can also block: the parent waits for the end of standard output, and the child waits for space in the pipe of standard error. Read the two streams on two threads, with asynchronous reads, or with `poll`.

- **The size limit of `spawnSync`.** `maxBuffer` is 1 MiB on each stream by default. Past the limit, Node.js stops the child and cuts the output:

```javascript run id=max-buffer
import { spawnSync } from "node:child_process";

const script = "head -c 2097152 /dev/zero | tr '\\0' x; echo done >&2";
const r = spawnSync("sh", ["-c", script], { encoding: "utf8" });
console.log(r.status, r.signal, r.error?.code);
console.log(r.stdout.length < 2097152);
```

```output
null SIGTERM ENOBUFS
true
```

- **Trailing newlines in Bash.** `$(...)` removes all of the newlines at the end of the output. Add a character after the output, and remove it from the result:

```bash run id=trailing-newlines
out=$(printf 'a\n\n')
printf '[%s]\n' "$out"
out=$(printf 'a\n\n'; printf x)
out=${out%x}
printf '[%s]\n' "$out"
```

```output
[a]
[a

]
```

- **A status after a signal.** POSIX gives a status only to a child that exits. Python sets `returncode` to minus the signal number. Go `ExitCode` returns -1. Node.js sets `status` to `null` and puts the name in `signal`. A shell gives a status greater than 128, and Bash uses 128 plus the signal number. So the value -9 or -1 can look like a status:

```python run id=signal-python
import subprocess

print(subprocess.run(["sh", "-c", "kill -9 $$"]).returncode)
```

```output
-9
```

```go run id=signal-go
package main

import (
	"fmt"
	"os/exec"
)

func main() {
	cmd := exec.Command("sh", "-c", "kill -9 $$")
	err := cmd.Run()
	fmt.Println(cmd.ProcessState.ExitCode(), err)
}
```

```output
-1 signal: killed
```

- **What the child gets from the parent.** The child gets a copy of the environment and the working directory of the parent. In Go, a nil `Env` and an empty `Dir` give these values. In Java, `environment()` starts as a copy of the environment of the parent. To change a value for the child only, set it on the command, not in the parent. See [[read-an-environment-variable]].

## Related tasks

- [[parse-command-line-arguments]]: the program on the other side reads the list that `run` passes.
- [[read-from-standard-input]]: what the child reads when the parent gives it input.
- [[read-an-environment-variable]]: the child gets a copy of the environment.
- [[os-command-injection]]: the weakness of a command line in one string.

## References
