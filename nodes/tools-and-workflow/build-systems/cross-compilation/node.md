---
tag: HVZS4X
kind: concept
slug: cross-compilation
name: Cross-compilation
aliases: [cross compile, cross compiler, cross-compiler, build host and target, target triple, target tuple, sysroot, toolchain prefix, Canadian Cross]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Cross-compilation makes a program on one machine for another operating system or CPU. It needs a compiler for the target, the libraries of the target, and a way to run or test the output.
history:
  year: 1969
  people: [Ken Thompson]
  note: The first UNIX, for a PDP-7, was compiled on a GECOS system and moved on paper tape. GCC later named the build, host and target machines of a cross compiler.
see_also: [cross-compile-a-program, build-configurations, compilers-and-interpreters, build-system-concept, hermetic-builds, go, rust, c]
sources:
  - url: https://en.wikipedia.org/wiki/Cross_compiler
    title: Wikipedia, Cross compiler
    locator: the lead; Use; Canadian Cross; Timeline of early cross compilers (1969); GCC and cross compilation; Plan 9
  - url: https://gcc.gnu.org/onlinedocs/gccint/Configure-Terms.html
    title: GNU Compiler Collection (GCC) Internals, 6.1 Configure Terms and History
    locator: build, host and target; native, cross, canadian, crossback; foo-bar-gcc
  - url: https://clang.llvm.org/docs/CrossCompilation.html
    title: Clang documentation, Cross-compilation using Clang
    locator: Introduction; Cross compilation issues; Target Triple; Toolchain Options; Target-Specific Libraries
  - url: https://go.dev/doc/install/source
    title: Go documentation, Installing Go from source
    locator: Optional environment variables ($GOOS and $GOARCH, the valid combinations)
  - url: https://learn.microsoft.com/en-us/dotnet/core/rid-catalog
    title: Microsoft Learn, .NET Runtime Identifier (RID) catalog
    locator: the introduction; RID graph (the pattern of a RID)
  - url: https://rust-lang.github.io/rustup/cross-compilation.html
    title: The rustup book, Cross-compilation
    locator: the whole page
  - url: https://doc.rust-lang.org/cargo/reference/config.html
    title: The Cargo Book, Configuration
    locator: target.<tuple>.linker; target.<tuple>.runner
  - url: https://www.qemu.org/docs/master/user/main.html
    title: QEMU documentation, QEMU User space emulator
    locator: Features (System call translation); Linux User space emulator, Command line options (-L path)
  - url: https://pkg.go.dev/cmd/cgo
    title: Go documentation, Command cgo
    locator: Using cgo with the go command (cross-compiling, CGO_ENABLED, CC_FOR_TARGET)
  - url: https://doc.rust-lang.org/rustc/platform-support.html
    title: The rustc book, Platform Support
    locator: Tier 1 with Host Tools; Tier 2 with Host Tools; Tier 3
---

## Idea

A compiler usually makes programs for the machine that it runs on. A **cross compiler** makes executable code for a platform other than the one on which the compiler runs.[^1] A compiler on a PC that makes programs for Android devices is an example.[^1] **Cross-compilation** is the use of such a compiler, together with the other tools and libraries of the target, to build a program.

There are two usual reasons to cross-compile.[^1] One machine can build a program for many platforms. And some targets, such as embedded systems with few resources, cannot run the compiler at all.

Three machines can take part in a build of a compiler. The GCC documentation names them:[^2]

- **build**: the machine that you build on.
- **host**: the machine that you build for, where the compiler that you build will run.
- **target**: the machine for which that compiler will make code.

For an application, which does not make code, only two of these names matter: the machine that builds and the machine that runs the program. Each tool has its own words for them. GCC calls them build and host. The Clang documentation calls the machine that compiles the host, and the machine that runs the program the target.[^3] Go calls them `GOHOSTOS`/`GOHOSTARCH` and `GOOS`/`GOARCH`.[^4] Read the words of each tool with care.

## How it works

A cross build needs four parts. Each part must match the target, not the build machine.

**1. A name for the target.** Most toolchains name a target with a string of parts. In Clang, a target triple has the general form `<arch><sub>-<vendor>-<sys>-<env>`: for example, the architecture `x86_64` or `arm`, the vendor `pc` or `apple`, the system `linux` or `win32`, and the environment `gnu` or `eabi`.[^3] Without a target, Clang assumes the host triple, and it makes code for the host platform.[^3] Rust uses the same kind of name, such as `aarch64-unknown-linux-gnu`. Go uses a pair of an operating system and an architecture, such as `linux` and `arm64`, and the documentation lists the valid combinations.[^4] .NET uses a runtime identifier (RID), such as `linux-x64`, `win-x64` or `osx-x64`.[^5]

**2. A compiler that makes code for the target.** A GCC compiler is built for one target. If GCC is configured with `--target=foo-bar`, the cross compiler is called `foo-bar-gcc`.[^2] GCC needs a build of binutils, with the assembler, for each target.[^1] Clang and LLVM are natively a cross compiler: one set of programs can compile for all targets, and an option selects the target.[^3] The Go and Rust compilers also have many targets in one program.

