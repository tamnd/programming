---
tag: Z1GF2A
kind: task
slug: use-a-third-party-package
name: Use a third-party package
aliases: [use a library, install a package, remove a package, uninstall a package, cargo remove, npm uninstall, pip uninstall, dotnet package remove]
area: tools-and-workflow/package-managers
level: intro
summary: >-
  The full life of one dependency in each language: add a package, import it, see the exact version in the lock file, then remove it again so that no trace is left in the manifest or the lock file.
see_also: [add-a-dependency, pin-dependency-versions, package-manager-concept, lock-files, package-registries, transitive-dependencies]
sources:
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: go get (the @none suffix); go mod tidy; go.sum files
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-add.html
    title: The Cargo Book, cargo add
    locator: DESCRIPTION
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-remove.html
    title: The Cargo Book, cargo remove
    locator: NAME; DESCRIPTION
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-install
    title: npm Docs (v11), npm install
    locator: Description (npm install <tarball file>); Configuration (save-prod)
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/package-lock-json
    title: npm Docs (v11), package-lock.json
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-uninstall
    title: npm Docs (v11), npm uninstall
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/venv.html
    title: The Python Standard Library, venv, Creation of virtual environments
    locator: the introduction
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_freeze/
    title: pip documentation, pip freeze
    locator: Description
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_uninstall/
    title: pip documentation, pip uninstall
    locator: Description; Options (-y, --yes)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-package-add
    title: Microsoft Learn, dotnet package add
    locator: Description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/package-references-in-project-files
    title: Microsoft Learn, NuGet PackageReference in project files
    locator: Locking dependencies (Enabling the lock file)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-package-remove
    title: Microsoft Learn, dotnet package remove
    locator: the note on the noun first form; Description
    accessed: 2026-11-16
  - url: https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html
    title: Maven, Introduction to the Dependency Mechanism
    locator: Transitive Dependencies
    accessed: 2026-11-16
---

## Task

Take one third-party package through its full life in a project. The package is `greet` version 1.0.0, with one function `hello(name)` that returns `Hello, <name>!`. Each solution does three steps:

1. Add the package with the package manager, import it, and run the program. It prints `Hello, package!`.
2. Find the exact version in the lock file, the file that pins the version so that the next install gets the same one. This file goes into version control with the manifest.
3. Remove the import from the code, then remove the package with the package manager. Count the lines that name `greet` in the manifest and in the lock file; each count must be 0. The program then prints `Hello without a package!`.

A run on the build machine of this site has no network, so each script first makes a local package source. With a public registry, only the name in the add command changes. [[add-a-dependency]] shows the add step for each language in more detail, and [[pin-dependency-versions]] shows how to install exactly what the lock file says. The outputs come from Linux with Go 1.26, Cargo 1.98, npm 11, Python 3.12 and .NET 10.

## Solutions

| Language | Add | Lock file | Remove |
| --- | --- | --- | --- |
| Go | `go get example.com/greet@v1.0.0` | `go.sum`, with the version in `go.mod` | remove the import, then `go mod tidy` |
| Rust | `cargo add greet` | `Cargo.lock` | `cargo remove greet` |
| JavaScript and TypeScript | `npm install greet` | `package-lock.json` | `npm uninstall greet` |
| Python | `pip install greet` in a virtual environment | `requirements.txt` from `pip freeze` | `pip uninstall greet`, then `pip freeze` again |
| C# | `dotnet package add Greet` | `packages.lock.json`, when you turn it on | `dotnet package remove Greet` |
| Java and Kotlin | an element `dependency` in `pom.xml` | none | delete the element |

**Go.** The import path in the code names the module, and `go get` adds the requirement to `go.mod`.[^1] `go.mod` records the version, and `go.sum` records the hashes of the module files.[^1] The script cuts each line to 40 characters, because the hashes are long. `go mod tidy` "removes requirements on modules that don’t provide any relevant packages", and it also removes the entries from `go.sum` that are not necessary.[^1] So the correct order is: first remove the import, then run `go mod tidy`. `go get example.com/greet@none` also removes a requirement, but if the code still imports the package, the next build command may add a new requirement.[^1]

