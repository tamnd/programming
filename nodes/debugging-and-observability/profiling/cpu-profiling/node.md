---
tag: NEW-cpu-profiling
kind: concept
slug: cpu-profiling
name: CPU profiling
aliases: [profiling, profiler, execution profile, sampling profiler, instrumenting profiler, deterministic profiling, flame graph]
area: debugging-and-observability/profiling
level: core
summary: >-
  A CPU profiler measures where a running program spends its processor time. It samples the call stack at intervals or counts each call, and reports the result per function or per path.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1982
  people: [Susan L. Graham, Peter B. Kessler, Marshall K. McKusick]
  note: The gprof paper combined two methods that profilers still use. A routine in the prologue of each function counts the arcs of the call graph, and a histogram of program counter samples gives the time.
see_also: [recursion, higher-order-functions, closures, resource-acquisition-is-initialization, interactive-debugging, structured-logging, delta-debugging]
same_as:
  wikidata: Q1138496
  wikipedia: Profiling_(computer_programming)
sources:
  - url: https://docs.python.org/3.14/library/profile.html
    title: Python documentation, The Python Profilers
    locator: Introduction to the profilers (and its Note); Instant User's Manual (ncalls); Reference Manual (run, runctx, Profile.runcall); What Is Deterministic Profiling?; Limitations
  - url: https://docs.python.org/3.14/library/sys.html
    title: Python documentation, sys, System-specific parameters and functions
    locator: sys.setprofile
  - paper: doi:10.1145/800230.806987
    title: Susan L. Graham, Peter B. Kessler and Marshall K. McKusick, gprof, a Call Graph Execution Profiler, Proceedings of the SIGPLAN '82 Symposium on Compiler Construction, 1982, pages 120 to 126 (read from the copy at https://docs-archive.freebsd.org/44doc/psd/18.gprof/paper.pdf)
    locator: 2 Types of Profiling; 3 Gathering Profile Data; 3.1 Execution Counts; 3.2 Execution Times; 7 Conclusions
  - url: https://sourceware.org/binutils/docs/gprof/Compiling.html
    title: GNU gprof manual, Compiling a Program for Profiling
    locator: section 2
  - url: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
    title: GCC manual, Program Instrumentation Options
    locator: -finstrument-functions
  - url: https://www.brendangregg.com/flamegraphs.html
    title: Brendan Gregg, Flame Graphs
    locator: Summary; Variations (flame charts); Origin; Updates
  - url: https://www.brendangregg.com/FlameGraphs/cpuflamegraphs.html
    title: Brendan Gregg, CPU Flame Graphs
    locator: 1.1 Too Much Data; 3 Description; 4 Instructions; 4.1 Linux perf; 4.3 DTrace; 6 C; 8.1 Background
  - url: https://man7.org/linux/man-pages/man1/perf-record.1.html
    title: Linux manual page, perf-record(1)
    locator: NAME; OPTIONS (-F, -g, --call-graph)
  - url: https://pkg.go.dev/runtime/pprof
    title: Go standard library, package runtime/pprof
    locator: Profiling a Go program; StartCPUProfile; StopCPUProfile; Profile.WriteTo
  - url: https://go.dev/src/runtime/pprof/pprof.go
    title: Go source, runtime/pprof/pprof.go
    locator: StartCPUProfile (the comment on the sampling rate and the constant hz)
  - url: https://openjdk.org/jeps/328
    title: JEP 328, Flight Recorder
    locator: the summary; Success Metrics; Description (the command line and the configuration sets)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/jdk.jfr/jdk/jfr/Configuration.html
    title: Java SE 25 API, jdk.jfr.Configuration
    locator: the class description; getConfiguration; getDescription
  - url: https://nodejs.org/api/cli.html
    title: Node.js documentation, Command-line API
    locator: --cpu-prof; --cpu-prof-interval
  - url: https://nodejs.org/api/inspector.html
    title: Node.js documentation, Inspector
    locator: CPU profiler
  - url: https://learn.microsoft.com/en-us/dotnet/core/diagnostics/dotnet-trace
    title: .NET documentation, dotnet-trace performance analysis utility
    locator: the introduction
---

## Idea

A program is slow, and the developer must decide what to change. A guess is often wrong, and a change in the wrong place costs time and adds risk. A profiler measures the program while it runs and reports where the processor time went: per function, per line or per path of calls.

