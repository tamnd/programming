// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

//! Writes a file and returns when its data is on the storage device.

use std::fs::File;
use std::io::{self, Write};
use std::path::Path;

/// Syncs the entries of `dir`, so that a new name in it is on disk. On Unix,
/// a directory opens for reading, and `sync_all` calls fsync on it.
pub fn sync_directory(dir: &Path) -> io::Result<()> {
    File::open(dir)?.sync_all()
}

/// Writes `text` to `path`, syncs the file, closes it, and syncs the
/// directory of `path`. Each step returns its error.
pub fn write_synced(path: &Path, text: &str) -> io::Result<()> {
    let mut f = File::create(path)?;
    // A File has no buffer: write_all gives all the bytes to the kernel.
    f.write_all(text.as_bytes())?;
    // sync_all also reports the errors that a drop of the File ignores.
    f.sync_all()?;
    drop(f);
    let abs = std::path::absolute(path)?;
    match abs.parent() {
        Some(dir) => sync_directory(dir),
        None => Ok(()),
    }
}
