// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

use std::fs;
use std::io;
use std::path::Path;

/// Returns all of the text of a UTF-8 file, with no change to it.
///
/// `fs::read_to_string` gives an error of kind `InvalidData` when the
/// bytes are not valid UTF-8. It keeps CR LF and a byte order mark.
pub fn read_text(path: impl AsRef<Path>) -> io::Result<String> {
    fs::read_to_string(path)
}
