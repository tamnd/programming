// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

use std::fs;
use std::io;
use std::path::Path;

/// Gives the file at `src` the name `dst`, and replaces a file at `dst`.
/// `fs::rename` is one rename call, so it is atomic. Across file systems it
/// gives an error of kind `ErrorKind::CrossesDevices` and does not copy.
pub fn rename_file(src: impl AsRef<Path>, dst: impl AsRef<Path>) -> io::Result<()> {
    fs::rename(src, dst)
}
