---
tag: P0JBJ3
kind: feature
owner: python
slug: the-release-cycle
name: The release cycle
aliases: [PEP 602, annual release cycle, Python release schedule, bugfix and security support, end of life, sys.version_info, sys.hexversion]
area: languages/python/versions
level: core
summary: >-
  CPython makes a feature version every October. Each one gets bugfix releases for 2 years, then security releases for 3 more years, and reaches end of life five years after its first release.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0602/", locator: "PEP 602, Implementation"}
see_also: [python/3.11, python/3.12, python/3.13, python/3.14, python/3.15]
sources:
  - url: https://peps.python.org/pep-0602/
    title: "PEP 602: Annual Release Cycle for Python"
    locator: Abstract; Implementation; Annual release cadence
    accessed: 2026-11-18
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions; Unsupported versions; Status key
    accessed: 2026-11-18
  - url: https://devguide.python.org/developer-workflow/development-cycle/
    title: Development cycle, Python Developer's Guide
    locator: Development cycle; Branches; Stages
    accessed: 2026-11-18
  - url: https://peps.python.org/api/release-cycle.json
    title: Python release cycle data, peps.python.org
    locator: the entries 3.10 to 3.16
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/sys.html#sys.version_info
    title: sys, System-specific parameters and functions
    locator: sys.version_info; sys.hexversion
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/c-api/apiabiversion.html
    title: API and ABI Versioning, Python/C API Reference Manual
    locator: Build-time version constants; Bit-packing macros
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0387/
    title: "PEP 387: Backwards Compatibility Policy"
    locator: Making Incompatible Changes
    accessed: 2026-11-18
  - url: https://go.dev/doc/devel/release
    title: Release History, The Go Programming Language
    locator: Release Policy
    accessed: 2026-11-18
  - url: https://doc.rust-lang.org/book/appendix-07-nightly-rust.html
    title: "Appendix G: How Rust is Made and Nightly Rust, The Rust Programming Language"
    locator: Choo, Choo! Release Channels and Riding the Trains
    accessed: 2026-11-18
  - url: https://nodejs.org/en/about/previous-releases
    title: Node.js Releases
    locator: Release phases
    accessed: 2026-11-18
  - url: https://openjdk.org/jeps/322
    title: "JEP 322: Time-Based Release Versioning"
    locator: Description
    accessed: 2026-11-18
---

## Synopsis

A CPython version number has the form `major.minor.micro`. A new minor version is a feature release, and a new micro version is a bugfix release.[^3] One feature version passes through these phases.[^1] [^2]

```text
3.X.0 development     about 17 months
  pre-alpha           5 months, while 3.(X-1).0 is in beta and rc; no releases
  alpha               7 months: 3.X.0a1, 3.X.0a2, ...
  beta                3 months, 4 releases: 3.X.0b1 to 3.X.0b4; no new features
  release candidate   2 months, 2 or more releases: 3.X.0rc1, 3.X.0rc2
3.X.0 final           October
bugfix                24 months: 3.X.1, 3.X.2, ..., about every 2 months, with installers
security              36 months: source-only releases, only when needed
end-of-life           five years after 3.X.0; the branch is frozen
```

The devguide gives each version one of five statuses: `feature`, `prerelease`, `bugfix`, `security` and `end-of-life`.[^2] On 2026-11-18, the statuses were these.[^2] [^4]

| Version | Status | First release | End of life | Release manager |
|---|---|---|---|---|
| 3.16 | feature | 2027-10-06 (planned) | 2032-10 | Savannah Ostrowski |
| [3.15](/python/3.15/) | bugfix | 2026-10-09 | 2031-10 | Hugo van Kemenade |
| [3.14](/python/3.14/) | bugfix | 2025-10-07 | 2030-10 | Hugo van Kemenade |
| [3.13](/python/3.13/) | security | 2024-10-07 | 2029-10 | Thomas Wouters |
| [3.12](/python/3.12/) | security | 2023-10-02 | 2028-10 | Thomas Wouters |
| [3.11](/python/3.11/) | security | 2022-10-24 | 2027-10 | Pablo Galindo Salgado |
| 3.10 | end-of-life | 2021-10-04 | 2026-10-01 | Pablo Galindo Salgado |

## Semantics

**r1.** A new major version is rare, and comes only when strongly incompatible changes are necessary. A new minor version is a feature release, made once a year from the main branch. A new micro version is a bugfix release, made about every two months from a maintenance branch.[^3]

