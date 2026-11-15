---
tag: HMPVDV
kind: task
slug: cross-compile-a-program
name: Cross-compile a program
aliases: [cross compile, build for another platform, build for Arm, build for Windows from Linux, GOOS and GOARCH, cargo build --target, clang --target, dotnet build -r]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Build one program on Linux x86-64 for 64-bit Arm Linux and for Windows, in Go, Rust, C with Clang and C#. Check the file type of each result, and run the Arm one in QEMU.
see_also: [cross-compilation, build-a-project, build-configurations, dotnet-build, go, rust, c, csharp]
sources:
  - url: https://go.dev/doc/install/source
    title: Go documentation, Installing Go from source
    locator: Optional environment variables ($GOOS and $GOARCH, the valid combinations)
  - url: https://pkg.go.dev/cmd/cgo
    title: Go documentation, Command cgo
    locator: Using cgo with the go command (cross-compiling, CGO_ENABLED, CC)
  - url: https://rust-lang.github.io/rustup/cross-compilation.html
    title: The rustup book, Cross-compilation
    locator: the whole page
  - url: https://doc.rust-lang.org/cargo/commands/cargo-build.html
    title: The Cargo Book, cargo build
    locator: Compilation Options (--target)
  - url: https://doc.rust-lang.org/cargo/reference/config.html
    title: The Cargo Book, Configuration
    locator: target.<tuple>.linker; target.<tuple>.runner
  - url: https://clang.llvm.org/docs/CrossCompilation.html
    title: Clang documentation, Cross-compilation using Clang
    locator: Target Triple; Toolchain Options; Target-Specific Libraries
  - url: https://www.qemu.org/docs/master/user/main.html
    title: QEMU documentation, QEMU User space emulator
    locator: Features; Linux User space emulator, Command line options (-L path)
  - url: https://learn.microsoft.com/en-us/dotnet/core/rid-catalog
    title: Microsoft Learn, .NET Runtime Identifier (RID) catalog
    locator: the introduction (RID values, the --runtime option)
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-publish
    title: Microsoft Learn, dotnet publish
    locator: Options (-r, --runtime); Examples
  - url: https://learn.microsoft.com/en-us/dotnet/core/deploying/
    title: Microsoft Learn, .NET application publishing overview
    locator: Publish self-contained; Publish framework-dependent; the apphost and UseAppHost
---

## Task

On a Linux machine with an x86-64 CPU, build a small program for two other platforms: Linux on a 64-bit Arm CPU, and, where the toolchain allows it, Windows on x86-64. The program prints a line that names the platform that it runs on. Then show that each output file is for the right platform, and run the Arm program with an emulator, because the build machine cannot run it directly.

Each solution must:

- Name the target in the build command or in a file that the project keeps, not by a change of the build machine.
- Leave the output in a known place.
- Show the file type of the output with the `file` command.

Each solution below is a shell script. It makes a temporary directory, writes the project files into it, builds, checks the output, and removes the directory. The outputs on this page come from Linux x86-64 with Go 1.26, Rust with the target `aarch64-unknown-linux-gnu`, Clang 18 with the GCC cross toolchain for `aarch64-linux-gnu`, the MinGW-w64 files for Windows, .NET 10, and QEMU in user mode. The `cut` commands keep the first fields of the output of `file`. For the ideas and the words, such as host, target and sysroot, see [[cross-compilation]].

## Solutions

| Language | How to name the target | What the build machine needs | Run in QEMU |
| --- | --- | --- | --- |
| Go | `GOOS=linux GOARCH=arm64` | only Go, for pure Go code | `qemu-aarch64 ./hello-linux-arm64` |
| Rust | `--target aarch64-unknown-linux-gnu` | the standard library of the target and a linker | `cargo run` with a runner |
| C | `clang --target=aarch64-linux-gnu` | the headers, the C library and the linker of the target | `qemu-aarch64 -L ...` |
| C# | `dotnet build -r linux-arm64` | the .NET SDK | not shown |

