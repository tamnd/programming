// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

use std::fmt;

/// A version MAJOR.MINOR.PATCH. The derived Ord compares the fields in the order
/// of their declaration, so the most important field comes first.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Version {
    pub major: u32,
    pub minor: u32,
    pub patch: u32,
}

impl Version {
    pub fn new(major: u32, minor: u32, patch: u32) -> Self {
        Version { major, minor, patch }
    }

    /// Reads a version such as "1.10.0". Returns None if the text is not three numbers.
    pub fn parse(text: &str) -> Option<Self> {
        let mut it = text.split('.').map(|p| p.parse::<u32>().ok());
        let v = Version::new(it.next()??, it.next()??, it.next()??);
        if it.next().is_some() {
            return None;
        }
        Some(v)
    }
}

impl fmt::Display for Version {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}.{}.{}", self.major, self.minor, self.patch)
    }
}

/// Sorts the versions by their natural order. Panics on a string that is not a version.
pub fn sort_versions(versions: &[&str]) -> Vec<String> {
    let mut vs: Vec<Version> = versions
        .iter()
        .map(|s| Version::parse(s).expect("a version MAJOR.MINOR.PATCH"))
        .collect();
    vs.sort();
    vs.iter().map(Version::to_string).collect()
}
