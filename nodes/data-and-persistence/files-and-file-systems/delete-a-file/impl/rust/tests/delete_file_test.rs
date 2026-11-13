// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

use std::fs;
use std::io::{ErrorKind, Read};
use std::os::unix::fs::{symlink, PermissionsExt};
use std::path::{Path, PathBuf};

use algo_delete_a_file::delete_file;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("delete-file-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

fn lexists(path: &Path) -> bool {
    fs::symlink_metadata(path).is_ok()
}

#[test]
fn cases() {
    for (name, rel, content, want) in [
        ("delete", "a.txt", Some("alpha\n"), true),
        ("missing", "a.txt", None, false),
        ("empty-file", "a.txt", Some(""), true),
        ("missing-parent", "no-such-dir/a.txt", None, false),
        ("name-with-spaces", "my notes.txt", Some("alpha\n"), true),
        ("non-ascii-name", "café 日本.txt", Some("alpha\n"), true),
    ] {
        let dir = new_dir(name);
        let path = dir.join(rel);
        if let Some(c) = content {
            fs::write(&path, c).unwrap();
        }
        assert_eq!(delete_file(&path).unwrap(), want, "{name}");
        assert!(!lexists(&path), "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn through_a_file() {
    let dir = new_dir("through");
    fs::write(dir.join("a.txt"), "alpha\n").unwrap();
    assert!(!delete_file(dir.join("a.txt/b.txt")).unwrap());
    assert!(dir.join("a.txt").exists());
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn directory() {
    let dir = new_dir("directory");
    fs::create_dir(dir.join("dir")).unwrap();
    assert!(delete_file(dir.join("dir")).is_err());
    assert!(dir.join("dir").is_dir());
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn links() {
    let dir = new_dir("links");
    fs::write(dir.join("a.txt"), "alpha\n").unwrap();
    fs::create_dir(dir.join("dir")).unwrap();
    symlink("a.txt", dir.join("to-file")).unwrap();
    symlink("dir", dir.join("to-dir")).unwrap();
    symlink("missing.txt", dir.join("broken")).unwrap();
    for link in ["to-file", "to-dir", "broken"] {
        assert!(delete_file(dir.join(link)).unwrap(), "{link}");
        assert!(!lexists(&dir.join(link)), "{link}");
    }
    assert_eq!(fs::read_to_string(dir.join("a.txt")).unwrap(), "alpha\n");
    assert!(dir.join("dir").is_dir());
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn read_only_file() {
    let dir = new_dir("read-only");
    let path = dir.join("a.txt");
    fs::write(&path, "alpha\n").unwrap();
    fs::set_permissions(&path, fs::Permissions::from_mode(0o444)).unwrap();
    assert!(delete_file(&path).unwrap());
    assert!(!lexists(&path));
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn directory_with_no_write_permission() {
    let dir = new_dir("locked");
    let locked = dir.join("locked");
    fs::create_dir(&locked).unwrap();
    let path = locked.join("a.txt");
    fs::write(&path, "alpha\n").unwrap();
    fs::set_permissions(&locked, fs::Permissions::from_mode(0o500)).unwrap();
    // Root can write to each directory, so there is no error for root.
    let probe = locked.join("probe");
    let root = fs::write(&probe, "").is_ok();
    if !root {
        let err = delete_file(&path).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::PermissionDenied);
        assert!(path.exists());
    }
    fs::set_permissions(&locked, fs::Permissions::from_mode(0o700)).unwrap();
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn open_file() {
    let dir = new_dir("open");
    let path = dir.join("a.txt");
    fs::write(&path, "alpha\n").unwrap();
    let mut f = fs::File::open(&path).unwrap();
    assert!(delete_file(&path).unwrap());
    assert!(!lexists(&path));
    let mut s = String::new();
    f.read_to_string(&mut s).unwrap(); // on Unix the data stays until the close
    assert_eq!(s, "alpha\n");
    fs::remove_dir_all(&dir).unwrap();
}
