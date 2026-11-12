// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_read_a_whole_file::read_text;

/// Returns a new empty directory for one test.
fn temp_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("read-text-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

/// Writes the bytes to a new file and returns its path.
fn write(name: &str, data: &[u8]) -> PathBuf {
    let path = temp_dir(name).join("input.txt");
    fs::write(&path, data).unwrap();
    path
}

#[test]
fn cases() {
    let large = "0123456789abcdef".repeat(65536);
    let cases = [
        ("lf", "alpha\nbeta\n"),
        ("paragraphs", "first line\n\nsecond paragraph\n"),
        ("non-ascii", "naïve café\n日本語\n🙂\n"),
        ("empty", ""),
        ("no-final-newline", "one\ntwo"),
        ("trailing-newlines", "end\n\n\n"),
        ("crlf", "one\r\ntwo\r\n"),
        ("mixed-endings", "lf\ncrlf\r\ncr\rend"),
        ("bom", "\u{FEFF}head\n"),
        ("whitespace", "  two spaces\ttab \n"),
        ("large", large.as_str()),
    ];
    for (name, text) in cases {
        assert_eq!(read_text(write(name, text.as_bytes())).unwrap(), text, "{name}");
    }
}

#[test]
fn missing_file() {
    let err = read_text(temp_dir("missing").join("missing.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn directory() {
    assert!(read_text(temp_dir("directory")).is_err());
}

#[test]
fn invalid_utf8() {
    let cases: [(&str, &[u8]); 4] = [
        ("invalid-byte", b"ok\n\xff\xfe\n"),
        ("truncated-at-end", b"ok\ncaf\xc3"),
        ("encoded-surrogate", b"a\xed\xa0\x80b\n"),
        ("overlong", b"\xc0\xaf\n"),
    ];
    for (name, data) in cases {
        let err = read_text(write(name, data)).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::InvalidData, "{name}");
    }
}
