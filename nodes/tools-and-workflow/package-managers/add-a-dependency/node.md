---
tag: YMNRGZ
kind: task
slug: add-a-dependency
name: Add a dependency
aliases: [add a library, add a package, install a dependency, go get, cargo add, npm install, pip install, dotnet package add, add a Maven dependency]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Add a third-party library to a project with the standard package manager of each language, record it in the manifest, and call it. Local package sources keep the runs offline.
see_also: [package-manager-concept, package-registries, version-constraints, lock-files, dependency-resolution, build-a-project]
sources:
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: go get; go mod tidy; GOPROXY protocol; Module proxies; Environment variables
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-add.html
    title: The Cargo Book, cargo add
    locator: DESCRIPTION; Source options
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-pack
    title: npm Docs (v11), npm pack
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-install
    title: npm Docs (v11), npm install
    locator: Description (npm install <tarball file>); Configuration (save-prod, save-dev)
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/venv.html
    title: The Python Standard Library, venv, Creation of virtual environments
    locator: the introduction
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_install/
    title: pip documentation, pip install
    locator: Package Index Options (--no-index, --find-links)
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/binary-distribution-format/
    title: Python Packaging User Guide, Wheel distribution format
    locator: File name convention; File contents
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/reference/requirements-file-format/
    title: pip documentation, Requirements File Format
    locator: Structure
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-pack
    title: Microsoft Learn, dotnet pack
    locator: Description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/configuring-nuget-behavior
    title: Microsoft Learn, Common NuGet configurations
    locator: How settings are applied (the clear element)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-package-add
    title: Microsoft Learn, dotnet package add
    locator: the note on the noun first form; Description
    accessed: 2026-11-16
  - url: https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html
    title: Maven, Introduction to the Dependency Mechanism
    locator: the introduction; Dependency Scope
    accessed: 2026-11-16
  - url: https://maven.apache.org/guides/introduction/introduction-to-repositories.html
    title: Maven, Introduction to Repositories
    locator: Artifact Repositories
    accessed: 2026-11-16
---

## Task

Add a third-party library to a project with the standard package manager of the language, and call it. The library is `greet` version 1.0.0, with one function `hello(name)` that returns `Hello, <name>!`. The program calls `hello("dependency")` and prints:

```text
Hello, dependency!
```

Each solution must:

- Add the library with the command of the package manager, or with one entry in the manifest, the file that lists the dependencies of the project.
- Show the entry that the tool wrote, so that a reader can see what goes into version control.
- Build and run the program.

Usually the library comes from the public registry of the language. The examples on this site must run without a network, so each script first makes a local package source with `greet` 1.0.0: a module proxy in a directory for Go, a path for Rust, a package file for npm, a wheel for pip, a directory feed for NuGet, and a directory repository for Maven. The commands that add the dependency are the same as for a registry. Each table row gives the command for the public registry too. The outputs come from Linux with Go 1.26, Cargo 1.98, npm 11, Python 3.12, .NET 10 and Maven 3.8.7 on JDK 25.

## Solutions

| Language | Manifest | Command for the public registry | Command in the script |
| --- | --- | --- | --- |
| Go | `go.mod` | `go get example.com/greet@v1.0.0` | the same, with `GOPROXY` set to a local directory |
| Rust | `Cargo.toml` | `cargo add greet@1` | `cargo add --path ../greet` |
| JavaScript and TypeScript | `package.json` | `npm install greet` | `npm install ../greet/greet-1.0.0.tgz` |
| Python | `requirements.txt` or `pyproject.toml` | `pip install "greet>=1,<2"` | the same, with `--no-index --find-links ../wheels` |
| C# | the `.csproj` file | `dotnet package add Greet` | the same, with a local source in `nuget.config` |
| Java and Kotlin | `pom.xml` | an element `dependency` in `pom.xml` | the same, with a local `file://` repository |

**Go.** `go get` "updates module dependencies in the go.mod file for the main module", then builds the packages.[^1] `go mod tidy` "ensures that the go.mod file matches the source code in the module", and it adds the missing entries to `go.sum`.[^1] A module proxy needs no special server: "even a site serving from a fixed file system (including a file:// URL) can be a module proxy".[^1] The directory `proxy` holds the files of the GOPROXY protocol for one version: `list`, `v1.0.0.info`, `v1.0.0.mod` and `v1.0.0.zip`.[^1] `GONOSUMDB` stops the go command from asking the checksum database about a module that is not public, and `GOFLAGS=-modcacherw` makes the module cache writable, so that the script can remove it.[^1]

