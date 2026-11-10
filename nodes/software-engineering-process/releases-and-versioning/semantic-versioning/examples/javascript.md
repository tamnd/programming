```javascript run id=precedence
const SEMVER = /^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$/;

function parse(text) {
  const match = SEMVER.exec(text);
  if (match === null) {
    throw new SyntaxError(`not a semantic version: ${text}`);
  }
  const [, major, minor, patch, prerelease, build] = match;
  return {
    core: [BigInt(major), BigInt(minor), BigInt(patch)],
    prerelease: prerelease === undefined ? [] : prerelease.split("."),
    build: build ?? "",
  };
}

const isNumeric = (identifier) => /^[0-9]+$/.test(identifier);

function compareIdentifiers(a, b) {
  if (isNumeric(a) && isNumeric(b)) {
    const [x, y] = [BigInt(a), BigInt(b)];
    return x < y ? -1 : x > y ? 1 : 0;
  }
  if (isNumeric(a)) return -1;
  if (isNumeric(b)) return 1;
  return a < b ? -1 : a > b ? 1 : 0;
}

function compare(left, right) {
  const a = parse(left);
  const b = parse(right);
  for (let i = 0; i < 3; i++) {
    if (a.core[i] !== b.core[i]) return a.core[i] < b.core[i] ? -1 : 1;
  }
  if (a.prerelease.length === 0 || b.prerelease.length === 0) {
    return (a.prerelease.length === 0) - (b.prerelease.length === 0);
  }
  for (let i = 0; i < Math.min(a.prerelease.length, b.prerelease.length); i++) {
    const result = compareIdentifiers(a.prerelease[i], b.prerelease[i]);
    if (result !== 0) return result;
  }
  return Math.sign(a.prerelease.length - b.prerelease.length);
}

const versions = ["1.0.0", "1.0.0-rc.1", "1.0.0-beta.11", "1.0.0-beta.2", "1.0.0-beta",
  "1.0.0-alpha.beta", "1.0.0-alpha.1", "1.0.0-alpha", "1.10.0", "1.9.0"];

console.log("compare:  ", [...versions].sort(compare).join(" "));
console.log("sort():   ", [...versions].sort().join(" "));
const collator = new Intl.Collator("en", { numeric: true });
console.log("collator: ", [...versions].sort(collator.compare).join(" "));

console.log(compare("1.4.2+build.7", "1.4.2+build.8"));
console.log(compare("9007199254740993.0.0", "9007199254740992.0.0"));
console.log(Number("9007199254740993") === Number("9007199254740992"));

for (const text of ["v1.2.3", "1.2", "01.2.3"]) {
  try {
    parse(text);
  } catch (error) {
    console.log(error.message);
  }
}
```

```output for=precedence
compare:   1.0.0-alpha 1.0.0-alpha.1 1.0.0-alpha.beta 1.0.0-beta 1.0.0-beta.2 1.0.0-beta.11 1.0.0-rc.1 1.0.0 1.9.0 1.10.0
sort():    1.0.0 1.0.0-alpha 1.0.0-alpha.1 1.0.0-alpha.beta 1.0.0-beta 1.0.0-beta.11 1.0.0-beta.2 1.0.0-rc.1 1.10.0 1.9.0
collator:  1.0.0 1.0.0-alpha 1.0.0-alpha.1 1.0.0-alpha.beta 1.0.0-beta 1.0.0-beta.2 1.0.0-beta.11 1.0.0-rc.1 1.9.0 1.10.0
0
1
true
not a semantic version: v1.2.3
not a semantic version: 1.2
not a semantic version: 01.2.3
```

- Verdict: idiom. JavaScript has no SemVer type. The numbered regex of the specification works in ECMAScript, and `Array.prototype.sort` takes a comparison function.
- The default `sort()` compares strings, so it gets the order wrong. A collator with `numeric: true` fixes 1.9.0 and 1.10.0, but it still puts the release before its pre-releases and does not follow item 11.4.
- The example parses numbers as `BigInt`. A `Number` cannot tell 9007199254740993 from 9007199254740992.
- The npm package `semver` (node-semver) calls itself "the semantic versioner for npm". It has `valid`, `compare`, `sort`, `satisfies`, `coerce` and a command-line tool `semver`. Its parser removes a leading "v" or "=" by default.
