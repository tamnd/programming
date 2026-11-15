---
tag: NEW-go-modules
kind: tool
slug: go-modules
name: Go modules
aliases: [Go modules, go.mod, go.sum, go mod, go get, go mod tidy, GOPROXY, GOSUMDB, proxy.golang.org, sum.golang.org, module proxy, checksum database, go.work]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Go modules are the dependency system of the go command. go.mod lists the minimum versions, minimal version selection picks the build list, and go.sum, a proxy and a checksum database check downloads.
role: [package-manager]
licence: bsd-3-clause
repo: https://github.com/golang/go
homepage: https://go.dev/ref/mod
history:
  year: 2018
  people: [Russ Cox]
  note: Go 1.11 added preliminary support for modules on 24 August 2018. Go 1.13 made proxy.golang.org the default proxy in 2019, and Go 1.16 turned on module mode by default in 2021.
see_also: [package-manager-concept, dependency-resolution, version-constraints, lock-files, transitive-dependencies, go, build-a-project, cross-compile-a-program]
sources:
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: Modules, packages, and versions; Major version suffixes; Minimal version selection (MVS); go get; go mod graph; go mod tidy; go mod vendor; Workspaces; Module proxies; Module cache; go.sum files; Checksum database; Environment variables
  - url: https://go.dev/blog/using-go-modules
    title: Tyler Bui-Palsulich and Eno Compton, Using Go Modules, The Go Blog, 19 March 2019
    locator: Introduction
  - url: https://proxy.golang.org/
    title: Go Module Mirror, Index, and Checksum Database
    locator: Services
  - url: https://github.com/golang/go/blob/master/LICENSE
    title: Go licence
    locator: the whole file (BSD 3-Clause)
  - url: https://go.dev/doc/go1.16
    title: Go 1.16 Release Notes
    locator: Modules
  - url: https://go.dev/doc/modules/gomod-ref
    title: go.mod file reference
    locator: require; At go 1.17 or higher
  - url: https://research.swtch.com/vgo-mvs
    title: Russ Cox, Minimal Version Selection (Go & Versioning, Part 4), 21 February 2018
    locator: the introduction
  - url: https://go.dev/doc/go1.13
    title: Go 1.13 Release Notes
    locator: Modules (GOPROXY, GOSUMDB)
  - url: https://go.dev/doc/modules/managing-dependencies
    title: Managing dependencies
    locator: Discovering available updates
  - url: https://go.dev/doc/go1.11
    title: Go 1.11 Release Notes
    locator: Modules, package versioning, and dependency management
  - url: https://go.dev/doc/devel/release
    title: Go Release History
    locator: the entries go1.11, go1.13, go1.16, go1.26.5, go1.26.11 and go1.27.4
  - url: https://go.dev/blog/module-mirror-launch
    title: Katie Hockman, Module Mirror and Checksum Database Launched, The Go Blog, 29 August 2019
    locator: the introduction
---

## What it is

Go modules are the system of the `go` command that versions and downloads dependencies. The Go Modules Reference says: "A module is a collection of packages that are released, versioned, and distributed together."[^1] A file `go.mod` at the root of the module gives the module path and the requirements on other modules.[^1][^2] The module path is also the import path of the root directory of the module.[^2]

The module system is part of the `go` command, so it has no separate install. It has these parts:

- `go.mod` lists the minimum version of each module that the main module needs.[^1]
- Minimal version selection (MVS) picks the versions for a build from these requirements.[^1]
- `go.sum` holds cryptographic hashes of the dependencies, and the `go` command checks each download against them.[^1]
- A module proxy serves the module files, and a checksum database checks that the files are the same for all users.[^1][^3]

The `go` command is free software under a BSD-style licence.[^4] The general ideas are on the pages [[package-manager-concept]], [[dependency-resolution]], [[version-constraints]], [[lock-files]] and [[transitive-dependencies]].

## Install and first use

Modules come with Go. Go 1.16 turned on module-aware mode by default, "regardless of whether a go.mod file is present".[^5] `go mod init` makes a `go.mod` file, `go get` adds or changes a requirement, and `go mod tidy` makes `go.mod` match the imports in the source code.[^1]

The script below does not use the network. It makes a module proxy in a local directory, in the layout of the proxy protocol, and sets `GOPROXY` to a `file://` URL. The reference says that any server that serves files from a fixed file system, "including a file:// URL", can be a module proxy.[^1] The proxy holds three versions of module `c`, and two modules `b` and `d` that require `c` at v1.2.0 and v1.1.0. The program `app` imports `b` and `d`. `GOSUMDB=off` turns off the checksum database, because the database does not know these modules.[^1] The output is from Go 1.26.5 on the build machine of this site.

