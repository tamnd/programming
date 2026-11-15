---
tag: BT63YD
kind: concept
slug: build-configurations
name: Build configurations
aliases: [build configuration, build type, debug build, release build, build profile, Debug and Release, CMAKE_BUILD_TYPE, NDEBUG, debug_assertions, build tags, feature flags at build time]
area: tools-and-workflow/build-systems
level: core
summary: >-
  A build configuration is a named set of build settings, such as Debug or Release. It sets the optimization, the debug information, the checks and the optional code of one build.
see_also: [build-system-concept, cmake, dotnet-build, cross-compilation, cross-compile-a-program, assertions, build-a-project, reproducible-builds]
sources:
  - url: https://github.com/Kitware/CMake/blob/master/Help/manual/cmake-buildsystem.7.rst
    title: CMake documentation source, cmake-buildsystem(7)
    locator: Build Configurations; Case Sensitivity; Default And Custom Configurations
  - url: https://doc.rust-lang.org/cargo/reference/profiles.html
    title: The Cargo Book, Profiles
    locator: opt-level; overflow-checks; Default profiles (dev, release); Custom profiles
  - url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
    title: Using the GNU Compiler Collection (GCC), Options That Control Optimization
    locator: -O0; -O2
  - url: https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
    title: Using the GNU Compiler Collection (GCC), Options for Debugging Your Program
    locator: -g
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/assert.html
    title: The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), assert
    locator: DESCRIPTION; CHANGE HISTORY
  - url: https://pkg.go.dev/cmd/go
    title: Go documentation, Command go
    locator: Build flags (-tags, -ldflags); Build constraints
  - url: https://doc.rust-lang.org/cargo/reference/features.html
    title: The Cargo Book, Features
    locator: the introduction
  - url: https://pkg.go.dev/cmd/link
    title: Go documentation, Command link
    locator: -X importpath.name=value
  - url: https://github.com/Kitware/CMake/blob/master/Help/variable/CMAKE_BUILD_TYPE.rst
    title: CMake documentation source, CMAKE_BUILD_TYPE
    locator: the whole page
  - url: https://doc.rust-lang.org/reference/conditional-compilation.html
    title: The Rust Reference, Conditional compilation
    locator: debug_assertions
  - url: https://docs.python.org/3/using/cmdline.html
    title: Python documentation, Command line and environment
    locator: -O; -OO
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: The Python Language Reference, Simple statements
    locator: 7.3 The assert statement
---

## Idea

One source tree can give different programs. A build for a developer keeps the debug information and the checks, and it compiles fast. A build for the users is optimized and smaller, and it leaves out the checks that cost time. A **build configuration** is a named set of build settings that makes one of these builds. Common names are Debug and Release. CMake defines four standard configurations, `Debug`, `Release`, `RelWithDebInfo` and `MinSizeRel`, and a project can add others.[^1] Cargo calls them profiles: `dev` is the default for `cargo build`, and `--release` selects `release`.[^2]

A configuration usually sets these things:

- **The optimization level.** GCC with `-O0`, the default, compiles fast and makes debugging give the expected results; `-O2` turns on nearly all optimizations that do not trade space for speed.[^3]
- **The debug information.** GCC with `-g` writes debugging information in the native format of the system, such as DWARF.[^4]
- **The checks.** In C, `assert` is turned off when `NDEBUG` is defined.[^5] In Rust, the release profile turns off `debug_assertions` and the checks for integer overflow.[^2]
- **Optional code.** Build tags in Go, features in Cargo, and macros in C choose code at build time.[^6][^7]
- **Values in the program.** A version string or a URL can be set at build time.[^8]

## How it works

The build tool keeps a table of settings for each configuration name, and it passes the settings of the chosen configuration to the compiler and the linker.

**CMake.** Each configuration defines compiler and linker flag variables for each language, with the names `CMAKE_<LANG>_FLAGS_<CONFIG>`.[^1] With a single-configuration generator, such as Makefiles or Ninja, the variable `CMAKE_BUILD_TYPE` chooses the configuration at configure time.[^9][^1] Its default is often an empty string, which is not the same as `Debug`.[^1] With a multi-configuration generator, such as Visual Studio, Xcode or Ninja Multi-Config, you choose the configuration at build time, and CMake ignores `CMAKE_BUILD_TYPE`.[^1] So CMake tells you to use a generator expression, such as `$<$<CONFIG:Debug>:DEBUG_BUILD>`, for logic that depends on the configuration.[^1]

**Cargo.** A profile is a table in `Cargo.toml`. The defaults of the two main profiles are:[^2]

| Setting | `dev` | `release` |
| --- | --- | --- |
| `opt-level` | 0 | 3 |
| `debug` | true | false |
| `debug-assertions` | true | false |
| `overflow-checks` | true | false |
| `incremental` | true | false |
| `codegen-units` | 256 | 16 |

