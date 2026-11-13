// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

//! Takes advisory locks on files with `File::try_lock` and
//! `File::try_lock_shared` (Rust 1.89 and later). On Unix they call flock, so
//! the lock belongs to the open file, and a drop of the `File` releases it.

use std::fs::{File, OpenOptions, TryLockError};
use std::io;
use std::path::Path;

/// Asks for a shared or an exclusive lock on `path`, without a wait. It
/// returns `Ok(None)` when another open file has a lock that conflicts. The
/// lock lives as long as the returned `File`. The file must exist.
pub fn try_lock_file(path: &Path, exclusive: bool) -> io::Result<Option<File>> {
    let f = OpenOptions::new().read(true).write(true).open(path)?;
    let got = if exclusive { f.try_lock() } else { f.try_lock_shared() };
    match got {
        Ok(()) => Ok(Some(f)),
        Err(TryLockError::WouldBlock) => Ok(None),
        Err(TryLockError::Error(e)) => Err(e),
    }
}

/// The entry of the cases: takes `held` ("none", "shared" or "exclusive"),
/// then asks for `want`.
pub fn try_lock(path: &Path, held: &str, want: &str) -> io::Result<bool> {
    let _first = match held {
        "none" => None,
        mode => try_lock_file(path, mode == "exclusive")?,
    };
    let second = try_lock_file(path, want == "exclusive")?;
    Ok(second.is_some())
}
