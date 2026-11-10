---
tag: NEW-semantic-versioning
kind: standard
slug: semantic-versioning
name: Semantic Versioning
aliases: [SemVer]
area: software-engineering-process/releases-and-versioning
level: core
summary: >-
  Semantic Versioning is a set of rules for version numbers of the form MAJOR.MINOR.PATCH. The number that changes tells the user if a release breaks the public API, adds to it or only fixes bugs.
history:
  year: 2009
  people: [Tom Preston-Werner]
  note: Tom Preston-Werner made the first commit of the site semver.org on 14 December 2009. The specification moved to its own repository in June 2011. The tag v2.0.0 points to a merge commit of 18 June 2013.
same_as:
  wikidata: Q48029236
sources:
  - url: https://semver.org/spec/v2.0.0.html
    title: Semantic Versioning 2.0.0
    locator: Summary; Introduction; Semantic Versioning Specification (SemVer), items 1 to 11; Backus–Naur Form Grammar for Valid SemVer Versions; FAQ; About
    accessed: 2026-11-10
  - url: https://semver.org/spec/v1.0.0.html
    title: Semantic Versioning 1.0.0
    locator: Semantic Versioning Specification (SemVer); Tagging Specification (SemVerTag); About
    accessed: 2026-11-10
  - url: https://github.com/semver/semver.org/commit/ca645805ca206e83c7153c64f9bda54afff06262
    title: semver/semver.org, commit ca64580, "First commit", Tom Preston-Werner, 2009-12-14
    locator: the commit and the file index.md
    accessed: 2026-11-10
  - url: https://github.com/semver/semver/commit/ec27d6a2cdde57246eb7442a484e8b6fae5f15a9
    title: semver/semver, commit ec27d6a, "Extract spec from https://github.com/mojombo/semver.org.", 2011-06-08
    locator: the commit message
    accessed: 2026-11-10
  - url: https://github.com/semver/semver/commit/7c834b3f3a4940d77ab593bc32583004d6a426a9
    title: semver/semver, commit 7c834b3, "Merge pull request #120 from mojombo/release-2.0, Mark 2.0.0 as RTM", 2013-06-18
    locator: the commit, which the tag v2.0.0 points to
    accessed: 2026-11-10
  - url: https://github.com/npm/node-semver/blob/main/README.md
    title: npm, node-semver README (package version 7.8.6)
    locator: Versions; Ranges; Prerelease Tags; Advanced Range Syntax (Hyphen Ranges, X-Ranges, Tilde Ranges, Caret Ranges); Coercion
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html
    title: The Cargo Book, Specifying Dependencies
    locator: Default requirements; Caret requirements; Tilde requirements; Wildcard requirements; Pre-releases; Version metadata; the recommendation on upper bounds
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/cargo/reference/semver.html
    title: The Cargo Book, SemVer Compatibility
    locator: SemVer compatibility; Change categories; item-remove; struct-add-public-field-when-no-private; enum-variant-new
    accessed: 2026-11-10
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: Versions; Pseudo-versions; Major version suffixes; Minimal version selection (MVS)
    accessed: 2026-11-10
  - url: https://pkg.go.dev/golang.org/x/mod/semver
    title: Go package golang.org/x/mod/semver
    locator: the package overview and func Compare
    accessed: 2026-11-10
  - url: https://getcomposer.org/doc/articles/versions.md
    title: Composer documentation, Versions and constraints
    locator: Tilde Version Range (~); Caret Version Range (^)
    accessed: 2026-11-10
  - url: https://peps.python.org/pep-0440/
    title: "PEP 440: Version Identification and Dependency Specification"
    locator: Semantic versioning; Compatible release
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/nuget/concepts/package-versioning
    title: NuGet documentation, Package versioning
    locator: Semantic Versioning 2.0.0; Version ranges; Normalized version numbers; Where NuGetVersion diverges from Semantic Versioning
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.management.automation.semanticversion
    title: PowerShell SDK, SemanticVersion Class
    locator: the class description
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_type_accelerators
    title: PowerShell documentation, about_Type_Accelerators
    locator: the table of type accelerators (semver)
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.version
    title: .NET API, Version Class
    locator: the class description
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/library/re.html
    title: The Python Standard Library, re, Python 3.14
    locator: Regular Expression Syntax, \d; Flags, re.ASCII
    accessed: 2026-11-10
  - url: https://docs.rs/semver/1.0.28/semver/
    title: semver 1.0.28 documentation
    locator: struct Version (Syntax, Total ordering, parse, cmp_precedence); struct VersionReq
    accessed: 2026-11-10
  - url: https://github.com/semver4j/semver4j
    title: semver4j
    locator: the README
    accessed: 2026-11-10
