---
tag: 64KY7E
kind: tool
slug: cargo
name: Cargo
aliases: [cargo, Cargo.toml, Cargo.lock, crates.io, crate, Rust package manager, cargo add, cargo build, Cargo workspaces, Cargo features]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Cargo is the package manager and build tool of Rust. It reads Cargo.toml, gets the crates from crates.io, records the versions in Cargo.lock, and calls the compiler.
role: [build, package-manager]
licence: mit-or-apache-2.0
repo: https://github.com/rust-lang/cargo
homepage: https://doc.rust-lang.org/cargo/
history:
  year: 2014
  people: [Carl Lerche, Yehuda Katz]
  note: The crates.io registry went online on 20 November 2014, and Rust 1.0 came out with Cargo on 15 May 2015.
see_also: [package-manager-concept, dependency-resolution, version-constraints, lock-files, transitive-dependencies, rust, cargo-test, build-a-project]
sources:
  - url: https://doc.rust-lang.org/cargo/index.html
    title: The Cargo Book, introduction
    locator: the first paragraph
  - url: https://blog.rust-lang.org/2015/05/15/Rust-1.0/
    title: The Rust Core Team, Announcing Rust 1.0, Rust Blog, 15 May 2015
    locator: Cargo and crates.io
  - url: https://doc.rust-lang.org/cargo/guide/why-cargo-exists.html
    title: The Cargo Book, Why Cargo Exists
    locator: "Enter: Cargo"
  - url: https://github.com/rust-lang/cargo/blob/master/README.md
    title: Cargo README
    locator: License
  - url: https://doc.rust-lang.org/cargo/getting-started/installation.html
    title: The Cargo Book, Installation
    locator: Install Rust and Cargo
  - url: https://doc.rust-lang.org/cargo/commands/cargo-add.html
    title: The Cargo Book, cargo-add(1)
    locator: DESCRIPTION
  - url: https://doc.rust-lang.org/cargo/commands/cargo-tree.html
    title: The Cargo Book, cargo-tree(1)
    locator: DESCRIPTION; the option --duplicates
  - url: https://doc.rust-lang.org/cargo/reference/features.html
    title: The Cargo Book, Features
    locator: the introduction; The default feature; Optional dependencies; Feature unification
  - url: https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html
    title: The Cargo Book, Specifying Dependencies
    locator: Specifying dependencies from crates.io; Default requirements; Specifying path dependencies; Multiple locations
  - url: https://doc.rust-lang.org/cargo/reference/workspaces.html
    title: The Cargo Book, Workspaces
    locator: the introduction
  - url: https://doc.rust-lang.org/cargo/guide/cargo-toml-vs-cargo-lock.html
    title: The Cargo Book, Cargo.toml vs Cargo.lock
    locator: the summary at the top
  - url: https://doc.rust-lang.org/cargo/reference/manifest.html
    title: The Cargo Book, The Manifest Format
    locator: the list of sections
  - url: https://doc.rust-lang.org/cargo/reference/resolver.html
    title: The Cargo Book, Dependency Resolution
    locator: the introduction; Version numbers; SemVer compatibility; Resolver versions
  - url: https://doc.rust-lang.org/cargo/commands/cargo.html
    title: The Cargo Book, cargo(1)
    locator: COMMANDS
  - url: https://doc.rust-lang.org/cargo/reference/profiles.html
    title: The Cargo Book, Profiles
    locator: the introduction; Default profiles
  - url: https://doc.rust-lang.org/cargo/commands/cargo-update.html
    title: The Cargo Book, cargo-update(1)
    locator: DESCRIPTION
  - url: https://doc.rust-lang.org/cargo/reference/publishing.html
    title: The Cargo Book, Publishing on crates.io
    locator: the introduction; cargo yank
  - url: https://doc.rust-lang.org/cargo/guide/cargo-home.html
    title: The Cargo Book, Cargo Home
    locator: the introduction
  - url: https://blog.rust-lang.org/2014/11/20/Cargo/
    title: "Alex Crichton, Cargo: Rust's community crate host, Rust Blog, 20 November 2014"
    locator: the introduction; What is Cargo?
  - url: https://doc.rust-lang.org/cargo/CHANGELOG.html
    title: The Cargo Book, Changelog
    locator: the headings Cargo 1.98 and Cargo 1.99
