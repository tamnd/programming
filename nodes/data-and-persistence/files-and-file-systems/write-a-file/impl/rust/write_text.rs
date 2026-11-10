// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

//! Writes a string to a file in one of three modes.

use std::fs::{self, File, OpenOptions};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::process;
use std::sync::atomic::{AtomicU64, Ordering};

/// The modes of [`write_text`].
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Mode {
    Truncate,
    Append,
    Atomic,
}

/// Writes `text` to `path`. A `&str` is UTF-8, so the bytes go to the file
/// with no change: no byte order mark and no change of the line ends.
pub fn write_text(path: &Path, text: &str, mode: Mode) -> io::Result<()> {
    match mode {
        // fs::write creates the file, or truncates it if it exists.
        Mode::Truncate => fs::write(path, text),
        Mode::Append => OpenOptions::new()
            .append(true)
            .create(true)
            .open(path)?
            .write_all(text.as_bytes()),
        Mode::Atomic => replace(path, text),
    }
}

fn replace(path: &Path, text: &str) -> io::Result<()> {
    let (tmp, mut file) = create_temp(path)?;
    let result = file
        .write_all(text.as_bytes())
        .and_then(|()| file.sync_all())
        .and_then(|()| {
            drop(file);
            fs::rename(&tmp, path)
        });
    if result.is_err() {
        let _ = fs::remove_file(&tmp);
    }
    result
}

// Makes a new file next to path. The name is in the same directory, so the
// rename does not cross a file system. create_new fails if the name exists.
fn create_temp(path: &Path) -> io::Result<(PathBuf, File)> {
    static COUNT: AtomicU64 = AtomicU64::new(0);
    let base = path.file_name().unwrap_or_default().to_string_lossy();
    loop {
        let n = COUNT.fetch_add(1, Ordering::Relaxed);
        let tmp = path.with_file_name(format!(".{base}.{}.{n}.tmp", process::id()));
        match OpenOptions::new().write(true).create_new(true).open(&tmp) {
            Ok(file) => return Ok((tmp, file)),
            Err(e) if e.kind() == io::ErrorKind::AlreadyExists => continue,
            Err(e) => return Err(e),
        }
    }
}
