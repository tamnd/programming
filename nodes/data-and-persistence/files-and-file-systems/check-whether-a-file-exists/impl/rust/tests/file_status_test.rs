// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::{symlink, PermissionsExt};
use std::path::{Path, PathBuf};
use std::process::Command;

use algo_check_whether_a_file_exists::file_status;

/// Returns a new directory with a.txt, empty.txt and dir in it.
fn tree(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("file-status-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(dir.join("dir")).unwrap();
    fs::write(dir.join("a.txt"), "alpha\n").unwrap();
    fs::write(dir.join("empty.txt"), "").unwrap();
    dir
}

#[test]
fn status() {
    let dir = tree("status");
    for (name, want) in [
        ("a.txt", "file"),
        ("empty.txt", "file"),
        ("dir", "directory"),
        ("dir/", "directory"),
        ("missing.txt", "missing"),
        ("no-such-dir/a.txt", "missing"),
        ("a.txt/b.txt", "missing"),
    ] {
        assert_eq!(file_status(dir.join(name)).unwrap(), want, "{name}");
    }
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn links() {
    let dir = tree("links");
    symlink("a.txt", dir.join("to-file")).unwrap();
    symlink("dir", dir.join("to-dir")).unwrap();
    symlink("missing.txt", dir.join("to-nothing")).unwrap();
    assert_eq!(file_status(dir.join("to-file")).unwrap(), "file");
    assert_eq!(file_status(dir.join("to-dir")).unwrap(), "directory");
    assert_eq!(file_status(dir.join("to-nothing")).unwrap(), "broken-link");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn device() {
    if Path::new("/dev/null").exists() {
        assert_eq!(file_status("/dev/null").unwrap(), "other");
    }
}

#[test]
fn named_pipe() {
    let dir = tree("pipe");
    let pipe = dir.join("pipe");
    assert!(Command::new("mkfifo").arg(&pipe).status().unwrap().success());
    assert_eq!(file_status(&pipe).unwrap(), "other");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn permission_error() {
    let dir = tree("locked");
    let locked = dir.join("locked");
    fs::create_dir(&locked).unwrap();
    fs::write(locked.join("a.txt"), "alpha\n").unwrap();
    fs::set_permissions(&locked, fs::Permissions::from_mode(0o000)).unwrap();
    // Root can search each directory, so there is no error for root.
    let root = fs::read_dir(&locked).is_ok();
    let got = file_status(locked.join("a.txt"));
    fs::set_permissions(&locked, fs::Permissions::from_mode(0o700)).unwrap();
    if !root {
        assert_eq!(got.unwrap_err().kind(), ErrorKind::PermissionDenied);
    }
    fs::remove_dir_all(&dir).unwrap();
}
