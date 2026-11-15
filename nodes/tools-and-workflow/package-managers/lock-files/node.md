---
tag: RYQCPN
kind: concept
slug: lock-files
name: Lock files
aliases: [lock file, lockfile, package-lock.json, Cargo.lock, uv.lock, packages.lock.json, go.sum, hash-checking mode, deterministic install]
area: tools-and-workflow/package-managers
level: core
summary: >-
  A lock file records the exact version, and often a hash, of every package that a resolution chose, so that later installs on other machines get the same packages.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2017
  people: []
  note: npm 5.0.0, released in May 2017, creates package-lock.json automatically.
see_also: [dependency-resolution, version-constraints, package-manager-concept, transitive-dependencies]
sources:
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/package-lock-json
    title: npm Docs (v11), package-lock.json
    locator: Description; packages, integrity and resolved
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/faq.html
    title: The Cargo Book, Frequently Asked Questions
    locator: Why have Cargo.lock in version control?
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/resolver.html
    title: The Cargo Book, Dependency Resolution
    locator: Lock file
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/projects/sync/
    title: uv documentation, Locking and syncing
    locator: Upgrading locked package versions; Exporting the lockfile
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: go.sum files; Authenticating modules
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/package-references-in-project-files
    title: NuGet documentation, PackageReference in project files
    locator: Locking dependencies; Enabling the lock file; restore behavior with lock file
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/repeatable-installs/
    title: pip documentation, Repeatable Installs
    locator: Pinning the package versions; Hash-checking
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/secure-installs/
    title: pip documentation, Secure installs
    locator: Hash-checking Mode; Forcing Hash-checking mode
    accessed: 2026-11-16
  - url: https://github.com/npm/npm/releases/tag/v5.0.0
    title: npm, release v5.0.0, 26 May 2017
    locator: the release notes
    accessed: 2026-11-16
---

## Idea

A manifest gives ranges, such as "any 1.x version of log". The resolver turns them into exact versions, and the result depends on the day: when a new version is published, the same manifest resolves to other packages. A **lock file** writes down the result of one resolution, so that the next install uses the same versions and does not resolve again.

npm describes its file `package-lock.json` this way: "It describes the exact tree that was generated, such that subsequent installs are able to generate identical trees, regardless of intermediate dependency updates." The file "is intended to be committed into source repositories", so that "teammates, deployments, and continuous integration are guaranteed to install exactly the same dependencies".[^1] Cargo gives the same purpose for `Cargo.lock`: it describes "the state of the world at the time of a successful build", and Cargo uses it "to provide deterministic builds at different times and on different systems".[^2]

## How it works

A lock file has one entry for each package in the whole tree, direct and transitive. An entry usually holds:

- the name and the **exact version** that the resolver chose;
- where the package came from, such as a registry URL, a Git commit or a local path;
- a **hash** of the package file. In `package-lock.json`, the field `integrity` is "A sha512 or sha1 Standard Subresource Integrity string for the artifact that was unpacked in this location".[^1]

The package manager uses the file in two ways:

1. **Prefer the locked versions.** When a lock file exists, the resolver starts from it. Cargo "gives the highest priority to versions contained in the Cargo.lock file", to balance reproducible builds with changes in the manifest.[^3] uv "will prefer the previously locked versions of packages", and a version changes only if the constraints of the project exclude it.[^4]
2. **Check the hashes.** After a download, the package manager computes the hash and compares it with the locked hash. A mismatch stops the install. In Go, the go command "computes a cryptographic hash and checks that it matches a hash in the main module’s go.sum file"; if the hash does not match, it "reports a security error and does not install the file".[^5]

The examples below resolve two packages, write a lock file with a SHA-256 hash for each, and install from it twice. When a new version of `log` is published, a new resolution would take it, but the install from the lock file keeps 1.2.0. When the bytes of `web` 2.0.1 change on the registry, the hash check fails.

## In languages

Each example keeps the registry in memory as a map from a name and a version to bytes, and prints the first 16 hexadecimal digits of each hash. Python uses `hashlib.sha256`, Go uses `crypto/sha256`, Java uses `MessageDigest` and `HexFormat`, and C# uses `SHA256.HashData` and `Convert.ToHexStringLower`.

## Variations and related ideas

- **The names of the files.** npm writes `package-lock.json`, Cargo writes `Cargo.lock`, and uv writes `uv.lock`.[^1][^2][^4] NuGet writes `packages.lock.json` when the MSBuild property `RestorePackagesWithLockFile` is true, and it "lists all the package dependencies".[^6]
- **Pinned requirements files.** A pip requirements file can pin each package with `==` and add a hash, as in `FooProject == 1.2 --hash=sha256:...`. In hash-checking mode, which you can force with `--require-hashes`, every requirement must have a hash and be pinned.[^7][^8] uv can export `uv.lock` to `requirements.txt` and to `pylock.toml`, the format of PEP 751.[^4]
- **Checksums without a lock file.** Go records the minimum version of each dependency in `go.mod`, and the file `go.sum` holds the hashes. When a hash is not in `go.sum`, the go command gets it from the checksum database, by default `sum.golang.org`.[^5]
- **Locked mode.** In CI you want an error, not a new resolution, when the manifest and the lock file disagree. With `dotnet restore --locked-mode`, NuGet will "either restore the exact packages as listed in the lock file or fail" if the dependencies of the project changed.[^6]

## Pitfalls

- **Libraries and lock files.** The lock file of a library does not control what its users get. Cargo warns that "Cargo.lock does not affect the consumers of your package, only Cargo.toml does that".[^2] NuGet says the same: the lock file of a common code project "has little say over the packages resolved for projects that depend on it".[^6] Test a library with the newest versions that its ranges allow, too.
- **Merge conflicts.** Two branches that both add dependencies change the same lock file. Cargo notes that "The lockfile can also be a source of merge conflicts".[^2] Resolve the conflict in the manifest, then let the tool write the lock file again; do not edit the file by hand.
- **A stale lock file is not an update policy.** A lock file keeps old versions with known bugs and vulnerabilities until someone updates it. Update on purpose, for example with `uv lock --upgrade`, and review the change.[^4]
- **Hashes need pinned versions.** pip requires that requirements are pinned in hash-checking mode. This "prevents a surprising hash mismatch upon the release of a new version that matches the requirement specifier".[^8]
- **`cargo install` ignores the lock file by default.** It "will select the latest dependencies unless --locked is passed in".[^2]

## Cost

A lock file is a text file of one entry for each package, and a large project has hundreds of entries. It saves the time of a resolution on each install. Hash checks cost one hash computation for each download. The cost is in the review: an update can change many lines of the file, and the change must still be checked.

## History

npm 3 already updated a file `npm-shrinkwrap.json` when you saved a dependency. npm 5.0.0, released on 26 May 2017, creates `package-lock.json` automatically, unless a file `npm-shrinkwrap.json` exists.[^9] pip added hash-checking mode in version 8.0.[^8] Today Cargo, npm, NuGet and uv each write a lock file, and Go keeps hashes in `go.sum`.[^2][^1][^6][^4][^5]

## References
