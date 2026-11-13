// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

use std::fs::{self, File, OpenOptions};
use std::io::{ErrorKind, Write};
use std::path::{Path, PathBuf};
use std::time::{Duration, SystemTime, UNIX_EPOCH};

use algo_get_the_modification_time_of_a_file::file_mtime;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("file-mtime-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

/// Sets the modification time of path to ns nanoseconds since 1970.
fn set_mtime(path: &Path, ns: i64) {
    let d = Duration::from_nanos(ns.unsigned_abs());
    let t = if ns >= 0 { UNIX_EPOCH + d } else { UNIX_EPOCH - d };
    File::open(path).unwrap().set_modified(t).unwrap();
}

#[test]
fn cases() {
    for (name, rel, ns, want) in [
        ("whole-second", "a.txt", 1709296496000000000, 1709296496),
        ("sub-second", "a.txt", 1709296496500000000, 1709296496),
        ("epoch", "a.txt", 0, 0),
        ("before-1970", "a.txt", -86400000000000, -86400),
        ("before-1970-sub-second", "a.txt", -1500000000, -2),
        ("after-2038", "a.txt", 4102444800000000000, 4102444800),
        ("directory", "dir", 1709296496000000000, 1709296496),
        ("name-with-spaces", "my notes.txt", 1709296496000000000, 1709296496),
    ] {
        let dir = new_dir(name);
        let path = dir.join(rel);
        if rel == "dir" {
            fs::create_dir(&path).unwrap();
        } else {
            fs::write(&path, "alpha\n").unwrap();
        }
        set_mtime(&path, ns);
        assert_eq!(file_mtime(&path).unwrap(), want, "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn errors() {
    let dir = new_dir("errors");
    assert_eq!(file_mtime(&dir.join("a.txt")).unwrap_err().kind(), ErrorKind::NotFound);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn symbolic_link() {
    let dir = new_dir("link");
    fs::write(dir.join("a.txt"), "alpha\n").unwrap();
    set_mtime(&dir.join("a.txt"), 1709296496000000000);
    std::os::unix::fs::symlink("a.txt", dir.join("link")).unwrap();
    assert_eq!(file_mtime(&dir.join("link")).unwrap(), 1709296496);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn write_sets_the_present_time() {
    let dir = new_dir("write");
    let path = dir.join("a.txt");
    fs::write(&path, "alpha\n").unwrap();
    set_mtime(&path, 0);
    let before = SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_secs() as i64 - 1;
    OpenOptions::new().append(true).open(&path).unwrap().write_all(b"beta\n").unwrap();
    assert!(file_mtime(&path).unwrap() >= before);
    fs::remove_dir_all(&dir).unwrap();
}
