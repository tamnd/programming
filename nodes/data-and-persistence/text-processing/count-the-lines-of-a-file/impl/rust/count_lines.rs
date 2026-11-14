// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

//! Counts the lines of a file without decoding it.

use std::fs::File;
use std::io::{self, ErrorKind, Read};
use std::path::Path;

/// Returns `(newlines, lines)` for the bytes of `r`. `newlines` is the number of
/// LF bytes, the number that `wc -l` prints. `lines` also counts a last line that
/// has no LF.
pub fn count<R: Read>(mut r: R) -> io::Result<(u64, u64)> {
    let mut buf = vec![0u8; 64 * 1024];
    let mut newlines = 0u64;
    let mut last = b'\n'; // an empty input has no line without an LF
    loop {
        let n = match r.read(&mut buf) {
            Ok(0) => break,
            Ok(n) => n,
            // A signal stopped the read before it read a byte: read again.
            Err(e) if e.kind() == ErrorKind::Interrupted => continue,
            Err(e) => return Err(e),
        };
        newlines += buf[..n].iter().filter(|&&b| b == b'\n').count() as u64;
        last = buf[n - 1];
    }
    Ok((newlines, newlines + u64::from(last != b'\n')))
}

/// Counts the lines of the file at `path`. A missing file gives an error of
/// kind `NotFound`.
pub fn count_lines(path: impl AsRef<Path>) -> io::Result<(u64, u64)> {
    count(File::open(path)?)
}
