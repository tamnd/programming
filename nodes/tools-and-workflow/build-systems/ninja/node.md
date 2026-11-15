---
tag: BEBZXM
kind: tool
slug: ninja
name: Ninja
aliases: [ninja, Ninja build, build.ninja, ninja-build]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  Ninja is a small build tool that runs a build graph from a build.ninja file. Other programs, such as CMake and Meson, generate the file, and Ninja does the incremental build as fast as it can.
role: [build]
licence: apache-2.0
repo: https://github.com/ninja-build/ninja
homepage: https://ninja-build.org/
history:
  year: 2011
  people: [Evan Martin, Peter Collingbourne]
  note: Evan Martin wrote Ninja for the Chromium browser and released it in 2011. Peter Collingbourne connected it to CMake. Evan Martin gave the project to another maintainer in 2014.
see_also: [make, cmake, build-graph, incremental-builds, build-system-concept, build-systems-a-la-carte]
sources:
  - url: https://github.com/ninja-build/ninja/blob/v1.13.1/README.md
    title: Ninja README, tag v1.13.1
    locator: the introduction; Building Ninja itself
  - url: https://ninja-build.org/manual.html
    title: The Ninja build system manual, v1.13.1, July 2025
    locator: Introduction; Philosophical overview; Design goals; Comparison to Make; Running Ninja; Extra tools; Conceptual overview; Rules; Build statements; Generating Ninja files from code; The phony rule; Default target statements; The Ninja log; Version compatibility; C/C++ header dependencies; Pools; Rule variables
  - url: https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/Preprocessor-Options.html
    title: GCC 13.3 manual, Options Controlling the Preprocessor
    locator: -MD; -MMD
  - url: https://neugierig.org/software/blog/2020/05/ninja.html
    title: Evan Martin, The Success and Failure of Ninja, 12 May 2020
    locator: the introduction; The tech; Defaults dominate; CMake
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake-generators.7.rst
    title: cmake-generators(7), CMake 3.28.3
    locator: Ninja Generators
  - url: https://github.com/ninja-build/ninja/blob/v1.13.1/COPYING
    title: Ninja licence file (Apache License 2.0), tag v1.13.1
    locator: the first lines
---

## What it is

Ninja is a [[build-system-concept|build tool]]. Its README says: "Ninja is a small build system with a focus on speed."[^1] Ninja reads a file called `build.ninja` that lists each output file, its inputs and the command that makes it. Like [[make]], it compares the modification times of the files and runs the commands that make the out-of-date outputs.[^2]

The difference from Make is in who writes the file. The Ninja manual says: "Where other build systems are high-level languages, Ninja aims to be an assembler."[^2] The language of `build.ninja` has no conditions and no built-in rules, and the manual names "convenient syntax for writing build files by hand" as a non-goal.[^2] Another program, called a generator, writes the file and makes the decisions, such as the compiler flags or a debug or release build.[^2] [[cmake]] and Meson are the most common generators. Ninja then does only the incremental build, with as little work as possible.

## Install and first use

Install Ninja with the package manager of the system, or download a binary from the releases of the project. The README says that "the only required file is the resulting ninja binary."[^1] The examples on this page ran with Ninja 1.11.1, GCC 13 and CMake 3.28.3 on Ubuntu 24.04.

Run `ninja` in a directory that contains `build.ninja`. With no targets on the command line, Ninja builds "all out-of-date targets", and the names of targets on the command line select what to build.[^2] Usually a generator writes the file. The run below writes a small file by hand, to show the parts. It has two rules, `cc` and `link`, and three build statements.

```bash run id=ninja-by-hand
d=$(mktemp -d)
cd "$d"
cat >calc.h <<'END'
int add(int a, int b);
END
cat >calc.c <<'END'
#include "calc.h"
int add(int a, int b) { return a + b; }
END
cat >main.c <<'END'
#include <stdio.h>
#include "calc.h"
int main(void) { printf("2 + 3 = %d\n", add(2, 3)); return 0; }
END
cat >build.ninja <<'END'
cflags = -O2 -Wall

rule cc
  command = cc -MD -MF $out.d $cflags -c $in -o $out
  depfile = $out.d
  deps = gcc
  description = CC $out

rule link
  command = cc $in -o $out
  description = LINK $out

build main.o: cc main.c
build calc.o: cc calc.c
build app: link main.o calc.o

default app
END
echo "--- ninja"
ninja -j 1 2>&1
./app
echo "--- ninja again"
ninja 2>&1
echo "--- after touch calc.h"
touch calc.h
ninja -j 1 2>&1
echo "--- ninja -t deps calc.o"
ninja -t deps calc.o 2>&1 | awk 'NR == 1 { sub(/:.*/, ":") } { print }'
echo "--- ninja -t commands app"
ninja -t commands app 2>&1
cd /
rm -rf "${d:?}"
```

