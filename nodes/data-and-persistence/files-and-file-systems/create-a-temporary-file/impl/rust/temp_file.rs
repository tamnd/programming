// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

//! The standard library of Rust gives the temporary directory, but no call
//! that makes a temporary file, and it has no random number generator. So
//! `make_temp_file` makes the name from the hash of a new `RandomState`,
//! whose keys are random, and opens it with `create_new`, which fails when
//! any file or link has the name (O_CREAT | O_EXCL). The name is hard to
//! guess, but the hash is not a secure random source: the safety comes from
//! `create_new`, which never opens a file that someone else made.

use std::fs::{self, OpenOptions};
use std::hash::{BuildHasher, RandomState};
use std::io::{self, ErrorKind, Read, Seek, SeekFrom, Write};
use std::os::unix::fs::OpenOptionsExt;
use std::path::{Path, PathBuf};
use std::time::SystemTime;

/// Makes a new, empty file in `dir` whose name starts with `prefix`, with the
/// mode 0o600, and returns its path.
pub fn make_temp_file(dir: &Path, prefix: &str) -> io::Result<PathBuf> {
    if prefix.contains('/') {
        return Err(io::Error::new(ErrorKind::InvalidInput, "invalid prefix"));
    }
    for attempt in 0..100u32 {
        let random = RandomState::new().hash_one((std::process::id(), attempt, SystemTime::now()));
        let path = dir.join(format!("{prefix}{random:016x}"));
        match OpenOptions::new().write(true).create_new(true).mode(0o600).open(&path) {
            Ok(_) => return Ok(path),
            Err(e) if e.kind() == ErrorKind::AlreadyExists => continue,
            Err(e) => return Err(e),
        }
    }
    Err(io::Error::new(ErrorKind::AlreadyExists, "no free temporary name after 100 attempts"))
}

/// Writes `data` to a new temporary file, reads it back, removes the file,
/// and returns the text.
pub fn use_temp_file(dir: &Path, prefix: &str, data: &str) -> io::Result<String> {
    let path = make_temp_file(dir, prefix)?;
    let result = (|| {
        let mut f = OpenOptions::new().read(true).write(true).open(&path)?;
        f.write_all(data.as_bytes())?;
        f.seek(SeekFrom::Start(0))?;
        let mut text = String::new();
        f.read_to_string(&mut text)?;
        Ok(text)
    })();
    fs::remove_file(&path)?;
    result
}
