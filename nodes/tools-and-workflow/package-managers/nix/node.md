---
tag: DV0WNQ
kind: tool
slug: nix
name: Nix
aliases: [Nix package manager, nix-shell, nix-env, Nix store, /nix/store, Nixpkgs, NixOS, derivation, Nix expression, flake, flake.nix, flake.lock, binary cache, purely functional package manager]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Nix is a purely functional package manager. It keeps each package in its own path in the Nix store, named by a hash of how it was built, so versions do not conflict and upgrades can roll back.
role: [package-manager, build]
licence: lgpl-2.1
repo: https://github.com/NixOS/nix
homepage: https://nixos.org/
history:
  year: 2003
  people: [Eelco Dolstra]
  note: Eelco Dolstra wrote Nix. Its first release is dated 15 June 2003, and his PhD thesis on the model, at Utrecht University, came out in 2006.
see_also: [package-manager-concept, reproducible-builds, lock-files, apt-tool, homebrew-pm, other-system-package-managers, dependency-resolution, vendoring]
sources:
  - url: https://nix.dev/manual/nix/stable/introduction
    title: Nix Reference Manual, Introduction
    locator: Multiple versions; Complete dependencies; Multi-user support; Atomic upgrades and rollbacks; Garbage collection; Transparent source/binary deployment; Portability; NixOS
  - url: https://nix.dev/tutorials/first-steps/ad-hoc-shell-environments
    title: nix.dev, Ad hoc shell environments
    locator: Create a shell environment; Towards reproducibility
  - url: https://nix.dev/concepts/flakes
    title: nix.dev, Flakes
    locator: What are flakes?; Should I use flakes in my project?
  - url: https://nix.dev/manual/nix/stable/command-ref/new-cli/nix3-develop
    title: nix develop, Nix Reference Manual
    locator: Synopsis; Examples; Description
  - url: https://nix.dev/tutorials/first-steps/reproducible-scripts
    title: nix.dev, Reproducible interpreted scripts
    locator: The script
  - url: https://en.wikipedia.org/wiki/Nix_(package_manager)
    title: Wikipedia, Nix (package manager)
    locator: the infobox; Nixpkgs; References
  - url: https://raw.githubusercontent.com/NixOS/nix/master/COPYING
    title: Nix, COPYING
    locator: the first lines
---

## What it is

Nix is "a purely functional package manager". The manual explains that Nix treats packages like values in a purely functional programming language: functions with no side effects build the packages, and a package never changes after it is built.[^1]

Nix keeps the packages in the Nix store, usually the directory `/nix/store`. Each package has its own directory there, for example `/nix/store/q06x3jll2yfzckz2bzqak089p43ixkkq-firefox-33.1/`. The first part of the name is a cryptographic hash of the build dependency graph of the package.[^1] So two versions, or two builds with different dependencies, get different paths and do not interfere with each other. The manual says that this prevents "DLL hell".[^1]

Nix runs on Linux and macOS.[^1] It can work as a system package manager like [[apt-tool]] or [[homebrew-pm]], and it can also give one project the exact tools that it needs. NixOS is a Linux distribution that uses Nix for the packages and for the configuration of the system.[^1] The general ideas are on [[package-manager-concept]].

## Install and first use

Nix is not on the build machine of this site, so the commands below are not run here. The command `nix-shell -p` starts a shell with some packages. Nix builds or downloads them, if they are not already in the store:[^2]

```text
$ nix-shell -p cowsay lolcat
[nix-shell:~]$ cowsay Hello, Nix! | lolcat
```

When you leave the shell with `exit`, the programs are not available any more.[^2] A shell can also pin the package set to one version of Nixpkgs, so that it gives the same programs anywhere and at any time:[^2]

```text
$ nix-shell -p git --run "git --version" --pure -I nixpkgs=https://github.com/NixOS/nixpkgs/tarball/2a601aafdc5605a5133a2ca506a34a3a73377247
git version 2.33.1
```

`--run` runs one command in the shell and then exits. `--pure` removes most environment variables of the system, so only the `git` from Nix is in the shell. `-I` gives the source of the package declarations, here a fixed commit of Nixpkgs.[^2]

## Concepts of the tool

**Store paths.** The hash in a store path comes from everything that goes into the build. The script below is a model in Python of this idea. It does not run Nix, and its hash is not the hash that Nix uses. It makes the path of each package from its name, its version and the store paths of its dependencies. Then it changes only the version of `zlib`.

```python run id=nix-store
# A model of Nix store paths. Real Nix hashes the whole build recipe with
# its own scheme; this model hashes the name, the version and the store
# paths of the dependencies, which is enough to show the idea.
import hashlib

def store_path(name, version, deps=()):
    recipe = f"{name}-{version}:" + ",".join(sorted(deps))
    h = hashlib.sha256(recipe.encode()).hexdigest()[:12]
    return f"/nix/store/{h}-{name}-{version}"

def build(zlib_version):
    zlib = store_path("zlib", zlib_version)
    openssl = store_path("openssl", "3.0.0")
    curl = store_path("curl", "8.0.0", [zlib, openssl])
    app = store_path("app", "1.0", [curl])
    return {"zlib": zlib, "openssl": openssl, "curl": curl, "app": app}

old = build("1.3")
new = build("1.3.1")
for name in old:
    same = "same path" if old[name] == new[name] else "new path"
    print(f"{name:8} {old[name]}")
    print(f"{'':8} {new[name]}  ({same})")
```

