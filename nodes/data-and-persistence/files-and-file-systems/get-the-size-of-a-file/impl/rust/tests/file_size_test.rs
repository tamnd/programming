// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

use std::fs::{self, File};
use std::io::{ErrorKind, Write};
use std::path::PathBuf;

use algo_get_the_size_of_a_file::file_size;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("file-size-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn cases() {
    for (name, rel, data, size) in [
        ("text", "a.txt", "alpha\n".as_bytes(), 6),
        ("non-ascii-text", "a.txt", "café\n".as_bytes(), 6),
        ("empty-file", "a.txt", b"".as_slice(), 0),
        ("binary-data", "a.bin", b"\x00\r\n\xc3\xbf\x00".as_slice(), 6),
        ("crlf-line-ends", "a.txt", b"a\r\nb\r\n".as_slice(), 6),
        ("name-with-spaces", "my notes.txt", b"alpha\n".as_slice(), 6),
        ("non-ascii-name", "café 日本.txt", b"alpha\n".as_slice(), 6),
    ] {
        let dir = new_dir(name);
        let path = dir.join(rel);
        fs::write(&path, data).unwrap();
        assert_eq!(file_size(&path).unwrap(), size, "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn errors() {
    let dir = new_dir("errors");
    assert_eq!(file_size(&dir.join("a.txt")).unwrap_err().kind(), ErrorKind::NotFound);
    fs::create_dir(dir.join("dir")).unwrap();
    assert_eq!(file_size(&dir.join("dir")).unwrap_err().kind(), ErrorKind::IsADirectory);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn symbolic_link() {
    let dir = new_dir("link");
    fs::write(dir.join("a.txt"), "alpha beta\n").unwrap();
    std::os::unix::fs::symlink("a.txt", dir.join("link")).unwrap();
    assert_eq!(file_size(&dir.join("link")).unwrap(), 11);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn sparse_file_of_5_gib() {
    let dir = new_dir("sparse");
    let path = dir.join("sparse.bin");
    File::create(&path).unwrap().set_len(5 << 30).unwrap();
    assert_eq!(file_size(&path).unwrap(), 5368709120);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn open_file() {
    let dir = new_dir("open");
    let path = dir.join("app.log");
    let mut f = File::create(&path).unwrap(); // a File has no buffer
    f.write_all(b"alpha\n").unwrap();
    assert_eq!(file_size(&path).unwrap(), 6);
    drop(f);
    fs::remove_dir_all(&dir).unwrap();
}
