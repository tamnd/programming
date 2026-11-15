---
tag: 6NV5Q0
kind: concept
slug: incremental-builds
name: Incremental builds
aliases: [incremental build, incremental compilation, rebuild, up to date, out of date, minimal build, early cutoff, build cache, dirty bit, verifying traces]
area: tools-and-workflow/build-systems
level: core
summary: >-
  An incremental build runs again only the steps whose inputs changed since the last build. The tool must find what changed, by file times or by content hashes, and must know every input of every step.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1975
  people: [Stuart Feldman]
  note: Make, in use on Unix systems from 1975, compared the modification times of files to decide what to build again. Later tools record hashes of the inputs of each step.
see_also: [build-graph, build-system-concept, build-a-project]
sources:
  - paper: doi:10.1145/3236774
    title: Andrey Mokhov, Neil Mitchell and Simon Peyton Jones, Build Systems à la Carte, Proceedings of the ACM on Programming Languages 2 (ICFP), Article 79, 2018
    locator: 2.1 Make (Definition 2.1 and footnote 5); 2.3 Shake (early cutoff, Fig. 3); 2.4 Bazel; 2.5 Summary (Table 1); 4.2 The Rebuilder (dirty bit, verifying traces, constructive traces); Table 2; 7.1 Other Build Systems (Ninja)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/make.html
    title: The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), make
    locator: DESCRIPTION
  - url: https://www.gnu.org/software/make/manual/html_node/How-Make-Works.html
    title: GNU make manual (version 4.4.1), 2.3 How make Processes a Makefile
    locator: when an object file is recompiled
  - url: https://ninja-build.org/manual.html
    title: The Ninja build system manual (version 1.13.1)
    locator: Philosophical overview; Comparison to Make; Header dependencies (deps); Rule variables (restat); The Ninja log
  - url: https://github.com/golang/go/blob/go1.26.5/src/cmd/go/alldocs.go
    title: Go 1.26.5 source, cmd/go documentation (alldocs.go)
    locator: Build and test caching
  - url: https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
    title: GNU make manual (version 4.4.1), 4.14 Generating Prerequisites Automatically
    locator: first paragraphs; the -M option
  - url: https://doc.rust-lang.org/cargo/reference/build-cache.html
    title: The Cargo Book, Build Cache
    locator: the incremental directory; Dep-info files
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
    title: Microsoft Learn, dotnet build
    locator: Options (--no-incremental)
  - url: https://bazel.build/basics/artifact-based-builds
    title: Bazel documentation, Artifact-Based Build Systems
    locator: a functional perspective (a second bazel build)
  - url: https://www.gnu.org/software/make/manual/html_node/Avoiding-Compilation.html
    title: GNU make manual (version 4.4.1), 9.4 Avoiding Recompilation of Some Files
    locator: the -t flag
  - paper: doi:10.1002/spe.4380090402
    title: Stuart I. Feldman, Make, a program for maintaining computer programs, Software, Practice and Experience 9(4), pages 255 to 265, 1979
    locator: abstract
---

## Idea

A full build runs every step. After a small edit, most steps would make the same outputs again. An **incremental build** runs again only the steps whose inputs changed, and keeps the other outputs from the last build.

The paper *Build Systems à la Carte* calls this property minimality: "A build system is minimal if it executes tasks at most once per build and only if they transitively depend on inputs that changed since the previous build."[^1] Minimality saves time, but it is safe only if the result is the same as the result of a clean build. To get both, a build tool must answer two questions for each step:

- **What changed?** The tool compares the inputs of the step now with the inputs at the last build. It can compare file modification times, or hashes of the content.
- **What does the change touch?** The tool follows the edges of the [[build-graph]] from each changed input to the steps that need it. A step that is not on such a path keeps its old output.

## How it works

**Modification times.** POSIX make uses the modification times of files to decide if a target is out of date.[^2] GNU Make recompiles an object file if the source file, or any of the header files named as prerequisites, has a later modification time than the object file, or if the object file does not exist.[^3] The tool stores nothing of its own. The paper describes the times as a dirty bit that the file system keeps for Make: a write to a file makes the targets that depend on it out of date, and a build of a target makes it up to date.[^1]