---

## What it is

Semantic Versioning (SemVer) is a set of rules for version numbers. A version has three numbers, MAJOR.MINOR.PATCH. The specification says what each number means.[^1]

- Increase MAJOR when you make incompatible API changes.
- Increase MINOR when you add functionality that is backward compatible.
- Increase PATCH when you make bug fixes that are backward compatible.

The specification starts from a problem that it calls "dependency hell". A system with many dependencies has two risks. If the dependency specifications are too tight, you get version lock: you cannot upgrade one package without a release of each package that depends on it. If they are too loose, you get version promiscuity: you assume that more future versions are compatible than is reasonable.[^1] SemVer gives the version number a meaning, so a tool can choose a range of versions that are safe.

The rules apply to a public API. Item 1 of the specification says that software that uses SemVer MUST declare a public API, in the code or in documentation.[^1] The version number describes changes to that API and nothing else. A change to private code does not need a major release. A change to documented behaviour can need one, even if no function signature changes. The Go modules reference asks for a major version after an incompatible change to the "public interface or documented functionality".[^9]

The specification is the document "Semantic Versioning 2.0.0" on the site semver.org. It uses the key words of RFC 2119, such as MUST, SHOULD and MAY. It has eleven numbered items, a grammar in Backus-Naur form, two regular expressions and a FAQ.[^1] The text is under the licence Creative Commons CC BY 3.0. The About section says that Tom Preston-Werner, the inventor of Gravatar and a cofounder of GitHub, wrote the original text.[^1]

SemVer is a convention, not a law of the package managers. npm, Cargo, Composer, NuGet and the Go module system use SemVer version numbers. Each of them adds its own range syntax and its own rules for releases before 1.0.0 and for pre-releases. The section Versions compares them.

### History

Tom Preston-Werner made the first commit of the repository semver.org on 14 December 2009. The file index.md of that commit already has the rules for the three numbers and a rule that a tag has the form "vX.Y.Z".[^3]

Version 1.0.0 of the specification had integers with no rule about leading zeros. A pre-release version was "an arbitrary string" after a dash, ordered by an ASCII sort. Its example is 1.0.0-alpha1 < 1.0.0-beta1 < 1.0.0-beta2 < 1.0.0-rc1 < 1.0.0. Version 1.0.0 had no build metadata. It also had a second part, the Tagging Specification (SemVerTag). That part said a tag in version control MUST be "vX.Y.Z", for example v3.1.0.[^2]

On 8 June 2011, the specification moved to its own repository, semver/semver, with the commit message "Extract spec from https://github.com/mojombo/semver.org."[^4] The repository has the tags v1.0.0-beta, v1.0.0-rc.1, v1.0.0 and v2.0.0. The tag v2.0.0 points to the merge commit of pull request 120, "Mark 2.0.0 as RTM", of 18 June 2013.[^5]

Version 2.0.0 made five main changes to version 1.0.0.[^1][^2]

- Numbers MUST NOT have leading zeros.
- A pre-release is a list of identifiers separated by dots, not one string.
- Numeric identifiers compare as numbers, so beta.11 comes after beta.2.
- Build metadata after a plus sign is part of the syntax and has no effect on precedence.
- The Tagging Specification is gone. The FAQ says that "v1.2.3" is not a semantic version. It is a tag name, and the semantic version is "1.2.3".

## Syntax or structure

A version has a version core, an optional pre-release and optional build metadata. The order is fixed.

```text
1.4.2-beta.11+exp.sha.5114f85
| | | |       |
| | | |       build metadata: exp, sha, 5114f85
| | | pre-release: beta, 11
| | patch: 2
| minor: 4
major: 1
```

