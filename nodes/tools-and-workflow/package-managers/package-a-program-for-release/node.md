---
tag: VK3ZP4
kind: task
slug: package-a-program-for-release
name: Package a program for release
aliases: [release build, build a release artefact, release artifact, distribute a program, go build -trimpath, cargo build --release, npm pack, zipapp, dotnet publish, executable JAR]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Build the file that users download and run: a binary for each platform, an npm package, a Python zip application, a .NET publish folder or an executable JAR, with the version inside.
see_also: [publish-a-package, build-a-project, cross-compilation, reproducible-builds, semantic-versioning, package-registries]
sources:
  - url: https://pkg.go.dev/cmd/go
    title: Go, Command go
    locator: Compile packages and dependencies (-trimpath, -ldflags); Environment variables (GOOS, GOARCH)
    accessed: 2026-11-16
  - url: https://pkg.go.dev/cmd/link
    title: Go, Command link
    locator: Flags (-X, -s)
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/profiles.html
    title: The Cargo Book, Profiles
    locator: release; strip; lto; Custom profiles
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-build.html
    title: The Cargo Book, cargo build
    locator: Compilation Options (--release)
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/environment-variables.html
    title: The Cargo Book, Environment Variables
    locator: Environment variables Cargo sets for crates
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/package-json
    title: npm Docs (v11), package.json
    locator: bin
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-pack
    title: npm Docs (v11), npm pack
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/folders
    title: npm Docs (v11), folders
    locator: Executables
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/zipapp.html
    title: Python documentation, zipapp
    locator: Command-Line Interface; Creating Standalone Applications with zipapp; Caveats
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-publish
    title: Microsoft Learn, dotnet publish
    locator: Description; Options (--self-contained)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/deploying/
    title: Microsoft Learn, .NET application publishing overview
    locator: Publish self-contained; Publish framework-dependent; Publish as single-file
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/jar.html
    title: Java SE 25, The jar Command
    locator: Description; Operation Modifiers (--main-class, --manifest)
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/java.html
    title: Java SE 25, The java Command
    locator: Synopsis (-jar)
    accessed: 2026-11-16
---

## Task

Build the release artefact of a small program `hello`, the file that a user downloads, installs and runs. The program prints its name and its version:

```text
hello 1.0.0
```

Each solution must:

- Build in release mode, with the optimizations of the tool.
- Put the version into the artefact, from one place, so that the program can print it.
- Make one file, or one directory, that a user can copy to another computer.
- Run the artefact as a user does.

A library is different: it goes to a registry as source or as a package, and the projects that use it build it. See [[publish-a-package]]. The outputs come from Linux on x86-64 with Go 1.26, Cargo 1.98, npm 11 on Node.js 24, Python 3.12, .NET 10 and JDK 25.

## Solutions

| Language | Artefact | Command | What the user needs |
| --- | --- | --- | --- |
| Go | one binary for each platform | `go build -trimpath` with `GOOS` and `GOARCH` | nothing |
| Rust | one binary for each platform | `cargo build --release` | nothing |
| JavaScript and TypeScript | a package tarball | `npm pack` | Node.js and npm |
| Python | a zip application, `.pyz` | `python3 -m zipapp` | Python |
| C# | a publish folder | `dotnet publish` | the .NET runtime, or nothing with `--self-contained` |
| Java and Kotlin | an executable JAR | `jar --create --main-class` | a Java runtime |

**Go.** `GOOS` is "The operating system for which to compile code", and `GOARCH` is "The architecture, or processor, for which to compile code".[^1] So one computer builds the binaries for each platform; see [[cross-compilation]]. `-trimpath` removes "all file system paths from the resulting executable".[^1] `-ldflags` passes flags to the linker.[^1] The linker flag `-X importpath.name=value` sets "the value of the string variable in importpath named name to value", and `-s` omits "the symbol table and debug information".[^2]

```bash run timeout=30 id=release-go
d=$(mktemp -d)
cd "$d"
mkdir hello
cd hello
go mod init example.com/hello 2>/dev/null
cat >main.go <<'END'
package main

import "fmt"

// version is set with the linker flag -X when a release is built.
var version = "dev"

func main() {
	fmt.Println("hello", version)
}
END
go run .

# One file for each platform: the version goes in with -X, -trimpath removes the paths of this computer, and -s removes the symbol table and the debug information.
flags="-s -X main.version=1.0.0"
GOOS=linux GOARCH=amd64 go build -trimpath -ldflags="$flags" -o dist/hello-linux-amd64 .
GOOS=windows GOARCH=amd64 go build -trimpath -ldflags="$flags" -o dist/hello-windows-amd64.exe .
GOOS=darwin GOARCH=arm64 go build -trimpath -ldflags="$flags" -o dist/hello-darwin-arm64 .
ls dist
./dist/hello-linux-amd64
tar -czf hello-1.0.0-linux-amd64.tar.gz -C dist hello-linux-amd64
tar -tzf hello-1.0.0-linux-amd64.tar.gz
cd /
rm -rf "${d:?}"
```

