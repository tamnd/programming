```typescript run id=caret
type Version = {
  major: number;
  minor: number;
  patch: number;
  prerelease: string[];
};

const SEMVER = /^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$/;

function parse(text: string): Version | undefined {
  const match = SEMVER.exec(text);
  if (match === null) return undefined;
  const [, major, minor, patch, prerelease] = match;
  return {
    major: Number(major),
    minor: Number(minor),
    patch: Number(patch),
    prerelease: prerelease === undefined ? [] : prerelease.split("."),
  };
}

function compareIdentifiers(a: string, b: string): number {
  const an = /^[0-9]+$/.test(a);
  const bn = /^[0-9]+$/.test(b);
  if (an && bn) return Math.sign(Number(a) - Number(b));
  if (an !== bn) return an ? -1 : 1;
  return a < b ? -1 : a > b ? 1 : 0;
}

function compare(a: Version, b: Version): number {
  const core = Math.sign(a.major - b.major) || Math.sign(a.minor - b.minor) || Math.sign(a.patch - b.patch);
  if (core !== 0) return core;
  if (a.prerelease.length === 0 || b.prerelease.length === 0) {
    return Number(a.prerelease.length === 0) - Number(b.prerelease.length === 0);
  }
  for (let i = 0; i < Math.min(a.prerelease.length, b.prerelease.length); i++) {
    const result = compareIdentifiers(a.prerelease[i], b.prerelease[i]);
    if (result !== 0) return result;
  }
  return Math.sign(a.prerelease.length - b.prerelease.length);
}

const sameCore = (a: Version, b: Version): boolean =>
  a.major === b.major && a.minor === b.minor && a.patch === b.patch;

function caretUpper(v: Version): Version {
  if (v.major > 0) return { major: v.major + 1, minor: 0, patch: 0, prerelease: ["0"] };
  if (v.minor > 0) return { major: 0, minor: v.minor + 1, patch: 0, prerelease: ["0"] };
  return { major: 0, minor: 0, patch: v.patch + 1, prerelease: ["0"] };
}

// npm rule: a pre-release matches only if the lower bound is a
// pre-release of the same major, minor and patch.
function satisfiesCaret(candidate: Version, lower: Version): boolean {
  if (candidate.prerelease.length > 0 && !(lower.prerelease.length > 0 && sameCore(candidate, lower))) {
    return false;
  }
  return compare(candidate, lower) >= 0 && compare(candidate, caretUpper(lower)) < 0;
}

const candidates = ["1.2.3-beta.4", "1.2.3", "1.2.9", "1.3.0-alpha", "2.0.0-alpha", "2.0.0", "0.2.9", "0.3.0"];
for (const range of ["1.2.3", "1.2.3-beta.2", "0.2.3"]) {
  const lower = parse(range);
  if (lower === undefined) continue;
  const matching = candidates.filter((text) => {
    const candidate = parse(text);
    return candidate !== undefined && satisfiesCaret(candidate, lower);
  });
  console.log(`^${range}: ${matching.join(" ")}`);
}
console.log(`v1.2.3: ${parse("v1.2.3") === undefined ? "invalid" : "valid"}`);
```

```output for=caret
^1.2.3: 1.2.3 1.2.9
^1.2.3-beta.2: 1.2.3-beta.4 1.2.3 1.2.9
^0.2.3: 0.2.9
v1.2.3: invalid
```

- Verdict: idiom. TypeScript has no SemVer type of its own. The numbered regex works in TypeScript as in JavaScript, and a type describes the parsed parts.
- The function `satisfiesCaret` follows the npm rules for a caret range with a full version: the upper bound moves the left-most non-zero number, and `-0` excludes the pre-releases of the upper bound.
- For ranges with all the npm forms, use the npm package `semver`.
- `Number` loses precision above 2^53. Use `BigInt` or compare decimal strings when a version can have larger numbers.