**Go.** `GOOS` and `GOARCH` give the target operating system and architecture, and the default is the platform of the build machine.[^1] The Go documentation lists the valid pairs, such as `linux/arm64` and `windows/amd64`.[^1] cgo is disabled by default when cross-compiling, so this pure Go program needs no C compiler.[^2] The Arm program runs in QEMU without `-L`, because it does not ask for a dynamic loader.

```bash run id=cross-go timeout=30
# Build one Go program for two other targets, and run the Linux one in QEMU.
d=$(mktemp -d)
cd "$d"
cat >go.mod <<'END'
module example.com/hello

go 1.26
END
cat >main.go <<'END'
package main

import (
	"fmt"
	"runtime"
)

func main() {
	fmt.Println("hello from", runtime.GOOS+"/"+runtime.GOARCH)
}
END
echo "host: $(go env GOHOSTOS)/$(go env GOHOSTARCH)"
GOOS=linux GOARCH=arm64 go build -o hello-linux-arm64 .
GOOS=windows GOARCH=amd64 go build -o hello.exe .
file -b hello-linux-arm64 | cut -d, -f1-2
file -b hello.exe | cut -d, -f1
qemu-aarch64 ./hello-linux-arm64
cd /
rm -rf "${d:?}"
```

```output for=cross-go
host: linux/amd64
ELF 64-bit LSB executable, ARM aarch64
PE32+ executable (console) x86-64
hello from linux/arm64
```

**Rust.** rustup installs the standard library of the host platform at first. `rustup target add aarch64-unknown-linux-gnu` installs the standard library for the target, and `cargo build --target` builds for it.[^3] The default of `--target` is the host architecture.[^4] `rustup target add` does not install a linker, and a linker for the target is typically necessary.[^3] The file `.cargo/config.toml` gives two settings for the target. `linker` names the linker, which Cargo passes to `rustc` with `-C linker`.[^5] `runner` names a program that runs the executables of the target: Cargo runs the runner with the executable as an argument, for `cargo run`, `cargo test` and `cargo bench`.[^5] Here the linker is the GCC cross compiler for Arm, and the runner is QEMU. The output goes to `target/<triple>/release/`.

```bash run id=cross-rust timeout=30
# Build one Rust program for 64-bit Arm Linux, and run it in QEMU through Cargo.
d=$(mktemp -d)
cd "$d"
cargo new -q --vcs none hello
cd hello
cat >src/main.rs <<'END'
fn main() {
    println!("hello from {}/{}", std::env::consts::OS, std::env::consts::ARCH);
}
END
mkdir .cargo
cat >.cargo/config.toml <<'END'
[target.aarch64-unknown-linux-gnu]
linker = "aarch64-linux-gnu-gcc"
runner = "qemu-aarch64 -L /usr/aarch64-linux-gnu"
END
export CARGO_INCREMENTAL=0
cargo build -q --release --target aarch64-unknown-linux-gnu
ls target/aarch64-unknown-linux-gnu/release/hello
file -b target/aarch64-unknown-linux-gnu/release/hello | cut -d, -f1-2
cargo run -q --release --target aarch64-unknown-linux-gnu
cd /
rm -rf "${d:?}"
```

```output for=cross-rust
target/aarch64-unknown-linux-gnu/release/hello
ELF 64-bit LSB pie executable, ARM aarch64
hello from linux/aarch64
```

**C with Clang.** One Clang program can compile for all its targets, and `--target` selects one.[^6] Without the option, Clang assumes the host triple.[^6] Clang also needs the headers, the libraries and the linker of the target. When the triple is the prefix of a cross toolchain that a package manager installed, Clang finds the other programs of that toolchain, such as the linker.[^6] On this machine, the triple `aarch64-linux-gnu` matches the GCC cross toolchain for Arm, and `x86_64-w64-mingw32` matches MinGW-w64 for Windows. The source uses the macros of the target, `_WIN32` and `__aarch64__`, to print its platform. The Arm program needs the dynamic loader and the C library of Arm Linux, so QEMU gets `-L` with the directory of those files.[^7]