```bash run timeout=30 id=add-go
d=$(mktemp -d)
cd "$d"
# A local module proxy that serves example.com/greet v1.0.0, so that the run needs no network.
mkdir -p "src/example.com/greet@v1.0.0" proxy/example.com/greet/@v
cat >"src/example.com/greet@v1.0.0/go.mod" <<'END'
module example.com/greet

go 1.26
END
cat >"src/example.com/greet@v1.0.0/greet.go" <<'END'
// Package greet makes greetings.
package greet

// Hello returns a greeting for name.
func Hello(name string) string { return "Hello, " + name + "!" }
END
v=proxy/example.com/greet/@v
echo v1.0.0 >"$v/list"
echo '{"Version":"v1.0.0","Time":"2026-11-01T00:00:00Z"}' >"$v/v1.0.0.info"
cp "src/example.com/greet@v1.0.0/go.mod" "$v/v1.0.0.mod"
(cd src && zip -q -r -D "../$v/v1.0.0.zip" "example.com/greet@v1.0.0")
export GOPROXY="file://$d/proxy" GONOSUMDB=example.com GOMODCACHE="$d/modcache" GOFLAGS=-modcacherw

# The project.
mkdir app
cd app
go mod init example.com/app 2>/dev/null
cat >main.go <<'END'
package main

import (
	"fmt"

	"example.com/greet"
)

func main() {
	fmt.Println(greet.Hello("dependency"))
}
END
go get example.com/greet@v1.0.0 2>&1
go mod tidy
cat go.mod go.sum
go run .
cd /
rm -rf "${d:?}"
```

```output for=add-go
go: downloading example.com/greet v1.0.0
go: added example.com/greet v1.0.0
module example.com/app

go 1.26.5

require example.com/greet v1.0.0
example.com/greet v1.0.0 h1:LJPKwXc5Z5XZ4OQGmo8KRJhlzFqjE0SL70tnhG4dbms=
example.com/greet v1.0.0/go.mod h1:IttiqmSPvbV5lGHLn+lQeyHlqnz/JmBP2UDYKMBt1ZI=
Hello, dependency!
```

**Rust.** `cargo add` adds a dependency to `Cargo.toml`. The source can be `crate@version`, which fetches "from a registry with a version constraint", or `--path`, which fetches "from the specified path".[^2] The entry in `Cargo.toml` keeps the version of the local crate as well as the path. `--offline` keeps both commands away from the network.

```bash run timeout=30 id=add-rust
d=$(mktemp -d)
cd "$d"
# A local library crate, so that the run needs no network.
cargo new --quiet --lib greet
cat >greet/src/lib.rs <<'END'
/// Returns a greeting for `name`.
pub fn hello(name: &str) -> String {
    format!("Hello, {name}!")
}
END

# The project.
cargo new --quiet app
cd app
cargo add --offline --path ../greet 2>&1
cat >src/main.rs <<'END'
fn main() {
    println!("{}", greet::hello("dependency"));
}
END
sed -n '/^\[dependencies\]/,$p' Cargo.toml
cargo run --quiet --offline
cd /
rm -rf "${d:?}"
```

```output for=add-rust
      Adding greet (local) to dependencies
     Locking 1 package to latest Rust 1.98.1 compatible version
[dependencies]
greet = { version = "0.1.0", path = "../greet" }
Hello, dependency!
```

**JavaScript and TypeScript.** `npm pack` copies a package as a tarball named `<name>-<version>.tgz` to the current directory, here `greet-1.0.0.tgz`.[^3] `npm install <tarball file>` installs "a package that is sitting on the filesystem".[^4] npm install "saves any specified packages into dependencies by default"; `-D` saves to `devDependencies` instead.[^4] The variable `npm_config_cache` keeps the cache of npm inside the temporary directory. TypeScript uses the same commands.

```bash run timeout=30 id=add-javascript
d=$(mktemp -d)
cd "$d"
export npm_config_cache="$d/npm-cache"
# A local package file greet-1.0.0.tgz, so that the run needs no network.
mkdir greet
cat >greet/package.json <<'END'
{ "name": "greet", "version": "1.0.0", "type": "module", "exports": "./index.js" }
END
cat >greet/index.js <<'END'
export function hello(name) {
  return `Hello, ${name}!`;
}
END
(cd greet && npm pack --silent)

# The project.
mkdir app
cd app
cat >package.json <<'END'
{ "name": "app", "version": "1.0.0", "private": true, "type": "module" }
END
npm install --silent --no-audit --no-fund ../greet/greet-1.0.0.tgz
npm pkg get dependencies
cat >main.js <<'END'
import { hello } from "greet";
console.log(hello("dependency"));
END
node main.js
cd /
rm -rf "${d:?}"
```

