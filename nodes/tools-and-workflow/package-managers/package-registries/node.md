---
tag: E4S2X3
kind: concept
slug: package-registries
name: Package registries
aliases: [package registry, package index, package repository, artifact repository, private registry, registry mirror, PyPI, crates.io, Maven Central, nuget.org]
area: tools-and-workflow/package-managers
level: core
summary: >-
  A package registry is a server that stores published packages and an index of their versions. Package managers download from it. Mirrors copy it; private registries hold internal packages.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1995
  people: []
  note: The Comprehensive Perl Archive Network (CPAN) has been online since October 1995.
see_also: [package-manager-concept, dependency-resolution, lock-files, version-constraints, transitive-dependencies]
sources:
  - url: https://docs.npmjs.com/about-the-public-npm-registry
    title: npm Docs, About the public npm registry
    locator: the first paragraph
    accessed: 2026-11-16
  - url: https://docs.pypi.org/
    title: PyPI User Documentation, Welcome to PyPI's user documentation
    locator: the first paragraph
    accessed: 2026-11-16
  - url: https://central.sonatype.org/publish/requirements/
    title: Maven Central Documentation, Requirements
    locator: Correct Coordinates
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/what-is-nuget
    title: Microsoft Learn, An introduction to NuGet
    locator: the introduction; The flow of packages between creators, hosts, and consumers
    accessed: 2026-11-16
  - url: https://docs.pypi.org/using-pypi/pypi-basics/
    title: PyPI User Documentation, PyPI Basics
    locator: Projects, releases and files (packages) on PyPI
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/registries.html
    title: The Cargo Book, Registries
    locator: the introduction; Using an Alternate Registry
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/using-npm/registry
    title: npm Docs (v11), registry
    locator: Description
    accessed: 2026-11-16
  - url: https://maven.apache.org/guides/mini/guide-mirror-settings.html
    title: Maven, Using Mirrors for Repositories
    locator: the introduction; the note on the official Maven repository
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: Module proxies; Environment variables
    accessed: 2026-11-16
  - url: https://pkg.go.dev/about
    title: pkg.go.dev, About
    locator: Adding a package; Removing a package
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_install/
    title: pip documentation, pip install
    locator: Package Index Options; Examples
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/publishing.html
    title: The Cargo Book, Publishing on crates.io
    locator: the introduction; cargo yank
    accessed: 2026-11-16
  - url: https://central.sonatype.org/publish/requirements/immutability/
    title: Maven Central Documentation, Immutability of Published Components
    locator: the first paragraphs
    accessed: 2026-11-16
  - url: https://docs.pypi.org/project-management/yanking/
    title: PyPI User Documentation, Yanking
    locator: the first paragraphs
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/using-npm/scope
    title: npm Docs (v11), scope
    locator: Description; Publishing scoped packages; Associating a scope with a registry
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/source-replacement.html
    title: The Cargo Book, Source Replacement
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/policies/unpublish
    title: npm Docs, npm Unpublish Policy
    locator: Packages published less than 72 hours ago
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/indexes/
    title: uv documentation, Package indexes
    locator: Searching across multiple indexes
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/package-source-mapping
    title: Microsoft Learn, Package Source Mapping
    locator: the introduction; Package Pattern precedence
    accessed: 2026-11-16
  - url: https://docs.pypi.org/using-pypi/package-security/common-attacks/
    title: PyPI User Documentation, Common attacks
    locator: Typosquatting; Dependency confusion
    accessed: 2026-11-16
  - url: https://www.cpan.org/
    title: The Comprehensive Perl Archive Network (CPAN)
    locator: Welcome to CPAN
    accessed: 2026-11-16
---

## Idea

A package manager needs a place to get packages from. A **package registry** is that place: a server that stores the packages that people publish, with the metadata of each version, and that answers the questions of the package manager. npm describes its public registry as "a database of JavaScript packages, each comprised of software and metadata".[^1] Other names are a package index (PyPI, the Python Package Index), a repository (Maven Central) and a host or feed (NuGet).[^2][^3][^4]

Each tier 1 language has one main public registry:

| Language | Registry | Default client |
|---|---|---|
| Python | PyPI, `pypi.org` | pip, uv |
| JavaScript and TypeScript | the npm registry, `registry.npmjs.org` | npm |
| Rust | crates.io | Cargo |
| Java and Kotlin | Maven Central, `repo.maven.apache.org/maven2` | Maven |
| C# | nuget.org | NuGet, `dotnet` |
| Go | the module mirror `proxy.golang.org`, with the site pkg.go.dev | the go command |

PyPI is "the official package repository for Python".[^2] pip is the standard client, and tools such as uv also use PyPI as their default source.[^5] For Rust, "The default registry is crates.io".[^6] npm "is configured to use the npm public registry at https://registry.npmjs.org by default".[^7] The official Maven repository is at `https://repo.maven.apache.org/maven2`.[^8] NuGet "maintains the central repository of over 100,000 unique packages at nuget.org".[^4] Go is different: a module lives in its own version control repository, and the default value of `GOPROXY` is `https://proxy.golang.org,direct`, so the go command first asks a public module proxy that Google operates.[^9] The site pkg.go.dev shows documentation for the modules, and its data "is downloaded from proxy.golang.org".[^10]

## How it works