A CPU profile answers two kinds of questions. The first is about time: which functions used most of the processor? The second is about counts: how many times did each function run? The gprof paper names both kinds. Counts show if an algorithm does what the developer expected, and a count that is too high can show an error in the algorithm. Counts are not proportional to time, because one call can take much longer than another.[^3]

There are two methods to collect the data:

- An **instrumenting** profiler adds code at each call and each return. It sees every call, so it knows the exact counts. Python's documentation calls this deterministic profiling.[^1]
- A **sampling** profiler stops the program at intervals and records where it is. It does not see every call. It gives a statistical picture of where the time goes, at a lower cost.[^1] [^3]

The examples on this page print only counts and fixed facts. A time changes on each run, so a page that printed times would not have a fixed output.

## How it works

### Instrumentation

The instrumenting profiler needs a hook at the entry and the exit of each function. There are three ways to get it:

- **The compiler inserts it.** The gprof compilers inserted a call to a monitoring routine in the prologue of each routine. The monitoring routine found its caller from the return address and counted each arc of the call graph, from a call site to a routine.[^3] GCC and gprof still use the option `-pg` for this.[^4] With `-finstrument-functions`, GCC calls `__cyg_profile_func_enter` and `__cyg_profile_func_exit` with the address of the current function and its call site.[^5]
- **The interpreter provides it.** Python calls a profile function at each call and each return, so `cProfile` needs no change to the code.[^1] `sys.setprofile` installs such a function from Python code.[^2]
- **The programmer writes it.** A wrapper function, a decorator or a scope guard counts each call. The examples in JavaScript, TypeScript, C++, Rust and C# use this method.

A profiler must not write a line of output at each call. The gprof paper says the volume of data would be too large, and the time to write it would be larger than the time of the program. The monitoring routine keeps a table of counts in memory and writes it when the program ends.[^3]

The C example uses the GCC hooks. Each hook receives the address of a function, and a table maps addresses to counts:

```text
fib(15) = 610
sum_squares(10) = 385
calls: main 1, fib 1973, sum_squares 1, square 10
```

### Sampling

A sampling profiler sets a timer. At each tick it records the program counter, or the whole call stack, of the running thread. The gprof paper describes the method: the profile infers the time of each routine from the distribution of the samples. The method is "inherently a statistical approximation". The intervals must be uniform, and the program must run for enough intervals for the distribution to be accurate. The gprof system took a sample of the program counter at each clock tick, 1/60 of a second.[^3]

A flame graph needs a profiler that samples whole stack traces, not only the program counter.[^7] These profilers do:

| Profiler | Default rate |
|---|---|
| Go `runtime/pprof` | 100 samples a second, through the signal `SIGPROF` [^10] |
| Node.js `--cpu-prof` (V8) | one sample each 1000 microseconds [^13] |
| JDK Flight Recorder | one sample each 20 ms with the `default` settings, each 10 ms with `profile` (see the Java example) |
| Linux `perf record` | the rate that `-F` gives; Gregg's examples use `-F 99` [^7] [^8] |

The Go source explains its choice. Operating systems cannot send the signal at more than about 500 Hz, and the processing of each signal is not cheap, mostly because it gets the stack trace. 100 Hz gives useful data without a large load on the system, and it is a round number that converts sample counts to seconds.[^10] Gregg uses 99 Hz and 199 Hz on purpose. An odd rate does not sample in lockstep with other periodic activity, which would give misleading results.[^7]

A sample does not know how many times a function was called. It only knows that the function was on the stack at that moment.[^7] The examples in Go, Java and JavaScript therefore count calls with their own counter and use the profiler only to show its API.

### Flame graphs

A profile of a large program has many thousands of distinct stacks. A text report of them can fill many screens.[^7] Brendan Gregg made the flame graph to show all of them at once. It is made in three steps: capture stacks, fold them, and draw the folded stacks with `flamegraph.pl`.[^7] A folded stack is one line: the names of the frames from the bottom up, joined with `;`, and a count. The Python example produces this format with `sys.setprofile`:

```text
main 1
main;check 1
main;load 1
main;load;check 3
main;load;parse 3
```

In the drawing, each box is a frame. The y-axis is the depth of the stack. The top box is the function that was on the CPU, and the boxes below it are its callers. The x-axis is the population of samples, sorted in alphabetical order. It is not time. A wider box was present in more samples.[^6] [^7] The sort merges the frames of equal paths into one wide box, so the main paths stand out.[^7]

