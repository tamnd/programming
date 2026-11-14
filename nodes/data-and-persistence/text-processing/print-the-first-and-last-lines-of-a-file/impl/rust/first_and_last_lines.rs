// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

//! Gets the first lines and the last lines of a text file in one pass.

use std::collections::VecDeque;
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;

/// Returns the first `count` lines and the last `count` lines of the UTF-8
/// file at `path`.
///
/// If the file has fewer than `count` lines, each vector has all the lines. A
/// line ends at LF, and the LF is not part of it. A last line with no LF is a
/// line too. A missing file gives an error of kind `NotFound`.
pub fn first_and_last_lines(
    path: impl AsRef<Path>,
    count: usize,
) -> io::Result<(Vec<String>, Vec<String>)> {
    let mut reader = BufReader::new(File::open(path)?);
    let mut first = Vec::new();
    // last holds the last `count` lines that the loop has seen. When it is
    // full, pop_front removes the oldest line before push_back adds the new one.
    let mut last = VecDeque::with_capacity(count);
    let mut line = String::new();
    // read_line reads up to and with the next LF, so a CR stays in the line.
    // It returns 0 at the end of the file.
    while count > 0 && reader.read_line(&mut line)? > 0 {
        if line.ends_with('\n') {
            line.pop();
        }
        if first.len() < count {
            first.push(line.clone());
        }
        if last.len() == count {
            last.pop_front();
        }
        last.push_back(std::mem::take(&mut line));
    }
    Ok((first, last.into()))
}
