// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

use std::fs;
use std::io;
use std::path::Path;
use std::time::UNIX_EPOCH;

/// Returns the modification time of the file at `path`, in whole seconds
/// since 1970-01-01T00:00:00Z. `fs::metadata` follows a symbolic link.
/// `duration_since` gives an error for a time before 1970, with the
/// distance in the other direction, so the code makes that distance negative
/// and takes the floor. A missing file gives `ErrorKind::NotFound`.
pub fn file_mtime(path: &Path) -> io::Result<i64> {
    let t = fs::metadata(path)?.modified()?;
    Ok(match t.duration_since(UNIX_EPOCH) {
        Ok(d) => d.as_secs() as i64,
        Err(e) => {
            let d = e.duration();
            -(d.as_secs() as i64) - i64::from(d.subsec_nanos() > 0)
        }
    })
}
