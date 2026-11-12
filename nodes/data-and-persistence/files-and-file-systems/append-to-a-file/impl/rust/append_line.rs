// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

use std::fs::OpenOptions;
use std::io::{self, ErrorKind, Write};
use std::path::Path;

/// Adds `line` and one `\n` to the end of the file, in one write call.
///
/// Append mode puts each write at the end of the file, but `write_all`
/// can split the data into more than one write. So the code calls `write`
/// once and reports an error if it wrote less.
pub fn append_line(path: impl AsRef<Path>, line: &str) -> io::Result<()> {
    if line.contains(['\n', '\r']) {
        return Err(io::Error::new(ErrorKind::InvalidInput, "the line must not hold \\n or \\r"));
    }
    let data = format!("{line}\n");
    let mut file = OpenOptions::new().append(true).create(true).open(path)?;
    let written = file.write(data.as_bytes())?;
    if written != data.len() {
        return Err(io::Error::other(format!("short write: {written} of {} bytes", data.len())));
    }
    Ok(())
}
