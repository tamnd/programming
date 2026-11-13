// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

use std::fs::{self, OpenOptions};
use std::io::{ErrorKind, Seek, SeekFrom, Write};
use std::path::PathBuf;

use algo_truncate_a_file::truncate_file;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("truncate-file-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn cases() {
    for (name, rel, length, before, after) in [
        ("shrink", "a.txt", 5, "alpha beta\n", "alpha"),
        ("extend", "a.txt", 8, "alpha", "alpha\0\0\0"),
        ("same-length", "a.txt", 6, "alpha\n", "alpha\n"),
        ("to-zero", "a.txt", 0, "alpha\n", ""),
        ("empty-file", "a.txt", 3, "", "\0\0\0"),
        ("name-with-spaces", "my notes.txt", 2, "alpha\n", "al"),
    ] {
        let dir = new_dir(name);
        let path = dir.join(rel);
        fs::write(&path, before).unwrap();
        truncate_file(&path, length).unwrap();
        assert_eq!(fs::read(&path).unwrap(), after.as_bytes(), "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn errors() {
    let dir = new_dir("errors");
    let path = dir.join("a.txt");
    assert_eq!(truncate_file(&path, 0).unwrap_err().kind(), ErrorKind::NotFound);
    assert!(fs::symlink_metadata(&path).is_err());
    fs::create_dir(dir.join("dir")).unwrap();
    assert_eq!(truncate_file(&dir.join("dir"), 0).unwrap_err().kind(), ErrorKind::IsADirectory);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn open_in_append_mode() {
    let dir = new_dir("append");
    let path = dir.join("app.log");
    fs::write(&path, "alpha beta\n").unwrap();
    let mut log = OpenOptions::new().append(true).open(&path).unwrap();
    truncate_file(&path, 0).unwrap();
    log.write_all(b"new\n").unwrap();
    drop(log);
    assert_eq!(fs::read_to_string(&path).unwrap(), "new\n");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn open_after_the_new_end() {
    let dir = new_dir("hole");
    let path = dir.join("data.bin");
    fs::write(&path, "alpha beta\n").unwrap();
    let mut f = OpenOptions::new().write(true).open(&path).unwrap();
    f.seek(SeekFrom::End(0)).unwrap();
    truncate_file(&path, 5).unwrap();
    f.write_all(b"X").unwrap();
    drop(f);
    assert_eq!(fs::read(&path).unwrap(), b"alpha\0\0\0\0\0\0X");
    fs::remove_dir_all(&dir).unwrap();
}