A flame chart looks similar but puts time on the x-axis. The Chrome web inspector added flame charts, inspired by flame graphs.[^6]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `cProfile` and `profile` count every call; `pstats` sorts the results; `sys.setprofile` for a custom profiler |
| Go | stdlib | `runtime/pprof` samples at 100 Hz; `go test -cpuprofile`; `net/http/pprof` for a server |
| Java | stdlib | JDK Flight Recorder: the `jdk.jfr` API and `-XX:StartFlightRecording` |
| JavaScript | stdlib | Node.js `--cpu-prof` and the `Profiler` domain of `node:inspector` |
| TypeScript | stdlib | the profiler of the JavaScript engine |
| C | library | `perf`, `gprof` with `-pg`, and the compiler hooks of `-finstrument-functions` |
| C++ | library | as C; a scope guard for manual counts |
| Rust | library | `perf` and other tools outside the language; a `Drop` guard for manual counts |
| C# | library | `dotnet-trace` outside the program; `[CallerMemberName]` for manual counts |
| Kotlin | stdlib | JDK Flight Recorder on the JVM, as in Java |
| Bash, PowerShell | idiom | the shell can time a command but has no profiler of the call stacks |
| SQL, Regex, HTML, CSS | n/a | no call stack to profile |

### Python

`cProfile` and `profile` are deterministic profilers: they record all call, return and exception events. `cProfile` is a C extension with reasonable overhead. `profile` is written in Python and adds significant overhead.[^1] The `ncalls` column shows two numbers for a recursive function. In the example, `1973/1` means 1973 calls in all, of which one was a primitive call, a call that was not caused by recursion.[^1] The example reads the counts from `pstats.Stats` and prints no times.

The generator expression in `total` shows 400 calls for 100 calls of `total`. A profile function receives the event `call` when a function is called "or some other code block entered".[^2] Each resume of the generator enters its code again.

### Go

`pprof.StartCPUProfile(w)` starts the sampler. The profile is buffered and written to `w`, and `StopCPUProfile` returns only after all the writes are complete.[^9] [^10] The result is a gzip-compressed protocol buffer for the `pprof` tool.[^9] The documentation shows three ways to collect a profile: `go test -cpuprofile cpu.prof`, a flag in `main`, and the HTTP handlers of `net/http/pprof` for a running server. `go tool pprof cpu.prof` reads the result.[^9]

### Java

JDK Flight Recorder is a framework that collects events from the application, the JVM and the operating system. `java -XX:StartFlightRecording` starts it from the command line.[^11] The `jdk.jfr` API starts and stops a recording from the program. A `Configuration` holds the settings of a recording, and the JDK ships two: `default` and `profile`.[^12] The example reads the sampling period of the event `jdk.ExecutionSample` from each one and prints whether the recording has events. The number of samples changes from run to run.

### JavaScript and TypeScript

Node.js has the V8 CPU profiler. `node --cpu-prof` starts it when the program starts and writes the profile to a file before the program exits.[^13] The `node:inspector` module sends the commands `Profiler.enable`, `Profiler.start` and `Profiler.stop` to the same profiler from inside the program.[^14] The JavaScript example prints the field names of the profile, because the samples change on each run. TypeScript runs on the same engine. Its example writes a generic wrapper that keeps the type of the function that it wraps.

### C, C++, Rust and C#

The C and C++ standards have no profiler. A tool outside the language does the work. `perf record` runs a command and records its profile in the file `perf.data`. The option `-F` sets the frequency and `-g` records the call stacks.[^8] gprof needs the program compiled and linked with `-pg`.[^4] The GCC option `-finstrument-functions` calls the two hooks at each entry and exit, and the attribute `no_instrument_function` excludes a function, such as the hooks themselves.[^5]

Rust has no profiler in its standard library, and `perf` samples a Rust program as it samples a C program. .NET has `dotnet-trace`, which collects traces of a running process without a native profiler.[^15] The C++, Rust and C# examples count calls by hand. C++ and Rust use a guard object whose destructor pops the frame, which is [[resource-acquisition-is-initialization|RAII]]. C# uses `[CallerMemberName]`, so the compiler passes the name of each function to the counter.

## Variations and related ideas

### Wall time and CPU time

