// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

use std::fs::File;
use std::io::{self, BufReader, BufWriter, ErrorKind, Read, Write};
use std::path::Path;

/// Copies `src` to `dst` through buffers of `size` bytes and returns the
/// number of bytes copied.
///
/// `into_inner` flushes the `BufWriter` and gives back the error of that
/// last write. A drop of the `BufWriter` also flushes, but it ignores the
/// error.
pub fn copy_buffered(src: impl AsRef<Path>, dst: impl AsRef<Path>, size: usize) -> io::Result<u64> {
    if size == 0 {
        return Err(io::Error::new(ErrorKind::InvalidInput, "size must be at least 1"));
    }
    let mut reader = BufReader::with_capacity(size, File::open(src)?);
    let mut writer = BufWriter::with_capacity(size, File::create(dst)?);
    let mut chunk = vec![0; size];
    let mut total = 0;
    loop {
        let n = match reader.read(&mut chunk) {
            Ok(0) => break,
            Ok(n) => n,
            Err(e) if e.kind() == ErrorKind::Interrupted => continue,
            Err(e) => return Err(e),
        };
        writer.write_all(&chunk[..n])?;
        total += n as u64;
    }
    writer.into_inner().map_err(|e| e.into_error())?;
    Ok(total)
}
