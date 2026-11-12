// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

use std::fs;
use std::io::ErrorKind;
use std::path::{Path, PathBuf};

use algo_read_and_write_with_buffers::copy_buffered;

const TEXT: &[u8] = b"alpha\nbeta\ngamma\n";

/// Returns a new empty directory for one test.
fn temp_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("copy-buffered-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

fn pattern(n: usize) -> Vec<u8> {
    (0..n).map(|i| i as u8).collect()
}

fn check(name: &str, data: &[u8], size: usize) {
    let dir = temp_dir(name);
    let (src, dst) = (dir.join("in.bin"), dir.join("out.bin"));
    fs::write(&src, data).unwrap();
    assert_eq!(copy_buffered(&src, &dst, size).unwrap(), data.len() as u64, "{name}");
    assert!(fs::read(&dst).unwrap() == data, "{name}: dst is not the same as src");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn copies() {
    let cases: [(&str, Vec<u8>, usize); 10] = [
        ("text", TEXT.to_vec(), 4096),
        ("several-buffers", TEXT.to_vec(), 4),
        ("empty", Vec::new(), 8),
        ("smaller-than-buffer", b"12345".to_vec(), 8),
        ("exact-buffer", b"12345678".to_vec(), 8),
        ("one-past-buffer", b"123456789".to_vec(), 8),
        ("size-one", TEXT.to_vec(), 1),
        ("binary", pattern(256), 64),
        ("large", pattern(1 << 20), 4096),
        ("large-odd-buffer", pattern(1 << 20), 4093),
    ];
    for (name, data, size) in &cases {
        check(name, data, *size);
    }
}

#[test]
fn replaces_a_longer_file() {
    let dir = temp_dir("replace");
    let (src, dst) = (dir.join("in.bin"), dir.join("out.bin"));
    fs::write(&src, "new").unwrap();
    fs::write(&dst, "old and longer").unwrap();
    assert_eq!(copy_buffered(&src, &dst, 8).unwrap(), 3);
    assert_eq!(fs::read(&dst).unwrap(), b"new");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn missing_src() {
    let dir = temp_dir("missing-src");
    let err = copy_buffered(dir.join("missing.bin"), dir.join("out.bin"), 8).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    assert!(!dir.join("out.bin").exists());
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn missing_dst_dir() {
    let dir = temp_dir("missing-dst-dir");
    fs::write(dir.join("in.bin"), TEXT).unwrap();
    let err = copy_buffered(dir.join("in.bin"), dir.join("no-such-dir").join("out.bin"), 8).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn zero_size() {
    // A usize cannot be negative, so -1 is not a case here.
    let dir = temp_dir("zero-size");
    fs::write(dir.join("in.bin"), TEXT).unwrap();
    let err = copy_buffered(dir.join("in.bin"), dir.join("out.bin"), 0).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::InvalidInput);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn full_device() {
    if !Path::new("/dev/full").exists() {
        return;
    }
    // 17 bytes stay in the buffer, so the error comes only at the flush.
    let dir = temp_dir("full");
    for n in [17, 1 << 20] {
        fs::write(dir.join("in.bin"), pattern(n)).unwrap();
        let err = copy_buffered(dir.join("in.bin"), "/dev/full", 4096).unwrap_err();
        assert_eq!(err.raw_os_error(), Some(28), "{n} bytes: {err}"); // ENOSPC is 28 on Linux.
    }
    fs::remove_dir_all(&dir).unwrap();
}
