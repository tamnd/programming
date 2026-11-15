---
tag: 61580G
kind: task
slug: pin-dependency-versions
name: Pin dependency versions
aliases: [pin versions, lock dependency versions, reproducible install, npm ci, cargo build --locked, dotnet restore --locked-mode, pip --require-hashes, go.sum]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Record the exact version and hash of each dependency in a lock file, commit it, and install exactly that set on each machine, so that a new release does not change the build.
see_also: [lock-files, add-a-dependency, version-constraints, dependency-resolution, transitive-dependencies, package-registries]
sources:
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: go.sum files; go mod verify; Minimal version selection (MVS); Build commands (-mod=readonly); go get
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/faq.html
    title: The Cargo Book, Frequently Asked Questions
    locator: Why have Cargo.lock in version control?
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-build.html
    title: The Cargo Book, cargo build
    locator: Manifest Options (--locked)
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-update.html
    title: The Cargo Book, cargo update
    locator: DESCRIPTION
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/package-lock-json
    title: npm Docs (v11), package-lock.json
    locator: Description; File Format (packages, integrity)
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-ci
    title: npm Docs (v11), npm ci
    locator: Description
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_freeze/
    title: pip documentation, pip freeze
    locator: Description
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_hash/
    title: pip documentation, pip hash
    locator: Description
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/secure-installs/
    title: pip documentation, Secure installs
    locator: Hash-checking Mode; Additional restrictions
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_lock/
    title: pip documentation, pip lock
    locator: Description; Options (--output)
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/projects/sync/
    title: uv documentation, Locking and syncing
    locator: Checking if the lockfile is up-to-date (--locked, --frozen)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/package-references-in-project-files
    title: Microsoft Learn, NuGet PackageReference in project files
    locator: Locking dependencies (Enabling the lock file; Locked mode; Make lock file part of your source repository)
    accessed: 2026-11-16
  - url: https://maven.apache.org/pom.html
    title: Maven, POM Reference
    locator: Dependency Version Requirement Specification
    accessed: 2026-11-16
---

## Task

Make the build of a project use the same version of each dependency on each machine and at each time, until a person decides to update it. The library is `greet` with one function `hello(name)`. The project depends on version 1.0.0, and the program prints:

```text
Hello, pin!
```

Each solution must:

- Record the exact version of the dependency, and if the tool can, a hash of the package file, in a file that goes into version control.
- Install from that record, and show that a newer release of `greet` does not change the build.
- Show what happens when the manifest and the record do not agree, and how an update is done on purpose.

The record is usually a lock file, a file that the package manager writes with the result of [[dependency-resolution]]. The manifest gives the [[version-constraints]] that a person wrote, and the lock file gives the versions that the tool chose. See [[lock-files]] for the idea. As in [[add-a-dependency]], each script first makes a local package source, so that the run needs no network. The outputs come from Linux with Go 1.26, Cargo 1.98, npm 11, Python 3.12 and .NET 10.

## Solutions

| Language | Record in version control | Install exactly from the record | Update on purpose |
| --- | --- | --- | --- |
| Go | `go.mod` and `go.sum` | each build command, because `go.mod` is read only | `go get example.com/greet@v1.1.0` |
| Rust | `Cargo.lock` | `cargo build --locked` | `cargo update` |
| JavaScript and TypeScript | `package-lock.json` | `npm ci` | `npm install greet@1.1.0` |
| Python | `requirements.txt` with `==` and `--hash` | `pip install --require-hashes -r requirements.txt` | a new `requirements.txt` |
| C# | `packages.lock.json` | `dotnet restore --locked-mode` | `dotnet restore` without locked mode |
| Java and Kotlin | `pom.xml` with exact versions | each build | a new version in `pom.xml` |

**Go.** Go has no separate lock file. The `require` directive in `go.mod` gives a minimum version, and minimal version selection uses the highest of the minimum versions that the modules require, not the newest release.[^1] So a new release on the proxy does not change the build. The file `go.sum` "contains cryptographic hashes of the module’s direct and indirect dependencies", and the go command checks each file that it downloads against it.[^1] "Since Go 1.16, the go command acts as if -mod=readonly were set instead: if any changes to go.mod are needed, the go command reports an error and suggests a fix."[^1] `go mod verify` "checks that dependencies of the main module stored in the module cache have not been modified since they were downloaded".[^1] To update, `go get` with the new version changes `go.mod` and `go.sum`.[^1]