```output for=nix-store
zlib     /nix/store/b46c982547a4-zlib-1.3
         /nix/store/f837254cfcf6-zlib-1.3.1  (new path)
openssl  /nix/store/802e2902e407-openssl-3.0.0
         /nix/store/802e2902e407-openssl-3.0.0  (same path)
curl     /nix/store/5d90a5632d87-curl-8.0.0
         /nix/store/3a93b7c83278-curl-8.0.0  (new path)
app      /nix/store/854ed0a2349b-app-1.0
         /nix/store/845d698a61b7-app-1.0  (new path)
```

`zlib` gets a different path, and so do `curl` and `app`, which depend on it directly or through `curl`. `openssl` does not depend on `zlib`, so its path stays the same. Both sets of paths can be in the store at the same time.

**Complete dependencies.** In a system such as RPM, the package says what its dependencies are, but nothing checks that the list is complete. A package can work on the machine of its author only because a dependency happens to be installed there.[^1] The manual says that Nix helps to make sure that the dependency specifications are complete.[^1] See also [[reproducible-builds]].

**Nix expressions and Nixpkgs.** A package is built from a Nix expression, in a functional language. The expression describes everything that goes into the build, which is called a derivation: other packages, sources, the build script and environment variables.[^1] The Nix Packages collection, Nixpkgs, is a large set of such expressions.[^1]

**Binary caches.** Nix expressions describe how to build from source. To save time, Nix first asks a binary cache, a web server with pre-built binaries, for the store path. If the cache has it, Nix downloads it; if not, Nix builds from source.[^1]

**Profiles, rollbacks and garbage collection.** Each user has a profile, a set of packages in the store that are on the `PATH` of that user. Users that are not root can install software safely.[^1] An upgrade does not overwrite packages in the store; it adds other paths. So the upgrade is atomic, and `nix-env --rollback` goes back to the old version.[^1] An uninstalled package stays in the store until the garbage collector, `nix-collect-garbage`, deletes the packages that no profile and no running program uses.[^1]

**Flakes.** A flake is a file `flake.nix` that declares inputs and outputs. Nix makes a file `flake.lock` that pins the inputs, as a [[lock-files|lock file]] does.[^3] The nix.dev page calls flakes "an experimental extension format with outstanding issues", and says that their features can usually be got without them.[^3]

**Development shells.** `nix develop` "starts a bash shell that provides an interactive build environment nearly identical to what Nix would use to build installable".[^4] In this shell, the environment variables and shell functions are set, so that a developer can build the package step by step.[^4]

## Common tasks

| Task | Command |
|---|---|
| Start a shell with some packages | `nix-shell -p PACKAGE...`[^2] |
| Run one command with some packages, then exit | `nix-shell -p PACKAGE --run "COMMAND"`[^2] |
| Remove most of the environment of the system from the shell | add `--pure`[^2] |
| Fix the version of Nixpkgs | add `-I nixpkgs=URL`[^2] |
| Install a package in the profile | `nix-env --install --attr nixpkgs.PACKAGE`[^1] |
| Upgrade a package, and go back | `nix-env --upgrade --attr nixpkgs.PACKAGE`, then `nix-env --rollback`[^1] |
| Uninstall a package | `nix-env --uninstall PACKAGE`[^1] |
| Delete the packages that nothing uses | `nix-collect-garbage`[^1] |
| Build a package from a flake | `nix build github:NixOS/nixpkgs#hello`[^3] |
| Start a shell with the build environment of a package | `nix develop nixpkgs#hello`[^4] |
| Run one build phase, for example the build | `nix develop --build`[^4] |

## Configuration

A script can declare its own dependencies. With the shebang line `#!/usr/bin/env nix-shell`, `nix-shell` is the interpreter of the script. Its option `-i` gives the program that runs the rest of the file, `--pure` removes most environment variables, `-p` lists the packages, and `-I` sets the search path for packages.[^5] With a fixed `-I`, the script gets the same packages on any machine that has Nix.[^5]

A project with flakes lists its inputs in `flake.nix`, for example a branch of Nixpkgs:[^3]

```text
{
  description = "My example flake";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };
  ...
}
```

## Alternatives

A system package manager such as [[apt-tool]] installs one version of each package for the whole system. [[homebrew-pm]] keeps each version in a keg, but links one of them into the prefix. Other system package managers are on [[other-system-package-managers]]. To fix the versions of the dependencies of one project in one language, use the package manager of that language and its lock file, or [[vendoring]].

## History and versions

| Date | Event |
|---|---|
| 15 June 2003 | The first release of Nix, by Eelco Dolstra.[^6] |
| November 2004 | Dolstra, de Jonge and Visser described Nix in the paper "Nix: A Safe and Policy-Free System for Software Deployment" at LISA '04.[^6] |
| 2006 | Dolstra's PhD thesis, "The Purely Functional Software Deployment Model", at Utrecht University.[^6] |
| Nix 2.4 | Flakes need Nix 2.4 or later, and they are experimental.[^3] |

Nix is under the GNU Lesser General Public License, version 2.1.[^6][^7]

## References
