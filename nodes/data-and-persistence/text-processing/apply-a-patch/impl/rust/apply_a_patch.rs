// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

//! Applies a unified diff to a text, with no fuzz and no offset.

use std::fmt;
use std::fs;
use std::io;
use std::path::Path;

/// The error of `apply_patch` and `apply_patch_file`. `line` is the number of
/// the line of the patch where the error is.
#[derive(Debug)]
pub enum PatchError {
    /// A file cannot be read.
    Io(io::Error),
    /// The patch is not a correct unified diff.
    Malformed { line: usize },
    /// A hunk does not match the text.
    Mismatch { line: usize },
}

impl fmt::Display for PatchError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            PatchError::Io(e) => write!(f, "{e}"),
            PatchError::Malformed { line } => write!(f, "malformed at line {line} of the patch"),
            PatchError::Mismatch { line } => write!(f, "mismatch at line {line} of the patch"),
        }
    }
}

impl std::error::Error for PatchError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            PatchError::Io(e) => Some(e),
            _ => None,
        }
    }
}

impl From<io::Error> for PatchError {
    fn from(e: io::Error) -> Self {
        PatchError::Io(e)
    }
}

/// Splits text after each LF. Each line keeps its LF, and a last line with no
/// LF is a line too.
pub fn split_lines(text: &str) -> Vec<&str> {
    text.split_inclusive('\n').collect()
}

/// Reads a number and an optional comma and count from the start of s. A
/// missing count is 1.
fn range(s: &str) -> Option<(usize, usize, &str)> {
    let digits = |s: &str| s.bytes().take_while(u8::is_ascii_digit).count();
    let n = digits(s);
    let start = s.get(..n)?.parse().ok()?;
    let mut rest = &s[n..];
    let mut count = 1;
    if let Some(r) = rest.strip_prefix(',') {
        let n = digits(r);
        count = r.get(..n)?.parse().ok()?;
        rest = &r[n..];
    }
    Some((start, count, rest))
}

/// Reads a hunk header such as `@@ -1,3 +1,4 @@`, and returns the old start,
/// the old count and the new count.
fn hunk_header(line: &str) -> Option<(usize, usize, usize)> {
    let (old_start, old_count, rest) = range(line.strip_prefix("@@ -")?)?;
    let (_, new_count, rest) = range(rest.strip_prefix(" +")?)?;
    rest.starts_with(" @@")
        .then_some((old_start, old_count, new_count))
}

/// Returns the text that the unified diff `patch` makes from the text `old`.
/// Each hunk must match the lines of `old` at the start of its old range.
pub fn apply_patch(old: &str, patch: &str) -> Result<String, PatchError> {
    let lines = split_lines(old);
    // The lines of the patch, with no LF. A CR stays in its line.
    let p: Vec<&str> = split_lines(patch)
        .into_iter()
        .map(|l| l.strip_suffix('\n').unwrap_or(l))
        .collect();
    let mut out = String::new();
    let mut cur = 0; // the index of the next line of old to use
    let mut hunks = 0;
    let mut i = 0;
    while i < p.len() {
        if !p[i].starts_with("@@") {
            i += 1; // a header line, or another line outside a hunk
            continue;
        }
        let (old_start, old_count, new_count) =
            hunk_header(p[i]).ok_or(PatchError::Malformed { line: i + 1 })?;
        hunks += 1;
        // An empty range starts after the line old_start.
        let pos = if old_count > 0 {
            old_start
                .checked_sub(1)
                .ok_or(PatchError::Malformed { line: i + 1 })?
        } else {
            old_start
        };
        if pos < cur || pos > lines.len() {
            return Err(PatchError::Mismatch { line: i + 1 });
        }
        out.extend(lines[cur..pos].iter().copied());
        cur = pos;
        i += 1;
        // The mark, the text, the number in the patch and the end of each
        // line: an LF, or "" before a backslash line.
        let mut body: Vec<(u8, &str, usize, &str)> = Vec::new();
        let (mut old_left, mut new_left) = (old_count, new_count);
        while old_left > 0 || new_left > 0 {
            let Some(&line) = p.get(i) else {
                return Err(PatchError::Malformed { line: i });
            };
            let (mark, text) = match line.as_bytes().first() {
                None => (b' ', ""),
                Some(&m) => (m, &line[1..]),
            };
            let malformed = PatchError::Malformed { line: i + 1 };
            match mark {
                b'\\' => {
                    let last = body.last_mut().ok_or(malformed)?;
                    last.3 = ""; // the line before has no LF
                    i += 1;
                    continue;
                }
                b' ' | b'-' | b'+' => {}
                _ => return Err(malformed),
            }
            if mark != b'+' {
                old_left = old_left.checked_sub(1).ok_or(malformed)?;
            }
            if mark != b'-' {
                new_left = new_left
                    .checked_sub(1)
                    .ok_or(PatchError::Malformed { line: i + 1 })?;
            }
            body.push((mark, text, i + 1, "\n"));
            i += 1;
        }
        if i < p.len() && p[i].starts_with('\\') {
            if let Some(last) = body.last_mut() {
                last.3 = "";
                i += 1;
            }
        }
        for (mark, text, number, end) in body {
            if mark != b'+' {
                let same = lines.get(cur).is_some_and(|l| {
                    l.len() == text.len() + end.len() && l.starts_with(text) && l.ends_with(end)
                });
                if !same {
                    return Err(PatchError::Mismatch { line: number });
                }
                cur += 1;
            }
            if mark != b'-' {
                out.push_str(text);
                out.push_str(end);
            }
        }
    }
    if hunks == 0 {
        return Err(PatchError::Malformed { line: p.len() });
    }
    out.extend(lines[cur..].iter().copied());
    Ok(out)
}

/// Returns `apply_patch` of the two UTF-8 files. A file that does not exist
/// gives `PatchError::Io` with an error of the kind `NotFound`.
pub fn apply_patch_file(
    path: impl AsRef<Path>,
    patch_path: impl AsRef<Path>,
) -> Result<String, PatchError> {
    let old = fs::read_to_string(path)?;
    let patch = fs::read_to_string(patch_path)?;
    apply_patch(&old, &patch)
}
