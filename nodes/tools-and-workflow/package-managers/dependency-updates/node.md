---
tag: 4KG6YF
kind: practice
slug: dependency-updates
name: Keeping dependencies up to date
aliases: [dependency updates, update dependencies, upgrade dependencies, outdated packages, dependency maintenance, update policy]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Update the dependencies of a project often and in small steps: find the new versions, read what changed, update one step at a time, run the tests, and let a bot open the pull requests.
see_also: [pin-dependency-versions, version-constraints, lock-files, dependency-resolution, transitive-dependencies, package-registries]
sources:
  - url: https://semver.org/
    title: Semantic Versioning 2.0.0
    locator: Summary; Introduction
    accessed: 2026-11-16
  - url: https://keepachangelog.com/en/1.1.0/
    title: Keep a Changelog 1.1.0
    locator: What is a changelog?; Why keep a changelog?
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/dependabot/dependabot-version-updates/about-dependabot-version-updates
    title: GitHub Docs, About Dependabot version updates
    locator: About Dependabot version updates; Updates for packages
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: go get (-u, -u=patch); go list -m (-u); retract directive
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/commands/cargo-update.html
    title: The Cargo Book, cargo update
    locator: DESCRIPTION; OPTIONS (--dry-run)
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-outdated
    title: npm Docs (v11), npm outdated
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-update
    title: npm Docs (v11), npm update
    locator: Description
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_list/
    title: pip documentation, pip list
    locator: Options (-o, --outdated)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-package-list
    title: Microsoft Learn, dotnet package list
    locator: Description; Options (--outdated, --vulnerable)
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/version-specifiers/
    title: Python Packaging User Guide, Version specifiers
    locator: Compatible release
    accessed: 2026-11-16
---

## Statement

Update the dependencies of a project on a regular schedule and in small steps. For each step, find what is new, read what changed, change one thing, and run the tests. A [[lock-files|lock file]] keeps the versions still between updates, so each update is a separate change that a person can review and revert.

## Meaning

A pinned dependency does not change by itself; see [[pin-dependency-versions]]. That is good for a reproducible build, but the pinned versions get older. They miss bug fixes and security fixes, and the gap to the newest version grows. Semantic Versioning describes the danger at both ends. If constraints are too tight, you get "version lock", "the inability to upgrade a package without having to release new versions of every dependent package"; if they are too loose, you get "version promiscuity".[^1]

An update policy answers these questions:

- **How often.** A small update each week is easier to review than a large one each year.
- **How big.** Semantic Versioning says to increment the MAJOR version "when you make incompatible API changes", the MINOR version "when you add functionality in a backward compatible manner", and the PATCH version "when you make backward compatible bug fixes".[^1] So update patch releases first, then minor releases, and do each major release as its own change with its own tests. The number is a promise of the maintainer, not a proof. The tests are the proof.
- **What changed.** A changelog is "a curated, chronologically ordered list of notable changes for each version of a project".[^2] Read the entries between the old and the new version before you merge, in particular for a major release.
- **Who does the work.** A bot can find the new versions and open the pull requests. Dependabot version updates "are automated pull requests that keep your dependencies updated, even when they don’t have any vulnerabilities".[^3] You turn it on with a file `dependabot.yml` in the repository, which gives the location of the manifests.[^3] See [[dependabot]] and [[renovate]].

Each package manager has a command that lists the new versions:

| Tool | List the new versions | Update within the constraints | Update past the constraints |
| --- | --- | --- | --- |
| Go | `go list -m -u all` | `go get -u=patch ./...` | `go get example.com/m@latest` |
| Cargo | `cargo update --dry-run` | `cargo update` | edit the version in `Cargo.toml` |
| npm | `npm outdated` | `npm update` | `npm install name@latest` |
| pip | `pip list --outdated` | `pip install --upgrade "name~=1.0.0"` | a new specifier in `requirements.txt` |
| NuGet | `dotnet package list --outdated` | `dotnet package add Name` | `dotnet package add Name --version 2.0.0` |

