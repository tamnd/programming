---
tag: 9HX2Q0
kind: concept
slug: process
name: Process
aliases: [OS process, running program, heavyweight process, child process, process isolation]
area: concurrency-and-parallelism/processes-and-threads
level: intro
summary: A process is a running program with its own address space. Its memory is separate from other processes, so processes share data only through the operating system.
applies_to: core-languages
viz: optional
history:
  year: 1965
  note: OSTEP traces the process to the multiprogrammed systems of the 1960s, among them an early paper on Multics from 1965 and a paper by Dennis and Van Horn from 1966.
see_also: [threads, run-a-subprocess, message-passing-concept, green-threads, start-a-thread, stack-and-heap]
sources:
  - url: https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-intro.pdf
    title: "Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces, chapter 4, The Abstraction: The Process"
    locator: 4.1 The Abstraction; 4.4 Process States; 4.5 Data Structures; References
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
    title: POSIX.1-2024, Base Definitions, chapter 3, Definitions
    locator: 3.5 Address Space; 3.64 Child Process; 3.189 Live Process; 3.285 Process Lifetime; 3.426 Zombie Process
  - url: https://learn.microsoft.com/en-us/windows/win32/procthread/about-processes-and-threads
    title: Microsoft Learn, Win32 apps, About Processes and Threads
    locator: the first two paragraphs
  - url: https://docs.python.org/3/library/multiprocessing.html
    title: The Python Standard Library, multiprocessing, Process-based parallelism
    locator: Introduction; Contexts and start methods; Programming guidelines (Safe importing of main module); Process.exitcode
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fork.html
    title: POSIX.1-2024, fork
    locator: DESCRIPTION; RETURN VALUE
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/exec.html
    title: POSIX.1-2024, exec
    locator: DESCRIPTION, paragraph 1
  - url: https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf
    title: "Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces, chapter 5, Interlude: Process API"
    locator: 5.4 Why? Motivating The API
  - url: https://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf
    title: "Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces, chapter 26, Concurrency: An Introduction"
    locator: the introduction (context switch between threads); 26.1 Why Use Threads?
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: The Python Standard Library, concurrent.futures
    locator: ProcessPoolExecutor
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/ProcessBuilder.html
    title: Java SE 21 API, java.lang.ProcessBuilder
    locator: the class description
  - url: https://pkg.go.dev/os/exec
    title: Go standard library, package os/exec
    locator: Overview
  - url: https://doc.rust-lang.org/std/process/struct.Command.html
    title: "The Rust Standard Library, Struct std::process::Command"
    locator: the struct description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.process
    title: .NET API reference, Process Class
    locator: the summary of the class
  - url: https://nodejs.org/api/child_process.html
    title: Node.js documentation, Child process
    locator: the introduction
  - url: https://www.erlang.org/doc/system/ref_man_processes.html
    title: Erlang System Documentation, Processes
    locator: the introduction
---

## Idea

A process is a running program.[^1] The program on the disk is only a set of instructions and data. The operating system loads it into memory and runs it, and the result is a process. Each process has its own address space, which is the set of memory locations that the process or its threads can reference.[^2] POSIX defines a live process as "An address space with one or more threads executing within that address space, and the required system resources for those threads".[^2] On Windows, a process also has open handles to system objects, a security context, a unique process identifier, environment variables and "at least one thread of execution".[^3]

Because each process has its own memory, one process cannot change a variable of another process. In Python, the module `multiprocessing` starts a child process that runs a function. Here, the child adds 100 to a global variable and ends with the exit code 3. The parent waits for the child with `join()` and then reads its own copy of the variable.

```python run id=idea
import multiprocessing
import os
import sys

counter = 0


def work():
    global counter
    counter += 100
    print("child: counter =", counter)
    sys.exit(3)


if __name__ == "__main__":
    child = multiprocessing.Process(target=work)
    child.start()
    child.join()
    print("child exit code:", child.exitcode)
    print("child pid differs:", child.pid != os.getpid())
    print("parent: counter =", counter)
```

```output for=idea
child: counter = 100
child exit code: 3
child pid differs: True
parent: counter = 0
```

The child changed its own copy of `counter`, and the parent still sees 0. The value that `sys.exit` gives becomes the exit code of the child.[^4] The parent prints only after `join()` returns, so the output is the same on every run. Two processes that must share data send it through the operating system, for example through a pipe, a socket, a file or a region of shared memory. [[threads]] are the other choice: the threads of one process share its memory.