```output for=add-javascript
greet-1.0.0.tgz
{
  "greet": "file:../greet/greet-1.0.0.tgz"
}
Hello, dependency!
```

**Python.** A virtual environment has its own Python packages, and by default it is isolated from the packages of the base Python.[^5] `--no-index` tells pip to ignore the package index, and `--find-links` gives a directory of package files to search.[^6] The package file is a wheel, a ZIP archive with a name such as `greet-1.0.0-py3-none-any.whl` and a directory `greet-1.0.0.dist-info` with the files `METADATA`, `WHEEL` and `RECORD`.[^7] The script writes the wheel with `zipfile`; a real project builds it with a build tool. The file `requirements.txt` records the requirement, one on each line.[^8]

```bash run timeout=30 id=add-python
d=$(mktemp -d)
cd "$d"
export PIP_DISABLE_PIP_VERSION_CHECK=1
# A local wheel greet-1.0.0, so that the run needs no network. A real project builds it with python3 -m build.
mkdir wheels
python3 - <<'END'
import zipfile

files = {
    "greet/__init__.py": 'def hello(name):\n    return f"Hello, {name}!"\n',
    "greet-1.0.0.dist-info/METADATA": "Metadata-Version: 2.1\nName: greet\nVersion: 1.0.0\n",
    "greet-1.0.0.dist-info/WHEEL": "Wheel-Version: 1.0\nGenerator: hand\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
}
files["greet-1.0.0.dist-info/RECORD"] = "".join(f"{name},,\n" for name in [*files, "greet-1.0.0.dist-info/RECORD"])
with zipfile.ZipFile("wheels/greet-1.0.0-py3-none-any.whl", "w") as wheel:
    for name, text in files.items():
        wheel.writestr(name, text)
END

# The project, with its own virtual environment.
mkdir app
cd app
python3 -m venv .venv
.venv/bin/pip install --quiet --no-index --find-links ../wheels "greet>=1,<2"
echo "greet>=1,<2" >requirements.txt
.venv/bin/pip list --format=freeze --exclude pip
cat >main.py <<'END'
from greet import hello

print(hello("dependency"))
END
.venv/bin/python main.py
cd /
rm -rf "${d:?}"
```

```output for=add-python
greet==1.0.0
Hello, dependency!
```

**C#.** `dotnet pack` "builds the project and creates NuGet packages", here into the directory `feed`.[^9] In `nuget.config`, the element `<clear />` makes NuGet ignore the sources from other configuration files, so that only the local source is left.[^10] `dotnet package add` adds a `<PackageReference>` element to the project file, then runs `dotnet restore`.[^11] The noun first form came with .NET 10; with the .NET 9 SDK or earlier, the command is `dotnet add package`.[^11] The variable `NUGET_PACKAGES` keeps the global packages folder inside the temporary directory.

The C# script is shown as text and does not run when this site is built, because the .NET commands in it can take longer than the time limit of a run. The output is from one run on the build machine of this site.

```text
d=$(mktemp -d)
cd "$d"
export NUGET_PACKAGES="$d/packages" DOTNET_NOLOGO=1
# A local package source, the directory feed, with Greet 1.0.0, so that the run needs no network.
mkdir greet
cat >greet/Greet.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <PackageId>Greet</PackageId>
    <Version>1.0.0</Version>
  </PropertyGroup>
</Project>
END
cat >greet/Greeter.cs <<'END'
namespace Greet;

public static class Greeter
{
    public static string Hello(string name) => $"Hello, {name}!";
}
END
dotnet pack greet -o feed -v quiet >/dev/null

# The project. The file nuget.config names the package sources.
mkdir app
cd app
cat >app.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
  </PropertyGroup>
</Project>
END
cat >nuget.config <<'END'
<configuration>
  <packageSources>
    <clear />
    <add key="local" value="../feed" />
  </packageSources>
</configuration>
END
cat >Program.cs <<'END'
System.Console.WriteLine(Greet.Greeter.Hello("dependency"));
END
dotnet package add Greet --version 1.0.0 >/dev/null
grep PackageReference app.csproj
dotnet run -v quiet
cd /
rm -rf "${d:?}"
```