The run below builds a small C program with GNU Make, then makes three changes. `touch util.h` changes only the time of the header, but both object files depend on it, so make compiles both. Next, the script writes `util.c` again with a different return value, and gives the file a time in 2001, as a copy from a backup can do. The file is older than `util.o`, so make says that `app` is up to date, and the program still prints the old value 42. Only a later `touch util.c` makes make compile it.

```bash run id=make-mtime
d=$(mktemp -d)
cd "$d"
cat >util.h <<'END'
int util(void);
END
cat >util.c <<'END'
#include "util.h"
int util(void) { return 42; }
END
cat >main.c <<'END'
#include <stdio.h>
#include "util.h"
int main(void) { printf("%d\n", util()); return 0; }
END
cat >Makefile <<'END'
app: main.o util.o
	cc -o app main.o util.o
main.o: main.c util.h
	cc -c main.c
util.o: util.c util.h
	cc -c util.c
END
make >/dev/null
echo '$ touch util.h; make'
touch util.h
make
echo '$ (write util.c again, with a time in 2001); make'
cat >util.c <<'END'
#include "util.h"
int util(void) { return 7; }
END
touch -d '2001-01-01' util.c
make
./app
echo '$ touch util.c; make'
touch util.c
make
./app
cd /
rm -rf "${d:?}"
```

```output for=make-mtime
$ touch util.h; make
cc -c main.c
cc -c util.c
cc -o app main.o util.o
$ (write util.c again, with a time in 2001); make
make: 'app' is up to date.
42
$ touch util.c; make
cc -c util.c
cc -o app main.o util.o
7
```

The paper says that Make requires that file timestamps "only go forward in time, which can be violated by backup software", and that you can fool Make with the command `touch`, so Make is minimal only if you do not do that.[^1]

**Hashes of the inputs.** Another rebuilder keeps a record, for each target, of the hashes of the inputs at its last build. On the next build, it computes the hashes again. If one of them is different, the target is out of date. The paper calls this record a verifying trace.[^1] A time that goes back, or a `touch` without an edit, does not fool it, because the content decides.

A trace can also hold things that are not files. In Ninja, outputs implicitly depend on the command line that made them, so a change to the compiler flags rebuilds them.[^4] Ninja keeps the command lines in a log file, `.ninja_log`.[^4] The Go build cache accounts for changes to Go source files, compilers and compiler options.[^5]

**Early cutoff.** When a step runs again and makes the same output as before, the steps after it do not need to run. The paper calls this early cutoff, and gives the example of a comment added to `main.c`: the compiler makes the same `main.o`, so `main.exe` does not need to be built again.[^1] A rebuilder that compares hashes gets early cutoff for free, because the hash of `main.o` did not change. In the comparison of the paper, Make and Excel do not support early cutoff, and Shake and Bazel do.[^1] A Ninja rule can set the variable `restat`: Ninja then checks the outputs after the command, and treats each output whose modification time did not change as though it never needed a build.[^4]

**Shared results.** A verifying trace keeps only hashes. A constructive trace also keeps the output, so a tool can copy a result from a cache in place of running the step, and it can share the cache with other users.[^1] In the paper, Bazel uses constructive traces, and Shake and Ninja use verifying traces.[^1] This is the base of a shared build cache.

**Finding the inputs.** Each rebuilder is only as good as its list of inputs. A C file reads its headers through `#include` lines, and a makefile must name each header as a prerequisite. The GNU Make manual says that you must be careful to update the makefile every time you add or remove an `#include`, and shows the compiler option `-M`, which writes these rules for you.[^6] Ninja can read the same rules from the compiler after each compile, with `deps = gcc`.[^4] Cargo writes a dep-info file, with a `.d` suffix, next to each artifact, in a syntax like a makefile, so that other build systems can know when to run Cargo again.[^7]