## How it works

### The state of a process

The operating system keeps the machine state of each process: what the program can read or change while it runs. This state includes the memory of the process, which is its address space, and the registers of the processor, among them the program counter.[^1] The operating system keeps a process list with an entry for each process. Some texts call this entry a process control block.[^1]

A process is in one of three basic states:[^1]

- **Running.** The process executes instructions on a processor.
- **Ready.** The process can run, but the operating system has chosen not to run it at the moment.
- **Blocked.** The process waits for an event, for example the end of a disk read. Another process can use the processor in the meantime.

```text
          scheduled
  Ready ------------> Running
    ^   <------------    |
    |     descheduled    | starts I/O
    |                    v
    +--- I/O done --- Blocked
```

With only a few processors, the operating system gives the illusion of many. It runs one process, stops it, runs another, and so on. This technique is time sharing of the CPU.[^1]

### Creation: fork, exec and spawn

On POSIX systems, `fork` creates a new process, the child, as a copy of the calling process, the parent. The child gets a unique process ID. It has its own copy of the file descriptors of the parent, and each copy refers to the same open file description as in the parent.[^5] `fork` returns 0 in the child and the process ID of the child in the parent, so the two copies can take different paths.[^5]

The functions of the `exec` family replace the program of the current process with a new program from an executable file. A successful `exec` does not return, because the new program overlays the old one.[^6] A shell runs a command with both calls. It forks, the child calls `exec`, and the parent waits for the child. OSTEP explains why the two calls are separate. The child can run code between `fork` and `exec`, and this code can change the environment of the new program, for example to redirect its output to a file.[^7]

POSIX also lists `posix_spawn` and `posix_spawnp` as ways to create a child process.[^2] They create the child and start the new program in one call. The process APIs of most languages, such as `ProcessBuilder` in Java or `Command` in Rust, also start a program in one step.

### Exit status and zombies

A process ends when its program ends or when a signal stops it. It leaves an exit status for its parent. Until the parent consumes this status, the remains of the process stay in the system as a zombie process.[^2] The lifetime of a process ends when its process ID goes back to the system.[^2] A parent that never waits for its children therefore keeps zombies, and each one holds a process ID.

### Processes and threads

| | Processes | Threads of one process |
|---|---|---|
| Memory | each has its own address space | all share the address space of the process |
| Shared data | only through the operating system | any variable that both threads can reach |
| A bad write | stays inside the process | can corrupt the data of every thread |
| Context switch | changes the address space | keeps the address space |
| Typical use | separate programs; isolation | parallel work on shared data |

When the operating system switches between two threads of one process, the address space stays the same, so it does not change the page table.[^8] POSIX says that many resources are shared among all threads of a process, among them the process ID and the parent process ID.[^2] OSTEP gives the rule for a choice. Threads share an address space, which makes it easy to share data. Processes "are a more sound choice for logically separate tasks where little sharing of data structures in memory is needed."[^8]

### Process-based concurrency

Several processes can run at the same time on several processors. Python uses this to get parallel work in the default build of CPython, where only one thread at a time runs Python bytecode. The `multiprocessing` module offers "effectively side-stepping the Global Interpreter Lock by using subprocesses instead of threads".[^4] A `ProcessPoolExecutor` gives the same through the `concurrent.futures` API.[^9]

The price is the copy. The arguments of a `multiprocessing.Process` usually need to be picklable, because the module sends them to the child.[^4] A `ProcessPoolExecutor` can execute and return only picklable objects.[^9]

Python has three start methods for a child process:[^4]

- **spawn** starts a fresh interpreter. The child inherits only the resources that it needs to run the target. This method is slower than the other two.
- **fork** forks the interpreter, so the child begins as a copy of the parent and inherits all its resources.
- **forkserver** starts a server process, which then forks a child for each request. It became the default on POSIX platforms in Python 3.14, to keep the speed of fork and avoid the problems of forking a multi-threaded process.

## In languages