A custom profile must name the profile that it inherits from with `inherits`.[^2]

**MSBuild and .NET.** For the configurations of a .NET project, see [[dotnet-build]] and [[msbuild]].

**The program sees the configuration.** A configuration can also change the source. C has the macro `NDEBUG`: when it is defined on the command line or before `#include <assert.h>`, assertions are not compiled into the program.[^5] Rust has the configuration option `debug_assertions`, which is on by default when you compile without optimizations; it controls the `debug_assert!` macro of the standard library.[^10] In Python, the option `-O` removes `assert` statements and the code that depends on the value of `__debug__`.[^11] `__debug__` is `True` normally and `False` when optimization is requested with `-O`.[^12]

## In languages

**C.** The script compiles one program two times, with `-O0 -g` for debug and with `-O2 -DNDEBUG` for release. The function `average` asserts that the list is not empty. When an `assert` fails, it writes the text of the expression, the file, the line and the function to the standard error, and calls `abort`.[^5] The debug program stops with a status that is not 0. The release program has no check, so it runs the next line. `readelf` counts the `.debug_` sections that `-g` adds to the file.

```bash run id=config-c
# Build one C program in a debug and in a release configuration.
d=$(mktemp -d)
cd "$d"
cat >avg.c <<'END'
#include <assert.h>
#include <stdio.h>

static int average(const int *xs, int n) {
    assert(n > 0);
    if (n <= 0) return 0;
    int sum = 0;
    for (int i = 0; i < n; i++) sum += xs[i];
    return sum / n;
}

int main(void) {
#ifdef NDEBUG
    puts("NDEBUG is defined: assert does nothing");
#else
    puts("NDEBUG is not defined: assert checks");
#endif
    int xs[] = {2, 4, 9};
    printf("average of 3 numbers: %d\n", average(xs, 3));
    fflush(stdout);
    printf("average of 0 numbers: %d\n", average(xs, 0));
    return 0;
}
END
cc -O0 -g -o debug avg.c
cc -O2 -DNDEBUG -o release avg.c
# The shell reports a program that a signal stopped on its standard error.
# This line sends that report away, so the output is the same in each run.
exec 2>/dev/null
for b in debug release; do
  echo "--- $b"
  ./$b 2>err.txt
  s=$?
  cat err.txt
  echo "exit status: $s"
  echo "debug sections: $(readelf -S --wide $b | grep -c '\.debug_')"
done
cd /
rm -rf "${d:?}"
```

```output for=config-c
--- debug
NDEBUG is not defined: assert checks
average of 3 numbers: 5
debug: avg.c:5: average: Assertion `n > 0' failed.
exit status: 134
debug sections: 6
--- release
NDEBUG is defined: assert does nothing
average of 3 numbers: 5
average of 0 numbers: 0
exit status: 0
debug sections: 0
```

**Rust.** The function `next` adds 1 to a `u8`. The script gives it 255. With the dev profile, overflow checks are on, so the addition panics.[^2] With the release profile, the checks are off, and the addition wraps to 0. `cfg!(debug_assertions)` shows the setting of each build.[^10]

```bash run id=config-rust timeout=30
# Build one Rust program with the dev profile and with the release profile.
d=$(mktemp -d)
cd "$d"
cargo new -q --vcs none counter
cd counter
cat >src/main.rs <<'END'
fn next(x: u8) -> u8 {
    x + 1
}

fn main() {
    println!("debug_assertions: {}", cfg!(debug_assertions));
    let x: u8 = std::env::args().count() as u8 + 254;
    println!("{} + 1 = {}", x, next(x));
}
END
export CARGO_INCREMENTAL=0
cargo build -q
cargo build -q --release
echo "--- target/debug/counter (dev profile)"
./target/debug/counter 2>&1 | grep -v '^note:' | sed -E 's/ \([0-9]+\) panicked/ panicked/'
echo "--- target/release/counter (release profile)"
./target/release/counter
cd /
rm -rf "${d:?}"
```

```output for=config-rust
--- target/debug/counter (dev profile)
debug_assertions: true

thread 'main' panicked at src/main.rs:2:5:
attempt to add with overflow
--- target/release/counter (release profile)
debug_assertions: false
255 + 1 = 0
```

**Go.** The `go` command has no Debug and Release names. A build constraint, a line comment that starts with `//go:build`, is a condition under which a file is part of the package.[^6] The option `-tags` adds tags that the build treats as satisfied.[^6] Here one file has `//go:build debug` and the other has `//go:build !debug`, so `-tags debug` chooses the first one. `-ldflags` passes arguments to the linker, and the linker option `-X importpath.name=value` sets a string variable.[^6][^8] `-X` works only when the variable has no initial value or a constant string.[^8]

