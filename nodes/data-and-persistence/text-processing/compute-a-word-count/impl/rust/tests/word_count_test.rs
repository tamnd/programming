// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_compute_a_word_count::{word_count, Counts};

/// Writes the bytes to a new file in the temporary directory and returns its path.
fn write(name: &str, data: &[u8]) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("word-count-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(name);
    fs::write(&path, data).unwrap();
    path
}

const fn counts(lines: u64, words: u64, bytes: u64) -> Counts {
    Counts { lines, words, bytes }
}

const CASES: &[(&str, &str, Counts)] = &[
    ("simple", "one two three\n", counts(1, 3, 14)),
    ("multiple-spaces", "  a   b  \n", counts(1, 2, 10)),
    ("tabs-and-lines", "a\tb\nc\n", counts(2, 3, 6)),
    ("empty", "", counts(0, 0, 0)),
    ("only-white-space", " \n\t\n", counts(2, 0, 4)),
    ("no-final-newline", "one two", counts(0, 2, 7)),
    ("crlf", "a b\r\nc\r\n", counts(2, 3, 8)),
    ("non-ascii", "na\u{ef}ve caf\u{e9}\n", counts(1, 2, 13)),
    ("unicode-spaces", "a\u{a0}b\u{3000}c\u{2028}d\n", counts(1, 4, 13)),
    ("not-white-space", "a\u{200b}b\u{ad}c\u{feff}d\n", counts(1, 1, 13)),
    ("control-separators", "a\x1cb\x1fc\n", counts(1, 1, 6)),
    ("nel", "a\u{85}b\n", counts(1, 2, 5)),
];

/// The bytes of files that are not valid UTF-8.
const INVALID: &[(&str, &[u8])] = &[
    ("invalid-bytes", b"ok\xff\xfe\n"),
    ("truncated", b"ok \xe2\x82"),
    ("surrogate", b"a\xed\xa0\x80b\n"),
];

#[test]
fn cases() {
    for (name, text, want) in CASES {
        let path = write(name, text.as_bytes());
        assert_eq!(word_count(&path).unwrap(), *want, "{name}");
        fs::remove_file(&path).unwrap();
    }
}

#[test]
fn many_lines() {
    // 100000 lines: more than one buffer of 64 KiB.
    let path = write("many-lines", &b"a b\n".repeat(100_000));
    assert_eq!(word_count(&path).unwrap(), counts(100_000, 200_000, 400_000));
    fs::remove_file(&path).unwrap();
}

#[test]
fn chunk_boundary() {
    // The buffer ends inside the 3 bytes of U+3000.
    let mut data = vec![b'x'; 64 * 1024 - 1];
    data.extend_from_slice("\u{3000}y\n".as_bytes());
    let path = write("chunk-boundary", &data);
    assert_eq!(word_count(&path).unwrap(), counts(1, 2, 64 * 1024 + 4));
    fs::remove_file(&path).unwrap();
}

#[test]
fn invalid_utf8() {
    for (name, data) in INVALID {
        let path = write(name, data);
        let err = word_count(&path).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::InvalidData, "{name}");
        fs::remove_file(&path).unwrap();
    }
}

#[test]
fn missing_file() {
    let err = word_count(std::env::temp_dir().join("word-count-missing.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
