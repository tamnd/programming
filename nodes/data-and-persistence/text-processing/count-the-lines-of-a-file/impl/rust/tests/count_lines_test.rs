// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_count_the_lines_of_a_file::count_lines;

/// Writes the bytes to a new file in the temporary directory and returns its path.
fn write(name: &str, data: &[u8]) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("count-lines-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(name);
    fs::write(&path, data).unwrap();
    path
}

const CASES: &[(&str, &[u8], u64, u64)] = &[
    ("lf", b"alpha\nbeta\ngamma\n", 3, 3),
    ("blank-lines", b"first\n\n\nlast\n", 4, 4),
    ("empty", b"", 0, 0),
    ("only-newline", b"\n", 1, 1),
    ("no-final-newline", b"one\ntwo\nthree", 2, 3),
    ("one-line-no-newline", b"x", 0, 1),
    ("crlf", b"one\r\ntwo\r\n", 2, 2),
    ("crlf-no-final-newline", b"one\r\ntwo", 1, 2),
    ("cr-only", b"one\rtwo\r", 0, 1),
    ("other-breaks", "a\x0bb\x0cc\u{85}d\u{2028}e\n".as_bytes(), 1, 1),
    ("invalid-utf8", b"ok\n\xff\xfe\n", 2, 2),
    ("nul-bytes", b"a\0b\n\0\n", 2, 2),
];

#[test]
fn cases() {
    for (name, data, newlines, lines) in CASES {
        let path = write(name, data);
        assert_eq!(count_lines(&path).unwrap(), (*newlines, *lines), "{name}");
        fs::remove_file(&path).unwrap();
    }
}

#[test]
fn many_chunks() {
    // 100000 lines and a last line with no LF: more than one buffer of 64 KiB.
    let mut data = b"x\n".repeat(100_000);
    data.extend_from_slice(b"end");
    let path = write("many-chunks", &data);
    assert_eq!(count_lines(&path).unwrap(), (100_000, 100_001));
    fs::remove_file(&path).unwrap();
}

#[test]
fn missing_file() {
    let err = count_lines(std::env::temp_dir().join("count-lines-missing.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
