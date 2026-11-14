// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_sort_the_lines_of_a_file::sort_lines;

/// Writes the text to a new file in the temporary directory and returns its path.
fn write(name: &str, text: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("sort-lines-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(name);
    fs::write(&path, text).unwrap();
    path
}

const CASES: &[(&str, &str, &[&str])] = &[
    ("simple", "pear\napple\nfig\n", &["apple", "fig", "pear"]),
    ("case", "b\nB\na\nA\n", &["A", "B", "a", "b"]),
    ("numbers", "10\n9\n100\n", &["10", "100", "9"]),
    ("empty", "", &[]),
    ("one-empty-line", "\n", &[""]),
    ("empty-lines", "b\n\na\n\n", &["", "", "a", "b"]),
    ("duplicates", "b\na\nb\n", &["a", "b", "b"]),
    ("no-final-newline", "b\na", &["a", "b"]),
    ("crlf", "b\r\na\r\n", &["a\r", "b\r"]),
    ("prefix", "ab\na\n", &["a", "ab"]),
    ("whitespace", "b\n a\n\tc\n", &["\tc", " a", "b"]),
    ("non-ascii", "\u{e9}\nz\ne\n", &["e", "z", "\u{e9}"]),
    // U+FF61 is below U+1F600, but its UTF-16 code unit is above 0xD83D.
    ("supplementary", "\u{1f600}\n\u{ff61}\n", &["\u{ff61}", "\u{1f600}"]),
    ("other-line-breaks", "b\u{2028}a\nb\x1ca\nb\x0ca\n", &["b\x0ca", "b\x1ca", "b\u{2028}a"]),
];

#[test]
fn cases() {
    for &(name, text, want) in CASES {
        let path = write(&format!("{name}.txt"), text);
        assert_eq!(sort_lines(&path).unwrap(), want, "{name}");
    }
}

#[test]
fn many_lines() {
    let path = write("many-lines.txt", &"b\na\n".repeat(50000));
    let mut want = vec!["a"; 50000];
    want.extend(vec!["b"; 50000]);
    assert_eq!(sort_lines(&path).unwrap(), want);
}

#[test]
fn not_found() {
    let err = sort_lines("no/such/dir/missing.txt").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
