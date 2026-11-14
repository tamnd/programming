// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

//! Reads one line of a text file by its number.

use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;

/// Returns line `number` of the UTF-8 file at `path`, or `None` if there is
/// no such line.
///
/// Line 1 is the first line. A line ends at LF, and the LF is not part of it.
/// A last line with no LF is a line too. The function reads the file only up
/// to the line. A missing file gives an error of kind `NotFound`.
pub fn read_line(path: impl AsRef<Path>, number: i32) -> io::Result<Option<String>> {
    let mut reader = BufReader::new(File::open(path)?);
    if number < 1 {
        return Ok(None);
    }
    // read_line, not lines: lines also removes a CR before the LF.
    let mut line = String::new();
    for _ in 0..number {
        line.clear();
        // read_line reads up to and with the next LF. It returns 0 at the
        // end of the file.
        if reader.read_line(&mut line)? == 0 {
            return Ok(None);
        }
    }
    if line.ends_with('\n') {
        line.pop();
    }
    Ok(Some(line))
}
