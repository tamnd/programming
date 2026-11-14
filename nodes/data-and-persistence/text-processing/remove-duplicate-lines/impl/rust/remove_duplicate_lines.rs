// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

//! Removes the duplicate lines of a text file.

use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;

/// Returns the lines of the UTF-8 file at `path`, each one only once, at the
/// place of its first copy. A line ends at LF, and the LF is not part of it. A
/// last line with no LF is a line too. Memory grows with the different lines,
/// not with the size of the file. A missing file gives an error of the kind
/// `ErrorKind::NotFound`, and a file that is not valid UTF-8 gives
/// `ErrorKind::InvalidData`.
pub fn remove_duplicate_lines(path: impl AsRef<Path>) -> io::Result<Vec<String>> {
    let mut reader = BufReader::new(File::open(path)?);
    let mut seen: HashSet<String> = HashSet::new();
    let mut lines = Vec::new();
    let mut line = String::new();
    // read_line reads up to and with the next LF, so a CR stays in the line.
    // It returns 0 at the end of the file.
    while reader.read_line(&mut line)? > 0 {
        if line.ends_with('\n') {
            line.pop();
        }
        // contains takes a &str, so a duplicate line needs no copy. A new
        // line is copied once into the set and moved into the result.
        if !seen.contains(line.as_str()) {
            seen.insert(line.clone());
            lines.push(std::mem::take(&mut line));
        }
        line.clear();
    }
    Ok(lines)
}
