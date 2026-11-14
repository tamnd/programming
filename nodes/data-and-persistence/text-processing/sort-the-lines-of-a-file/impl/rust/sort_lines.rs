// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

//! Sorts the lines of a text file.

use std::fs;
use std::io;
use std::path::Path;

/// Returns the lines of the UTF-8 file at `path`, sorted by code point. A line
/// ends at LF, and the LF is not part of it. A last line with no LF is a line
/// too. A missing file gives an error of the kind `ErrorKind::NotFound`, and a
/// file that is not valid UTF-8 gives `ErrorKind::InvalidData`.
pub fn sort_lines(path: impl AsRef<Path>) -> io::Result<Vec<String>> {
    let text = fs::read_to_string(path)?;
    // str::lines also removes a CR before each LF, so split at LF only.
    let mut lines: Vec<String> = text.split('\n').map(String::from).collect();
    // After the LF at the end of the last line, split gives one empty string.
    // An empty file gives [""] too, which is no lines.
    if lines.last().is_some_and(|line| line.is_empty()) {
        lines.pop();
    }
    // Strings compare by their bytes, and the order of the UTF-8 bytes is the
    // order of the code points. Equal lines are equal bytes, so an unstable
    // sort gives the same result as a stable one.
    lines.sort_unstable();
    Ok(lines)
}
