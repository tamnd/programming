// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

//! Removes lines from a file and writes the rest back under the same name.

use std::fs::{self, File, OpenOptions};
use std::io::{self, BufRead, BufReader, BufWriter, ErrorKind, Write};
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicU64, Ordering};

/// Removes `count` lines from the file at `path`, from line `start`, and
/// returns how many it removed.
///
/// Line 1 is the first line. A line ends at LF, and the LF goes with the line.
/// The result is less than `count` if the file ends first. If no line goes,
/// the file is not written. Otherwise the rest of the file goes to a new file
/// in the same directory, and `fs::rename` puts it in the place of the old
/// one. A `start` less than 1 or a `count` less than 0 gives an error of kind
/// `InvalidInput`, and a missing file gives an error of kind `NotFound`.
pub fn remove_lines(path: impl AsRef<Path>, start: i32, count: i32) -> io::Result<i32> {
    let path = path.as_ref();
    if start < 1 || count < 0 {
        return Err(io::Error::new(
            ErrorKind::InvalidInput,
            format!("bad range: start {start}, count {count}"),
        ));
    }
    let src = File::open(path)?;
    let permissions = src.metadata()?.permissions();
    let (tmp_path, tmp) = create_temp(path)?;
    let result = copy_and_replace(src, tmp, &tmp_path, path, permissions, start, count);
    if !matches!(result, Ok(n) if n > 0) {
        // The rename did not happen, so the new file is still there.
        let _ = fs::remove_file(&tmp_path);
    }
    result
}

fn copy_and_replace(
    src: File,
    tmp: File,
    tmp_path: &Path,
    path: &Path,
    permissions: fs::Permissions,
    start: i32,
    count: i32,
) -> io::Result<i32> {
    // remove_lines checked that start and count are not negative. u64 line
    // numbers do not overflow.
    let (start, count) = (
        u64::from(start.unsigned_abs()),
        u64::from(count.unsigned_abs()),
    );
    let mut reader = BufReader::new(src);
    let mut writer = BufWriter::new(tmp);
    let mut line = Vec::new();
    let mut removed = 0;
    for n in 1u64.. {
        line.clear();
        // read_until reads up to and with the next LF, so a CR stays in the
        // line. It returns 0 at the end of the file.
        if reader.read_until(b'\n', &mut line)? == 0 {
            break;
        }
        if n >= start && n - start < count {
            removed += 1;
        } else {
            writer.write_all(&line)?;
        }
    }
    if removed == 0 {
        return Ok(0);
    }
    let tmp = writer.into_inner().map_err(|e| e.into_error())?;
    // The new file has the default permissions, so it gets the old ones.
    tmp.set_permissions(permissions)?;
    tmp.sync_all()?;
    drop(tmp);
    fs::rename(tmp_path, path)?;
    Ok(removed)
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
