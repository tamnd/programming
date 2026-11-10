```python run id=unicode-digits
import re

CORE = r"^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)$"

text = "1٠.2.3"
print(text, [f"U+{ord(character):04X}" for character in text])
print("str pattern:", bool(re.match(CORE, text)))
print("re.ASCII:", bool(re.match(CORE, text, re.ASCII)))
print("int() of the major part:", int(text.split(".")[0]))
```

```output for=unicode-digits
1٠.2.3 ['U+0031', 'U+0660', 'U+002E', 'U+0032', 'U+002E', 'U+0033']
str pattern: True
re.ASCII: False
int() of the major part: 10
```

The `dataclass` below keeps the build metadata in a field with `compare=False`. So `==` and `hash` ignore it, and two builds of one version fall into one set entry. The order comes from a key with the rules of item 11.

```python run id=version-class
import functools
import re
from dataclasses import dataclass, field

SEMVER = re.compile(
    r"^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)"
    r"(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?"
    r"(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$",
    re.ASCII,
)


@functools.total_ordering
@dataclass(frozen=True)
class Version:
    major: int
    minor: int
    patch: int
    prerelease: tuple[str, ...] = ()
    build: tuple[str, ...] = field(default=(), compare=False)

    @classmethod
    def parse(cls, text):
        match = SEMVER.match(text)
        if match is None:
            raise ValueError(f"not a semantic version: {text!r}")
        prerelease = match["prerelease"]
        build = match["buildmetadata"]
        return cls(
            int(match["major"]),
            int(match["minor"]),
            int(match["patch"]),
            tuple(prerelease.split(".")) if prerelease else (),
            tuple(build.split(".")) if build else (),
        )

    def _key(self):
        identifiers = tuple((0, int(part)) if part.isdigit() else (1, part) for part in self.prerelease)
        return (self.major, self.minor, self.patch, 0 if self.prerelease else 1, identifiers)

    def __lt__(self, other):
        return self._key() < other._key()

    def __str__(self):
        text = f"{self.major}.{self.minor}.{self.patch}"
        if self.prerelease:
            text += "-" + ".".join(self.prerelease)
        if self.build:
            text += "+" + ".".join(self.build)
        return text


releases = [Version.parse(text) for text in ["2.0.0", "1.10.0", "1.9.0", "1.10.0-rc.1", "1.10.0+linux", "1.10.0+macos"]]
print([str(version) for version in sorted(releases)])
print(Version.parse("1.10.0+linux") == Version.parse("1.10.0+macos"))
print(sorted(str(version) for version in set(releases)))
print(max(releases))
```

```output for=version-class
['1.9.0', '1.10.0-rc.1', '1.10.0', '1.10.0+linux', '1.10.0+macos', '2.0.0']
True
['1.10.0', '1.10.0-rc.1', '1.9.0', '2.0.0']
2.0.0
```

The program below computes the npm bounds of a caret range for a full version. The upper bound moves the left-most non-zero number, and `-0` makes it the lowest pre-release of that version. A candidate that is a pre-release matches only if the lower bound is a pre-release with the same major, minor and patch. This is the npm rule for pre-releases.

```python run id=caret
def split(text):
    core, _, prerelease = text.partition("-")
    return tuple(int(part) for part in core.split(".")), prerelease


def precedence_key(text):
    core, prerelease = split(text)
    if not prerelease:
        return (*core, 1, ())
    identifiers = tuple((0, int(part)) if part.isdigit() else (1, part) for part in prerelease.split("."))
    return (*core, 0, identifiers)


def caret_upper(lower):
    (major, minor, patch), _ = split(lower)
    if major > 0:
        return f"{major + 1}.0.0-0"
    if minor > 0:
        return f"0.{minor + 1}.0-0"
    return f"0.0.{patch + 1}-0"


def caret_matches(lower, candidate):
    lower_core, lower_pre = split(lower)
    candidate_core, candidate_pre = split(candidate)
    if candidate_pre and not (lower_pre and candidate_core == lower_core):
        return False
    return precedence_key(lower) <= precedence_key(candidate) < precedence_key(caret_upper(lower))


candidates = ["0.2.9", "0.3.0", "1.2.9", "1.9.0", "2.0.0-alpha", "2.0.0"]
for lower in ["1.2.3", "0.2.3", "0.0.3"]:
    allowed = [candidate for candidate in candidates if caret_matches(lower, candidate)]
    print(f"^{lower} := >={lower} <{caret_upper(lower)} -> {allowed}")
for candidate in ["1.2.3-beta.4", "1.2.3-beta.10", "1.2.3", "1.2.4-beta.2"]:
    print(f"^1.2.3-beta.2 matches {candidate}: {caret_matches('1.2.3-beta.2', candidate)}")
```

```output for=caret
^1.2.3 := >=1.2.3 <2.0.0-0 -> ['1.2.9', '1.9.0']
^0.2.3 := >=0.2.3 <0.3.0-0 -> ['0.2.9']
^0.0.3 := >=0.0.3 <0.0.4-0 -> []
^1.2.3-beta.2 matches 1.2.3-beta.4: True
^1.2.3-beta.2 matches 1.2.3-beta.10: True
^1.2.3-beta.2 matches 1.2.3: True
^1.2.3-beta.2 matches 1.2.4-beta.2: False
```

- Verdict: idiom. The Python standard library has no SemVer type. The official regex with the named groups works with `re`, and a tuple key gives the order of item 11.
- In a `str` pattern, `\d` matches any Unicode decimal digit, and `int()` accepts these digits too. The flag `re.ASCII` makes `\d` match only `[0-9]`.
- A `dataclass` field with `compare=False` is left out of `==` and `hash`. This gives build metadata the SemVer meaning in sets and dictionaries.
- The function `caret_matches` handles only a full version after the caret. npm also accepts partial versions such as `^1.2` and `^0.x`, and the npm package `semver` implements all of these forms.