```bash run timeout=30 id=pin-go
d=$(mktemp -d)
cd "$d"
# A local module proxy, so that the run needs no network. publish adds one version of example.com/greet.
mkdir -p proxy/example.com/greet/@v
export GOPROXY="file://$d/proxy" GONOSUMDB=example.com GOMODCACHE="$d/modcache" GOFLAGS=-modcacherw
publish() {
	m="src/example.com/greet@$1" v=proxy/example.com/greet/@v
	mkdir -p "$m"
	printf 'module example.com/greet\n\ngo 1.26\n' >"$m/go.mod"
	printf 'package greet\n\nfunc Hello(name string) string { return "%s, " + name + "!" }\n' "$2" >"$m/greet.go"
	echo "$1" >>"$v/list"
	echo "{\"Version\":\"$1\",\"Time\":\"2026-11-01T00:00:00Z\"}" >"$v/$1.info"
	cp "$m/go.mod" "$v/$1.mod"
	(cd src && zip -q -r -D "../$v/$1.zip" "example.com/greet@$1")
}
publish v1.0.0 Hello

# The project requires v1.0.0. go.mod and go.sum go into version control.
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
	fmt.Println(greet.Hello("pin"))
}
END
go get example.com/greet@v1.0.0 2>/dev/null
go mod tidy
grep greet go.mod go.sum

# A new version on the proxy does not change the build.
(cd .. && publish v1.1.0 Hi)
go list -m -versions example.com/greet
go run .
go mod verify

# An update is a separate step that changes go.mod and go.sum.
go get example.com/greet@v1.1.0 2>&1
go run .
cd /
rm -rf "${d:?}"
```

```output for=pin-go
go.mod:require example.com/greet v1.0.0
go.sum:example.com/greet v1.0.0 h1:iKgrQC8wuZamfFcL2Kv8GPP/UoQYI2tQ3TPWfPP/7AQ=
go.sum:example.com/greet v1.0.0/go.mod h1:IttiqmSPvbV5lGHLn+lQeyHlqnz/JmBP2UDYKMBt1ZI=
example.com/greet v1.0.0 v1.1.0
Hello, pin!
all modules verified
go: downloading example.com/greet v1.1.0
go: upgraded example.com/greet v1.0.0 => v1.1.0
Hi, pin!
```

**Rust.** "The purpose of a Cargo.lock lockfile is to describe the state of the world at the time of a successful build."[^2] "While cargo new defaults to tracking Cargo.lock in version control, whether you do is dependent on the needs of your package."[^2] The flag `--locked` "Asserts that the exact same dependencies and versions are used as when the existing Cargo.lock file was originally generated", and Cargo stops with an error if the lock file is missing or must change.[^3] In the script, the library becomes 0.2.0 and the manifest asks for 0.2.0, so the build with `--locked` stops. `cargo update` updates the dependencies in `Cargo.lock` "to the latest version".[^4]

```bash run timeout=30 id=pin-rust
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

# The project. cargo build writes Cargo.lock, and Cargo.lock goes into version control.
cargo new --quiet app
cd app
cargo add --quiet --offline --path ../greet
cat >src/main.rs <<'END'
fn main() {
    println!("{}", greet::hello("pin"));
}
END
cargo build --quiet --offline
grep -A 1 'name = "greet"' Cargo.lock

# With --locked, Cargo uses Cargo.lock as it is, or stops.
cargo run --quiet --offline --locked

# greet becomes 0.2.0, and the manifest asks for it, but Cargo.lock still says 0.1.0.
sed -i 's/^version = "0.1.0"/version = "0.2.0"/' ../greet/Cargo.toml
sed -i 's/greet = { version = "0.1.0"/greet = { version = "0.2.0"/' Cargo.toml
cargo build --quiet --offline --locked 2>&1 | sed "s|$d/||"

# An update is a separate step that rewrites Cargo.lock.
cargo update --quiet --offline
grep -A 1 'name = "greet"' Cargo.lock
cd /
rm -rf "${d:?}"
```

```output for=pin-rust
name = "greet"
version = "0.1.0"
Hello, pin!
error: cannot update the lock file app/Cargo.lock because --locked was passed to prevent this
help: to generate the lock file without accessing the network, remove the --locked flag and use --offline instead.
name = "greet"
version = "0.2.0"
```

**JavaScript and TypeScript.** npm writes `package-lock.json` for each operation that changes `node_modules` or `package.json`, and "This file is intended to be committed into source repositories".[^5] Each entry has the field `integrity`, "A sha512 or sha1 Standard Subresource Integrity string for the artifact that was unpacked in this location".[^5] `npm ci` is for automated environments such as continuous integration.[^6] "If dependencies in the package lock do not match those in package.json , npm ci will exit with an error, instead of updating the package lock."[^6] It never writes to `package.json` or to the lock file: "installs are essentially frozen".[^6] TypeScript projects use the same commands.

