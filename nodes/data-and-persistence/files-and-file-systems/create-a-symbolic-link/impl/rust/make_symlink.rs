// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

//! Make a symbolic link and read its target back.

use std::fs;
use std::io;
use std::os::unix::fs::symlink;
use std::path::{Path, PathBuf};

/// Makes a symbolic link at `link` that holds `target`, and returns the
/// target that the link holds, read back with `fs::read_link`. The target is
/// stored as it is: it need not exist, and a relative target is read from the
/// directory of the link. `symlink` is in `std::os::unix::fs`, because
/// Windows has two calls, `symlink_file` and `symlink_dir`. When `link`
/// exists, also as a dangling link, the error kind is `AlreadyExists`, and
/// when the directory of `link` does not exist, it is `NotFound`.
pub fn make_symlink(target: impl AsRef<Path>, link: impl AsRef<Path>) -> io::Result<PathBuf> {
    symlink(target, link.as_ref())?;
    fs::read_link(link)
}