A registry has three parts:

1. **Names and versions.** A package has a name that is unique on the registry, and each version is a separate release. On PyPI, a project is "a collection of releases and files", and a release has one or more files, for example a source archive and a binary wheel.[^5] Maven Central uses three coordinates, `groupId`, `artifactId` and `version`, and the `groupId` starts with a reverse domain name.[^3]
2. **An index.** The package manager reads the index to find the versions of a name and their dependencies, without a download of each package. A Cargo registry contains an index, a searchable list of the crates that it has.[^6] pip reads indexes that comply with "PEP 503 (the simple repository API)".[^11]
3. **A publish API.** An author uploads a new version, usually with a token. Cargo uses `cargo publish`, and a registry "may also provide a web API to support publishing new crates directly from Cargo".[^6]

The main rule of a public registry is that a **published version does not change**. Then a version number always means the same bytes, and a [[lock-files|lock file]] can record a hash of them. Cargo warns that "a publish is generally permanent. The version can never be overwritten, and the code cannot be deleted".[^12] Maven Central says: "we do not remove or modify components once they are publicly available".[^13] On the Go side, "published versions cannot be reused or modified, and this includes retracted versions".[^10]

A broken version is **yanked** or retracted, not deleted. "The semantics of a yanked version are that no new dependencies can be created against that version, but all existing dependencies continue to work."[^12] On PyPI, a yanked release "is always ignored by an installer, unless it is the only release that matches a version specifier", with `==` or `===`.[^14] Go uses a `retract` directive in `go.mod`, and a new version that carries it.[^10]

The examples below build a registry in memory. It refuses a second publish of the same version, it yanks a version, and it shows that a new install skips the yanked version while an exact pin still gets it. Then two registries, one internal and one public, have a package with the same name, and the example compares two ways to choose between them.

## In languages

Each example keeps a map from a package name to a map from a version to a yanked flag. Python uses dicts and `max` with a key function, Go uses maps and `slices.Compare` on the parts of a version, Java uses `HashMap` with a `Comparator` and `Optional`, and C# uses `Dictionary`, `System.Version.Parse` and LINQ `MaxBy`. The four programs print the same output.

## Variations and related ideas

- **Private registries.** A company can run its own registry for internal packages. NuGet supports "private hosts alongside the public nuget.org host", in the cloud, on a private network, or "on just your local file system".[^4] Cargo reads other registries from the table `[registries]` in `.cargo/config.toml`, and a dependency selects one with the key `registry`. But "crates.io does not accept packages that depend on crates from other registries".[^6]
- **Scopes and namespaces.** An npm scope groups packages under a name such as `@myorg/mypackage`. "Each npm user/organization has their own scope, and only you can add packages in your scope."[^15] A scope can point to a private registry, and "a scope only ever points to one registry".[^15]
- **Mirrors and proxies.** A mirror is a copy of a registry, nearer or under your control. Maven gives these reasons for a mirror: a closer and faster server, an internal repository "which you have greater control over", or a repository manager that gives a local cache.[^8] Cargo can replace crates.io with a mirror or with a local directory, on the assumption that "the source code is exactly the same from both sources".[^16] With `GOPROXY=https://corp.example.com,https://proxy.golang.org`, the go command asks the company proxy first and goes to the next entry after a 404 or 410 response.[^9]
- **Unpublish windows.** Some registries let you remove a mistake for a short time. On the npm public registry, if no other package depends on it, "you can unpublish anytime within the first 72 hours after publishing".[^17]

## Pitfalls

- **Dependency confusion.** When a tool reads more than one index, an attacker can publish a public package with the name of your private package and a higher version. pip searches all its locations with "no priority", and its documentation says that the use of `--extra-index-url` for private packages is unsafe.[^11] uv by default stops "at the first index on which a given package is available", to prevent dependency confusion.[^18] NuGet gives Package Source Mapping, which can "filter, per package, which source(s) NuGet will search".[^19] The example shows both rules: the best match over all indexes takes the attacker's `billing 99.0.0`, and the first index rule takes the internal `billing 1.0.0`.
- **Typosquatting.** "An attacker publishes a malicious project with a name that is a common misspelling of a popular, legitimate project".[^20] Check the name before you add a package, and install from a pinned requirements file or a lock file.[^20]
- **Deleted and yanked versions.** Do not plan to delete a bad release. Publish a new version with the fix; Maven Central suggests a patch release.[^13] Yank or retract the bad one, so that new resolutions skip it.[^12][^10]
- **Mirrors that differ.** A mirror that is not exact breaks reproducible builds. Maven warns that the known mirrors of Maven Central "may not have the same contents".[^8] Hashes in a lock file find such a difference.

## Cost

A request to a registry costs a network round trip, and a resolution can need the index entry of each package in the tree. Package managers keep a local cache for this reason, and a mirror near the build machines cuts the time of each install. A private registry costs the work to run it, to back it up and to control its tokens.

## History

The Comprehensive Perl Archive Network (CPAN) "has been online since October 1995".[^21] npm added scoped packages in npm 2, and the primary npm registry has supported them since 2015-04-19.[^15] Today each tier 1 language has a main registry, and Go downloads modules through a module proxy and checks them with the checksum database `sum.golang.org`.[^2][^6][^3][^4][^9][^7]

## References
