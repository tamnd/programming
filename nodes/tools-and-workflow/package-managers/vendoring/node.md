---
tag: MT53MG
kind: practice
slug: vendoring
name: Vendoring dependencies
aliases: [vendoring, vendor directory, vendored dependencies, go mod vendor, cargo vendor, offline dependencies]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  Keep copies of the dependencies in the repository, or next to it, so that a build needs no registry and no network. Go, Cargo and pip each have a command that makes the copy.
see_also: [pin-dependency-versions, lock-files, package-registries, hermetic-builds, reproducible-builds, transitive-dependencies]
sources:
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: Vendoring; go mod vendor; Build commands (-mod=vendor)
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-vendor.html
    title: The Cargo Book, cargo vendor
    locator: DESCRIPTION
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/source-replacement.html
    title: The Cargo Book, Source Replacement
    locator: the introduction; Directory Sources
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_download/
    title: pip documentation, pip download
    locator: Description; Overview
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_install/
    title: pip documentation, pip install
    locator: Package Index Options (--no-index, --find-links)
    accessed: 2026-11-16
---

## Statement

To vendor a dependency is to keep a copy of its source, or of its package file, under the control of the project, and to build from that copy instead of from the registry. The Go documentation gives two reasons: interoperation with older versions of Go, and "to ensure that all files used for a build are stored in a single file tree".[^1] A build from vendored copies needs no registry, no mirror and no network.

## Meaning

A [[lock-files|lock file]] records which version to use, but the build still downloads the files. A vendor directory holds the files too. The project then controls three things:

- **Availability.** A registry can be down, and a version can be removed. The vendored copy stays. See [[package-registries]].
- **Review.** Each change to a dependency appears as a change to files in the repository, so a reviewer can read it in the same pull request.
- **Offline builds.** With `-mod=vendor`, "the go command will not use the network or the module cache".[^1]

Each tool keeps the copy in step with the lock file in its own way:

| Tool | Command | Copy | How the build finds it |
| --- | --- | --- | --- |
| Go | `go mod vendor` | the packages, in `vendor/`, with the list `vendor/modules.txt` | automatically, when `go.mod` says `go 1.14` or higher |
| Cargo | `cargo vendor` | the unpacked crates, in `vendor/` | the configuration that `cargo vendor` prints, in `.cargo/config.toml` |
| pip | `pip download --dest vendor` | the package files, such as wheels | `pip install --no-index --find-links vendor` |

**Go.** `go mod vendor` "constructs a directory named vendor in the main module’s root directory containing copies of all packages needed to build and test packages in the main module".[^1] It also writes `vendor/modules.txt`, the list of the packages and the module versions that they came from.[^1] If `go.mod` changes after that, the go command reports an error, and you must run `go mod vendor` again.[^1]

**Cargo.** `cargo vendor` "will vendor all crates.io and git dependencies for a project into the specified directory".[^2] It prints the configuration that tells Cargo to use the copy, and you add it to `.cargo/config.toml`.[^2] The configuration uses source replacement: the source of the crates is replaced with a directory source, which contains "the unpacked version of *.crate files", so that it is suitable "to check everything into source control".[^3] It copies crates.io and Git dependencies; a path dependency is already a local directory.[^2]

**pip.** `pip download` "does the same resolution and downloading as pip install , but instead of installing the dependencies, it collects the downloaded distributions into the directory provided".[^4] The directory "can later be passed as the value to pip install --find-links to facilitate offline or locked down package installation".[^4] `--no-index` stops pip from using the package index at all.[^5]

## Examples

Each script makes a local source for the package `greet`, vendors it, deletes the source, and then builds or installs from the copy only.

**Go.** The proxy and the module cache are deleted before the last build, and `GOPROXY=off` forbids each download.[^1] The build uses `vendor/` automatically, because the `go` line in `go.mod` is 1.14 or higher.[^1]

```bash run timeout=30 id=vendor-go
d=$(mktemp -d)
cd "$d"
# A local module proxy with example.com/greet v1.0.0. It stands for the public proxy.
v=proxy/example.com/greet/@v m=src/example.com/greet@v1.0.0
mkdir -p "$v" "$m"
printf 'module example.com/greet\n\ngo 1.26\n' >"$m/go.mod"
printf 'package greet\n\nfunc Hello(name string) string { return "Hello, " + name + "!" }\n' >"$m/greet.go"
echo v1.0.0 >"$v/list"
echo '{"Version":"v1.0.0","Time":"2026-11-01T00:00:00Z"}' >"$v/v1.0.0.info"
cp "$m/go.mod" "$v/v1.0.0.mod"
(cd src && zip -q -r -D "../$v/v1.0.0.zip" "example.com/greet@v1.0.0")
export GOPROXY="file://$d/proxy" GONOSUMDB=example.com GOMODCACHE="$d/modcache" GOFLAGS=-modcacherw

# The project depends on greet.
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
	fmt.Println(greet.Hello("vendor"))
}
END
go get example.com/greet@v1.0.0 2>/dev/null
go mod tidy

# Copy the dependencies into the directory vendor. It goes into version control.
go mod vendor
find vendor -type f | sort
cat vendor/modules.txt

# Without the proxy and the module cache, the build uses vendor.
rm -rf "${GOMODCACHE:?}" "$d/proxy"
GOPROXY=off go run .
cd /
rm -rf "${d:?}"
```

