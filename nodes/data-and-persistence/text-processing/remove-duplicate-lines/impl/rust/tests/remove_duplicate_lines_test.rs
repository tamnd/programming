// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_remove_duplicate_lines::remove_duplicate_lines;

/// Writes the text to a new file in the temporary directory and returns its path.
fn write(name: &str, text: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("remove-duplicate-lines-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(name);
    fs::write(&path, text).unwrap();
    path
}

const CASES: &[(&str, &str, &[&str])] = &[
    ("simple", "b\na\nb\nc\na\n", &["b", "a", "c"]),
    ("adjacent", "a\na\nb\n", &["a", "b"]),
    ("no-duplicates", "c\nb\na\n", &["c", "b", "a"]),
    ("empty", "", &[]),
    ("empty-lines", "\na\n\n\n", &["", "a"]),
    ("all-same", "x\nx\nx\n", &["x"]),
    ("no-final-newline", "a\nb\na", &["a", "b"]),
    ("crlf", "a\r\na\n", &["a\r", "a"]),
    ("case", "A\na\nA\n", &["A", "a"]),
    ("whitespace", "a \na\n a\na \n", &["a ", "a", " a"]),
    // U+00E9, then e and U+0301: the same letter, but not the same code points.
    ("normalization", "\u{e9}\ne\u{301}\n\u{e9}\n", &["\u{e9}", "e\u{301}"]),
];

#[test]
fn cases() {
    for &(name, text, want) in CASES {
        let path = write(&format!("{name}.txt"), text);
        assert_eq!(remove_duplicate_lines(&path).unwrap(), want, "{name}");
    }
}

#[test]
fn many_lines() {
    // 100000 lines with 1000 different lines.
    let text: String = (0..100000).map(|i| format!("{}\n", i % 1000)).collect();
    let want: Vec<String> = (0..1000).map(|i| i.to_string()).collect();
    let path = write("many-lines.txt", &text);
    assert_eq!(remove_duplicate_lines(&path).unwrap(), want);
}

#[test]
fn not_found() {
    let err = remove_duplicate_lines("no/such/dir/missing.txt").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