```output for=ninja-by-hand
--- ninja
[1/3] CC main.o
[2/3] CC calc.o
[3/3] LINK app
2 + 3 = 5
--- ninja again
ninja: no work to do.
--- after touch calc.h
[1/3] CC main.o
[2/3] CC calc.o
[3/3] LINK app
--- ninja -t deps calc.o
calc.o:
    calc.c
    /usr/include/stdc-predef.h
    calc.h

--- ninja -t commands app
cc -MD -MF main.o.d -O2 -Wall -c main.c -o main.o
cc -MD -MF calc.o.d -O2 -Wall -c calc.c -o calc.o
cc main.o calc.o -o app
```

The first `ninja` runs the three commands and prints the `description` of each rule in place of the full command line. The second `ninja` has nothing to do. After `touch calc.h`, Ninja compiles `main.c` and `calc.c` again and links `app`, because both source files include `calc.h`. The file does not list `calc.h` anywhere: the compiler wrote the header dependencies during the first build, and `ninja -t deps` shows them. The list also has the system header `/usr/include/stdc-predef.h`, because `-MD` lists the system headers too. The option `-MMD` is "Like -MD except mention only user header files, not system header files".[^3] `ninja -t commands app` prints the commands that build `app`, with the variables expanded. The run uses `-j 1` only to keep the order of the output the same in each run.

## Concepts of the tool

**Rules and build statements.** A rule gives a short name to a command, and a build statement uses a rule to make outputs from inputs. The manual says that "build statements describe the dependency graph of your project, while rule statements describe how to generate the files along a given edge of the graph."[^2] A build statement has the form `build outputs: rulename inputs`.[^2] In the command, `$in` is the list of inputs and `$out` is the list of outputs.[^2] A build statement can have more than one output, which Make does not handle well.[^2][^4]

**Variables.** A line such as `cflags = -O2 -Wall` declares a variable, and `$cflags` uses it.[^2] A variable that is set below a build statement applies only to that statement, so one rule can compile one file with other flags.[^2]

**The command line is an input.** "Outputs implicitly depend on the command line that was used to generate them", so a change to the compile flags rebuilds the outputs.[^2] Ninja records the command of each output in the file `.ninja_log`, and it rebuilds an output when the command in the build file is different from the command in the log.[^2] Make has no such check: a change to `CFLAGS` does not make an object file out of date.

**Header dependencies.** Only the compiler knows the full list of headers that a source file includes.[^2] With `depfile = $out.d`, Ninja reads the dependency file that `gcc -MD -MF $out.d` writes.[^2] With `deps = gcc`, Ninja reads the file directly after the compile, keeps the dependencies in its own database `.ninja_deps`, and deletes the `.d` file.[^2] The manual says that this is faster for large projects, because Ninja does not load many dependency files when it starts.[^2] A header from this list that is later deleted does not stop the build with an error.[^2]

**Parallel by default.** "Builds are always run in parallel, based by default on the number of CPUs your system has."[^2] Evan Martin, the author of Ninja, writes that this default finds missing dependencies early, because a build file that does not list an input fails or gives wrong results sooner, while Make runs one command at a time unless the user gives `-j`.[^4] Pools limit the number of jobs for some rules, for example the link steps of large programs.[^2]

**Phony and default targets.** `build all: phony app tests` makes an alias. When no target is given, Ninja builds "every output that is not named as an input elsewhere", unless a `default` statement selects other targets.[^2]

## Common tasks

