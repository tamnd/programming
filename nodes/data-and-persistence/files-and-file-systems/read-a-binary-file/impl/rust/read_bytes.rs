// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

use std::fs::{self, File};
use std::io::{self, Read};
use std::path::Path;

/// Returns all of the bytes of a file, with no decoding.
pub fn read_bytes(path: impl AsRef<Path>) -> io::Result<Vec<u8>> {
    fs::read(path)
}

/// Returns the first `n` bytes of a file.
///
/// If the file has fewer than `n` bytes, the error is of kind
/// `UnexpectedEof`. `read_exact` calls `read` again until the buffer is
/// full; one call to `read` can give fewer bytes than it asks for.
pub fn read_exact(path: impl AsRef<Path>, n: usize) -> io::Result<Vec<u8>> {
    let mut data = vec![0; n];
    File::open(path)?.read_exact(&mut data)?;
    Ok(data)
}