```bash run timeout=30 id=pin-javascript
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

# The project. npm install writes package-lock.json, and package-lock.json goes into version control.
mkdir app
cd app
cat >package.json <<'END'
{ "name": "app", "version": "1.0.0", "private": true, "type": "module" }
END
cat >main.js <<'END'
import { hello } from "greet";
console.log(hello("pin"));
END
npm install --silent --no-audit --no-fund ../greet/greet-1.0.0.tgz
node -p 'JSON.stringify(require("./package-lock.json").packages["node_modules/greet"], null, 2)'

# npm ci installs exactly what package-lock.json says.
npm ci --silent --no-audit --no-fund
node main.js

# The manifest asks for greet 1.1.0, but package-lock.json still says 1.0.0.
sed -i 's/"version": "1.0.0"/"version": "1.1.0"/' ../greet/package.json
(cd ../greet && npm pack --silent >/dev/null)
npm pkg set dependencies.greet=file:../greet/greet-1.1.0.tgz
npm ci --no-audit --no-fund 2>&1 | grep -o '`npm ci` can only install packages when .* are in sync'
cd /
rm -rf "${d:?}"
```

```output for=pin-javascript
{
  "version": "1.0.0",
  "resolved": "file:../greet/greet-1.0.0.tgz",
  "integrity": "sha512-953cSqLcVxIeeBhYiRQKpS2La+EIuuNczJ5Ao97SgL2g5h7OVyQPlB6VLlTRt53Zx7c55EdLz2QCizgV7JgNnQ=="
}
Hello, pin!
`npm ci` can only install packages when your package.json and package-lock.json or npm-shrinkwrap.json are in sync
```

**Python.** With pip, a common record is a requirements file with an exact version and a hash on each line. `pip freeze` prints the installed packages "in requirements format", and `pip hash` computes "a hash of a local package archive".[^7][^8] `--require-hashes` turns on hash-checking mode.[^9] In this mode, "Requirements must be pinned (either to a URL, filesystem path or using == )", and a package file with other bytes fails the check.[^9] In the script, a second wheel `greet-1.0.0` with other contents takes the place of the first one, and pip stops.

```bash run timeout=30 id=pin-python
d=$(mktemp -d)
cd "$d"
export PIP_DISABLE_PIP_VERSION_CHECK=1
# wheel VERSION GREETING writes a wheel of greet into the directory wheels, so that the run needs no network.
wheel() {
	python3 - "$1" "$2" "$d/wheels" <<'END'
import sys
import zipfile

version, greeting, folder = sys.argv[1], sys.argv[2], sys.argv[3]
info = f"greet-{version}.dist-info"
files = {
    "greet/__init__.py": f'def hello(name):\n    return f"{greeting}, {{name}}!"\n',
    f"{info}/METADATA": f"Metadata-Version: 2.1\nName: greet\nVersion: {version}\n",
    f"{info}/WHEEL": "Wheel-Version: 1.0\nGenerator: hand\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
}
files[f"{info}/RECORD"] = "".join(f"{name},,\n" for name in [*files, f"{info}/RECORD"])
with zipfile.ZipFile(f"{folder}/greet-{version}-py3-none-any.whl", "w") as archive:
    for name, text in files.items():
        archive.writestr(zipfile.ZipInfo(name, date_time=(2026, 11, 1, 0, 0, 0)), text)
END
}
mkdir wheels
wheel 1.0.0 Hello

# The project. requirements.txt pins the exact version and the hash, and goes into version control.
mkdir app
cd app
echo 'from greet import hello; print(hello("pin"))' >main.py
python3 -m venv .venv
.venv/bin/pip install --quiet --no-index --find-links ../wheels greet
hash=$(.venv/bin/pip hash ../wheels/greet-1.0.0-py3-none-any.whl | tail -n 1)
echo "$(.venv/bin/pip freeze) $hash" >requirements.txt
cat requirements.txt

# A new version does not change an install from requirements.txt.
wheel 1.1.0 Hi
.venv/bin/pip uninstall --quiet --yes greet
.venv/bin/pip install --quiet --no-index --find-links ../wheels --require-hashes -r requirements.txt
.venv/bin/python main.py

# Other bytes under the same name fail the hash check.
wheel 1.0.0 Bye
.venv/bin/pip uninstall --quiet --yes greet
.venv/bin/pip install --quiet --no-index --find-links ../wheels --require-hashes -r requirements.txt 2>&1 | grep -o 'THESE PACKAGES DO NOT MATCH THE HASHES.*FILE'
cd /
rm -rf "${d:?}"
```

```output for=pin-python
greet==1.0.0 --hash=sha256:eed15633ae37e4f7bd0ac397f28a4f7e8c39d450038871d08f392d625d8f2c69
Hello, pin!
THESE PACKAGES DO NOT MATCH THE HASHES FROM THE REQUIREMENTS FILE
```

Two other tools write a real lock file. `pip lock` is marked "EXPERIMENTAL" and writes `pylock.toml` by default.[^10] uv writes `uv.lock`; with `--locked`, "If the lockfile is not up-to-date, uv will raise an error instead of updating the lockfile", and `--frozen` uses the lock file without the check.[^11]