---

## What it is

Cargo is the package manager of Rust. The Cargo Book says that Cargo "downloads your Rust package's dependencies, compiles your packages, makes distributable packages, and uploads them to crates.io", the package registry of the Rust community.[^1] Cargo is also a build tool. The Rust 1.0 announcement calls it "the Rust package manager and build system".[^2] A programmer runs a command such as `cargo build`, and Cargo calls the compiler `rustc` with the correct options.[^3]

The book gives four jobs of Cargo:[^3]

- It uses two metadata files with information about the package: `Cargo.toml` and `Cargo.lock`.
- It fetches and builds the dependencies of the package.
- It calls `rustc` or another build tool with the correct parameters.
- It sets conventions that make work with Rust packages easier.

A package that Cargo builds is a crate, and the public registry of crates is crates.io.[^1] This page is about Cargo as a tool. The general ideas are on other pages: [[package-manager-concept]], [[dependency-resolution]], [[version-constraints]], [[lock-files]] and [[transitive-dependencies]].

Cargo is free software. Its README says that it is "primarily distributed under the terms of both the MIT license and the Apache License (Version 2.0)".[^4]

## Install and first use

Cargo comes with Rust. The book says that the easiest way to get Cargo is to install the current stable release of Rust with `rustup`, which also installs `cargo`.[^5]

The script below makes a workspace with two packages. The library `greet` has an optional feature, `shout`. The program `app` depends on `greet` through a path, so the run needs no network and nothing comes from crates.io. `cargo add` writes the dependency into `app/Cargo.toml`, `cargo tree` shows the dependency graph, and `cargo run` builds and runs the program, first without the feature and then with it.[^6][^7][^8] The output is from Cargo 1.98.1 on the build machine of this site.

```bash run timeout=30 id=cargo-workspace
# A workspace with two packages: a library with an optional feature,
# and a program that depends on it by path. Nothing comes from crates.io.
d=$(mktemp -d)
cd "$d"
cargo --version
cargo new -q --vcs none --lib greet
cargo new -q --vcs none app
cat > Cargo.toml <<'TOML'
[workspace]
members = ["app", "greet"]
resolver = "3"
TOML
cat > greet/src/lib.rs <<'RS'
pub fn greet(name: &str) -> String {
    let text = format!("Hello, {name}!");
    if cfg!(feature = "shout") {
        text.to_uppercase()
    } else {
        text
    }
}
RS
cat >> greet/Cargo.toml <<'TOML'

[features]
shout = []
TOML
cargo add -q --offline -p app --path greet
cat > app/src/main.rs <<'RS'
fn main() {
    println!("{}", greet::greet("Cargo"));
}
RS
echo "--- app/Cargo.toml"
cat app/Cargo.toml
echo "--- cargo tree"
cargo tree -p app | sed "s|$d|.|"
echo "--- cargo run"
cargo run -q -p app
cargo run -q -p app --features greet/shout
echo "--- the files at the root"
ls
echo "--- Cargo.lock"
cat Cargo.lock
cd / && rm -rf "${d:?}"
```

```output for=cargo-workspace
cargo 1.98.1 (797e8a9bc 2026-08-05)
--- app/Cargo.toml
[package]
name = "app"
version = "0.1.0"
edition = "2024"

[dependencies]
greet = { version = "0.1.0", path = "../greet" }
--- cargo tree
app v0.1.0 (./app)
└── greet v0.1.0 (./greet)
--- cargo run
Hello, Cargo!
HELLO, CARGO!
--- the files at the root
Cargo.lock
Cargo.toml
app
greet
target
--- Cargo.lock
# This file is automatically @generated by Cargo.
# It is not intended for manual editing.
version = 4

[[package]]
name = "app"
version = "0.1.0"
dependencies = [
 "greet",
]

[[package]]
name = "greet"
version = "0.1.0"
```