The rules for each part come from items 2, 9 and 10.[^1]

- **Version core.** The form is X.Y.Z. Each of X, Y and Z is a non-negative integer with no leading zeros. So 0 is valid, but 00 and 01 are not. Each number MUST increase numerically: 1.9.0 is followed by 1.10.0.
- **Pre-release.** A hyphen follows the patch number, then one or more identifiers separated by dots. An identifier has only ASCII letters, digits and hyphens, `[0-9A-Za-z-]`. It MUST NOT be empty. A numeric identifier MUST NOT have leading zeros. The examples in the specification are 1.0.0-alpha, 1.0.0-alpha.1, 1.0.0-0.3.7, 1.0.0-x.7.z.92 and 1.0.0-x-y-z.--.
- **Build metadata.** A plus sign follows the patch number or the pre-release, then one or more identifiers separated by dots. The character set is the same, and an identifier MUST NOT be empty. A build identifier can have leading zeros, as in 1.0.0-alpha+001.

The grammar defines an alphanumeric identifier as a sequence that has at least one letter or hyphen. A numeric identifier is "0", or a positive digit followed by zero or more digits.[^1] The top rules of the grammar are below.

```text
<valid semver> ::= <version core>
                 | <version core> "-" <pre-release>
                 | <version core> "+" <build>
                 | <version core> "-" <pre-release> "+" <build>

<version core> ::= <major> "." <minor> "." <patch>

<pre-release identifier> ::= <alphanumeric identifier>
                           | <numeric identifier>

<build identifier> ::= <alphanumeric identifier>
                     | <digits>

<numeric identifier> ::= "0"
                       | <positive digit>
                       | <positive digit> <digits>
```

There is no prefix. The text "v1.2.3" is not a semantic version.[^1] There is no size limit. The FAQ says that a version string of 255 characters is "probably overkill", and that a system can set its own limit.[^1]

### The official regular expressions

The FAQ gives two regular expressions. The first has named groups. The FAQ says that it works in PCRE, Python and Go. The second has numbered groups, and the FAQ adds ECMAScript to the list.[^1] The program below uses the first one. It runs with the flag `re.ASCII`. Without this flag, `\d` in a Python `str` pattern matches any Unicode decimal digit, and the specification allows only ASCII.[^17]

```python run id=parse
import re

SEMVER = re.compile(
    r"^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)"
    r"(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?"
    r"(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$",
    re.ASCII,
)

samples = [
    "1.2.3",
    "1.0.0-alpha.1+exp.sha.5114f85",
    "1.0.0-x-y-z.--",
    "1.0.0+001",
    "v1.2.3",
    "1.2",
    "01.2.3",
    "1.0.0-01",
    "1.0.0-alpha..1",
    "1.0.0+",
]

for text in samples:
    match = SEMVER.match(text)
    if match:
        parts = {name: value for name, value in match.groupdict().items() if value is not None}
        print(f"{text:30} {parts}")
    else:
        print(f"{text:30} invalid")
```

```output for=parse
1.2.3                          {'major': '1', 'minor': '2', 'patch': '3'}
1.0.0-alpha.1+exp.sha.5114f85  {'major': '1', 'minor': '0', 'patch': '0', 'prerelease': 'alpha.1', 'buildmetadata': 'exp.sha.5114f85'}
1.0.0-x-y-z.--                 {'major': '1', 'minor': '0', 'patch': '0', 'prerelease': 'x-y-z.--'}
1.0.0+001                      {'major': '1', 'minor': '0', 'patch': '0', 'buildmetadata': '001'}
v1.2.3                         invalid
1.2                            invalid
01.2.3                         invalid
1.0.0-01                       invalid
1.0.0-alpha..1                 invalid
1.0.0+                         invalid
```

A regular expression can check the syntax and split the parts. It cannot compute precedence. For that, the program must compare the parts with the rules of item 11.

## Examples

### Precedence

Item 11 defines precedence, the order of two versions.[^1]

