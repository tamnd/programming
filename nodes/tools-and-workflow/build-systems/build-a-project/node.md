---
tag: 4A6TCY
kind: task
slug: build-a-project
name: Build a project
aliases: [build a program, compile a project, build from the command line, build a small program with two files, make an executable]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Build a program of two source files with the usual build tool of each language, from one command, and run the result. The task shows the build file, the command and where the output goes.
see_also: [build-system-concept, build-graph, incremental-builds, compilers-and-interpreters]
sources:
  - url: https://github.com/golang/go/blob/go1.26.5/src/cmd/go/alldocs.go
    title: Go 1.26.5 source, cmd/go documentation (alldocs.go)
    locator: Compile packages and dependencies (go build, the -o flag); Build and test caching
  - url: https://doc.rust-lang.org/cargo/commands/cargo-build.html
    title: The Cargo Book, cargo build
    locator: DESCRIPTION; Output Options (--target-dir); Compilation Options (--release)
  - url: https://doc.rust-lang.org/cargo/reference/build-cache.html
    title: The Cargo Book, Build Cache
    locator: target/debug and target/release
  - url: https://doc.rust-lang.org/cargo/reference/profiles.html
    title: The Cargo Book, Profiles
    locator: debug; incremental; Default profiles (dev)
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/javac.html
    title: Java SE 25 Tools Reference, The javac Command
    locator: the -d option
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/jar.html
    title: Java SE 25 Tools Reference, The jar Command
    locator: --create; --main-class; -C
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
    title: Microsoft Learn, dotnet build
    locator: Description (outputs, implicit restore); Options (--output, --no-incremental, --no-restore)
  - url: https://www.gnu.org/software/make/manual/html_node/Rule-Introduction.html
    title: GNU make manual (version 4.4.1), 2.1 What a Rule Looks Like
    locator: the tab at the start of each recipe line
  - url: https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
    title: GNU make manual (version 4.4.1), 4.6 Phony Targets
    locator: first paragraph
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake.1.rst
    title: CMake 3.28.3 documentation source, cmake(1)
    locator: Synopsis; Description (generators)
  - url: https://ninja-build.org/manual.html
    title: The Ninja build system manual (version 1.13.1)
    locator: Philosophical overview
  - url: https://docs.python.org/3/library/zipapp.html
    title: The Python Standard Library (3.12), zipapp
    locator: Basic Example; Caveats
  - url: https://packaging.python.org/en/latest/tutorials/packaging-projects/
    title: Python Packaging User Guide, Packaging Python Projects
    locator: Generating distribution archives
  - url: https://nodejs.org/api/typescript.html
    title: Node.js v26.13.1 documentation, Modules, TypeScript
    locator: Type stripping; Determining module system
  - url: https://www.typescriptlang.org/docs/handbook/compiler-options.html
    title: TypeScript documentation, tsc CLI Options
    locator: Using the CLI; --outDir; --noEmit
  - url: https://docs.npmjs.com/cli/v11/commands/npm-run
    title: npm Docs (v11), npm run
    locator: Description
  - url: https://kotlinlang.org/docs/command-line.html
    title: Kotlin documentation, Kotlin command-line compiler
    locator: Create and run an application
---

## Task

Build a small program from the command line with the usual build tool of the language, and run it. The program has two source files: a module with a function `hello(name)` that returns `Hello, <name>!`, and a main file that calls `hello("build")` and prints the result. The output of each program is:

```text
Hello, build!
```

Each solution must:

- Keep the build in a file that the project stores with the code, such as `go.mod`, `Cargo.toml`, a `.csproj` file, a `Makefile` or a `CMakeLists.txt`. When a language has no such file, the build is one command.
- Build with one command, and put the outputs in a known place, away from the sources where the tool allows it.
- Run the result.

Each solution below is a shell script. It makes a temporary directory, writes the project files into it, builds, runs the program, and removes the directory. The outputs on this page come from Linux with Go 1.26, Cargo, JDK 25, .NET 10, GCC, CMake 3.28 with Ninja, Python 3 and Node.js.