**3. The libraries and headers of the target.** A program for Arm Linux must link against a C library for Arm Linux, not the one of the build machine. To cross-compile GCC, part of the C standard library of the target must be on the build machine.[^1] A sysroot is a directory that holds the files of the target as if it were the root of its file system. The Clang option `--sysroot` changes the logical root for headers and libraries.[^3] When a distribution installs the cross toolchain, the files of the target can be in other places, and you often give the include paths (`-I`) and library paths (`-L`) yourself.[^3] A Rust target needs its standard library: `rustup target add` installs it.[^6]

**4. A linker for the target.** The compiler of the language makes object code, but the last step, the link, needs a linker that knows the target. `rustup target add` installs only the Rust standard library, and other tools, particularly a linker, are typically necessary.[^6] In Cargo, `target.<tuple>.linker` names the linker for one target, and Cargo passes it to `rustc` with `-C linker`.[^7]

The first run below asks each toolchain on a Linux x86-64 machine for its own target, and for some of the other targets that it knows. `gcc -dumpmachine` prints the target of a GCC compiler. The machine has a GCC cross compiler for Arm, with the name `aarch64-linux-gnu-gcc`, so its target is the prefix of its name.

```bash run id=cross-triples
# Ask each toolchain on this machine for its host and its targets.
echo "--- uname"
uname -s -m
echo "--- gcc and a GCC cross compiler"
gcc -dumpmachine
aarch64-linux-gnu-gcc -dumpmachine
echo "--- clang"
clang -print-target-triple
clang --target=aarch64-linux-gnu -print-target-triple
clang --target=riscv64-linux-gnu -print-target-triple
echo "--- rustc"
rustc -vV | grep '^host'
rustc --print target-list | grep -E '^(aarch64-unknown-linux-gnu|x86_64-pc-windows-msvc|wasm32-unknown-unknown)$'
echo "--- go"
echo "$(go env GOHOSTOS)/$(go env GOHOSTARCH)"
go tool dist list | grep -E '^(linux/arm64|windows/amd64|wasip1/wasm)$'
```

```output for=cross-triples
--- uname
Linux x86_64
--- gcc and a GCC cross compiler
x86_64-linux-gnu
aarch64-linux-gnu
--- clang
x86_64-pc-linux-gnu
aarch64-unknown-linux-gnu
riscv64-unknown-linux-gnu
--- rustc
host: x86_64-unknown-linux-gnu
aarch64-unknown-linux-gnu
wasm32-unknown-unknown
x86_64-pc-windows-msvc
--- go
linux/amd64
linux/arm64
wasip1/wasm
windows/amd64
```

The second run shows where the GCC cross compiler for Arm finds the files of its target. The cross compiler prints `/` as its sysroot, so it uses the root of the build machine. It finds the headers in directories under `aarch64-linux-gnu`, the name of the target. The search list also has `/usr/include`, a directory of the build machine; the compiler uses a header from it when the directories of the target do not have that header. The C library that the link uses is a 64-bit Arm file, and the program asks for the dynamic loader of Arm Linux, `/lib/ld-linux-aarch64.so.1`. The x86-64 machine cannot run the program directly. QEMU in user mode runs it: it translates the system calls of the program to system calls of the host, and its option `-L` sets the prefix of the ELF interpreter, here the directory of the Arm libraries.[^8]

```bash run id=cross-sysroot
# Show where a cross compiler finds the C library of its target.
d=$(mktemp -d)
cd "$d"
printf '#include <stdio.h>\nint main(void) { puts("hi"); return 0; }\n' >hi.c
echo "--- the headers of the target"
aarch64-linux-gnu-gcc -print-sysroot
aarch64-linux-gnu-gcc -E -v hi.c 2>&1 | sed -n '/^#include <...> search starts here:/,/^End of search list./p'
echo "--- the C library of the target"
aarch64-linux-gnu-gcc -print-file-name=libc.so.6
aarch64-linux-gnu-gcc -o hi hi.c
readelf -h hi | grep -E 'Class|Machine'
readelf -l hi | grep -o 'interpreter: [^]]*'
echo "--- run it with QEMU and the libraries of the target"
qemu-aarch64 -L /usr/aarch64-linux-gnu ./hi
cd /
rm -rf "${d:?}"
```

```output for=cross-sysroot
--- the headers of the target
/
#include <...> search starts here:
 /usr/lib/gcc-cross/aarch64-linux-gnu/13/include
 /usr/lib/gcc-cross/aarch64-linux-gnu/13/../../../../aarch64-linux-gnu/include
 /usr/include
End of search list.
--- the C library of the target
/usr/lib/gcc-cross/aarch64-linux-gnu/13/../../../../aarch64-linux-gnu/lib/../lib/libc.so.6
  Class:                             ELF64
  Machine:                           AArch64
interpreter: /lib/ld-linux-aarch64.so.1
--- run it with QEMU and the libraries of the target
hi
```

## In languages