1. Split the version into major, minor, patch and pre-release identifiers, in that order. Build metadata has no effect (item 11.1).
2. Compare major, minor and patch as numbers, from left to right. The first difference decides (item 11.2). Example: 1.0.0 < 2.0.0 < 2.1.0 < 2.1.1.
3. When the three numbers are equal, a pre-release has lower precedence than the normal version (item 11.3). Example: 1.0.0-alpha < 1.0.0.
4. When both are pre-releases, compare the identifiers from left to right until one differs (item 11.4).
   - Identifiers of digits only compare as numbers (11.4.1).
   - Identifiers with letters or hyphens compare in ASCII order (11.4.2).
   - A numeric identifier has lower precedence than an alphanumeric one (11.4.3).
   - If all earlier identifiers are equal, the longer list has higher precedence (11.4.4).

The specification gives this example of the full order:[^1]

```text
1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0
```

Each step of the example tests one rule. alpha < alpha.1 is rule 11.4.4. alpha.1 < alpha.beta is rule 11.4.3. beta.2 < beta.11 is rule 11.4.1. rc.1 < 1.0.0 is rule 11.3.

The program below turns each version into a Python tuple, so that the built-in tuple order gives SemVer precedence. A normal version gets the flag 1 and a pre-release gets the flag 0, so the pre-release sorts first. Each pre-release identifier becomes `(0, number)` or `(1, text)`. The first element puts numbers before text. Two identifiers of the same kind compare by the second element. A shorter tuple sorts before a longer tuple with the same start, which is rule 11.4.4. The key ignores the build metadata.

```python run id=precedence
import random
import re

SEMVER = re.compile(
    r"^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)"
    r"(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?"
    r"(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$",
    re.ASCII,
)


def precedence_key(text):
    match = SEMVER.match(text)
    if match is None:
        raise ValueError(f"not a semantic version: {text!r}")
    core = (int(match["major"]), int(match["minor"]), int(match["patch"]))
    if match["prerelease"] is None:
        return (*core, 1, ())
    identifiers = tuple(
        (0, int(part)) if part.isdigit() else (1, part)
        for part in match["prerelease"].split(".")
    )
    return (*core, 0, identifiers)


ordered = [
    "1.0.0-alpha",
    "1.0.0-alpha.1",
    "1.0.0-alpha.beta",
    "1.0.0-beta",
    "1.0.0-beta.2",
    "1.0.0-beta.11",
    "1.0.0-rc.1",
    "1.0.0",
]
shuffled = random.Random(7).sample(ordered, len(ordered))

by_precedence = sorted(shuffled, key=precedence_key)
print("precedence:", " < ".join(by_precedence))
print("same as the specification:", by_precedence == ordered)

print("string sort:", " < ".join(sorted(shuffled)))
print("string sort of cores:", sorted(["1.9.0", "1.10.0", "1.2.0"]))
print("key sort of cores:", sorted(["1.9.0", "1.10.0", "1.2.0"], key=precedence_key))

left, right = "1.0.0+build.1", "1.0.0+build.2"
print(f"{left} and {right}: same precedence {precedence_key(left) == precedence_key(right)}, same text {left == right}")
```

```output for=precedence
precedence: 1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0
same as the specification: True
string sort: 1.0.0 < 1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.11 < 1.0.0-beta.2 < 1.0.0-rc.1
string sort of cores: ['1.10.0', '1.2.0', '1.9.0']
key sort of cores: ['1.2.0', '1.9.0', '1.10.0']
1.0.0+build.1 and 1.0.0+build.2: same precedence True, same text False
```

A plain string sort puts 1.0.0 first, because it is a prefix of the other strings. It also puts beta.11 before beta.2 and 1.10.0 before 1.9.0. The two versions with different build metadata have the same precedence but are different strings.

The examples for other languages show the same rules with their standard libraries or their SemVer libraries. The Rust and Java examples also check version ranges with the rules of Cargo and npm.

## Versions

### SemVer 1.0.0 and 2.0.0

