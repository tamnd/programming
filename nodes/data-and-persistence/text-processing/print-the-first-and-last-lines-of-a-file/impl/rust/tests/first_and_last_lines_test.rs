// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_first_and_last_lines::first_and_last_lines;

fn temp_file(name: &str, text: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("first-and-last-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(format!("{name}.txt"));
    fs::write(&path, text).unwrap();
    path
}

fn numbers(from: u32, to: u32) -> Vec<String> {
    (from..to).map(|i| i.to_string()).collect()
}

fn text(lines: &[String]) -> String {
    lines.iter().map(|l| format!("{l}\n")).collect()
}

#[test]
fn cases() {
    let s = |v: &[&str]| v.iter().map(|x| x.to_string()).collect::<Vec<_>>();
    let cases: Vec<(&str, String, usize, Vec<String>, Vec<String>)> = vec![
        ("two", "a\nb\nc\nd\ne\n".into(), 2, s(&["a", "b"]), s(&["d", "e"])),
        ("one", "first\nmiddle\nlast\n".into(), 1, s(&["first"]), s(&["last"])),
        ("ten", text(&numbers(1, 13)), 10, numbers(1, 11), numbers(3, 13)),
        ("empty", "".into(), 2, s(&[]), s(&[])),
        ("zero", "a\nb\n".into(), 0, s(&[]), s(&[])),
        ("short", "a\nb\n".into(), 5, s(&["a", "b"]), s(&["a", "b"])),
        ("exact", "a\nb\n".into(), 2, s(&["a", "b"]), s(&["a", "b"])),
        ("overlap", "a\nb\nc\n".into(), 2, s(&["a", "b"]), s(&["b", "c"])),
        ("one-line", "a\n".into(), 1, s(&["a"]), s(&["a"])),
        ("empty-lines", "\n\na\n\n".into(), 1, s(&[""]), s(&[""])),
        ("only-newline", "\n".into(), 1, s(&[""]), s(&[""])),
        ("no-final-newline", "a\nb\nc".into(), 1, s(&["a"]), s(&["c"])),
        ("crlf", "a\r\nb\r\n".into(), 1, s(&["a\r"]), s(&["b\r"])),
        ("non-ascii", "caf\u{e9}\nna\u{ef}ve\nzo\u{eb}\n".into(), 1, s(&["caf\u{e9}"]), s(&["zo\u{eb}"])),
        // 100000 lines with the numbers 0 to 99999.
        ("many-lines", text(&numbers(0, 100000)), 3, numbers(0, 3), numbers(99997, 100000)),
    ];
    for (name, text, count, first, last) in cases {
        let path = temp_file(name, &text);
        let got = first_and_last_lines(&path, count).unwrap();
        assert_eq!(got, (first, last), "case {name}");
        fs::remove_file(path).unwrap();
    }
}

#[test]
fn not_found() {
    let err = first_and_last_lines("no/such/dir/missing.txt", 1).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}