```bash run id=cross-c
# Compile one C program with Clang for 64-bit Arm Linux and for 64-bit Windows.
d=$(mktemp -d)
cd "$d"
cat >hello.c <<'END'
#include <stdio.h>

int main(void) {
#if defined(_WIN32)
    puts("hello from Windows");
#elif defined(__aarch64__)
    puts("hello from 64-bit Arm");
#else
    puts("hello from another target");
#endif
    return 0;
}
END
clang --target=aarch64-linux-gnu -o hello-arm64 hello.c
clang --target=x86_64-w64-mingw32 -o hello.exe hello.c
file -b hello-arm64 | cut -d, -f1-2
file -b hello.exe | cut -d, -f1
qemu-aarch64 -L /usr/aarch64-linux-gnu ./hello-arm64
cd /
rm -rf "${d:?}"
```

```output for=cross-c
ELF 64-bit LSB pie executable, ARM aarch64
PE32+ executable (console) x86-64
hello from 64-bit Arm
```

**C#.** A runtime identifier (RID), such as `linux-x64` or `win-x64`, names the platform where an application runs.[^8] `dotnet build` and `dotnet publish` take a RID with `--runtime` (`-r`).[^8][^9] The documentation tells you to give `--self-contained` or `--no-self-contained` with a RID.[^9] A self-contained output includes the .NET runtime, and a framework-dependent output needs a .NET runtime on the target.[^10] The build also makes a platform-specific executable, the apphost, because the property `UseAppHost` defaults to `true`.[^10] The run builds for `linux-arm64`, framework-dependent. `file` shows that the apphost `app` is an ELF executable for Arm. The program needs a .NET runtime for Arm to run, so the script does not run it in QEMU. For a release, use `dotnet publish` with the same options; see [[dotnet-build]] for the build command.

```bash run id=cross-csharp timeout=30
# Build one .NET program for 64-bit Arm Linux.
d=$(mktemp -d)
cd "$d"
cat >app.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
  </PropertyGroup>
</Project>
END
cat >Program.cs <<'END'
using System.Runtime.InteropServices;

System.Console.WriteLine($"hello from {RuntimeInformation.OSArchitecture}");
END
dotnet build -r linux-arm64 --self-contained false --tl:off -nologo -v q 2>&1 | grep -v -E 'Warn|Error|^$|Time Elapsed'
ls bin/Debug/net10.0/linux-arm64 | grep -E '^app'
file -b bin/Debug/net10.0/linux-arm64/app | cut -d, -f1-2
file -b bin/Debug/net10.0/linux-arm64/app.dll | cut -d, -f1
cd /
rm -rf "${d:?}"
```

```output for=cross-csharp
Build succeeded.
app
app.deps.json
app.dll
app.pdb
app.runtimeconfig.json
ELF 64-bit LSB pie executable, ARM aarch64
PE32+ executable (console) Aarch64 Mono/.Net assembly
```

## Edge cases

- **cgo in Go.** With cgo on, a cross build needs a C cross compiler. Set `CGO_ENABLED=1` and give the compiler in the `CC` variable.[^2]
- **No linker for the target.** In Rust, the compile works, but the link fails, because `rustup target add` installs only the standard library.[^3] Give a linker with `target.<tuple>.linker` or the variable `CARGO_TARGET_<tuple>_LINKER`.[^5]
- **Libraries of the target.** A C program that uses a library, such as zlib, needs the build of that library for the target. If the build system looks for libraries on the build machine, it finds the ones of the host.[^6]
- **A self-contained .NET program.** With `--self-contained`, the output includes the .NET runtime for the RID, so the target needs no runtime.[^10]
- **Windows programs.** The Windows outputs on this page are not run. Run them on Windows, or in a Windows test environment.

## Pitfalls

- **A build for the host by mistake.** If you forget the target, the compilers here build for the build machine. Check the output with `file` before you ship it.
- **Tests only in an emulator.** QEMU in user mode translates the system calls of the program to system calls of the host.[^7] Timing, the kernel and the hardware are those of the host. Test on the real target too.
- **A hard-coded host path.** A path such as `/usr/lib/x86_64-linux-gnu` in a build file makes a cross build link the libraries of the host.

## Related tasks

- [[build-a-project]] builds the same kind of program for the host.

## References