**C#.** The MSBuild property `RestorePackagesWithLockFile` makes NuGet restore write `packages.lock.json` in the project directory.[^12] Microsoft tells you to check in the lock file for an application, but not for a library project on which other projects depend.[^12] "If locked mode is true , restore will either restore the exact packages as listed in the lock file or fail if you updated the defined package dependencies for the project after lock file was created."[^12] `dotnet restore --locked-mode` turns on locked mode, and the MSBuild property `RestoreLockedMode` does the same.[^12] The script leaves out the long line `contentHash` of the lock file, so the line before it ends with a comma.

The C# script is shown as text and does not run when this site is built, because the .NET commands in it can take longer than the time limit of a run. The output is from one run on the build machine of this site.

```text
d=$(mktemp -d)
cd "$d"
export NUGET_PACKAGES="$d/packages" DOTNET_NOLOGO=1
# pack VERSION puts Greet VERSION into the local package source, the directory feed.
mkdir greet
cat >greet/Greeter.cs <<'END'
namespace Greet;

public static class Greeter
{
    public static string Hello(string name) => $"Hello, {name}!";
}
END
pack() {
	cat >greet/Greet.csproj <<END
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <PackageId>Greet</PackageId>
    <Version>$1</Version>
  </PropertyGroup>
</Project>
END
	dotnet pack greet -o feed -v quiet >/dev/null
}
pack 1.0.0

# The project. RestorePackagesWithLockFile makes restore write packages.lock.json, which goes into version control.
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
echo 'System.Console.WriteLine(Greet.Greeter.Hello("pin"));' >Program.cs
dotnet package add Greet --version 1.0.0 >/dev/null
grep -v contentHash packages.lock.json

# Locked mode restores exactly what packages.lock.json says.
dotnet restore --locked-mode -v quiet
dotnet run --no-restore -v quiet

# The project file asks for Greet 1.1.0, but packages.lock.json still says 1.0.0.
(cd .. && pack 1.1.0)
sed -i 's/Version="1.0.0"/Version="1.1.0"/' app.csproj
dotnet restore --locked-mode 2>&1 | grep -o 'error NU1004: .* to \[[^)]*)' | head -n 1
cd /
rm -rf "${d:?}"
```

```text
{
  "version": 1,
  "dependencies": {
    "net10.0": {
      "Greet": {
        "type": "Direct",
        "requested": "[1.0.0, )",
        "resolved": "1.0.0",
      }
    }
  }
}
Hello, pin!
error NU1004: The package reference Greet version has changed from [1.0.0, ) to [1.1.0, )
```

**Java and Kotlin.** Maven takes the version of each dependency from `pom.xml`, so a pin is an exact version there. In the POM, `1.0` is a soft requirement, "Use 1.0 if no other version appears earlier in the dependency tree", and `[1.0]` is a hard requirement, "Use 1.0 and only 1.0".[^13] A version range such as `[1.2,1.3]` lets the version change when new releases come, so do not use ranges in an application that must build the same way each time.[^13] The element below makes 1.0.0 a hard requirement. It is not a run block.

```text
<dependency>
  <groupId>com.example</groupId>
  <artifactId>greet</artifactId>
  <version>[1.0.0]</version>
</dependency>
```

## Edge cases

- **A library, not an application.** Users of a library resolve its dependencies again with their own constraints, so the lock file of the library does not control their builds. NuGet says that a library project should not check in its lock file.[^12] Cargo leaves the choice to the needs of the package.[^2]
- **Transitive dependencies.** A lock file records each package in the tree, not only the direct ones. A pin in the manifest alone does not hold the [[transitive-dependencies]] still. The requirements file in hash-checking mode must list each dependency, because "Hashes are required for all dependencies".[^9]
- **Many platforms.** A package can have one file for each platform, with a different hash for each. The hash list must hold each file that some machine can install.[^9] The lock file of `pip lock` is "only guaranteed to be valid for the current python version and platform".[^10]
- **A version that is removed.** A pin cannot install a version that the registry no longer serves. See [[package-registries]] for yanked and unpublished versions.

## Pitfalls

- **A lock file that is not committed.** Without it, each machine resolves again and can get other versions. Commit the lock file of an application with the manifest.
- **An install command that writes the lock file in CI.** `npm install`, `cargo build` and `dotnet restore` can change the lock file. In continuous integration use `npm ci`, `--locked` or `--locked-mode`, so that a difference stops the build.[^6][^3][^12]
- **A pin that is never updated.** A pinned version does not get security fixes. Update on purpose, read the changes, and run the tests.
- **A merge conflict in the lock file.** Do not edit the lock file by hand. Take one side, then run the package manager again, so that it writes the lock file from the manifest.

## References
