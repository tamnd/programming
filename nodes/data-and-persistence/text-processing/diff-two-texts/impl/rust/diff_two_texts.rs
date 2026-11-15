// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

//! Compares two texts line by line.

use std::fs;
use std::io;
use std::path::Path;

/// Splits `text` at each LF. A final LF does not start a new line.
pub fn split_lines(text: &str) -> Vec<&str> {
    // split_terminator skips the empty part after a final LF.
    text.split_terminator('\n').collect()
}

/// Returns the edit script that changes the lines of `old` into the lines of
/// `new`. Each line of the result is a space and a line that stays, a `-` and
/// a line of `old` that goes, or a `+` and a line of `new` that comes, with an
/// LF at the end. The script has the smallest number of `-` and `+` lines.
/// When two scripts are equally short, a `-` comes before a `+`.
pub fn diff_lines(old: &str, new: &str) -> String {
    let (a, b) = (split_lines(old), split_lines(new));
    let (n, m) = (a.len(), b.len());
    let w = m + 1;
    // lcs[i * w + j] is the length of a longest common subsequence of a[i..]
    // and b[j..]. Row n and column m stay 0.
    let mut lcs = vec![0u32; (n + 1) * w];
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            lcs[i * w + j] = if a[i] == b[j] {
                lcs[(i + 1) * w + j + 1] + 1
            } else {
                lcs[(i + 1) * w + j].max(lcs[i * w + j + 1])
            };
        }
    }
    let mut out = String::new();
    let mut line = |mark: char, s: &str| {
        out.push(mark);
        out.push_str(s);
        out.push('\n');
    };
    let (mut i, mut j) = (0, 0);
    while i < n && j < m {
        if a[i] == b[j] {
            line(' ', a[i]);
            i += 1;
            j += 1;
        } else if lcs[(i + 1) * w + j] >= lcs[i * w + j + 1] {
            line('-', a[i]);
            i += 1;
        } else {
            line('+', b[j]);
            j += 1;
        }
    }
    for s in &a[i..] {
        line('-', s);
    }
    for s in &b[j..] {
        line('+', s);
    }
    out
}

/// Returns `diff_lines` of the two UTF-8 files. A missing file gives an error
/// of the kind `ErrorKind::NotFound`, and a file that is not valid UTF-8 gives
/// `ErrorKind::InvalidData`.
pub fn diff_files(old_path: impl AsRef<Path>, new_path: impl AsRef<Path>) -> io::Result<String> {
    let old = fs::read_to_string(old_path)?;
    let new = fs::read_to_string(new_path)?;
    Ok(diff_lines(&old, &new))
}
