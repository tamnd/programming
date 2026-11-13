// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

use std::fs;
use std::io;
use std::path::Path;

/// Copies the bytes and the permission bits of the file at `src` to `dst`,
/// and replaces a file at `dst`.
///
/// `fs::copy` copies the permission bits, and on Linux it uses
/// copy_file_range. But it opens `dst` with O_TRUNC, so a copy of a file
/// onto itself makes the file empty. So the function compares the device and
/// inode numbers first, and gives an error of kind `InvalidInput` for the
/// same file. A directory at `src` or `dst` gives `IsADirectory`.
pub fn copy_file(src: &Path, dst: &Path) -> io::Result<()> {
    let s = fs::metadata(src)?;
    if s.is_dir() {
        return Err(io::Error::new(io::ErrorKind::IsADirectory, "src is a directory"));
    }
    match fs::metadata(dst) {
        Ok(d) if d.is_dir() => {
            return Err(io::Error::new(io::ErrorKind::IsADirectory, "dst is a directory"));
        }
        Ok(d) if same_file(&s, &d) => {
            return Err(io::Error::new(io::ErrorKind::InvalidInput, "src and dst are the same file"));
        }
        _ => {}
    }
    fs::copy(src, dst)?;
    Ok(())
}

#[cfg(unix)]
fn same_file(a: &fs::Metadata, b: &fs::Metadata) -> bool {
    use std::os::unix::fs::MetadataExt;
    a.dev() == b.dev() && a.ino() == b.ino()
}

#[cfg(not(unix))]
fn same_file(_: &fs::Metadata, _: &fs::Metadata) -> bool {
    false
}