## In languages

The examples run two rebuilders on the same graph: `main.o` from `main.c` and `util.h`, `util.o` from `util.c` and `util.h`, and `app` from the two object files. A file is a string, and the "compiler" drops comments. The first rebuilder compares modification times from a counter, as Make does. The second keeps the SHA-256 hashes of the inputs of each target from its last build. Each step of the table makes the same change to both, and prints the targets that each one builds again:

- After `touch util.h`, the time rebuilder builds everything again, and the hash rebuilder builds nothing.
- After a comment in `main.c`, the hash rebuilder builds only `main.o`. The output is the same, so `app` keeps its old trace: this is early cutoff.
- After a copy of `util.c` with a different body and an old time, the time rebuilder builds nothing, and its `app` no longer matches a clean build.

## Variations and related ideas

- **Four rebuilders.** The paper separates the rebuilder from the scheduler, and names four kinds of rebuilder: a dirty bit, verifying traces, constructive traces and deep constructive traces.[^1] Its Table 2 puts Make with the dirty bit, Ninja and Shake with verifying traces, and Bazel with constructive traces.[^1] Ninja combines the topological scheduler of Make with the verifying traces of Shake.[^1]
- **Build caches in language tools.** The `go` command caches build outputs for reuse in future builds.[^5] Cargo keeps the incremental output of rustc in an `incremental` directory, a cache used to speed up later builds.[^7] `dotnet build` has the option `--no-incremental`, which turns off incremental compilation and forces a clean rebuild of the dependency graph of the project.[^8]
- **Builds that do nothing.** When nothing changed, the build must only check. Bazel exits in less than a second, with a message that the target is up to date, when you run `bazel build` a second time without changes.[^9] The design goals of Ninja start with fast, instant incremental builds, even for large projects.[^4]
- **Incremental compilation in the compiler.** A compiler can also keep its own state between runs, and recompile less than one file. This is a separate layer from the build tool, which decides which commands to run.

## Pitfalls

- **A missing input.** If a header is not a prerequisite, an edit to it does not rebuild the files that include it, and the program mixes old and changed code. Let the compiler write the header dependencies.[^6]
- **Times that go back.** A copy from a backup, an unpacked archive or a checkout can give a source file an old time. A rebuilder that uses times then keeps an old output, as in the run above. After such a copy, do a clean build.
- **Flags that are not inputs.** In make, only files have times, so a change to a variable such as `CFLAGS` does not by itself make a target out of date.[^2] A tool that records the command line, such as Ninja, does rebuild.[^4]
- **Inputs outside the tool.** The Go build cache does not detect changes to C libraries imported with cgo; you must clean the cache or use the `-a` flag.[^5] Any tool can miss an input that it does not see, such as a system library or an environment variable.
- **Marking files up to date by hand.** The `-t` flag of GNU Make marks targets up to date by changing their modification times, without running the recipes.[^10] It can save a long build after a change to a comment in a header, but a wrong use leaves old outputs that look current.

## Cost

A check by time reads one time per file, from the file system. A check by hash must read and hash every input, so a build that does nothing costs more. Tools reduce this cost by keeping the times and hashes from the last build, and computing a hash again only when a time changed. The traces also need space: a verifying trace holds hashes, and a constructive trace holds the outputs too.[^1]

The saving depends on the shape of the graph. An edit to a header that many files include rebuilds them all. An edit to one source file rebuilds one object file and the link. Early cutoff can stop a chain of steps after the first one, when that step makes the same output again.[^1]

## History

Make was in use on Unix systems from 1975, and Stuart Feldman described it in 1979.[^11] It decides by modification times, and a makefile lists the dependencies.[^2] Later tools record more about each step. Ninja records the command line of each output,[^4] and Bazel records hashes of the files, and keeps a cache of outputs that a team can share.[^1] In 2018, the paper of Mokhov, Mitchell and Peyton Jones named the choice of rebuilder as one of the two main choices in the design of a build system, next to the scheduler.[^1]

## References
