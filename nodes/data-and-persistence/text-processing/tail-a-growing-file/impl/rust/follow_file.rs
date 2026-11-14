// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

//! Follows a file that grows, as tail -F does.

use std::fs::{self, File, OpenOptions};
use std::io::{self, ErrorKind, Read, Seek, SeekFrom, Write};
use std::os::unix::fs::MetadataExt;
use std::path::{Path, PathBuf};
use std::thread;
use std::time::Duration;

/// Reads the lines that are added to a file, one poll at a time. The follower
/// keeps the file open, so it can read the rest of the file after a rename or
/// a delete.
pub struct Follower {
    path: PathBuf,
    file: Option<File>,
    offset: u64,
    /// The bytes after the last LF.
    pending: Vec<u8>,
}

impl Follower {
    /// Opens the file at path and starts at its end. The error kind is
    /// NotFound if no file is at path.
    pub fn new(path: impl AsRef<Path>) -> io::Result<Follower> {
        let path = path.as_ref().to_path_buf();
        let file = File::open(&path)?;
        let offset = file.metadata()?.len();
        Ok(Follower {
            path,
            file: Some(file),
            offset,
            pending: Vec::new(),
        })
    }

    /// Returns the complete lines that were added since the last poll.
    pub fn poll(&mut self) -> io::Result<Vec<String>> {
        let mut lines = Vec::new();
        // Look at the path before the read. Then the read gets each byte that
        // was written before a rename or a delete.
        let now = not_found_is_none(fs::metadata(&self.path))?;
        if let Some(open) = self.file.as_ref().map(File::metadata).transpose()? {
            if open.len() < self.offset {
                // The file is shorter than the offset, so it was truncated.
                self.flush(&mut lines);
                self.offset = 0;
            }
            self.read(&mut lines)?;
            let same = now.is_some_and(|now| now.dev() == open.dev() && now.ino() == open.ino());
            if !same {
                // The file was deleted or renamed. Its rest is read.
                self.flush(&mut lines);
                self.file = None;
            }
        }
        if self.file.is_none() {
            if let Some(file) = not_found_is_none(File::open(&self.path))? {
                self.file = Some(file);
                self.offset = 0;
                self.read(&mut lines)?;
            }
        }
        Ok(lines)
    }

    /// Calls emit with each new line. Polls the file each interval, and
    /// returns when stop returns true.
    pub fn follow(
        &mut self,
        interval: Duration,
        mut stop: impl FnMut() -> bool,
        mut emit: impl FnMut(String),
    ) -> io::Result<()> {
        loop {
            for line in self.poll()? {
                emit(line);
            }
            if stop() {
                return Ok(());
            }
            thread::sleep(interval);
        }
    }

    fn read(&mut self, lines: &mut Vec<String>) -> io::Result<()> {
        let Some(file) = self.file.as_mut() else {
            return Ok(());
        };
        file.seek(SeekFrom::Start(self.offset))?;
        let n = file.read_to_end(&mut self.pending)?;
        self.offset += n as u64;
        // Split the bytes at each LF first, and decode each line after. A
        // read can end in the middle of a UTF-8 character.
        let mut start = 0;
        while let Some(lf) = self.pending[start..].iter().position(|&b| b == b'\n') {
            lines.push(String::from_utf8_lossy(&self.pending[start..start + lf]).into_owned());
            start += lf + 1;
        }
        self.pending.drain(..start);
        Ok(())
    }

    fn flush(&mut self, lines: &mut Vec<String>) {
        if !self.pending.is_empty() {
            lines.push(String::from_utf8_lossy(&self.pending).into_owned());
            self.pending.clear();
        }
    }
}

fn not_found_is_none<T>(result: io::Result<T>) -> io::Result<Option<T>> {
    match result {
        Ok(value) => Ok(Some(value)),
        Err(err) if err.kind() == ErrorKind::NotFound => Ok(None),
        Err(err) => Err(err),
    }
}

/// Writes initial to path, makes a follower, does the steps and returns the
/// lines of each poll.
pub fn follow_file(
    path: impl AsRef<Path>,
    initial: &str,
    steps: &[&str],
) -> io::Result<Vec<Vec<String>>> {
    let path = path.as_ref();
    fs::write(path, initial)?;
    let mut follower = Follower::new(path)?;
    let mut old = path.as_os_str().to_owned();
    old.push(".1");
    let mut polls = Vec::new();
    for step in steps {
        let (op, text) = step.split_once(':').unwrap_or((step, ""));
        match op {
            "poll" => polls.push(follower.poll()?),
            "append" => append(path, text)?,
            "append-old" => append(Path::new(&old), text)?,
            "truncate" => fs::write(path, text)?,
            "rotate" => {
                fs::rename(path, &old)?;
                create(path, "")?;
            }
            "delete" => fs::remove_file(path)?,
            "create" => create(path, text)?,
            _ => {
                return Err(io::Error::new(
                    ErrorKind::InvalidInput,
                    format!("bad step: {step:?}"),
                ))
            }
        }
    }
    Ok(polls)
}

fn append(path: &Path, text: &str) -> io::Result<()> {
    OpenOptions::new()
        .append(true)
        .create(true)
        .open(path)?
        .write_all(text.as_bytes())
}

fn create(path: &Path, text: &str) -> io::Result<()> {
    OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(path)?
        .write_all(text.as_bytes())
}