The example files start a child process and show that the child works on its own copy of the data. The C++ example uses `fork`. The other examples start `sh` with an environment variable, and the child changes its copy of the variable. The table gives the verdict for each tier 1 language.

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `subprocess`, `multiprocessing`, `os.fork` |
| JavaScript | stdlib | `node:child_process` in Node.js |
| TypeScript | stdlib | as JavaScript |
| Java | stdlib | `ProcessBuilder`, `Process`, `ProcessHandle` |
| Kotlin | stdlib | the Java classes on the JVM |
| C# | stdlib | `System.Diagnostics.Process` |
| Go | stdlib | `os/exec`; `os.StartProcess` |
| Rust | stdlib | `std::process::Command` |
| C | stdlib | the POSIX calls `fork`, `exec`, `waitpid` and `posix_spawn` |
| C++ | stdlib | the POSIX calls of C |
| Bash | native | each command runs in a process; `&`, `$!` and `wait` |
| PowerShell | stdlib | `Start-Process`, `Get-Process` |
| SQL | n/a | a query runs inside the database server |
| Regex | n/a | a pattern has no processes |
| HTML/CSS | n/a | markup and style rules have no processes |

- **Java.** `ProcessBuilder` is the class that is "used to create operating system processes". Its `start()` method returns a new `Process`, and it can be called many times to start more processes with the same attributes.[^10]
- **Go.** The package `os/exec` runs external commands. It does not start a shell: it "intentionally does not invoke the system shell and does not expand any glob patterns".[^11]
- **Rust.** `Command` is "A process builder, providing fine-grained control over how a new process should be spawned".[^12]
- **C#.** The class `Process` "Provides access to local and remote processes and enables you to start and stop local system processes."[^13]
- **JavaScript.** Node.js has the module `node:child_process`, which spawns subprocesses "in a manner that is similar, but not identical, to popen(3)".[^14]

## Variations and related ideas

- **[[threads]].** Threads run inside one process and share its memory. They are cheaper to switch between, but a data race in one thread can break the others.
- **Lightweight processes in a runtime.** Erlang calls its unit of concurrency a process, but the Erlang runtime creates and schedules it. Erlang processes "are lightweight (grow and shrink dynamically) with a small memory footprint, fast to create and terminate".[^15] See [[green-threads]].
- **Process pools.** A pool keeps a set of worker processes and gives them tasks, so the program does not pay for a new process for each task. `ProcessPoolExecutor` is one.[^9]
- **Inter-process communication.** Pipes, sockets, files and shared memory carry data between processes. A design that sends values and shares nothing follows [[message-passing-concept]].
- **[[run-a-subprocess]].** The task shows how to start a command, pass its arguments safely and read its output and exit status.

## Pitfalls

- **Expecting shared variables.** A child process gets a copy of the data, or a fresh start. A change in the child does not reach the parent, as the example in the Idea section shows. Send results back through a pipe, a queue or the exit status.
- **fork in a multi-threaded process.** The child contains a replica of the calling thread only, with the whole address space. A lock that another thread held at the time of `fork` can stay locked forever in the child. POSIX therefore says that the child must only execute async-signal-safe operations until it calls `exec`.[^5] The Python documentation also says that "safely forking a multithreaded process is problematic".[^4]
- **The main module and spawn.** With the spawn and forkserver start methods, a new interpreter imports the main module. Code at the top level that starts a process would run again in the child. Protect the entry point of the program with `if __name__ == '__main__':`.[^4]
- **Zombies.** A parent that does not wait for its children leaves zombie processes.[^2] Call `wait`, `join()` or the equivalent for every child.
- **The shell.** A command string that passes through a shell can expand variables, globs and other syntax in the arguments. Pass a list of arguments when possible; `os/exec` in Go does not start a shell at all.[^11]

## Cost

- **Start.** A new process needs its own address space and a new copy, or a fresh load, of the program. Python's spawn start method is "rather slow compared to using fork or forkserver".[^4]
- **Switch.** A context switch between processes changes the address space. A switch between threads of one process does not.[^8]
- **Communication.** Data between processes is copied, and in Python it is pickled first.[^4] For large data, the copy can cost more than the work itself.

## History

The process comes from the multiprogrammed and time-shared systems of the 1960s. OSTEP gives two early sources for its definition of the process and its states. One is a paper on the supervisor of Multics, from 1965, which "described many of the basic ideas and terms that we find in modern systems". The other is a paper by Jack B. Dennis and Earl C. Van Horn, from 1966, which "defined many of the early terms and concepts around building multiprogrammed systems."[^1] The [[threads]] page tells how Saltzer's thesis of 1966 gave the process the alternative name "thread".

## References