```bash run timeout=30 id=go-mvs
# A module proxy in a local directory, in the layout of the GOPROXY
# protocol, so that the go command needs no network.
d=$(mktemp -d)
cd "$d"
export GOPROXY="file://$d/proxy" GOSUMDB=off GOMODCACHE="$d/modcache" GOFLAGS=-modcacherw
go version

# publish MODULE VERSION [DEPENDENCY VERSION]: put one module version in the proxy.
publish() {
  local mod=$1 ver=$2 name=${1##*/} src="$d/src/$1@$2"
  mkdir -p "$src" "proxy/$mod/@v"
  printf 'module %s\n\ngo 1.22\n' "$mod" > "$src/go.mod"
  printf 'package %s\n\nconst Version = "%s"\n' "$name" "$ver" > "$src/$name.go"
  if [ -n "${3:-}" ]; then
    printf '\nrequire %s %s\n' "$3" "$4" >> "$src/go.mod"
    printf 'package %s\n\nimport "%s"\n\nfunc Uses() string { return %s.Version }\n' \
      "$name" "$3" "${3##*/}" > "$src/uses.go"
  fi
  cp "$src/go.mod" "proxy/$mod/@v/$ver.mod"
  (cd "$d/src" && zip -q -r "$d/proxy/$mod/@v/$ver.zip" "$mod@$ver")
  echo "{\"Version\":\"$ver\",\"Time\":\"2026-01-01T00:00:00Z\"}" > "proxy/$mod/@v/$ver.info"
  echo "$ver" >> "proxy/$mod/@v/list"
}
publish example.com/c v1.1.0
publish example.com/c v1.2.0
publish example.com/c v1.3.0
publish example.com/b v1.0.0 example.com/c v1.2.0
publish example.com/d v1.0.0 example.com/c v1.1.0

mkdir app && cd app
go mod init example.com/app 2>/dev/null
cat > main.go <<'GO'
package main

import (
	"fmt"

	"example.com/b"
	"example.com/d"
)

func main() {
	fmt.Println("b", b.Version, "uses c", b.Uses())
	fmt.Println("d", d.Version, "uses c", d.Uses())
}
GO
go get example.com/b@v1.0.0 example.com/d@v1.0.0 2>&1
go mod tidy
echo "--- go.mod"
cat go.mod
echo "--- the build list"
go list -m all
echo "--- the versions of c in the proxy"
go list -m -versions example.com/c
echo "--- go run"
go run .
echo "--- go get example.com/c@latest"
go get example.com/c@latest 2>&1
go run .
go mod tidy
echo "--- the lines of c in go.sum"
grep 'example.com/c' go.sum
cd / && rm -rf "${d:?}"
```

```output for=go-mvs
go version go1.26.5 linux/amd64
go: downloading example.com/b v1.0.0
go: downloading example.com/d v1.0.0
go: downloading example.com/c v1.2.0
go: added example.com/b v1.0.0
go: added example.com/c v1.2.0
go: added example.com/d v1.0.0
--- go.mod
module example.com/app

go 1.26.5

require (
	example.com/b v1.0.0
	example.com/d v1.0.0
)

require example.com/c v1.2.0 // indirect
--- the build list
example.com/app
example.com/b v1.0.0
example.com/c v1.2.0
example.com/d v1.0.0
--- the versions of c in the proxy
example.com/c v1.1.0 v1.2.0 v1.3.0
--- go run
b v1.0.0 uses c v1.2.0
d v1.0.0 uses c v1.2.0
--- go get example.com/c@latest
go: downloading example.com/c v1.3.0
go: upgraded example.com/c v1.2.0 => v1.3.0
b v1.0.0 uses c v1.3.0
d v1.0.0 uses c v1.3.0
--- the lines of c in go.sum
example.com/c v1.3.0 h1:6o8GNJ8xeFO+URo1+FGHOAYs3YV3dqUFiHSG5GS+PWI=
example.com/c v1.3.0/go.mod h1:slkmCTK66XZBf3CTta1WAoTNx6XLaBNrYJ6tHb7WT0g=
```

The proxy had v1.3.0 of `c`, but the build list has v1.2.0: the highest of the two minimum versions that `b` and `d` require. The version changed only when `go get example.com/c@latest` asked for it. After that, `go.mod` requires v1.3.0 and `go.sum` has the hashes of v1.3.0.

## Concepts of the tool

**go.mod.** The `module` line gives the module path, the `go` line gives the version of the Go language, and each `require` line gives a module path and a minimum version.[^1] At `go 1.17` or higher, `go.mod` has a `require` for each module that provides a package that the main module imports, also through other packages. The `go` command puts the indirect requirements in a separate block with the comment `// indirect`.[^6] The output above shows this: `c` is in the second block.

**Minimal version selection.** MVS works on a graph of module versions, in which each edge is a minimum required version. It starts at the main module, goes through the graph and keeps the highest required version of each module. The reference says that these versions "are the minimum versions that satisfy all requirements".[^1] The result is the build list, and `go list -m all` prints it.[^1] Russ Cox described the algorithm in 2018.[^7] The reference says that "Unlike other dependency management systems", Go does not store the build list in a lock file. MVS is deterministic, and the build list does not change when someone releases a version of a dependency, so the `go` command computes it again in each command.[^1] The page [[dependency-resolution]] has a model of MVS and compares it with other resolvers.