A CPU profile shows the time that a thread spent on the processor. A thread that waits for a disk, a lock or the network is not on the CPU, so a CPU profile does not show the wait. Gregg's page lists off-CPU flame graphs as a separate kind for this time.[^6] The JDK describes its event `jdk.ExecutionSample`, in the output of `jfr metadata`, as a snapshot of a thread that executes Java code. Threads that wait or execute native code are not in it.

### Profiling and debugging

A profile can also find bugs. Python's documentation says that call counts can identify bugs in code, through "surprising counts".[^1] A function that runs 1973 times for `fib(15)` shows exponential [[recursion]] without memoization. [[interactive-debugging]] shows the state of one moment. A profile summarizes the whole run.

### Tracing

A tracer records each event with its time, in order. A profiler adds the events up. Gregg first traced each CPU function call of MySQL and drew the calls with time on the x-axis. The overhead of tracing was too high, so he switched to timed sampling.[^6]

## Pitfalls

### The profiler changes the program

An instrumenting profiler runs extra code at each call. Python's documentation warns that the profilers add overhead to Python code but not to C functions, so C code looks faster than any Python code. The profilers are for an execution profile, not for benchmarks; `timeit` is for benchmarks.[^1]

### Counts are not time

gprof assumed that all calls to a routine take the same time. The paper warns that this assumption can hide a call site whose calls are always faster or slower than the average.[^3] A function with a high count may be cheap, and a function with a count of 1 may hold most of the time.

### Short runs and odd rates

A sampling profiler needs enough samples. At 100 Hz, a run of half a second gives about 50 samples, and a function that ran for 5 ms may have none.[^3] [^10] A rate that matches a periodic task in the program samples the same phase of that task each time. Gregg uses 99 Hz for this reason.[^7]

### Broken stacks

A sampler walks the stack with the frame pointer. Many compilers, GCC among them, omit the frame pointer as an optimization. The flame graph then misses towers. Gregg gives two fixes: compile with `-fno-omit-frame-pointer`, or install debug information with DWARF data and use the DWARF stack walker of `perf`.[^7] A JIT compiler causes a second problem. The JVM compiles methods at run time and has no symbol table for a system profiler, so `perf` shows hexadecimal addresses for Java methods.[^7]

### The profile is not complete

`StartCPUProfile` buffers the profile. Until `StopCPUProfile` returns, the writer may have nothing in it.[^9] A program that exits without the call leaves an empty file. The Go `wrong` example prints the size of the buffer before `StopCPUProfile`: 0 bytes.

### The code runs in another namespace

`cProfile.run(command)` executes the string in the namespace of the module `__main__`. A local variable of the caller is not in that namespace, and the Python example fails with `NameError`. `runctx` takes the global and local namespaces as arguments, and `Profile.runcall` takes a function and its arguments.[^1]

## Cost

The cost of an instrumenting profiler grows with the number of calls. The cost of a sampling profiler grows with the rate, and samples that are too frequent overwhelm the program.[^3] Python's documentation says that statistical profiling traditionally has less overhead, because the code needs no instrumentation.[^1]

Some published numbers:

- gprof added five to thirty percent to the execution time of the program.[^3]
- Python says deterministic profiling adds only a small overhead in typical applications, because the interpreter already adds so much.[^1]
- JEP 328 set a success metric of at most 1% overhead for Flight Recorder with the default settings on the SPECjbb2015 benchmark, and no measurable overhead when it is not enabled.[^11]

A profiler also needs memory. A counter per function or per arc is small.[^3] A sampler stores one stack per sample, so a long run at a high rate makes a large file. The folded format adds equal stacks together, so its size depends on the number of distinct paths.[^7]

## History

For calls to monitoring routines, the gprof paper cites the `prof` command of the Unix Programmer's Manual of January 1979.[^3] Graham, Kessler and McKusick at Berkeley wrote gprof while they worked on a code generator. They presented it at the SIGPLAN '82 Symposium on Compiler Construction. gprof added the call graph: it charged the time of each routine to the routines that called it.[^3]

Brendan Gregg invented flame graphs when he worked on a performance problem in MySQL. Function tracing slowed the target too much, so he sampled instead. Sampling has gaps, so the order in time was lost. He sorted the samples to merge equal frames. The warm colors suggested hot CPUs, and the picture looked like flames. He released flame graphs in December 2011.[^6]

## References