| Topic | 1.0.0 | 2.0.0 |
|---|---|---|
| Leading zeros in X.Y.Z | no rule | MUST NOT |
| Pre-release | an arbitrary string after a dash | identifiers separated by dots, `[0-9A-Za-z-]` |
| Pre-release order | lexicographic ASCII sort | item 11.4, numeric identifiers as numbers |
| Build metadata | none | after `+`, ignored in precedence |
| Tags in version control | MUST be "vX.Y.Z" (SemVerTag) | no rule; the FAQ says "v1.2.3" is a tag name |

Sources: the two specifications.[^1][^2]

Under 1.0.0, the pre-release 1.0.0-beta10 sorts before 1.0.0-beta2, because the comparison is on characters. Under 2.0.0, a project writes 1.0.0-beta.10, and the identifier 10 compares as a number. node-semver still removes a leading "=" or "v" from a version, and says this is for compatibility with version 1.0.0 of the specification.[^6]

### Major version zero

Item 4 says that major version zero (0.y.z) is for initial development. Anything MAY change at any time, and the public API SHOULD NOT be considered stable.[^1] So SemVer itself gives no compatibility promise between 0.1.0 and 0.1.1. The package managers do not follow item 4 exactly. They treat the left-most non-zero number as the major number:

- npm: `^0.2.3` means `>=0.2.3 <0.3.0-0`, and `^0.0.3` means `>=0.0.3 <0.0.4-0`.[^6]
- Cargo: the default requirement `0.2.3` means `>=0.2.3, <0.3.0`. The Cargo book says: "This is different from SemVer which considers all pre-1.0.0 packages to be incompatible."[^7] The Cargo SemVer guide says that a 0.0.z release is always a major change.[^8]
- Composer: `^0.3` means `>=0.3.0 <0.4.0`.[^11]
- Go: a version is unstable if its major version is 0 or it has a pre-release suffix. Unstable versions have no compatibility requirements.[^9]

### Ranges in the package managers

SemVer defines versions and their order. It does not define ranges. Each tool has its own syntax.

| Range | npm | Cargo | Composer |
|---|---|---|---|
| `^1.2.3` | `>=1.2.3 <2.0.0-0` | `>=1.2.3, <2.0.0` | `>=1.2.3 <2.0.0` |
| `1.2.3` with no operator | exactly 1.2.3 | same as `^1.2.3` | exactly 1.2.3 |
| `~1.2.3` | `>=1.2.3 <1.3.0-0` | `>=1.2.3, <1.3.0` | `>=1.2.3 <1.3.0` |
| `~1.2` | `>=1.2.0 <1.3.0-0` | `>=1.2.0, <1.3.0` | `>=1.2 <2.0.0` |

Sources: node-semver,[^6] the Cargo book[^7] and Composer.[^11]

The rows show two differences. A bare version in Cargo is a caret requirement, but in npm it is an exact match. The tilde with two numbers in Composer allows the next minor versions, while npm and Cargo allow only patch releases. The X-range `1.x` of npm and the wildcard `1.*` of Cargo both mean 1.0.0 or higher and lower than 2.0.0.[^6][^7] The `-0` in the npm upper bound is the lowest pre-release of that version, so that `^1.2.3` does not match 2.0.0-alpha.

Other systems use SemVer numbers with other rules.

- **Go modules.** Each version starts with the letter "v", for example v1.12.134 or v2.0.9+meta. Build metadata is ignored when versions are compared.[^9] From v2, the module path must end with a major version suffix such as `/v2`. This implements the import compatibility rule: "If an old package and a new package have the same import path, the new package must be backwards compatible with the old package."[^9] A pseudo-version such as `v0.0.0-20191109021931-daa7c04131f5` names a commit that has no tag. The go command picks versions with minimal version selection, and it does not keep a lock file.[^9]
- **NuGet.** NuGet 4.3.0 and later support SemVer 2.0.0. A plain range such as "1.0" means 1.0 or higher, not a caret range. NuGet has a fourth number, Revision. It compares pre-release labels without case, so 1.0.0-alpha and 1.0.0-Alpha are equal.[^13]
- **Python packages.** PEP 440 accepts the form MAJOR.MINOR.PATCH. A semantic version with a hyphen or a plus sign is not allowed in the public version field.[^12] The operator `~=` is the "compatible release" clause: `~= 1.4.5` means `>= 1.4.5, == 1.4.*`.[^12]