**Major versions.** From major version 2, the module path must end with a suffix such as `/v2`, so `example.com/mod` at v1.0.0 becomes `example.com/mod/v2` at v2.0.0.[^1] This follows the import compatibility rule: two packages with the same import path must be compatible.[^1] The reference adds that the suffixes "let multiple major versions of a module coexist in the same build".[^1]

**go.sum.** Each line of `go.sum` has a module path, a version, and a hash. A version that ends in `/go.mod` gives the hash of the `go.mod` file only.[^1] When the `go` command downloads a `.mod` or `.zip` file, it computes the hash and compares it with the hash in `go.sum`.[^1] The output above shows these lines for `c`.

**The proxy and the checksum database.** The default value of `GOPROXY` is `https://proxy.golang.org,direct`. The `go` command asks proxy.golang.org first, and `direct` lets it go to the version control repository of the module.[^1][^8] The checksum database at sum.golang.org is "a global source of go.sum lines".[^1] It lets the `go` command find a proxy or an origin server that serves wrong code. The reference says that it makes untrusted proxies possible, because they "can't serve the wrong code without it going unnoticed".[^1]

**The module cache.** The `go` command keeps the downloaded module files in the module cache, at `$GOPATH/pkg/mod` by default. `GOMODCACHE` changes the location. The cache has no maximum size, and the `go` command does not remove files from it.[^1] The files in the cache are read-only. The flag `-modcacherw` makes them writable, so that `rm -r` can delete the cache; the script above sets it in `GOFLAGS`.[^1]

**Workspaces.** A `go.work` file names several module directories on disk. MVS then uses all of them as main modules.[^1] `go work init` makes the file, and `go work use` adds a module to it.[^1]

## Common tasks

| Task | Command |
|---|---|
| Make a module | `go mod init example.com/app`[^1] |
| Add or change a requirement | `go get example.com/b@v1.0.0`[^1] |
| Update a dependency to the highest released version | `go get example.com/c@latest`[^1] |
| Make `go.mod` and `go.sum` match the imports | `go mod tidy`[^1] |
| Print the build list | `go list -m all`[^1] |
| List the updates that are available | `go list -m -u all`[^9] |
| List the versions of a module | `go list -m -versions example.com/c`[^1] |
| Print the requirement graph | `go mod graph`[^1] |
| Copy the dependencies into `vendor` | `go mod vendor`[^1] |
| Work on several modules together | `go work init ./app ./lib`[^1] |

`go mod graph` prints one edge on each line: a module version and one of its requirements, as `path@version`.[^1] `go mod tidy` adds the missing requirements, removes the requirements that no package uses, and does the same for the lines of `go.sum`.[^1]

## Configuration

The module system has no configuration file other than `go.mod`, `go.sum` and `go.work`. The other settings are environment variables, which `go env -w` can store:[^1]

| Variable | Use |
|---|---|
| `GOPROXY` | the list of module proxies, such as `https://proxy.golang.org,direct` |
| `GOSUMDB` | the checksum database; `off` turns it off |
| `GOPRIVATE` | patterns of module paths that are private; the default for `GONOPROXY` and `GONOSUMDB` |
| `GOMODCACHE` | the directory of the module cache |
| `GOFLAGS` | flags for each `go` command, such as `-modcacherw` or `-mod=vendor` |

A company with private modules sets `GOPRIVATE` to their path prefixes, so that the `go` command does not send these paths to the public proxy or to the checksum database.[^1] When `GOSUMDB` is `off`, the `go` command accepts all modules that it does not know, "at the cost of giving up the security guarantee of verified repeatable downloads".[^1]

The `go.mod` file can also hold `replace` directives, which put other contents in the place of a module version, and `exclude` directives, which stop the use of a module version.[^1] Both apply only in the `go.mod` file of the main module, and the `go` command ignores them in other modules.[^1]

## Alternatives

Before modules, Go code had to be in `GOPATH`. The Go 1.11 release notes describe modules as "an alternative to GOPATH with integrated support for versioning and package distribution", and say that with modules, developers "are no longer confined to working inside GOPATH".[^10] Modules are the default mode of the `go` command.[^5] The page [[cargo]] describes the package manager of Rust, which uses a lock file and picks the highest compatible versions.

## History and versions

Russ Cox wrote a series of posts on versions in Go in 2018. Part 4, "Minimal Version Selection", is dated 21 February 2018.[^7] The release history gives these dates:[^11]

| Version | Release date | Change to modules |
|---|---|---|
| Go 1.11 | 24 August 2018 | preliminary support for modules, called experimental[^10] |
| Go 1.13 | 3 September 2019 | `GOPROXY` defaults to `https://proxy.golang.org,direct`, and `GOSUMDB` checks the hashes[^8] |
| Go 1.16 | 16 February 2021 | module-aware mode is on by default; `go build` and `go test` no longer change `go.mod` and `go.sum`[^5] |

The Go team announced on 29 August 2019 that the module mirror, the index and the checksum database were ready for production, and that the `go` command of Go 1.13 uses them by default.[^12]

On 10 November 2026, the release history lists Go 1.27.4 and Go 1.26.11.[^11] The output on this page is from Go 1.26.5.

## References