```text
    <PackageReference Include="Greet" Version="1.0.0" />
Hello, dependency!
```

**Java and Kotlin.** Maven has no command that adds a dependency. You add an element `dependency` with the three coordinates `groupId`, `artifactId` and `version` to `pom.xml`, and Maven gets the file and its transitive dependencies when it builds.[^12] A remote repository can use the protocol `file://`, and Maven copies what it downloads into the local repository, a directory on the computer where Maven runs.[^13]

The script is shown as text and does not run when this site is built, because a Maven build can take longer than the time limit of a run. The output is from one run on the build machine of this site. Maven 3.8.7 on JDK 25 also prints warnings about `sun.misc.Unsafe` to standard error; they are left out.

```text
# M2_REPO selects the local repository; the default is ~/.m2/repository.
d=$(mktemp -d)
cd "$d"
# A local Maven repository in the directory repo, with com.example:greet:1.0.0, so that the run needs no network.
mkdir -p greet/src/com/example/greet repo/com/example/greet/1.0.0
cat >greet/src/com/example/greet/Greet.java <<'END'
package com.example.greet;

public final class Greet {
    public static String hello(String name) {
        return "Hello, " + name + "!";
    }
}
END
javac -d greet/out greet/src/com/example/greet/Greet.java
jar --create --file repo/com/example/greet/1.0.0/greet-1.0.0.jar -C greet/out .
cat >repo/com/example/greet/1.0.0/greet-1.0.0.pom <<'END'
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example</groupId>
  <artifactId>greet</artifactId>
  <version>1.0.0</version>
</project>
END

# The project. The dependency goes in the element dependencies of pom.xml.
mkdir -p app/src/main/java/app
cd app
cat >pom.xml <<'END'
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example</groupId>
  <artifactId>app</artifactId>
  <version>1.0.0</version>
  <properties>
    <maven.compiler.release>25</maven.compiler.release>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>
  <repositories>
    <repository>
      <id>local</id>
      <url>file://${project.basedir}/../repo</url>
    </repository>
  </repositories>
  <dependencies>
    <dependency>
      <groupId>com.example</groupId>
      <artifactId>greet</artifactId>
      <version>1.0.0</version>
    </dependency>
  </dependencies>
  <build>
    <plugins>
      <plugin>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.14.0</version>
      </plugin>
      <plugin>
        <artifactId>maven-jar-plugin</artifactId>
        <version>3.4.2</version>
      </plugin>
    </plugins>
  </build>
</project>
END
cat >src/main/java/app/Main.java <<'END'
package app;

import com.example.greet.Greet;

public class Main {
    public static void main(String[] args) {
        System.out.println(Greet.hello("dependency"));
    }
}
END
m2=${M2_REPO:-$HOME/.m2/repository}
mvn -B -q -Dmaven.repo.local="$m2" package
java -cp "target/app-1.0.0.jar:$m2/com/example/greet/1.0.0/greet-1.0.0.jar" app.Main
cd /
rm -rf "${d:?}"
```

```text
Hello, dependency!
```

## Edge cases

- **A dependency only for development.** Test tools do not belong in the dependencies of the program. Use `npm install -D`, `cargo add --dev`, or the Maven scope `test`.[^4][^2][^12]
- **A package that is already there.** `cargo add` updates the existing entry, and `dotnet package add` updates the `<PackageReference>` element to the latest compatible version.[^2][^11]
- **A version range.** Without a version, the tools take the newest release. Give a range when you need one: `cargo add greet@1`, `pip install "greet>=1,<2"`, or `npm install greet@^1.0.0`. See [[version-constraints]].
- **C and C++.** The compilers of these languages come with no package manager. A project uses a system package, a separate package manager, or a copy of the source in the repository.

## Pitfalls

- **The wrong name.** A misspelt name can install a package from an attacker. Copy the name from the registry page of the project. See [[package-registries]].
- **An install without a manifest entry.** A package that you install by hand, but do not record, is missing on the next machine. Use the command that writes the manifest, and commit the manifest and the [[lock-files|lock file]].
- **Packages in the base Python.** The packages of the base Python are shared by each program that uses it. Make a virtual environment for each project, so that only the packages that you install in it are available.[^5]
- **A path dependency in a published package.** A path such as `../greet` works only on your computer. Before you publish, depend on a registry version.

## References