| Task | Command |
|---|---|
| Build the default targets | `ninja` |
| Build in another directory | `ninja -C build`[^2] |
| Run 20 jobs at the same time | `ninja -j 20`[^2] |
| Print the commands, but do not run them | `ninja -n`[^2] |
| Print the full commands while it builds | `ninja -v`[^2] |
| Remove the built files | `ninja -t clean`[^2] |
| Show the stored header dependencies | `ninja -t deps`[^2] |
| List the targets | `ninja -t targets all`[^2] |
| Write a compilation database for Clang tools | `ninja -t compdb`[^2] |
| Draw the graph with Graphviz | `ninja -t graph \| dot -Tpng -ograph.png`[^2] |

The manual says that many of the options of Ninja "intentionally match those of Make".[^2] The `-t` option runs the tools that are built into Ninja.[^2] The tool `compdb` writes a compilation database in the JSON format that the Clang tools read.[^2]

## Configuration

Ninja has no configuration file other than `build.ninja`. Projects normally do not edit this file, because the generator writes it again. With CMake, the option `-G Ninja` selects one of the two Ninja generators, and the other is `Ninja Multi-Config`.[^5] The run below lets CMake generate `build.ninja` for a project with one source file, then counts the build statements in the file, lists two of the targets, and builds the project. Each line from `ninja -t targets all` shows a target and the rule that makes it. The rules have names that CMake made, such as `C_EXECUTABLE_LINKER__hello_`.

```bash run id=ninja-from-cmake
d=$(mktemp -d)
cd "$d"
cat >hello.c <<'END'
#include <stdio.h>
int main(void) { puts("hello"); return 0; }
END
printf 'cmake_minimum_required(VERSION 3.20)\nproject(hello LANGUAGES C)\nadd_executable(hello hello.c)\n' >CMakeLists.txt
cmake -S . -B build -G Ninja >/dev/null 2>&1
echo "configure exit status: $?"
grep -c '^build ' build/build.ninja
ninja -C build -t targets all 2>&1 | grep -E '^(hello|CMakeFiles/hello.dir/hello.c.o):'
ninja -C build 2>&1
cd /
rm -rf "${d:?}"
```

```output for=ninja-from-cmake
configure exit status: 0
12
CMakeFiles/hello.dir/hello.c.o: C_COMPILER__hello_unscanned_
hello: C_EXECUTABLE_LINKER__hello_
ninja: Entering directory `build'
[1/2] Building C object CMakeFiles/hello.dir/hello.c.o
[2/2] Linking C executable hello
```

Some variables in the file change how Ninja works:

- `builddir` gives the directory for the files `.ninja_log` and `.ninja_deps`.[^2]
- `ninja_required_version` gives the minimum version of Ninja that the file needs.[^2]
- A rule can set `restat = 1`. After the command runs, Ninja checks the outputs again, and an output whose modification time did not change is treated as if it was not built, so the steps after it can be skipped.[^2]
- A rule can set `pool` to limit how many of its commands run at the same time.[^2]

A generator written in Python can use `misc/ninja_syntax.py` from the Ninja distribution to write the file.[^2]

## Alternatives

- [[make]] reads makefiles that people write by hand, with pattern rules, built-in rules and functions.[^2] Ninja has "almost no features" in comparison, and the manual says that "Ninja by itself is unlikely to be useful for most projects."[^2]
- [[cmake]] and Meson generate the files for Ninja, so most users use Ninja through one of them.
- Bazel and Buck2 are build systems that read the build description and run the commands themselves, with no separate generator.

## History and versions

Evan Martin wrote Ninja for his work on the Chromium browser. The manual says that Chromium has "over 30,000 source files", and that its other build systems took ten seconds to start building after a change to one file, while Ninja takes less than a second.[^2] He released Ninja in 2011 and gave the ownership of the project to another maintainer in 2014.[^4]

In his blog post of 2020, Evan Martin writes that the split between a generator and Ninja, which describes only the commands and the files, is the main contribution of Ninja.[^4] He writes that Peter Collingbourne connected Ninja to CMake, first to build LLVM, and that the CMake authors later took over this work.[^4] He also lists Chrome, Android and all Meson projects as users of Ninja.[^4]

Ninja is free software under the Apache License 2.0.[^6] Its versions follow the form major.minor.patch, and the build file can ask for a minimum version with `ninja_required_version`.[^2] The manual for version 1.13.1 is dated July 2025, and it marks the features that were added in each version, such as `deps` in 1.3.[^2]

## References
