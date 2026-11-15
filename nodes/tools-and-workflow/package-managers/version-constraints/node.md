---
tag: PBRTQC
kind: concept
slug: version-constraints
name: Version constraints
aliases: [version constraint, version requirement, version range, version specifier, caret range, tilde range, compatible release, semver range]
area: tools-and-workflow/package-managers
level: core
summary: >-
  A version constraint names the versions of a dependency that a project accepts. Each ecosystem has its own range syntax, such as the caret and tilde of npm and Cargo, or intervals in Maven.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2013
  people: [Tom Preston-Werner]
  note: The tag v2.0.0 of the Semantic Versioning specification, which most range syntaxes build on, points to a merge commit of 18 June 2013.
see_also: [semantic-versioning, dependency-resolution, lock-files, package-manager-concept, transitive-dependencies]
sources:
  - url: https://semver.org/spec/v2.0.0.html
    title: Semantic Versioning 2.0.0
    locator: Summary; Semantic Versioning Specification, items 4 and 8
    accessed: 2026-11-16
  - url: https://github.com/npm/node-semver
    title: npm, node-semver (README)
    locator: Ranges; Advanced Range Syntax, Tilde Ranges, Caret Ranges; Prerelease Tags
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html
    title: The Cargo Book, Specifying Dependencies
    locator: Version requirement syntax; Default requirements; Caret requirements; Tilde requirements
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/version-specifiers/
    title: Python Packaging User Guide, Version specifiers
    locator: Version specifiers; Compatible release
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/concepts/package-versioning
    title: NuGet documentation, Package versioning
    locator: Version basics; Version ranges
    accessed: 2026-11-16
  - url: https://maven.apache.org/pom.html
    title: Maven, POM Reference
    locator: Dependency Version Requirement Specification; Version Order Specification
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: Major version suffixes; require directive
    accessed: 2026-11-16
  - url: https://github.com/semver/semver/commit/7c834b3f3a4940d77ab593bc32583004d6a426a9
    title: semver/semver, commit 7c834b3, Mark 2.0.0 as RTM, the commit of the tag v2.0.0
    locator: the commit date, 2013-06-18
    accessed: 2026-11-16
---

## Idea

A dependency in a manifest file has a name and a **version constraint**: the set of versions that the project accepts. An exact version is the narrowest constraint. A range lets the package manager choose a newer version that has bug fixes, while it keeps out versions that can break the project.

Most ranges rely on Semantic Versioning. Under it, a version is `MAJOR.MINOR.PATCH`, and you increment MAJOR "when you make incompatible API changes", MINOR "when you add functionality in a backward compatible manner" and PATCH "when you make backward compatible bug fixes".[^1] So a range from 1.2.3 up to, but not including, 2.0.0 accepts the fixes and the new features of version 1, and it keeps out the next incompatible version.

Each ecosystem writes ranges in its own way, and the same text can mean different things. In npm, `1.2.3` is one exact version.[^2] In Cargo, `1.2.3` is the range from 1.2.3 up to, but not including, 2.0.0.[^3]

## How it works

A constraint is a set of comparisons that a version must all satisfy. In npm, a comparator set such as `>=1.2.7 <1.3.0` matches 1.2.7, 1.2.8 and 1.2.99, but not 1.2.6, 1.3.0 or 1.1.0.[^2] In Python the comparisons are joined with commas, as in `~= 0.9, >= 1.0, != 1.3.4.*, < 2.0`, and "The comma (“,”) is equivalent to a logical and operator".[^4]

Short forms expand to such ranges:

| Constraint | npm | Cargo |
|---|---|---|
| `^1.2.3` | `>=1.2.3 <2.0.0-0` | `>=1.2.3, <2.0.0` |
| `^0.2.3` | `>=0.2.3 <0.3.0-0` | `>=0.2.3, <0.3.0` |
| `^0.0.3` | `>=0.0.3 <0.0.4-0` | `>=0.0.3, <0.0.4` |
| `~1.2.3` | `>=1.2.3 <1.3.0-0` | `>=1.2.3, <1.3.0` |
| `1.2.3` | exactly 1.2.3 | the same as `^1.2.3` |

The **caret** allows the changes that "do not modify the left-most non-zero element" of the version, in the words of the npm documentation.[^2] So for a version 0.x, a change of the minor number counts as a break. Cargo calls the caret requirement "the default version requirement strategy", and it treats versions as compatible "if their left-most non-zero major/minor/patch component is the same". Cargo notes that "This is different from SemVer which considers all pre-1.0.0 packages to be incompatible".[^3] SemVer itself says that "Anything MAY change at any time" in major version zero.[^1]