```output for=release-go
hello dev
hello-darwin-arm64
hello-linux-amd64
hello-windows-amd64.exe
hello 1.0.0
hello-linux-amd64
```

**Rust.** "The release profile is intended for optimized artifacts used for releases and in production", and `--release` selects it.[^3][^4] The output goes to `target/release`.[^3] The section `[profile.release]` of `Cargo.toml` changes the settings: `strip` "directs rustc to strip either symbols or debuginfo from a binary", and "LTO can produce better optimized code, using whole-program analysis, at the cost of longer linking time".[^3] The macro `env!("CARGO_PKG_VERSION")` gives the version from `Cargo.toml`.[^5] For another platform, `--target` takes a target tuple of the form `<arch><sub>-<vendor>-<sys>-<abi>`; the default is the host architecture.[^4]

```bash run timeout=30 id=release-rust
d=$(mktemp -d)
cd "$d"
cargo new --quiet hello
cd hello
cat >src/main.rs <<'END'
fn main() {
    // Cargo sets CARGO_PKG_VERSION from the version in Cargo.toml.
    println!("hello {}", env!("CARGO_PKG_VERSION"));
}
END
# Settings for the release profile: remove the symbols, and optimize across crates.
cat >>Cargo.toml <<'END'

[profile.release]
strip = "symbols"
lto = true
END
cargo run --quiet --offline

# The optimized build goes to target/release.
cargo build --release --quiet --offline
./target/release/hello
tar -czf hello-0.1.0-x86_64-linux.tar.gz -C target/release hello
tar -tzf hello-0.1.0-x86_64-linux.tar.gz
cd /
rm -rf "${d:?}"
```

```output for=release-rust
hello 0.1.0
hello 0.1.0
hello
```

**JavaScript and TypeScript.** The field `bin` of `package.json` is "a map of command name to local file name"; the file must start with `#!/usr/bin/env node`.[^6] `npm pack` writes the tarball `<name>-<version>.tgz` to the current directory.[^7] A user installs it with `npm install --global`, and npm links the command into `{prefix}/bin` on Unix.[^8] The same file can go to the registry; see [[publish-a-package]]. A TypeScript program puts the compiled JavaScript in the package.

```bash run timeout=30 id=release-javascript
d=$(mktemp -d)
cd "$d"
export npm_config_cache="$d/npm-cache"
mkdir -p hello/bin
cd hello
# The field bin names the command, and the field files names what goes into the package.
cat >package.json <<'END'
{
  "name": "hello",
  "version": "1.0.0",
  "type": "module",
  "bin": { "hello": "bin/hello.js" },
  "files": ["bin"]
}
END
cat >bin/hello.js <<'END'
#!/usr/bin/env node
import { readFileSync } from "node:fs";

const pkg = JSON.parse(readFileSync(new URL("../package.json", import.meta.url)));
console.log("hello", pkg.version);
END
chmod +x bin/hello.js

# The release file is a package tarball.
npm pack --silent
tar -tzf hello-1.0.0.tgz | sort

# Install it as a user does, into the directory prefix, and run the command.
npm install --global --prefix "$d/prefix" --silent ./hello-1.0.0.tgz
"$d/prefix/bin/hello"
cd /
rm -rf "${d:?}"
```

```output for=release-javascript
hello-1.0.0.tgz
package/bin/hello.js
package/package.json
hello 1.0.0
```

**Python.** `zipapp` makes "zip files containing Python code, which can be executed directly by the Python interpreter".[^9] `--main` writes a `__main__.py` that calls the function, and `--python` adds a `#!` line and, on POSIX, makes the archive executable.[^9] For a program with dependencies, install them into the directory first with `python -m pip install -r requirements.txt --target myapp`.[^9] A library, or a program that users install with pip, is a wheel instead; see [[publish-a-package]].