`cargo add --path` wrote both a path and a version. The book describes this in its section "Multiple locations": Cargo uses the path in local builds, and it uses the version from the registry when the package is published.[^9] The two packages share one `Cargo.lock` and one `target` directory at the root of the workspace.[^10]

## Concepts of the tool

**The manifest and the lock file.** The book gives a short summary of the two files. `Cargo.toml` describes the dependencies "in a broad sense, and is written by you". `Cargo.lock` "contains exact information about your dependencies", and Cargo maintains it.[^11] The book also says: "When in doubt, check Cargo.lock into the version control system (e.g. Git)."[^11] The manifest has sections such as `[package]`, `[dependencies]`, `[dev-dependencies]` for tests and examples, `[build-dependencies]` for build scripts, `[features]` and `[profile.*]`.[^12]

**Version requirements.** In `Cargo.toml`, a dependency such as `time = "0.1.12"` is a version requirement, not one exact version. With no operator, it is a default requirement, which is the same as a caret requirement: `1.2.3` means `>=1.2.3, <2.0.0`, and `0.2.3` means `>=0.2.3, <0.3.0`.[^9] Cargo treats two versions as compatible "if their left-most non-zero major/minor/patch component is the same".[^9] The page [[version-constraints]] compares this rule with the rules of other tools.

**Resolution.** The resolver picks one version for each requirement and stores the result in `Cargo.lock`.[^13] In general, Cargo prefers the highest version that is available. It gives the highest priority to the versions that are already in the lock file.[^13] Cargo unifies requirements that are compatible: if package A wants `bitflags = "1.0"` and package B wants `bitflags = "1.1"`, both get one 1.x version. If two packages want two exact versions that are compatible, such as `=0.4.11` and `=0.4.8`, resolution fails. If the requirements are not compatible, such as `rand = "0.7"` and `rand = "0.6"`, Cargo builds two versions of the crate.[^13]

The model below is not Cargo. It applies the compatibility rule to the three examples of the book, with lists of published versions that are made up. It ran with Python 3.12.

```python run id=cargo-unify
# A model of how Cargo unifies the versions of one crate. It is not Cargo.
# The lists of published versions are made up for the example.

def parse(v):
    return tuple(int(x) for x in v.split("."))

def compat_key(v):
    # Versions are compatible if their left-most non-zero part is the same.
    for i, x in enumerate(v):
        if x != 0 or i == len(v) - 1:
            return v[: i + 1]

def matches(req, v):
    if req.startswith("="):
        return v == parse(req[1:])
    low = parse(req)
    low = low + (0,) * (3 - len(low))
    return v >= low and compat_key(v) == compat_key(low)

def resolve(name, published, reqs):
    versions = [parse(v) for v in published]
    groups = {}
    for pkg, req in reqs:
        ok = {v for v in versions if matches(req, v)}
        key = compat_key(max(ok))
        groups.setdefault(key, []).append((pkg, req, ok))
    print(f"{name}: " + ", ".join(f"{p} wants {r!r}" for p, r in reqs))
    for key, group in sorted(groups.items()):
        common = set.intersection(*(ok for _, _, ok in group))
        users = " and ".join(p for p, _, _ in group)
        if not common:
            print(f"  error: no one version satisfies {users}")
        else:
            print(f"  {'.'.join(map(str, max(common)))} for {users}")

resolve("bitflags", ["1.0.4", "1.2.1", "1.3.2", "2.4.0"],
        [("A", "1.0"), ("B", "1.1")])
resolve("log", ["0.4.8", "0.4.11", "0.4.20"],
        [("A", "=0.4.11"), ("B", "=0.4.8")])
resolve("rand", ["0.6.5", "0.7.3", "0.8.5"],
        [("A", "0.7"), ("B", "0.6")])
```

```output for=cargo-unify
bitflags: A wants '1.0', B wants '1.1'
  1.3.2 for A and B
log: A wants '=0.4.11', B wants '=0.4.8'
  error: no one version satisfies A and B
rand: A wants '0.7', B wants '0.6'
  0.6.5 for B
  0.7.3 for A
```

`cargo tree --duplicates` shows the crates that come in more than one version. The book says that fewer duplicates can make build times shorter and executables smaller.[^7]