The **tilde** is narrower. With a major, minor and patch version, only patch-level changes are allowed.[^3][^2] The `-0` at the end of the npm upper bounds excludes the prereleases of the next version, such as 2.0.0-beta.

The examples below expand caret, tilde and exact constraints into a range from a lowest version up to a first version that is not allowed, and test eleven versions against each range.

## In languages

Each example parses a version into three numbers, so that 1.10.0 sorts after 1.9.0. The Python and Go examples compare tuples and arrays of numbers. The Java example uses a record that implements `Comparable`, and the C# example uses the built-in type `System.Version`, which has the comparison operators. None of the examples handles prerelease tags.

## Variations and related ideas

- **Python.** A version specifier is a list of clauses with the operators `~=`, `==`, `!=`, `<=`, `>=`, `<`, `>` and `===`. The compatible release clause `~= 2.2` is the same as `>= 2.2, == 2.*`, and `~= 1.4.5` is the same as `>= 1.4.5, == 1.4.*`.[^4]
- **NuGet.** NuGet uses interval notation: `[1.0,2.0)` means 1.0 ≤ x < 2.0, and `[1.0]` means exactly 1.0. A plain `1.0` is a minimum version, inclusive.[^5]
- **Maven.** Maven has the same interval notation, and it splits requirements into two kinds. A plain `1.0` is a soft requirement: "Use 1.0 if no other version appears earlier in the dependency tree". A range such as `[1.0,2.0)` is a hard requirement. Maven "picks the highest version of each project that satisfies all the hard requirements", and the build fails if no version satisfies them.[^6]
- **Go.** A `require` directive "declares a minimum required version", with no upper limit.[^7] A new major version gets a new module path, such as `example.com/mod/v2`. This follows the import compatibility rule: "If an old package and a new package have the same import path, the new package must be backwards compatible with the old package".[^7] So two major versions are two different modules.
- **Prereleases.** In npm, a version with a prerelease tag, such as `1.2.3-alpha.3`, satisfies a range only if a comparator with the same major, minor and patch numbers also has a prerelease tag.[^2] Python has its own rules for the handling of pre-releases.[^4]

## Pitfalls

- **The same text, another meaning.** `1.2.3` is exact in npm, the same as `^1.2.3` in Cargo, a minimum in NuGet and a soft requirement in Maven.[^2][^3][^5][^6] Check the syntax of the ecosystem before you copy a constraint.
- **Versions below 1.0.** Under the caret, `^0.2.3` does not accept 0.3.0, and `^0.0.3` accepts no other version.[^2][^3] A library at 0.x that adds a feature in 0.3.0 does not reach users with `^0.2.3`.
- **Ranges that are too wide.** `*` or `>=1.0` accepts the next major version, with its incompatible changes. SemVer gives the meaning of the numbers, but only the authors of a package can follow it.[^1]
- **Ranges that are too narrow.** If every library pins an exact version, two libraries that need different patch versions of the same package can make the resolution fail. See [[dependency-resolution]].
- **String order.** Compare versions as numbers, not as strings: in a comparison of strings, 1.10.0 sorts before 1.9.0. SemVer requires that each element "MUST increase numerically", as in 1.9.0, 1.10.0, 1.11.0.[^1]

## Cost

A range check is cheap: parse the version and compare a few numbers. The cost of a constraint is in what it lets through. A wide range lets new, untested versions into the build; a narrow range makes upgrades and resolution harder. Most projects use a range in the manifest and record the exact versions in a lock file; see [[lock-files]].

## History

Tom Preston-Werner first wrote the Semantic Versioning specification.[^1] Its version 2.0.0, which comes after 1.0.0 and two release candidates, was marked as final in a commit of 18 June 2013.[^1][^8] node-semver describes a version by the 2.0.0 specification, Cargo builds its default requirements on SemVer compatibility, and NuGet supports SemVer 2.0.0 from NuGet 4.3.0.[^2][^3][^5] Maven follows the precedence rules of SemVer 1.0.0 in most cases, but its version order is not compatible with SemVer 2.0.0.[^6] Python has its own version scheme and specifiers, defined in the Python packaging specifications.[^4] Go chose minimum versions and major version suffixes for its modules.[^7]

## References
