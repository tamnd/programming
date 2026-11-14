// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

//! Replaces a text with another text in each file of a list.

use std::fs::{self, File, OpenOptions};
use std::io::{self, ErrorKind, Write};
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicU64, Ordering};

/// Replaces each instance of `old` with `new` in each file, and returns the
/// number of instances in each file.
///
/// `old` and `new` are plain text, not patterns, and an instance can contain
/// LF bytes. Instances do not overlap. A file with no instance is not written.
/// A changed file goes to a new file in the same directory, and `fs::rename`
/// puts it in the place of the old one. The files are done in order, and the
/// first error stops the function: the files before have the new text, and
/// the files after do not change. An empty `old` gives an error of kind
/// `InvalidInput`, and a missing file gives an error of kind `NotFound`.
pub fn replace_in_files<P: AsRef<Path>>(
    paths: &[P],
    old: &str,
    new: &str,
) -> io::Result<Vec<usize>> {
    if old.is_empty() {
        return Err(io::Error::new(ErrorKind::InvalidInput, "old is empty"));
    }
    let mut counts = Vec::with_capacity(paths.len());
    for path in paths {
        let path = path.as_ref();
        let data = fs::read(path)?;
        let (replaced, n) = replace(&data, old.as_bytes(), new.as_bytes());
        if n > 0 {
            write_atomically(path, &replaced)?;
        }
        counts.push(n);
    }
    Ok(counts)
}

/// Returns `data` with each instance of `old` changed to `new`, and the number
/// of instances. After an instance, the search goes on after its end, so
/// instances do not overlap.
fn replace(data: &[u8], old: &[u8], new: &[u8]) -> (Vec<u8>, usize) {
    let mut out = Vec::with_capacity(data.len());
    let mut n = 0;
    let mut from = 0;
    while let Some(at) = find(&data[from..], old) {
        out.extend_from_slice(&data[from..from + at]);
        out.extend_from_slice(new);
        from += at + old.len();
        n += 1;
    }
    out.extend_from_slice(&data[from..]);
    (out, n)
}

/// Returns the index of the first instance of `needle` in `haystack`.
fn find(haystack: &[u8], needle: &[u8]) -> Option<usize> {
    haystack.windows(needle.len()).position(|w| w == needle)
}

/// Writes `data` to a new file next to `path`, and renames it over `path`.
fn write_atomically(path: &Path, data: &[u8]) -> io::Result<()> {
    let permissions = fs::metadata(path)?.permissions();
    let (tmp_path, mut tmp) = create_temp(path)?;
    let result = (|| {
        tmp.write_all(data)?;
        // The new file has the default permissions, so it gets the old ones.
        tmp.set_permissions(permissions)?;
        tmp.sync_all()?;
        fs::rename(&tmp_path, path)
    })();
    if result.is_err() {
        let _ = fs::remove_file(&tmp_path);
    }
    result
}

/// Creates a new file next to `path` with a name that no file has.
fn create_temp(path: &Path) -> io::Result<(PathBuf, File)> {
    static NEXT: AtomicU64 = AtomicU64::new(0);
    let dir = path.parent().unwrap_or(Path::new(""));
    let name = path.file_name().unwrap_or_default().to_string_lossy();
    loop {
        let n = NEXT.fetch_add(1, Ordering::Relaxed);
        let tmp_path = dir.join(format!(".{name}.{}.{n}.tmp", std::process::id()));
        // create_new fails if the file exists, so the code does not write
        // over a file with the same name.
        match OpenOptions::new()
            .write(true)
            .create_new(true)
            .open(&tmp_path)
        {
            Ok(file) => return Ok((tmp_path, file)),
            Err(e) if e.kind() == ErrorKind::AlreadyExists => continue,
            Err(e) => return Err(e),
        }
    }
}
