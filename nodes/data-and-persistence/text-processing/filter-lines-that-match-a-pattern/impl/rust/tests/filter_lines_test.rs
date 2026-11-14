// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_filter_lines::filter_lines;

fn temp_file(name: &str, text: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("filter-lines-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(format!("{name}.txt"));
    fs::write(&path, text).unwrap();
    path
}

#[test]
fn cases() {
    // 100000 lines with the numbers 0 to 99999. Five of them hold only nines.
    let numbers: String = (0..100000).map(|i| format!("{i}\n")).collect();
    let cases: &[(&str, &str, &str, &[&str])] = &[
        ("literal", "apple\nbanana\ncherry\n", "an", &["banana"]),
        ("several", "cat\ndog\ncatalog\n", "cat", &["cat", "catalog"]),
        ("start", "ab\nba\n", "^a", &["ab"]),
        ("end", "ab\nba\n", "a$", &["ba"]),
        ("dot", "abc\nac\naxc\n", "a.c", &["abc", "axc"]),
        ("star", "ac\nabc\nabbc\nadc\n", "ab*c", &["ac", "abc", "abbc"]),
        ("whole-line", "a\nab\nba\n", "^a$", &["a"]),
        ("empty", "", "a", &[]),
        ("no-match", "abc\n", "x", &[]),
        ("empty-lines", "a\n\nb\n\n", "^$", &["", ""]),
        ("match-all", "a\n\nb\n", ".*", &["a", "", "b"]),
        ("duplicates", "a\nb\na\n", "a", &["a", "a"]),
        ("no-final-newline", "a\nab", "b", &["ab"]),
        ("crlf-end", "ab\r\nab\n", "b$", &["ab"]),
        ("crlf-dot", "a\r\n", "^a.$", &["a\r"]),
        ("case", "Apple\napple\n", "apple", &["apple"]),
        ("non-ascii", "caf\u{e9}\ncafe\n", "\u{e9}", &["caf\u{e9}"]),
        ("many-lines", &numbers, "^9*$", &["9", "99", "999", "9999", "99999"]),
    ];
    for (name, text, pattern, want) in cases {
        let path = temp_file(name, text);
        let got = filter_lines(&path, pattern).unwrap();
        assert_eq!(got, *want, "case {name}");
        fs::remove_file(path).unwrap();
    }
}

#[test]
fn not_found() {
    let err = filter_lines("no/such/dir/missing.txt", "a").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