```output for=vendor-go
vendor/example.com/greet/greet.go
vendor/modules.txt
# example.com/greet v1.0.0
## explicit; go 1.26
example.com/greet
Hello, vendor!
```

**Rust.** The dependency comes from a local Git repository, because `cargo vendor` copies Git and registry dependencies, and the script cannot reach crates.io.[^2] The script writes the configuration that `cargo vendor` prints to `.cargo/config.toml`. After the repository is deleted, `cargo run --offline` still builds.

```bash run timeout=30 id=vendor-rust
d=$(mktemp -d)
cd "$d"
# A local Git repository with the crate greet. It stands for a remote source.
cargo new --quiet --lib greet
cat >greet/src/lib.rs <<'END'
/// Returns a greeting for `name`.
pub fn hello(name: &str) -> String {
    format!("Hello, {name}!")
}
END
git -C greet add . >/dev/null
git -C greet -c user.name=ci -c user.email=ci commit --quiet -m greet

# The project depends on greet from Git.
cargo new --quiet app
cd app
cargo add --quiet --git "file://$d/greet" greet
cat >src/main.rs <<'END'
fn main() {
    println!("{}", greet::hello("vendor"));
}
END

# Copy the sources into the directory vendor, and save the configuration that cargo vendor prints. DIR stands for the temporary directory.
mkdir .cargo
cargo vendor 2>/dev/null >.cargo/config.toml
sed "s|$d|DIR|" .cargo/config.toml
ls vendor

# Without the Git repository, the build uses vendor.
rm -rf "${d:?}/greet"
cargo run --quiet --offline
cd /
rm -rf "${d:?}"
```

```output for=vendor-rust
[source."git+file://DIR/greet"]
git = "file://DIR/greet"
replace-with = "vendored-sources"

[source.vendored-sources]
directory = "vendor"
greet
Hello, vendor!
```

**Python.** The directory `index` stands for the package index. After `pip download`, the script deletes it and installs from `vendor` only.[^4][^5]

```bash run timeout=30 id=vendor-python
d=$(mktemp -d)
cd "$d"
export PIP_DISABLE_PIP_VERSION_CHECK=1
# A local wheel greet-1.0.0 in the directory index. It stands for the package index.
mkdir index
python3 - "$d/index" <<'END'
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

# The project. pip download copies the package files into the directory vendor.
mkdir app
cd app
echo 'greet==1.0.0' >requirements.txt
echo 'from greet import hello; print(hello("vendor"))' >main.py
python3 -m pip download --quiet --no-index --find-links ../index --dest vendor -r requirements.txt
ls vendor

# Without the index, the install uses only vendor.
rm -rf "${d:?}/index"
python3 -m venv .venv
.venv/bin/pip install --quiet --no-index --find-links vendor -r requirements.txt
.venv/bin/python main.py
cd /
rm -rf "${d:?}"
```

```output for=vendor-python
greet-1.0.0-py3-none-any.whl
Hello, vendor!
```

## Criticism and limits

- **The repository grows.** A vendor directory can hold more code than the project. Each update of a dependency changes many files, and the diff is hard to review.
- **The copy must not be edited.** "Cargo treats vendored sources as read-only as it does to registry and git sources."[^2] To change a dependency, use `[patch]` or a path dependency to your own copy, so that Cargo knows that the crate is no longer read-only.[^2]
- **The copy can be stale.** Vendoring does not update anything. A copy that nobody updates misses security fixes. Run the vendor command again after each change of the lock file; Go checks that `vendor/modules.txt` agrees with `go.mod`, and reports an error if not.[^1]
- **Package files depend on the platform.** A wheel can be for one platform only. `pip download` has the options `--platform`, `--python-version`, `--implementation` and `--abi` to get the files for another platform.[^4]
- **A cache or a mirror is often enough.** A private registry or a mirror gives most of the availability, without the copy in the repository. See [[package-registries]].

## References
