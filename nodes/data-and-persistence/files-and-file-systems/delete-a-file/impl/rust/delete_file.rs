// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

use std::fs;
use std::io::{self, ErrorKind};
use std::path::Path;

/// Deletes the file at `path`. Gives `Ok(true)` if a file was there and
/// `Ok(false)` if not. `fs::remove_file` does not delete a directory, and it
/// deletes a symbolic link, not its target. Each error other than "no such
/// entry" goes to the caller.
pub fn delete_file(path: impl AsRef<Path>) -> io::Result<bool> {
    match fs::remove_file(path) {
        Ok(()) => Ok(true),
        Err(e) if matches!(e.kind(), ErrorKind::NotFound | ErrorKind::NotADirectory) => Ok(false),
        Err(e) => Err(e),
    }
}