## Solutions

| Language | Build file | Command | Output |
| --- | --- | --- | --- |
| Go | `go.mod` | `go build -o hello .` | the file named by `-o` |
| Rust | `Cargo.toml` | `cargo build` | `target/debug/hello` |
| Java | none | `javac -d out ...` and `jar --create ...` | `hello.jar` |
| C# | `hello.csproj` | `dotnet build` | `bin/Debug/net10.0/` |
| C | `Makefile` | `make` | `hello` |
| C++ | `CMakeLists.txt` | `cmake -S . -B build` and `cmake --build build` | `build/hello` |
| Python | none | `python3 -m zipapp ...` | `hello.pyz` |
| TypeScript | `package.json` | none: `node main.ts` runs the source | none |

**Go.** `go.mod` names the module `example.com/hello`, and the package `greet` is in the directory `greet`. `go build` compiles the packages named by the import paths, along with their dependencies, but it does not install the results.[^1] The `-o` flag makes it write the executable to the named file.[^1] Without `-o`, a build of one main package writes an executable named after the last element of the import path.[^1]

```bash run id=build-go
d=$(mktemp -d)
cd "$d"
mkdir greet
cat >go.mod <<'END'
module example.com/hello

go 1.26
END
cat >greet/greet.go <<'END'
// Package greet makes greetings.
package greet

// Hello returns a greeting for name.
func Hello(name string) string {
	return "Hello, " + name + "!"
}
END
cat >main.go <<'END'
package main

import (
	"fmt"

	"example.com/hello/greet"
)

func main() {
	fmt.Println(greet.Hello("build"))
}
END
go build -o hello .
./hello
cd /
rm -rf "${d:?}"
```

```output for=build-go
Hello, build!
```

**Rust.** `Cargo.toml` names the package. The library is in `src/lib.rs`, and the program is in `src/main.rs`. `cargo build` compiles the local packages and all of their dependencies.[^2] The outputs go to the directory `target` in the root of the workspace, unless `--target-dir` gives another one.[^2] A plain build uses the dev profile and writes to `target/debug/`; `--release` uses the release profile and writes to `target/release/`.[^3] The dev profile is the default for `cargo build`, and it turns on debug information and incremental compilation.[^4] Incremental compilation saves additional information to disk to make later compiles faster.[^4] This build runs once in an empty directory, so the `[profile.dev]` table in `Cargo.toml` turns both settings off with `debug = false` and `incremental = false`.[^4]

```bash run id=build-rust
d=$(mktemp -d)
cd "$d"
mkdir src
cat >Cargo.toml <<'END'
[package]
name = "hello"
version = "0.1.0"
edition = "2024"

[profile.dev]
debug = false
incremental = false
END
cat >src/lib.rs <<'END'
/// Returns a greeting for `name`.
pub fn hello(name: &str) -> String {
    format!("Hello, {name}!")
}
END
cat >src/main.rs <<'END'
fn main() {
    println!("{}", hello::hello("build"));
}
END
cargo build --quiet
./target/debug/hello
cd /
rm -rf "${d:?}"
```

```output for=build-rust
Hello, build!
```

**Java.** The JDK has no build file. `javac -d out` writes the class files to `out`, in subdirectories that follow the package names.[^5] `jar --create` packs them into one archive, `-C out .` takes the files from `out`, and `--main-class` sets the entry point of the application.[^6] `java -jar` then runs the archive.

```bash run id=build-java
d=$(mktemp -d)
cd "$d"
mkdir -p src/greet src/app
cat >src/greet/Greet.java <<'END'
package greet;

public final class Greet {
    public static String hello(String name) {
        return "Hello, " + name + "!";
    }
}
END
cat >src/app/Main.java <<'END'
package app;

import greet.Greet;

public class Main {
    public static void main(String[] args) {
        System.out.println(Greet.hello("build"));
    }
}
END
javac -d out src/greet/Greet.java src/app/Main.java
jar --create --file hello.jar --main-class app.Main -C out .
java -jar hello.jar
jar --list --file hello.jar
cd /
rm -rf "${d:?}"
```

