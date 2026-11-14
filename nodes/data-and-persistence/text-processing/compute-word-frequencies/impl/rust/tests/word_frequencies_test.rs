// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_compute_word_frequencies::word_frequencies;

/// Writes the bytes to a new file in the temporary directory and returns its path.
fn write(name: &str, data: &[u8]) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("word-frequencies-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(name);
    fs::write(&path, data).unwrap();
    path
}

const CASES: &[(&str, &[u8], usize, &[(&str, u64)])] = &[
    ("simple", b"the cat and the hat\n", 2, &[("the", 2), ("and", 1)]),
    ("case", b"The THE the tHe\n", 5, &[("the", 4)]),
    ("punctuation", b"it's well-dressed, isn't it?\n", 10,
        &[("it", 2), ("dressed", 1), ("isn", 1), ("s", 1), ("t", 1), ("well", 1)]),
    ("empty", b"", 3, &[]),
    ("no-letters", b"123 ... 42\n-- !!\n", 3, &[]),
    ("ties", b"b a c b a c\n", 2, &[("a", 2), ("b", 2)]),
    ("digits", b"abc123def 42 x9\n", 10, &[("abc", 1), ("def", 1), ("x", 1)]),
    ("n-zero", b"the cat and the hat\n", 0, &[]),
    ("n-above-count", b"x y x\n", 10, &[("x", 2), ("y", 1)]),
    ("crlf", b"one two\r\ntwo\r\n", 5, &[("two", 2), ("one", 1)]),
    ("non-ascii", "caf\u{e9} na\u{ef}ve cafe\n".as_bytes(), 10, &[("caf", 1), ("cafe", 1), ("na", 1), ("ve", 1)]),
    ("leading-separators", b"  ...hello, world. Hello\n", 5, &[("hello", 2), ("world", 1)]),
    ("no-final-newline", b"dog cat dog", 5, &[("dog", 2), ("cat", 1)]),
    ("not-utf8", b"ok \xff\xfe ok\n", 5, &[("ok", 2)]),
];

fn pairs(want: &[(&str, u64)]) -> Vec<(String, u64)> {
    want.iter().map(|&(w, c)| (w.to_string(), c)).collect()
}

#[test]
fn cases() {
    for &(name, data, n, want) in CASES {
        let path = write(&format!("{name}.txt"), data);
        assert_eq!(word_frequencies(&path, n).unwrap(), pairs(want), "{name}");
    }
}

#[test]
fn many_lines() {
    let path = write("many-lines.txt", "a b a\n".repeat(100000).as_bytes());
    assert_eq!(word_frequencies(&path, 3).unwrap(), pairs(&[("a", 200000), ("b", 100000)]));
}

#[test]
fn long_line() {
    // One line of 100000 words: longer than the buffer of 64 KiB.
    let path = write("long-line.txt", "word ".repeat(100000).as_bytes());
    assert_eq!(word_frequencies(&path, 3).unwrap(), pairs(&[("word", 100000)]));
}

#[test]
fn not_found() {
    let err = word_frequencies("no/such/dir/missing.txt", 3).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