**r2.** Each release has a tag of the form `vX.Y.ZTN` in the source repository. `X`, `Y` and `Z` are the major, minor and micro versions. `T` is the release level: `a` for alpha, `b` for beta, `rc` for release candidate, and nothing for a final release. `N` is the serial number of the release in that level.[^3] Example: [`tags`](#tags).

**r3.** From Python 3.9, a feature version comes out every twelve months, in October.[^1] The development of 3.(X+1).0 starts when 3.X.0 beta 1 is released, so feature versions are twelve months apart.[^1]

**r4.** A feature version is developed for about 17 months. The first five months overlap with the beta and release candidate stages of the previous version, and have no releases. Then come seven months of alpha releases, three months of four beta releases, and two months of two or more release candidates. The phase ends with the final release 3.X.0.[^1]

**r5.** Before beta 1, the main branch accepts new features, bug fixes and security fixes. After beta 1, it accepts no new features.[^2] The maintenance branch of the version is made at beta 1, which is the feature freeze.[^3] During the release candidates, only bug fixes that other core team members reviewed can go in, and they must be severe, such as crashes. When the final release is made, only the release manager can change the branch.[^3]

**r6.** After 3.X.0, the version gets bugfix releases for two years. These are full releases, with sources and installers for Windows and macOS, about every other month. For the next three years, the version gets only security fixes, in source-only releases with no fixed schedule. The last source-only release is five years after 3.X.0.[^1]

**r7.** The two years of full support apply from Python 3.13. Python 3.9 to 3.12 have 18 months of full support and then three and a half years of security fixes.[^1] [^2] Example: [`phase`](#phase).

**r8.** A maintenance branch must not break compatibility between micro releases, such as 3.12.1 and 3.12.2. Low-risk changes, such as bug fixes, test improvements and documentation edits, can be backported. New features, changes of semantics and performance improvements are not backported as a matter of course.[^3]

**r9.** A version moves to security mode some time after the next feature version comes out, usually after one more bugfix release. For example, 3.11 went to security mode after 3.11.9, which came after 3.12.2.[^3] A security branch takes only fixes for issues that attackers can exploit, such as crashes and privilege escalation. Only release managers can merge to it.[^3]

**r10.** At end of life, the branch is frozen and removed from the repository. A tag with the same name, such as `3.8`, records its final state.[^3]

**r11.** A feature that will be removed must give a warning in at least two minor versions before the removal. PEP 387 prefers five years of warnings, which is the lifetime of a minor version.[^7]

**r12.** `sys.version_info` holds the five parts of the running version: `major`, `minor`, `micro`, `releaselevel` and `serial`. The release level is `'alpha'`, `'beta'`, `'candidate'` or `'final'`.[^5] `sys.hexversion` packs the same parts into one integer that increases with each version, including the pre-releases.[^5] The release level uses 4 bits, with the values `0xA`, `0xB`, `0xC` and `0xF`.[^6] Example: [`tags`](#tags).

## Examples

This parses release tags in the form of rule r2, and packs each one into an integer in the same way as `sys.hexversion`. The packed values sort in release order, and the last line checks the packing against the running interpreter.[^6]

```python run id=tags
import re
import sys

LEVELS = {"a": ("alpha", 0xA), "b": ("beta", 0xB), "rc": ("candidate", 0xC), "": ("final", 0xF)}


def parse(tag):
    major, minor, micro, level, serial = re.fullmatch(
        r"v(\d+)\.(\d+)\.(\d+)(a|b|rc|)(\d*)", tag
    ).groups()
    name, code = LEVELS[level]
    info = (int(major), int(minor), int(micro), name, int(serial or 0))
    packed = info[0] << 24 | info[1] << 16 | info[2] << 8 | code << 4 | info[4]
    return info, packed


tags = ["v3.14.0a1", "v3.14.0b1", "v3.14.0rc1", "v3.14.0", "v3.14.1"]
for tag in tags:
    info, packed = parse(tag)
    print(f"{tag:<11}{info} {packed:#010x}")

print(sorted(tags, key=lambda t: parse(t)[1]) == tags)
v = sys.version_info
code = {"alpha": 0xA, "beta": 0xB, "candidate": 0xC, "final": 0xF}[v.releaselevel]
print(sys.hexversion == (v.major << 24 | v.minor << 16 | v.micro << 8 | code << 4 | v.serial))
```

```output for=tags
v3.14.0a1  (3, 14, 0, 'alpha', 1) 0x030e00a1
v3.14.0b1  (3, 14, 0, 'beta', 1) 0x030e00b1
v3.14.0rc1 (3, 14, 0, 'candidate', 1) 0x030e00c1
v3.14.0    (3, 14, 0, 'final', 0) 0x030e00f0
v3.14.1    (3, 14, 1, 'final', 0) 0x030e01f0
True
True
```

This computes the planned phase of each version on a date, from the first release, the end of life and rules r6 and r7. The real end of the bugfix phase is the date of the last bugfix release, which the release manager picks, so the computed date is only close to it (rule r9).

```python run id=phase
from datetime import date

# First release and end of life, from the release-cycle data.
RELEASES = {
    "3.10": ("2021-10-04", "2026-10"),
    "3.11": ("2022-10-24", "2027-10"),
    "3.12": ("2023-10-02", "2028-10"),
    "3.13": ("2024-10-07", "2029-10"),
    "3.14": ("2025-10-07", "2030-10"),
    "3.15": ("2026-10-09", "2031-10"),
}


def add_months(day, months):
    years, month = divmod(day.month - 1 + months, 12)
    return date(day.year + years, month + 1, 1)


def phase(version, today):
    first = date.fromisoformat(RELEASES[version][0])
    eol = date.fromisoformat(RELEASES[version][1] + "-01")
    full_support = 24 if int(version.split(".")[1]) >= 13 else 18
    if today < first:
        return "feature or prerelease"
    if today < add_months(first, full_support):
        return "bugfix"
    if today < eol:
        return "security"
    return "end-of-life"


today = date(2026, 11, 18)
for version in RELEASES:
    print(version, phase(version, today))
print(add_months(date(2025, 10, 7), 24), add_months(date(2025, 10, 7), 60))
```

```output for=phase
3.10 end-of-life
3.11 security
3.12 security
3.13 security
3.14 bugfix
3.15 bugfix
2027-10-01 2030-10-01
```

The results agree with the status table in the synopsis. For 3.14, the last line gives the planned start of the security phase and the end of life.

Code that must run on more than one version tests `sys.version_info` against a tuple. Here a program uses `tomllib` on 3.11 and later, and the package `tomli`, which has the same interface, on older versions. The names of the release levels sort in alphabetical order, so a comparison with a full 5-tuple also orders the pre-releases correctly.

```python run id=gate
import sys

if sys.version_info >= (3, 11):
    import tomllib
else:
    import tomli as tomllib

print(tomllib.loads('requires-python = ">=3.11"'))
print(sys.version_info >= (3, 11), sys.version_info[:2] >= (3, 11))
print(sys.version_info > (3, 11, 0, "beta"), (3, 11, 0, "beta") < (3, 11, 0, "final"))
```

```output for=gate
{'requires-python': '>=3.11'}
True True
True True
```

## Pitfalls

### Comparing versions as strings

A version string compares character by character. So `"3.10"` is smaller than `"3.9"`, because the character `1` comes before `9`.

```python wrong id=p1
supported = ["3.9", "3.10", "3.11"]
print(max(supported))
print("3.10" >= "3.9")
```

```output for=p1
3.9
False
```

Convert each part to an integer and compare tuples. Or compare `sys.version_info` with a tuple of integers, as in [`gate`](#gate).

```python run id=p1-fix
supported = ["3.9", "3.10", "3.11"]


def key(version):
    return tuple(int(part) for part in version.split("."))


print(max(supported, key=key))
print(key("3.10") >= key("3.9"))
```

```output for=p1-fix
3.11
True
```

### Expecting installers for a version in security mode

A version in security mode gets no more binary installers from python.org. Its releases are source-only.[^2] For example, 3.11.9 was the last 3.11 release with installers for Windows and macOS (rule r9). A program that needs a later security fix and an installer from python.org must move to a version in bugfix mode.

### Counting two years of bugfix support for every version

The two years of bugfix support start with 3.13. For 3.9 to 3.12, the bugfix phase was 18 months (rule r7). So 3.12, which came out on 2023-10-02, had its last bugfix release in April 2025, not in October 2025 (see [Python 3.12](/python/3.12/)). The end of life is five years after the first release for all of them.[^1]

### Reading the support dates as exact

Dates that the status page shows in italics are planned, and the release managers can change them.[^2] The end of life of 3.10 was 2026-10-01, and of 3.9 it was 2025-10-31.[^4] Check the status page or the release-cycle data before you plan an upgrade.

## Comparison

**Go** makes major releases, such as Go 1.26.0 on 2026-02-10 and Go 1.27.0 on 2026-08-19. Each major release is supported until there are two newer major releases, and minor revisions such as Go 1.6.1 fix critical problems and security problems.[^8] So a Go release gets about one year of fixes, against five years for a Python version.

**Rust** uses a train model with three channels: nightly, beta and stable. Every six weeks, the beta branch becomes the next stable release, and a new beta branches off from nightly.[^9]

**Node.js** puts each major version in the Current phase for six months. Up to Node.js 26, the odd-numbered versions then end, and the even-numbered versions move to long-term support (LTS). From Node.js 27, the cycle is annual, and every major version moves to LTS. An LTS version typically gets fixes for critical bugs for a total of 30 months.[^10]

**Java** increments the feature version of the JDK every six months, for example JDK 10 in March 2018 and JDK 11 in September 2018.[^11]

Python is the only one of these four with one fixed cycle of five years for every version, and with a separate phase that gives only security fixes.

## References