```bash run id=config-go
# Select code with a build tag, and set a variable at link time.
d=$(mktemp -d)
cd "$d"
cat >go.mod <<'END'
module example.com/app

go 1.26
END
cat >main.go <<'END'
package main

import "fmt"

var version = "dev"

func main() {
	fmt.Println("version:", version)
	fmt.Println("extra checks:", extraChecks)
}
END
cat >checks_on.go <<'END'
//go:build debug

package main

const extraChecks = true
END
cat >checks_off.go <<'END'
//go:build !debug

package main

const extraChecks = false
END
echo "--- go build"
go build -o app . && ./app
echo "--- go build -tags debug"
go build -tags debug -o app . && ./app
echo "--- go build -ldflags=-X main.version=1.4.0"
go build -ldflags='-X main.version=1.4.0' -o app . && ./app
cd /
rm -rf "${d:?}"
```

```output for=config-go
--- go build
version: dev
extra checks: false
--- go build -tags debug
version: dev
extra checks: true
--- go build -ldflags=-X main.version=1.4.0
version: 1.4.0
extra checks: false
```

**Python.** Python compiles the source when it runs, so the configuration is an option of the interpreter. The extended form `assert expression1, expression2` is the same as `if __debug__: if not expression1: raise AssertionError(expression2)`.[^12] With `-O`, the check is gone, and the function returns its default value.

```bash run id=config-python
# Run one Python program normally and with the option -O.
d=$(mktemp -d)
cd "$d"
cat >avg.py <<'END'
def average(xs):
    assert xs, "average of an empty list"
    return sum(xs) / len(xs) if xs else 0.0

print("__debug__ is", __debug__)
print("average of [2, 4, 9]:", average([2, 4, 9]))
try:
    print("average of []:", average([]))
except AssertionError as e:
    print("AssertionError:", e)
END
echo "--- python3 avg.py"
python3 avg.py
echo "--- python3 -O avg.py"
python3 -O avg.py
cd /
rm -rf "${d:?}"
```

```output for=config-python
--- python3 avg.py
__debug__ is True
average of [2, 4, 9]: 5.0
AssertionError: average of an empty list
--- python3 -O avg.py
__debug__ is False
average of [2, 4, 9]: 5.0
average of []: 0.0
```

## Variations and related ideas

- **More than two configurations.** CMake has `RelWithDebInfo`, an optimized build with debug information, and `MinSizeRel`, a build for a small size.[^1] Cargo has the `opt-level` values `"s"` and `"z"` to optimize for size.[^2]
- **Features.** Cargo features are a way to express conditional compilation and optional dependencies. A package names its features in the `[features]` table, and `--features` turns them on.[^7] A feature selects what the program does; a profile selects how it is compiled.
- **Platform as a configuration.** The target platform is another axis of the build. CMake, Cargo and .NET put the platform and the configuration in different settings; see [[cross-compilation]].
- **Run-time flags.** A feature flag that a program reads at run time changes the behavior without a rebuild. A build-time setting removes the code, so it costs nothing at run time, but it needs a build for each choice.

## Pitfalls

- **Tests only on the debug build.** The release build is a different program. Code that works only because of a check, or a bug that only optimization shows, is found only by a test of the release build.
- **Side effects in assertions.** `assert(x = f())` in C and `assert f()` in Python call the function only when the checks are on. With `NDEBUG` or `-O`, the call is gone.[^5][^11]
- **Assertions that guard the input.** Do not use `assert` to check data from outside the program. The release build removes the check, as the C and Python runs show. Use a normal test and an error.
- **An empty build type in CMake.** A single-configuration build with no `CMAKE_BUILD_TYPE` is not a Debug build. Give the build type every time.[^1]
- **Case in configuration names.** In a string comparison in CMake, `Debug` is not `DEBUG`. Generator expressions such as `$<CONFIG:Debug>` ignore case.[^1]
- **Overflow in release.** In Rust, an overflow that panics in a debug build wraps in a release build with the default profile. To keep the check, set `overflow-checks = true` in the release profile.[^2]

## Cost

Each configuration is a separate build, with its own outputs, so build tools keep them in separate directories, such as `target/debug` and `target/release` in Cargo. A team that ships the release build and tests the debug build pays for two builds. Higher optimization levels may make code faster, but they take longer to compile, and they can make the code harder to debug.[^2] Debug information makes the files larger, as the six `.debug_` sections in the C run show.

## History

C has turned `assert` off with `NDEBUG` for a long time: POSIX derived `assert` from Issue 1 of the System V Interface Definition.[^5] Build tools then named whole sets of settings. CMake has the four standard configurations, and with the generators for Visual Studio and Xcode, you choose the configuration at build time.[^1] Cargo has profiles, and the dev and release profiles have different defaults for the checks.[^2] Python changed the names of optimized bytecode files in version 3.5, so that a file compiled with `-O` gets `.opt-1` before the `.pyc` extension.[^11]

## References