```bash run timeout=30 id=release-python
d=$(mktemp -d)
cd "$d"
mkdir -p hello/hello
cat >hello/hello/__init__.py <<'END'
__version__ = "1.0.0"
END
cat >hello/hello/cli.py <<'END'
from hello import __version__


def main():
    print("hello", __version__)
END

# One executable file: zipapp writes __main__.py, which calls hello.cli:main, and a #! line.
mkdir dist
python3 -m zipapp hello --main hello.cli:main --python "/usr/bin/env python3" --output dist/hello.pyz
python3 -c 'import sys, zipfile; print(*sorted(zipfile.ZipFile(sys.argv[1]).namelist()), sep="\n")' dist/hello.pyz
./dist/hello.pyz
cd /
rm -rf "${d:?}"
```

```output for=release-python
__main__.py
hello/
hello/__init__.py
hello/cli.py
hello 1.0.0
```

**C#.** `dotnet publish` "compiles the application, reads through its dependencies specified in the project file, and publishes the resulting set of files to a directory".[^10] The folder has the code in a `.dll` file, a `.deps.json` file and a `.runtimeconfig.json` file.[^10] This is a framework-dependent release: "The environment that runs the app must have a version of the .NET runtime installed that the app can use."[^11] `--self-contained` publishes "the .NET runtime with your application", and a single-file release bundles "all application-dependent files" into one binary.[^10][^11] The script sets the version with the element `Version` of the project file, and the program reads it from the assembly.

The C# script is shown as text and does not run when this site is built, because the .NET commands in it can take longer than the time limit of a run. The output is from one run on the build machine of this site.

```text
d=$(mktemp -d)
cd "$d"
export NUGET_PACKAGES="$d/packages" DOTNET_NOLOGO=1
mkdir hello
cat >hello/hello.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Version>1.0.0</Version>
  </PropertyGroup>
</Project>
END
cat >hello/Program.cs <<'END'
var version = typeof(Program).Assembly.GetName().Version!;
Console.WriteLine($"hello {version.ToString(3)}");
END

# A framework-dependent release: the computer that runs it needs the .NET runtime.
dotnet publish hello --configuration Release --output dist -v quiet >/dev/null
ls dist
dotnet dist/hello.dll
cd /
rm -rf "${d:?}"
```

```text
hello
hello.deps.json
hello.dll
hello.pdb
hello.runtimeconfig.json
hello 1.0.0
```

**Java and Kotlin.** The jar command is "a general-purpose archiving and compression tool".[^12] `--main-class` gives the entry point, and `--manifest` adds lines from a file to the manifest, here the version.[^12] `java -jar` "Executes a program encapsulated in a JAR file"; the manifest must have a line `Main-Class: classname`.[^13] A program with dependencies needs them on the class path, or in one JAR with all the classes.

```bash run timeout=30 id=release-java
d=$(mktemp -d)
cd "$d"
mkdir -p src/app
cat >src/app/Main.java <<'END'
package app;

public class Main {
    public static void main(String[] args) {
        // The version comes from the manifest of the JAR file.
        String version = Main.class.getPackage().getImplementationVersion();
        System.out.println("hello " + version);
    }
}
END
printf 'Implementation-Version: 1.0.0\n' >manifest.txt
javac -d classes src/app/Main.java

# The JAR file with the entry point Main-Class and the version in its manifest.
mkdir dist
jar --create --file dist/hello-1.0.0.jar --main-class app.Main --manifest manifest.txt -C classes .
jar --list --file dist/hello-1.0.0.jar
java -jar dist/hello-1.0.0.jar
cd /
rm -rf "${d:?}"
```

```output for=release-java
META-INF/
META-INF/MANIFEST.MF
app/
app/Main.class
hello 1.0.0
```

## Edge cases

- **One build for each platform.** A Go or Rust binary runs only on the operating system and processor that it was built for. Name each file after its platform, as the Go script does.
- **Native extensions in Python.** "If your application depends on a package that includes a C extension, that package cannot be run from a zip file".[^9] Ship such a package next to the archive, or use a wheel.
- **A runtime on the target.** The npm, zipapp, framework-dependent .NET and JAR releases need a runtime of the right version. A self-contained .NET release needs none.[^11]

## Pitfalls

- **A debug build.** `cargo build` without `--release` uses the dev profile, which has `opt-level = 0`. Check the profile before you ship.[^3]
- **Paths of the build machine.** Without `-trimpath`, a Go binary holds the file system paths of the computer that built it.[^1] See [[reproducible-builds]].
- **Two versions.** A version that is typed in two places drifts. Set it once, in the manifest or in the build command, and read it from there, as each script does.
- **Too much in the package.** A package can take test data, secrets or build output by mistake. List the files before the release, as the npm and JAR scripts do.

## References
