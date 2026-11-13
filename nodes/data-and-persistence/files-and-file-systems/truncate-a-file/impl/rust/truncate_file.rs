// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

use std::fs::OpenOptions;
use std::io;
use std::path::Path;

/// Sets the size of the existing file at `path` to `length` bytes.
///
/// The file is opened for writing with no `create`, so a missing file gives
/// an error of kind `NotFound`. `File::set_len` cuts the bytes after
/// `length`, or adds zero bytes up to `length`. `length` is a `u64`, so it
/// cannot be under 0.
pub fn truncate_file(path: &Path, length: u64) -> io::Result<()> {
    OpenOptions::new().write(true).open(path)?.set_len(length)
}