```output for=build-java
Hello, build!
META-INF/
META-INF/MANIFEST.MF
app/
app/Main.class
greet/
greet/Greet.class
```

**C#.** The project file `hello.csproj` uses the SDK `Microsoft.NET.Sdk`, and `dotnet build` builds the project and its dependencies into a set of binaries.[^7] The code of the project is in an IL file with a `.dll` extension, next to an executable, a `.pdb` file of symbols, a `.deps.json` file and a `.runtimeconfig.json` file.[^7] The default output path is `./bin/<configuration>/<framework>/`.[^7] The script hides the log of the build, and lists the output directory.

```bash run id=build-csharp
d=$(mktemp -d)
cd "$d"
cat >hello.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <Nullable>enable</Nullable>
  </PropertyGroup>
</Project>
END
cat >Greet.cs <<'END'
static class Greet
{
    public static string Hello(string name) => "Hello, " + name + "!";
}
END
cat >Program.cs <<'END'
System.Console.WriteLine(Greet.Hello("build"));
END
dotnet build --nologo -v quiet >/dev/null
./bin/Debug/net10.0/hello
ls bin/Debug/net10.0
cd /
rm -rf "${d:?}"
```

```output for=build-csharp
Hello, build!
hello
hello.deps.json
hello.dll
hello.pdb
hello.runtimeconfig.json
```

**C with make.** The `Makefile` has a rule for each object file and one for the program. Each recipe line starts with a tab.[^8] `clean` is a phony target: it is not the name of a file, but a name for a recipe that runs when you ask for it.[^9] The script runs `make` two times. The second time, nothing changed, so make does nothing.

```bash run id=build-c
d=$(mktemp -d)
cd "$d"
cat >greet.h <<'END'
const char *greet_hello(void);
END
cat >greet.c <<'END'
#include "greet.h"
const char *greet_hello(void) { return "Hello, build!"; }
END
cat >main.c <<'END'
#include <stdio.h>
#include "greet.h"
int main(void) { puts(greet_hello()); return 0; }
END
cat >Makefile <<'END'
CC = cc
CFLAGS = -Wall -O2

hello: main.o greet.o
	$(CC) -o hello main.o greet.o
main.o: main.c greet.h
	$(CC) $(CFLAGS) -c main.c
greet.o: greet.c greet.h
	$(CC) $(CFLAGS) -c greet.c

.PHONY: clean
clean:
	rm -f hello main.o greet.o
END
make
./hello
make
make clean
cd /
rm -rf "${d:?}"
```

```output for=build-c
cc -Wall -O2 -c main.c
cc -Wall -O2 -c greet.c
cc -o hello main.o greet.o
Hello, build!
make: 'hello' is up to date.
rm -f hello main.o greet.o
```

**C++ with CMake.** CMake is a build system generator.[^10] `cmake -S . -B build` reads `CMakeLists.txt` and writes a build for another tool into the directory `build`, and `cmake --build build` runs that build.[^10] Here the generator is Ninja (`-G Ninja`). Ninja is intended to be used with a separate program that generates its input files.[^11] `add_library` and `add_executable` declare two targets, and `target_link_libraries` links the library into the program.

```bash run id=build-cpp
d=$(mktemp -d)
cd "$d"
cat >CMakeLists.txt <<'END'
cmake_minimum_required(VERSION 3.20)
project(hello LANGUAGES CXX)
add_library(greet greet.cpp)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE greet)
END
cat >greet.h <<'END'
#include <string>
std::string hello(const std::string& name);
END
cat >greet.cpp <<'END'
#include "greet.h"
std::string hello(const std::string& name) { return "Hello, " + name + "!"; }
END
cat >main.cpp <<'END'
#include <iostream>
#include "greet.h"
int main() { std::cout << hello("build") << '\n'; }
END
cmake -S . -B build -G Ninja >/dev/null
cmake --build build
./build/hello
cd /
rm -rf "${d:?}"
```

