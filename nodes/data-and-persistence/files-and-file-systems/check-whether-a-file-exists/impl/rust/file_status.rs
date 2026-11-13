// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

use std::fs;
use std::io::{self, ErrorKind};
use std::path::Path;

/// Tells what is at `path`: "file", "directory", "other", "missing" or
/// "broken-link".
///
/// `Path::exists` is not used, because it gives false also for a permission
/// error. `Path::try_exists` reports that error, but it does not tell a
/// file from a directory. Here each error other than "no such entry" is
/// returned.
pub fn file_status(path: impl AsRef<Path>) -> io::Result<&'static str> {
    let path = path.as_ref();
    let meta = match fs::metadata(path) {
        // metadata follows symbolic links.
        Ok(meta) => meta,
        Err(e) if matches!(e.kind(), ErrorKind::NotFound | ErrorKind::NotADirectory) => {
            let is_link = fs::symlink_metadata(path).is_ok_and(|m| m.file_type().is_symlink());
            return Ok(if is_link { "broken-link" } else { "missing" });
        }
        Err(e) => return Err(e),
    };
    Ok(if meta.is_file() {
        "file"
    } else if meta.is_dir() {
        "directory"
    } else {
        "other"
    })
}
