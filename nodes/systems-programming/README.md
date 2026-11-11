This area is about programs that work close to the operating system and the hardware. It covers the OS interface, processes, I/O, memory mapping, sockets, low-level languages and the view of the kernel that a programmer needs.

## What it holds

- The OS interface: system calls, POSIX and Win32, and how a program uses them.
- Processes and signals: creating processes, exit codes and signals.
- File descriptors and I/O models: file descriptors, blocking and non-blocking I/O, and I/O models.
- Memory mapping: mapping files and memory into the address space.
- Inter-process communication: pipes, shared memory and other ways for processes to talk.
- Networking sockets: network sockets from a program.
- Low-level C and Rust: systems code in C and Rust, and unsafe code.
- Assembly language: assembly languages and how to read them.
- Linkers and object files: object files, libraries and linkers from the view of a programmer.
- The kernel from a programmer's view: the kernel as a programmer sees it.

## What it does not hold

- Operating system design as a subject. This site links out for it.
- Threads and locks as concurrency ideas. They are in [Concurrency and parallelism](/concurrency-and-parallelism/).
- Bare-metal code with no OS. It is in [Embedded and hardware](/embedded-and-hardware/).
- HTTP and web servers. They are in [Web development](/web-development/).
