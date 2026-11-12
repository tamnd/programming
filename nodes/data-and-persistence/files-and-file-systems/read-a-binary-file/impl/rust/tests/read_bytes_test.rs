// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_read_a_binary_file::{read_bytes, read_exact};

/// Returns a new empty directory for one test.
fn temp_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("read-bytes-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

fn hex(s: &str) -> Vec<u8> {
    (0..s.len()).step_by(2).map(|i| u8::from_str_radix(&s[i..i + 2], 16).unwrap()).collect()
}

fn pattern(n: usize) -> Vec<u8> {
    (0..n).map(|i| i as u8).collect()
}

fn data(name: &str) -> Vec<u8> {
    match name {
        "signature" => hex("89504e470d0a1a0a0000000d49484452"),
        "bytes" => hex("0001027f80feff"),
        "empty" => Vec::new(),
        "all-bytes" => pattern(256),
        "zeros" => vec![0; 8],
        "crlf" => b"a\r\nb\r\n".to_vec(),
        "ctrl-z" => hex("611a62"),
        "not-utf8" => hex("c328fffe80"),
        "large" => pattern(1 << 20),
        _ => panic!("no data {name}"),
    }
}

/// Writes the data of one case to a new file and returns its path.
fn write(test: &str, name: &str) -> PathBuf {
    let path = temp_dir(&format!("{test}-{name}")).join("input.bin");
    fs::write(&path, data(name)).unwrap();
    path
}

#[test]
fn reads_bytes() {
    for name in ["signature", "bytes", "empty", "all-bytes", "zeros", "crlf", "ctrl-z", "not-utf8", "large"] {
        assert_eq!(read_bytes(write("bytes", name)).unwrap(), data(name), "case {name}");
    }
}

#[test]
fn reads_exact() {
    let cases = [
        ("signature-only", "signature", 8, hex("89504e470d0a1a0a")),
        ("first-four", "all-bytes", 4, hex("00010203")),
        ("whole-file", "zeros", 8, vec![0; 8]),
        ("zero-count", "empty", 0, Vec::new()),
        ("across-ctrl-z", "ctrl-z", 3, hex("611a62")),
        ("large", "large", 1 << 20, pattern(1 << 20)),
    ];
    for (name, file, n, want) in cases {
        assert_eq!(read_exact(write("exact", file), n).unwrap(), want, "case {name}");
    }
}

#[test]
fn short_file() {
    for (name, file, n) in [("one-past-end", "signature", 17), ("empty-file", "empty", 1)] {
        let err = read_exact(write("short", file), n).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::UnexpectedEof, "case {name}");
    }
}

#[test]
fn missing_file() {
    let path = temp_dir("missing").join("missing.bin");
    assert_eq!(read_bytes(&path).unwrap_err().kind(), ErrorKind::NotFound);
    assert_eq!(read_exact(&path, 1).unwrap_err().kind(), ErrorKind::NotFound);
}

#[test]
fn directory() {
    let dir = temp_dir("directory");
    assert_eq!(read_bytes(&dir).unwrap_err().kind(), ErrorKind::IsADirectory);
    assert_eq!(read_exact(&dir, 1).unwrap_err().kind(), ErrorKind::IsADirectory);
}