The task [[cross-compile-a-program]] builds one program for another target in Go, Rust, C with Clang and C#. The table compares the tools.

| Language | How to name the target | What else the build needs |
| --- | --- | --- |
| Go | `GOOS` and `GOARCH` | nothing for pure Go; a C cross compiler for cgo |
| Rust | `--target <triple>` | `rustup target add`, and a linker for the target |
| C with Clang | `--target=<triple>` | the headers, the C library and a linker of the target |
| C with GCC | a compiler built for the target, such as `aarch64-linux-gnu-gcc` | binutils and the C library of the target |
| C# (.NET) | a RID, with `-r` or `<RuntimeIdentifier>` | the platform-specific assets for the RID |

**Go.** `$GOOS` is the target operating system, and `$GOARCH` is the target architecture.[^4] The choices of `$GOOS` include `linux`, `darwin`, `windows` and `wasip1`, and the choices of `$GOARCH` include `amd64`, `arm64` and `wasm`.[^4] cgo is disabled by default when cross-compiling; `CGO_ENABLED=1` turns it on.[^9] Then you must give a C cross compiler, for example with the `CC` variable.[^9]

**Rust.** At first, rustup installs only the standard library for the host platform, and `rustup target add` installs others.[^6] Then `cargo build --target=<triple>` builds for the target.[^6] The Rust project sorts its targets into tiers. Tier 1 targets can be thought of as "guaranteed to work", tier 2 targets as "guaranteed to build", and tier 3 targets are not built or tested automatically.[^10]

**C#.** RID values identify the platforms where an application runs, and NuGet packages use them for platform-specific assets.[^5] The `--runtime` option of `dotnet build`, `dotnet publish` and other commands takes a RID.[^5] For a package with native dependencies, the RID tells on which platforms the package can be restored.[^5]

## Variations and related ideas

- **The names of builds.** GCC names a build by the three machines.[^2] If build, host and target are the same, it is a native build. If build and host are the same but the target differs, it is a cross. If all three differ, it is a canadian. If host and target are the same but build differs, some people call it a crossed native or host-x-host: a cross compiler builds a native compiler for another system. If build and target are the same but host differs, there is a proposal to call it a crossback.[^2] The name Canadian Cross comes from a time when Canada had three national political parties.[^1]
- **Run the output on the build machine.** An emulator lets a cross build run its tests. QEMU user mode runs a Linux program for another CPU on a Linux host.[^8] In Cargo, `target.<tuple>.runner` names a program that runs the executables of the target, for `cargo run`, `cargo test` and `cargo bench`.[^7] The task [[cross-compile-a-program]] uses this with QEMU.
- **Portable code.** Code for a virtual machine, such as the IL of .NET or the class files of Java, runs on each platform that has the virtual machine. That part of the build does not depend on the target.
- **Multiplatform systems.** The toolchain of Plan 9 does not make a difference between cross and native compilation, and its makefiles do not depend on the architecture.[^1]

## Pitfalls

- **The build system finds the libraries of the host.** The dependencies that a configure step checks for, such as libxml or libz, match the host platform, not the target.[^3] If the build system does not know that you cross-compile, it gets every dependency wrong, and the build fails at build time, not at configure time.[^3]
- **Code that runs during the build.** A build step that compiles a helper program and runs it, such as a code generator, must compile that helper for the build machine, not for the target. A cross build therefore can need two compilers.
- **A target without a target name.** If you do not give Clang the target, it makes code for the host, and the build breaks later, when it assembles or links.[^3]
- **Missing standard library.** `rustup target add` installs only the standard library. Without a linker for the target, the build stops at the link.[^6]
- **cgo turns off.** An import of the special package `C` implies the `cgo` build constraint, so when cgo is off, the go tool does not build the files with that import.[^9] A cross build with cgo needs `CGO_ENABLED=1` and a C cross compiler.[^9]
- **Tests that do not run.** A cross build makes a program that the build machine cannot run. Run the tests on the target, in an emulator, or with a runner. An emulator is not the real device: the timing, the system and the hardware can differ.

## Cost

A cross build saves the time and the cost of a build machine for each target, and it allows a target that is too small to compile. The costs are in the set-up: a toolchain, a C library and a linker for each target, kept in step with the versions on the target. Clang, Go and Rust put many targets in one compiler, so the extra cost of a target is its libraries and its linker.[^3][^6] An emulator that runs the tests is slower than the real CPU.

## History

The first version of UNIX was developed by Ken Thompson on a PDP-7 in 1969. According to Wikipedia, it was cross-compiled on a GECOS system, and the output was moved on paper tape.[^1] In the 1980s, Aztec C offered native and cross compilers for home computers such as the Apple II and the Commodore 64.[^1] GCC supports many platforms as a cross compiler, and its documentation describes the build, host and target machines of its configuration.[^2][^1] Clang and LLVM give one compiler for all targets.[^3] In Go, a pure Go program needs only the two target variables.[^4][^9] In Rust, it needs a target name, the standard library of the target and a linker.[^6]

## References
