// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_read_line::read_line;

fn temp_file(name: &str, text: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("read-line-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(format!("{name}.txt"));
    fs::write(&path, text).unwrap();
    path
}

#[test]
fn cases() {
    // 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
    let numbers: String = (0..100000).map(|i| format!("{i}\n")).collect();
    let cases: &[(&str, &str, i32, Option<&str>)] = &[
        ("first", "a\nb\nc\n", 1, Some("a")),
        ("middle", "a\nb\nc\n", 2, Some("b")),
        ("last", "a\nb\nc\n", 3, Some("c")),
        ("past-the-end", "a\nb\n", 3, None),
        ("zero", "a\n", 0, None),
        ("negative", "a\n", -1, None),
        ("empty", "", 1, None),
        ("empty-line", "a\n\nb\n", 2, Some("")),
        ("only-newline", "\n", 1, Some("")),
        ("no-final-newline", "a\nb", 2, Some("b")),
        ("crlf", "a\r\nb\r\n", 1, Some("a\r")),
        ("lone-cr", "a\rb\nc\n", 2, Some("c")),
        ("non-ascii", "caf\u{e9}\nzo\u{eb}\n", 2, Some("zo\u{eb}")),
        ("many-lines", &numbers, 100000, Some("99999")),
        ("many-lines-past-the-end", &numbers, 100001, None),
    ];
    for (name, text, number, want) in cases {
        let path = temp_file(name, text);
        let got = read_line(&path, *number).unwrap();
        assert_eq!(got.as_deref(), *want, "case {name}");
        fs::remove_file(path).unwrap();
    }
}

#[test]
fn not_found() {
    let err = read_line("no/such/dir/missing.txt", 1).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
