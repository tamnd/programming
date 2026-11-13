// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

use std::fs;
use std::io::{self, ErrorKind};
use std::os::unix::fs::PermissionsExt;
use std::path::Path;

/// Sets the mode of the file at `path`, and returns the new permission bits
/// (`st_mode & 0o7777`). `mode` is three or four octal digits, as for the
/// chmod command; anything else gives `ErrorKind::InvalidInput`. The portable
/// `Permissions` type has only a read-only flag, so the code uses
/// `PermissionsExt::from_mode`, which is for Unix. `fs::set_permissions`
/// follows a symbolic link, and the umask does not apply. A missing file gives
/// `ErrorKind::NotFound`.
pub fn set_mode(path: impl AsRef<Path>, mode: &str) -> io::Result<u32> {
    let path = path.as_ref();
    let valid = (3..=4).contains(&mode.len()) && mode.bytes().all(|b| (b'0'..=b'7').contains(&b));
    if !valid {
        return Err(io::Error::new(ErrorKind::InvalidInput, format!("invalid mode: {mode:?}")));
    }
    let bits = u32::from_str_radix(mode, 8).map_err(|e| io::Error::new(ErrorKind::InvalidInput, e))?;
    fs::set_permissions(path, fs::Permissions::from_mode(bits))?;
    Ok(fs::metadata(path)?.permissions().mode() & 0o7777)
}