`go list -m -u` adds information about available upgrades; the example below shows the newer version in brackets.[^4] `go get -u=patch` upgrades each dependency "to the latest patch version".[^4] `cargo update --dry-run` "Displays what would be updated, but doesn’t actually write the lockfile."[^5] `npm outdated` checks the registry, and its column `wanted` "is the maximum version of the package that satisfies the semver range specified in package.json".[^6] `npm update` updates "respecting the semver constraints of both your package and its dependencies".[^7] `pip list --outdated` lists the outdated packages.[^8] `dotnet package list --outdated` finds newer versions of the packages that the project uses, and `--vulnerable` lists the packages with known vulnerabilities.[^9]

## Examples

Both scripts start with `greet` 1.0.0 and then publish new releases to a local package source. Each script lists the new versions, takes the patch release first, then the newest release that is allowed in a second step. The greeting changes from `Hello` to `Hi` in 1.1.0, so the output shows which version runs.

**Go.** `go get -u=patch` takes v1.0.1, and only then `@latest` takes v1.1.0.[^4]

```bash run timeout=30 id=update-go
d=$(mktemp -d)
cd "$d"
# A local module proxy. publish adds one version of example.com/greet.
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

# The project requires v1.0.0.
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
	fmt.Println(greet.Hello("update"))
}
END
go get example.com/greet@v1.0.0 2>/dev/null
go mod tidy

# Two new releases: a patch and a minor version.
(cd .. && publish v1.0.1 Hello && publish v1.1.0 Hi)
go list -m -u example.com/greet

# Step 1: only the patch release.
go get -u=patch example.com/greet 2>&1
go run .

# Step 2: the newest release, in a separate change.
go get example.com/greet@latest 2>&1
go run .
cd /
rm -rf "${d:?}"
```

```output for=update-go
example.com/greet v1.0.0 [v1.1.0]
go: downloading example.com/greet v1.0.1
go: upgraded example.com/greet v1.0.0 => v1.0.1
Hello, update!
go: downloading example.com/greet v1.1.0
go: upgraded example.com/greet v1.0.1 => v1.1.0
Hi, update!
```

**Python.** `pip list --outdated` shows 2.0.0, the newest release. The compatible release specifier `~=1.0.0` allows only 1.0.x, so the first step takes 1.0.1.[^10] The second step allows each 1.x release and takes 1.1.0. The major release 2.0.0 waits for its own change.

```bash run timeout=30 id=update-python
d=$(mktemp -d)
cd "$d"
export PIP_DISABLE_PIP_VERSION_CHECK=1
# wheel VERSION GREETING writes a wheel of greet into the directory wheels, which stands for the package index.
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
        archive.writestr(name, text)
END
}
mkdir wheels
wheel 1.0.0 Hello

# The project pins greet 1.0.0.
mkdir app
cd app
echo 'from greet import hello; print(hello("update"))' >main.py
python3 -m venv .venv
pip=".venv/bin/pip --quiet"
$pip install --no-index --find-links ../wheels greet==1.0.0

# Three new releases: a patch, a minor and a major version.
wheel 1.0.1 Hello
wheel 1.1.0 Hi
wheel 2.0.0 Hey
.venv/bin/pip list --outdated --no-index --find-links ../wheels

# Step 1: only patch releases of 1.0.
$pip install --upgrade --no-index --find-links ../wheels "greet~=1.0.0"
.venv/bin/pip freeze
.venv/bin/python main.py

# Step 2: the newest release before 2.0.
$pip install --upgrade --no-index --find-links ../wheels "greet>=1,<2"
.venv/bin/pip freeze
.venv/bin/python main.py
cd /
rm -rf "${d:?}"
```

```output for=update-python
Package Version Latest Type
------- ------- ------ -----
greet   1.0.0   2.0.0  wheel
greet==1.0.1
Hello, update!
greet==1.1.0
Hi, update!
```

## Criticism and limits

- **Updates break things.** A minor or patch release can still break a program, because the promise of Semantic Versioning depends on the maintainer. Good tests are the condition for frequent updates.
- **Bots make noise.** A bot that opens one pull request for each package and each release can open more pull requests than a team can review. Group the updates, and set a schedule. See [[renovate]].
- **Transitive dependencies.** Most packages in the lock file are [[transitive-dependencies]]. `npm outdated` shows only the direct dependencies by default, and `npm outdated --all` shows them all; and `dotnet package list` has `--include-transitive`.[^6][^9]
- **A new version is not always better.** A new release can contain a new bug. Go lets an author retract a version, and the go command does not upgrade to a retracted version automatically.[^4]

## References
