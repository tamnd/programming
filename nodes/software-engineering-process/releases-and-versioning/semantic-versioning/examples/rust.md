```rust run id=precedence deps=deps/rust/Cargo.lock
use semver::{BuildMetadata, Version};

fn main() {
    let mut versions: Vec<Version> = [
        "1.0.0", "1.0.0-rc.1", "1.0.0-beta.11", "1.0.0-beta.2",
        "1.0.0-beta", "1.0.0-alpha.beta", "1.0.0-alpha.1", "1.0.0-alpha",
    ]
    .iter()
    .map(|text| Version::parse(text).expect("valid version"))
    .collect();
    versions.sort();
    let line: Vec<String> = versions.iter().map(Version::to_string).collect();
    println!("{}", line.join(" < "));

    let a = Version::parse("1.4.2+build.7").expect("valid version");
    let b = Version::parse("1.4.2+build.8").expect("valid version");
    println!("a == b: {}", a == b);
    println!("same precedence: {}", a.cmp_precedence(&b).is_eq());
    println!("build of a: {}, empty: {}", a.build, a.build == BuildMetadata::EMPTY);

    for text in ["v1.2.3", "1.2", "01.2.3", "1.2.3-01", "1.2.3-alpha..1"] {
        match Version::parse(text) {
            Ok(version) => println!("{text}: parsed as {version}"),
            Err(error) => println!("{text}: {error}"),
        }
    }
}
```

```output for=precedence
1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0
a == b: false
same precedence: true
build of a: build.7, empty: false
v1.2.3: unexpected character 'v' while parsing major version number
1.2: unexpected end of input while parsing minor version number
01.2.3: invalid leading zero in major version number
1.2.3-01: invalid leading zero in pre-release identifier
1.2.3-alpha..1: empty identifier segment in pre-release identifier
```

`VersionReq` reads a requirement with the rules of Cargo. A bare version is a caret requirement. A requirement matches a pre-release only if one of its comparators has a pre-release with the same major, minor and patch.

```rust run id=requirements deps=deps/rust/Cargo.lock
use semver::{Version, VersionReq};

fn main() -> Result<(), semver::Error> {
    let candidates = ["0.2.9", "0.3.0", "1.2.9", "1.3.0", "1.9.0", "2.0.0-rc.1", "2.0.0"];
    for text in ["1.2.3", "^0.2.3", "0.0.3", "~1.2", "~1", "1.*", ">=1.2, <1.5", ">=2.0.0-rc.1"] {
        let requirement = VersionReq::parse(text)?;
        let mut matches = Vec::new();
        for candidate in candidates {
            if requirement.matches(&Version::parse(candidate)?) {
                matches.push(candidate);
            }
        }
        let shown = requirement.to_string();
        println!("{text:<14} reads as {shown:<16} matches {matches:?}");
    }

    let beta = VersionReq::parse("1.0.0-beta")?;
    for candidate in ["1.0.0-alpha", "1.0.0-rc.1", "1.0.0", "1.4.0", "1.0.1-alpha"] {
        println!("1.0.0-beta matches {candidate}: {}", beta.matches(&Version::parse(candidate)?));
    }
    Ok(())
}
```

```output for=requirements
1.2.3          reads as ^1.2.3           matches ["1.2.9", "1.3.0", "1.9.0"]
^0.2.3         reads as ^0.2.3           matches ["0.2.9"]
0.0.3          reads as ^0.0.3           matches []
~1.2           reads as ~1.2             matches ["1.2.9"]
~1             reads as ~1               matches ["1.2.9", "1.3.0", "1.9.0"]
1.*            reads as 1.*              matches ["1.2.9", "1.3.0", "1.9.0"]
>=1.2, <1.5    reads as >=1.2, <1.5      matches ["1.2.9", "1.3.0"]
>=2.0.0-rc.1   reads as >=2.0.0-rc.1     matches ["2.0.0-rc.1", "2.0.0"]
1.0.0-beta matches 1.0.0-alpha: false
1.0.0-beta matches 1.0.0-rc.1: true
1.0.0-beta matches 1.0.0: true
1.0.0-beta matches 1.4.0: true
1.0.0-beta matches 1.0.1-alpha: false
```

- Verdict: library. The Rust standard library has no SemVer type. The examples use the crate `semver` 1.0.28. Its documentation calls it a parser and evaluator for the Cargo flavour of Semantic Versioning.
- `Version` implements `Ord` with SemVer precedence, so `sort` gives the order of item 11. The operator `==` treats 1.4.2+build.7 and 1.4.2+build.8 as different versions. `cmp_precedence` ignores the build metadata, so it finds them equal.
- `Version::parse` rejects the "v" prefix, a missing patch number and leading zeros, with an error that names the problem.
- `VersionReq::parse` reads Cargo requirements. `VersionReq::matches` applies the Cargo rule for pre-releases.