**Features.** Features give conditional compilation and optional dependencies. A package lists its features in the `[features]` table, and each feature is on or off.[^8] All features are off unless something turns them on, and the `default` feature names the features that are on by default.[^8] A dependency with `optional = true` makes a feature of the same name.[^8] When several packages use one dependency, Cargo builds it once with the union of the features that they turn on. So the book says that features should be additive: a feature must not turn off a function of the crate.[^8]

**Workspaces.** A workspace is a set of packages that Cargo manages together. The members share one `Cargo.lock` and one output directory, and commands such as `cargo check --workspace` run on all of them.[^10] The run above uses a workspace.

**Resolver versions.** The key `resolver` in `[workspace]` or `[package]` selects the behavior of the resolver. Version `"2"` changes how features are unified, and it is the default for the 2021 edition. Version `"3"` is the default for the 2024 edition and needs Rust 1.84 or later.[^13]

## Common tasks

| Task | Command |
|---|---|
| Make a package with a program | `cargo new hello`[^14] |
| Add a dependency from crates.io | `cargo add serde`[^6] |
| Add a dependency from a directory | `cargo add --path ../greet`[^6] |
| Build, run and test | `cargo build`, `cargo run`, `cargo test`[^14] |
| Build with optimizations | `cargo build --release`[^15] |
| Show the dependency tree | `cargo tree`[^7] |
| Show crates in more than one version | `cargo tree --duplicates`[^7] |
| Update the lock file to the highest compatible versions | `cargo update`[^16] |
| Update one crate only | `cargo update serde`[^16] |
| Publish to crates.io | `cargo publish`[^17] |
| Stop other projects from starting to use a broken version | `cargo yank --version 1.0.1`[^17] |

`cargo update` changes `Cargo.lock`, not `Cargo.toml`. It updates the dependencies in the lock file to the highest versions that the requirements allow. With a crate name, it does a conservative update of that crate only.[^16]

A publish to crates.io is permanent: the book says that "The version can never be overwritten, and the code cannot be deleted."[^17] A yank does not delete any code. After a yank, a dependency cannot start to use the version, but the existing dependencies on it continue to work.[^17]

Tests run with `cargo test`; see [[cargo-test]].

## Configuration

**Cargo.toml.** Most of the configuration of a package is in its manifest: the metadata in `[package]`, the dependencies, the features and the profiles.[^12]

**Profiles.** A profile is a set of compiler settings, such as the optimization level and the debug information. Cargo has four built-in profiles: `dev`, `release`, `test` and `bench`.[^15] `cargo build` uses `dev`, which has `opt-level = 0` and `debug = true`. The option `--release` selects `release`, which has `opt-level = 3`.[^15] A `[profile.release]` section in `Cargo.toml` changes these settings. The page [[build-configurations]] compares profiles with the build types of other tools.

**The Cargo home.** Cargo keeps the downloaded crates and its global configuration in the Cargo home, which is `$HOME/.cargo/` by default. The environment variable `CARGO_HOME` changes the location.[^18] The file `config.toml` in the Cargo home holds the global configuration.[^18]

## Alternatives

`rustup` installs Cargo together with Rust.[^5] A project can also call `rustc` directly, without Cargo.[^3] General build systems such as [[make]], [[cmake]] and [[bazel]] can run Cargo or `rustc` as one step of a larger build.

Other language package managers have the same role as Cargo: [[go-modules]] for Go, [[nuget-pm]] for .NET, and [[maven]] and [[gradle]] for Java.

## History and versions

Carl Lerche and Yehuda Katz designed Cargo. When crates.io went online on 20 November 2014, Alex Crichton wrote that Cargo follows "the tradition of successes like Bundler and NPM".[^19] The announcement of Rust 1.0 on 15 May 2015 presented Cargo, and called crates.io "a central package repository for Rust code".[^2]

Each Cargo release has the version number of the Rust release that ships it. The changelog gives these release dates:[^20]

| Version | Release date |
|---|---|
| 1.98 | 20 August 2026 |
| 1.99 | 1 October 2026 |

The output on this page is from Cargo 1.98.1.

## References
