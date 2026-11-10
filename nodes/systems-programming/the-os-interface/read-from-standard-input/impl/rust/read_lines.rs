// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

//! Read the lines of standard input.

use std::io::{self, BufRead};

/// Reads the reader to its end and returns its lines, each without "\n" or "\r\n".
///
/// A line that is not valid UTF-8 gives an error of kind `InvalidData`.
pub fn read_lines(reader: impl BufRead) -> io::Result<Vec<String>> {
    reader.lines().collect()
}

/// Reads standard input to its end and returns its lines.
pub fn read_stdin_lines() -> io::Result<Vec<String>> {
    // The lock gives a buffered reader that implements BufRead.
    read_lines(io::stdin().lock())
}
