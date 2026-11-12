This area is about programs that work close to the operating system and the hardware. It covers the OS interface, processes, I/O, memory mapping, sockets, low-level languages and the view of the kernel that a programmer needs.

## What it holds

- **The OS interface.** How a program asks the operating system for services, and the information that the OS gives each process.
- **Command-line programs.** How to design and build a program that runs from a shell.
- **Processes and signals.** How a program starts other programs, waits for them, talks to them and handles signals.
- **File descriptors and I/O models.** File descriptors and handles, and the models of blocking, non-blocking and asynchronous I/O.
- **Memory mapping.** Virtual memory as a program uses it: mapped files, anonymous mappings and protections.
- **Inter-process communication.** The ways that processes on one computer send data to each other.
- **Networking sockets.** The socket API and the programs that use it directly: clients, servers and name resolution.
- **Network protocols for programmers.** The protocols under sockets that programmers need to know: IP, TCP, UDP, DNS, TLS and email.
- **Terminals and consoles.** How a program reads keys from a terminal and writes text, colour and cursor movement to it.
- **Low-level C and Rust.** Practical systems work in C and Rust that crosses the two languages: calls between them, data layout on the wire, and code with no standard library. The language features are in the C and Rust trees.
- **Assembly language.** How to read and write assembly for the common architectures, and when a programmer needs it.
- **Linkers and object files.** The tasks a programmer does with object files, libraries and linkers. The internals and the formats are in language-implementation.
- **The kernel from a programmer's view.** The parts of the kernel that application and tools programmers meet: modules, drivers, virtual file systems, eBPF and the isolation features under containers.

## What it does not hold

- Operating system design as a subject. This site links out for it.
- Threads and locks as concurrency ideas. They are in [Concurrency and parallelism](/concurrency-and-parallelism/).
- Bare-metal code with no OS. It is in [Embedded and hardware](/embedded-and-hardware/).
- HTTP and web servers. They are in [Web development](/web-development/).