```bash run timeout=30 id=use-go
d=$(mktemp -d)
cd "$d"
# A local module proxy with example.com/greet v1.0.0, so that the run needs no network.
v=proxy/example.com/greet/@v m=src/example.com/greet@v1.0.0
mkdir -p "$v" "$m"
printf 'module example.com/greet\n\ngo 1.26\n' >"$m/go.mod"
printf 'package greet\n\nfunc Hello(name string) string { return "Hello, " + name + "!" }\n' >"$m/greet.go"
echo v1.0.0 >"$v/list"
echo '{"Version":"v1.0.0","Time":"2026-11-01T00:00:00Z"}' >"$v/v1.0.0.info"
cp "$m/go.mod" "$v/v1.0.0.mod"
(cd src && zip -q -r -D "../$v/v1.0.0.zip" "example.com/greet@v1.0.0")
export GOPROXY="file://$d/proxy" GONOSUMDB=example.com GOMODCACHE="$d/modcache" GOFLAGS=-modcacherw

# 1. Add the package, import it, and run the program.
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
	fmt.Println(greet.Hello("package"))
}
END
go get example.com/greet@v1.0.0 2>/dev/null
go mod tidy
go run .

# 2. The pin: go.mod has the version, go.sum has the hashes. Both go into version control.
grep -h greet go.mod go.sum | cut -c 1-40

# 3. Remove the import, then let go mod tidy remove the requirement.
cat >main.go <<'END'
package main

import "fmt"

func main() {
	fmt.Println("Hello without a package!")
}
END
go mod tidy
grep -c greet go.mod go.sum
go run .
cd /
rm -rf "${d:?}"
```

```output for=use-go
Hello, package!
require example.com/greet v1.0.0
example.com/greet v1.0.0 h1:iKgrQC8wuZam
example.com/greet v1.0.0/go.mod h1:Ittiq
go.mod:0
go.sum:0
Hello without a package!
```

**Rust.** `cargo add` writes the dependency to `Cargo.toml`, from a registry or, as here, from a path.[^2] The first build writes `Cargo.lock` with the exact version. `cargo remove` does the opposite of `cargo add`: "Remove one or more dependencies from a Cargo.toml manifest."[^3] The output shows that `Cargo.lock` no longer names `greet` either.

```bash run timeout=30 id=use-rust
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

# 1. Add the package, use it, and run the program.
cargo new --quiet app
cd app
cargo add --quiet --offline --path ../greet
cat >src/main.rs <<'END'
fn main() {
    println!("{}", greet::hello("package"));
}
END
cargo run --quiet --offline

# 2. The pin: Cargo.lock has the exact version. It goes into version control.
grep -A 1 'name = "greet"' Cargo.lock

# 3. Remove the use, then remove the package.
cat >src/main.rs <<'END'
fn main() {
    println!("Hello without a package!");
}
END
cargo remove --quiet --offline greet
grep -c greet Cargo.toml Cargo.lock
cargo run --quiet --offline
cd /
rm -rf "${d:?}"
```

```output for=use-rust
Hello, package!
name = "greet"
version = "0.1.0"
Cargo.toml:0
Cargo.lock:0
Hello without a package!
```

**JavaScript and TypeScript.** `npm install` "saves any specified packages into dependencies by default", and it writes `package-lock.json`.[^4][^5] npm uninstall "uninstalls a package, completely removing everything npm installed on its behalf", removes the package from `package.json`, and updates `package-lock.json`.[^6] The last count is the number of entries named `greet` in `node_modules`. TypeScript projects use the same commands.

```bash run timeout=30 id=use-javascript
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
(cd greet && npm pack --silent >/dev/null)

# 1. Add the package, import it, and run the program.
mkdir app
cd app
cat >package.json <<'END'
{ "name": "app", "version": "1.0.0", "private": true, "type": "module" }
END
npm install --silent --no-audit --no-fund ../greet/greet-1.0.0.tgz
cat >main.js <<'END'
import { hello } from "greet";
console.log(hello("package"));
END
node main.js

# 2. The pin: package-lock.json has the exact version and a hash. It goes into version control.
node -p 'require("./package-lock.json").packages["node_modules/greet"].version'

# 3. Remove the import, then remove the package.
echo 'console.log("Hello without a package!");' >main.js
npm uninstall --silent --no-audit --no-fund greet
grep -c greet package.json package-lock.json
ls node_modules 2>&1 | grep -c greet
node main.js
cd /
rm -rf "${d:?}"
```

```output for=use-javascript
Hello, package!
1.0.0
package.json:0
package-lock.json:0
0
Hello without a package!
```

**Python.** A virtual environment is "isolated from the packages in the base environment, so that only those explicitly installed in the virtual environment are available".[^7] `pip freeze` prints the installed packages "in requirements format", so the script writes its output to `requirements.txt` after each change. The first count, 1, shows that `pip uninstall` leaves the line in `requirements.txt`; the second count, 0, comes after `pip freeze` runs again.[^8] `pip uninstall` removes the package, and `--yes` means "Don’t ask for confirmation of uninstall deletions."[^9]