```output for=build-cpp
[1/4] Building CXX object CMakeFiles/greet.dir/greet.cpp.o
[2/4] Building CXX object CMakeFiles/hello.dir/main.cpp.o
[3/4] Linking CXX static library libgreet.a
[4/4] Linking CXX executable hello
Hello, build!
```

**Python.** Python runs the source, so there is no compile step that you must run. To give one file that runs, `python -m zipapp` packs a directory into an archive, and `-m "hello:main"` sets the function that runs first.[^12] To publish a library, the Python Packaging User Guide uses `python3 -m build` in the directory of `pyproject.toml`. It writes a source distribution (`.tar.gz`) and a built distribution (`.whl`) to `dist`.[^13]

```bash run id=build-python
d=$(mktemp -d)
cd "$d"
mkdir app
cat >app/greet.py <<'END'
def hello(name):
    return "Hello, " + name + "!"
END
cat >app/hello.py <<'END'
from greet import hello


def main():
    print(hello("build"))
END
python3 -m zipapp app -m "hello:main" -o hello.pyz
python3 hello.pyz
cd /
rm -rf "${d:?}"
```

```output for=build-python
Hello, build!
```

**TypeScript.** Node.js runs TypeScript files that contain only erasable TypeScript syntax: it replaces the types with whitespace and does no type checking.[^14] To use `import` and `export` in a `.ts` file, the nearest `package.json` must have `"type": "module"`, and an import must give the file extension, as in `./greet.ts`.[^14]

```bash run id=build-typescript
d=$(mktemp -d)
cd "$d"
cat >package.json <<'END'
{ "type": "module" }
END
cat >greet.ts <<'END'
export function hello(name: string): string {
  return `Hello, ${name}!`;
}
END
cat >main.ts <<'END'
import { hello } from "./greet.ts";
console.log(hello("build"));
END
node main.ts
cd /
rm -rf "${d:?}"
```

```output for=build-typescript
Hello, build!
```

To make JavaScript files for a browser or an older runtime, use the TypeScript compiler. `tsc` with no file names compiles the project of the nearest `tsconfig.json`, and `--outDir` sets the folder for the output files.[^15] The `scripts` object of `package.json` can name such a command, for example `"build": "tsc --outDir dist"`, and `npm run build` then runs it, because `npm run` runs a command from that object.[^16]

**Kotlin.** The Kotlin documentation compiles a program to a JAR file with the command `kotlinc hello.kt -include-runtime -d hello.jar`. The `-d` option gives the output path, and `-include-runtime` puts the Kotlin runtime library in the JAR, so that `java -jar` can run it.[^17] Then `java -jar hello.jar` runs the program.

## Edge cases

- **A second build with no changes.** make prints that the target is up to date, as in the C run. The `go` command caches build outputs for reuse in later builds.[^1] `dotnet build` builds incrementally unless you give `--no-incremental`, which forces a clean rebuild.[^7]
- **A clean build.** `make clean` removes the outputs in the C run. A separate output directory, such as `target`, `bin` or `build`, is one directory that you can delete.
- **Dependencies from a package registry.** None of the projects here has one. `dotnet build` runs an implicit restore of the NuGet packages first; `--no-restore` turns this off.[^7]
- **Debug and release builds.** Cargo writes a release build to `target/release/`.[^3] `dotnet build` uses the configuration in the output path, `Debug` by default here.

## Pitfalls

- **Spaces in place of a tab.** A recipe line in a makefile must start with a tab. The GNU Make manual calls this an obscurity that catches the unwary.[^8]
- **Class files next to the sources.** Without `-d`, javac puts each class file in the same directory as its source file.[^5]
- **No type check in Node.js.** `node main.ts` runs a file with a type error. Run `tsc --noEmit` to check types without writing files.[^15]
- **A build in the source tree.** Give CMake a separate build directory with `-B`, so that the generated files do not mix with the sources.[^10]
- **C extensions in a zip application.** A package that includes a C extension cannot run from a zip file, because the loader of the operating system needs the code in the file system.[^12]

## References
