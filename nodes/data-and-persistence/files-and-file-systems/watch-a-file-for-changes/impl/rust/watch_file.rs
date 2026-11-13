// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

//! Watch a file for changes: poll its size and modification time.

use std::fs::{self, File, FileTimes, OpenOptions};
use std::io::{self, ErrorKind, Write};
use std::path::{Path, PathBuf};
use std::thread;
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};

/// Seconds; the cases set the mtime to BASE_MTIME + step.
pub const BASE_MTIME: u64 = 1_700_000_000;

/// The size and the mtime of a file.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
struct Signature {
    size: u64,
    mtime: SystemTime,
}

/// The signature of the file, or None when it does not exist.
fn signature(path: &Path) -> io::Result<Option<Signature>> {
    let meta = match fs::metadata(path) {
        Ok(meta) => meta,
        Err(e) if e.kind() == ErrorKind::NotFound => return Ok(None),
        Err(e) => return Err(e),
    };
    if meta.is_dir() {
        return Err(io::Error::new(ErrorKind::IsADirectory, format!("{} is a directory", path.display())));
    }
    Ok(Some(Signature { size: meta.len(), mtime: meta.modified()? }))
}

/// Remembers the last signature of a file and tells what changed since then.
pub struct FileWatcher {
    path: PathBuf,
    last: Option<Signature>,
}

impl FileWatcher {
    pub fn new(path: impl AsRef<Path>) -> io::Result<Self> {
        let path = path.as_ref().to_path_buf();
        let last = signature(&path)?;
        Ok(FileWatcher { path, last })
    }

    /// created, deleted, modified or none, for the time since the last poll.
    pub fn poll(&mut self) -> io::Result<&'static str> {
        let now = signature(&self.path)?;
        let before = std::mem::replace(&mut self.last, now);
        Ok(match (before, now) {
            (None, None) => "none",
            (None, Some(_)) => "created",
            (Some(_), None) => "deleted",
            (Some(b), Some(n)) if b != n => "modified",
            _ => "none",
        })
    }
}

/// Calls on_change once for each burst of changes, after the file is quiet,
/// until stop returns true.
pub fn watch(
    path: impl AsRef<Path>,
    mut on_change: impl FnMut(&Path),
    interval: Duration,
    quiet: Duration,
    stop: impl Fn() -> bool,
) -> io::Result<()> {
    let mut watcher = FileWatcher::new(path)?;
    let mut pending: Option<Instant> = None; // the last change that on_change has not seen
    while !stop() {
        if watcher.poll()? != "none" {
            pending = Some(Instant::now());
        } else if pending.is_some_and(|t| t.elapsed() >= quiet) {
            pending = None;
            on_change(&watcher.path);
        }
        thread::sleep(interval);
    }
    Ok(())
}

/// Makes the change of one step of a case (the entry of the cases uses it).
fn apply_step(path: &Path, step: &str, number: u64) -> io::Result<()> {
    let (kind, text) = step.split_once(':').unwrap_or((step, ""));
    match kind {
        "create" | "write" => fs::write(path, text)?,
        "append" => OpenOptions::new().append(true).open(path)?.write_all(text.as_bytes())?,
        "replace" => {
            let mut tmp = path.as_os_str().to_owned();
            tmp.push(".tmp");
            fs::write(&tmp, text)?;
            fs::rename(&tmp, path)?;
        }
        "recreate" => {
            fs::remove_file(path)?;
            fs::write(path, text)?;
        }
        "delete" => fs::remove_file(path)?,
        "mkdir" => fs::create_dir(path)?,
        "touch" | "none" => {}
        _ => return Err(io::Error::new(ErrorKind::InvalidInput, format!("unknown step {step}"))),
    }
    if kind != "none" && path.is_file() {
        let t = UNIX_EPOCH + Duration::from_secs(BASE_MTIME + number);
        File::options().write(true).open(path)?.set_times(FileTimes::new().set_accessed(t).set_modified(t))?;
    }
    Ok(())
}

/// The entry of the cases: polls after each step and returns the results.
pub fn watch_changes(path: impl AsRef<Path>, steps: &[&str]) -> io::Result<Vec<String>> {
    let path = path.as_ref();
    let mut watcher = FileWatcher::new(path)?;
    let mut results = Vec::new();
    for (i, step) in steps.iter().enumerate() {
        apply_step(path, step, i as u64 + 1)?;
        results.push(watcher.poll()?.to_string());
    }
    Ok(results)
}