## Pitfalls

- **String sort.** A string sort puts 1.10.0 before 1.9.0, beta.11 before beta.2, and 1.0.0 before 1.0.0-alpha. Parse the parts and sort with the rules of item 11.
- **The "v" prefix.** "v1.2.3" is not a semantic version.[^1] Some tools remove the "v" (node-semver, semver4j). Others reject it (the Rust crate `semver`). Go requires it.[^6][^9][^10] Remove the prefix from a tag name in one place, before the parse.
- **Leading zeros.** 01.2.3 and 1.0.0-01 are not valid. A parser that calls an integer conversion on each part accepts them with no error.
- **Unicode digits.** In a Python `str` pattern, `\d` matches any Unicode decimal digit. Other regex engines can do the same. Use `[0-9]`, or the flag `re.ASCII`.[^17]
- **Build metadata and equality.** Two versions that differ only in build metadata have the same precedence.[^1] They are still different texts. Decide if your map keys and sets use the text or the precedence. The Rust crate `semver` makes them unequal for `==` and equal for `cmp_precedence`.[^18]
- **Pre-releases in ranges.** npm and Cargo exclude pre-release versions from a range unless the range asks for one. In npm, a pre-release matches only if a comparator with the same major, minor and patch has a pre-release tag. So `>1.2.3-alpha.3` matches 1.2.3-alpha.7 but not 3.4.5-alpha.9.[^6][^7]
- **Caret below 1.0.0.** `^0.2.3` does not allow 0.3.0, and `^0.0.3` allows only 0.0.3 and its pre-releases. An author who releases 0.3.0 with an additive change gets no automatic upgrade for users.[^6][^7]
- **Breaking changes in minor releases.** The version number is a promise from a person. Some changes break users with no change to a function name. The Cargo SemVer guide classes the removal of a public item, a public field added to a struct that has no private fields, and a variant added to an enum as major changes.[^8] The guide also says: "These are only guidelines".[^8] If a minor release breaks the API by accident, the FAQ says to release a minor version that restores compatibility. The released version MUST NOT change.[^1]
- **Upper bounds that are too tight.** A requirement such as `>=2.0, <2.4` can make the dependency graph unsolvable when another package needs 2.4. The Cargo book advises an upper bound no lower than the next incompatible version, and to use the lock file for tighter control.[^7]
- **Lock files.** A range lets the resolver pick a later patch or minor version. Two builds of the same code can get different dependencies. A lock file such as Cargo.lock records the exact versions.[^7] Go has no lock file. Minimal version selection is deterministic, and the build list has the minimum versions that satisfy all requirements.[^9]
- **System.Version is not SemVer.** In .NET, `System.Version` represents the version number of an assembly, an operating system or the runtime.[^16] It has up to four numbers and no pre-release. Use `NuGet.Versioning` or another SemVer type.

## Tools and libraries

| Language | Verdict | SemVer support |
|---|---|---|
| Python | idiom | the official regex with `re`, and a tuple key for `sorted`[^17] |
| JavaScript, TypeScript | idiom | the numbered regex in the language; the npm package `semver` (node-semver) for ranges[^6] |
| Go | idiom | the official regex with `regexp`; the module `golang.org/x/mod/semver` with the "v" prefix[^10] |
| Rust | library | the crate `semver`: `Version`, `VersionReq` with Cargo rules[^18] |
| Java | library | semver4j: `Semver.parse`, `satisfies` with npm-style ranges[^19] |
| C# | library | `System.Version` is not SemVer; the package `NuGet.Versioning` has `NuGetVersion.Parse` and `VersionComparer`[^13][^16] |
| C++ | idiom | `std::regex` with the numbered regex and a comparator |
| PowerShell | stdlib | the type accelerator `[semver]` for `System.Management.Automation.SemanticVersion`[^14][^15] |
| Bash | library | the command `semver` of the npm package `semver`[^6] |

Package managers are the main users of the standard. npm and Cargo resolve ranges with the rules of node-semver and the crate `semver`.[^6][^7] The go command uses the rules of the Go modules reference.[^9] NuGet uses `NuGet.Versioning`.[^13]

## References