```bash run timeout=30 id=use-python
d=$(mktemp -d)
cd "$d"
export PIP_DISABLE_PIP_VERSION_CHECK=1
# A local wheel greet-1.0.0, so that the run needs no network.
mkdir wheels
python3 - "$d/wheels" <<'END'
import sys
import zipfile

info = "greet-1.0.0.dist-info"
files = {
    "greet/__init__.py": 'def hello(name):\n    return f"Hello, {name}!"\n',
    f"{info}/METADATA": "Metadata-Version: 2.1\nName: greet\nVersion: 1.0.0\n",
    f"{info}/WHEEL": "Wheel-Version: 1.0\nGenerator: hand\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
}
files[f"{info}/RECORD"] = "".join(f"{name},,\n" for name in [*files, f"{info}/RECORD"])
with zipfile.ZipFile(f"{sys.argv[1]}/greet-1.0.0-py3-none-any.whl", "w") as wheel:
    for name, text in files.items():
        wheel.writestr(name, text)
END

# 1. Add the package in a virtual environment, import it, and run the program.
mkdir app
cd app
python3 -m venv .venv
.venv/bin/pip install --quiet --no-index --find-links ../wheels greet
echo 'from greet import hello; print(hello("package"))' >main.py
.venv/bin/python main.py

# 2. The pin: pip freeze writes the exact versions to requirements.txt. It goes into version control.
.venv/bin/pip freeze >requirements.txt
cat requirements.txt

# 3. Remove the import, uninstall the package, and write requirements.txt again.
echo 'print("Hello without a package!")' >main.py
.venv/bin/pip uninstall --quiet --yes greet
grep -c greet requirements.txt
.venv/bin/pip freeze >requirements.txt
grep -c greet requirements.txt
.venv/bin/python main.py
cd /
rm -rf "${d:?}"
```

```output for=use-python
Hello, package!
greet==1.0.0
1
0
Hello without a package!
```

**C#.** `dotnet package add` adds a `<PackageReference>` element to the project file and restores the package.[^10] NuGet writes the lock file `packages.lock.json` only when the project sets the property `RestorePackagesWithLockFile`.[^11] `dotnet package remove` removes "a NuGet package reference from a project".[^12] The script then runs `dotnet restore`, which writes the lock file from the project file. With the .NET 9 SDK or earlier, the commands are `dotnet add package` and `dotnet remove package`.[^12]

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

# 1. Add the package, use it, and run the program. The lock file is opt-in.
mkdir app
cd app
cat >app.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <RestorePackagesWithLockFile>true</RestorePackagesWithLockFile>
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
echo 'System.Console.WriteLine(Greet.Greeter.Hello("package"));' >Program.cs
dotnet package add Greet --version 1.0.0 >/dev/null
dotnet run -v quiet

# 2. The pin: packages.lock.json has the exact version. It goes into version control.
grep '"resolved"' packages.lock.json

# 3. Remove the use, then remove the package.
echo 'System.Console.WriteLine("Hello without a package!");' >Program.cs
dotnet package remove Greet >/dev/null
dotnet restore -v quiet
grep -c Greet app.csproj packages.lock.json
dotnet run -v quiet
cd /
rm -rf "${d:?}"
```

```text
Hello, package!
        "resolved": "1.0.0",
app.csproj:0
packages.lock.json:0
Hello without a package!
```

**Java and Kotlin.** Maven has no add or remove command. You add an element `dependency` to `pom.xml`, and Maven also gets the libraries that it needs, the transitive dependencies.[^13] To remove the package, delete the element and the `import` lines that use it, then build again. See [[add-a-dependency]] for a full Maven example.

## Edge cases

- **A package that other packages also need.** Removing your direct dependency does not remove a package that another dependency still needs. The lock file keeps it as a [[transitive-dependencies|transitive dependency]].
- **Code that still uses the package.** Remove the uses first. In Go, `go mod tidy` keeps the requirement while an import is left; in the other languages, the next build fails with an unknown name.
- **A development dependency.** Test tools go in a separate group: `npm install -D`, `cargo add --dev`, and `cargo remove --dev`.[^4][^2][^3]

## Pitfalls

- **A manual edit of the lock file.** Do not edit the lock file by hand. Use the add and remove commands, and they keep the manifest and the lock file in step.
- **An uninstall that leaves the manifest.** `pip uninstall` leaves `requirements.txt` as it is, as the Python output shows. Write `requirements.txt` again, or remove the line by hand, or the next install brings the package back.
- **A package that is never removed.** Each dependency is code that you must update and trust. Remove the packages that the code no longer uses. See [[package-registries]] for the risks of packages from a public registry.

## References
