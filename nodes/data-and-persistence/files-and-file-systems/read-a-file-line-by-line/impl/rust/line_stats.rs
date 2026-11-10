// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

//! Read a text file line by line.

use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;

/// Returns the number of lines of a UTF-8 text file and the number of code points in its longest line.
///
/// A line that is not valid UTF-8 gives an error of kind `InvalidData`.
pub fn line_stats(path: impl AsRef<Path>) -> io::Result<(usize, usize)> {
    let reader = BufReader::new(File::open(path)?);
    let mut lines = 0;
    let mut longest = 0;
    // Each line comes without its "\n" or "\r\n".
    for line in reader.lines() {
        let line = line?;
        lines += 1;
        longest = longest.max(line.chars().count());
    }
    Ok((lines, longest))
}
