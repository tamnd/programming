// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

use std::fs;
use std::io::{self, ErrorKind};
use std::path::Path;

/// Returns the size in bytes of the regular file at `path`. `fs::metadata`
/// follows a symbolic link, so a link gives the size of its target. A missing
/// file gives `ErrorKind::NotFound`. `fs::metadata` also gives a size for a
/// directory, but it is not the size of a file, so the code returns
/// `ErrorKind::IsADirectory`.
pub fn file_size(path: &Path) -> io::Result<u64> {
    let meta = fs::metadata(path)?;
    if meta.is_dir() {
        return Err(io::Error::new(ErrorKind::IsADirectory, format!("{}: is a directory", path.display())));
    }
    Ok(meta.len())
}
