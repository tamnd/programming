// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_diff_two_texts::{diff_files, diff_lines};

/// (name, the old text, the new text, the edit script)
const CASES: &[(&str, &str, &str, &str)] = &[
    ("simple", "a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"),
    (
        "change-a-line",
        "one\ntwo\nthree\n",
        "one\n2\nthree\n",
        " one\n-two\n+2\n three\n",
    ),
    ("identical", "a\nb\n", "a\nb\n", " a\n b\n"),
    (
        "lines-of-a-poem",
        "The cat sat\non the mat.\nIt was flat.\n",
        "The cat sat\non the hat.\nIt was flat.\nThe end.\n",
        " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n",
    ),
    ("both-empty", "", "", ""),
    ("old-empty", "", "a\nb\n", "+a\n+b\n"),
    ("new-empty", "a\nb\n", "", "-a\n-b\n"),
    ("all-different", "a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"),
    ("insert-at-start", "b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"),
    ("delete-at-end", "a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"),
    ("swap", "a\nb\n", "b\na\n", "-a\n b\n+a\n"),
    (
        "repeated-lines",
        "a\na\nb\n",
        "a\nb\nb\n",
        " a\n-a\n b\n+b\n",
    ),
    ("empty-line", "a\n\nb\n", "a\nb\n", " a\n-\n b\n"),
    ("no-final-newline", "a\nb", "a\nb\n", " a\n b\n"),
    ("crlf", "a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"),
    ("trailing-space", "a \nb\n", "a\nb\n", "-a \n+a\n b\n"),
    (
        "normalization",
        "caf\u{e9}\n",
        "cafe\u{301}\n",
        "-caf\u{e9}\n+cafe\u{301}\n",
    ),
];

/// Returns a new directory in the temporary directory, for one test.
fn temp_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("diff-two-texts-{}-{name}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn cases() {
    for (name, old, new, want) in CASES {
        assert_eq!(diff_lines(old, new), *want, "{name}");
    }
}

#[test]
fn many_lines() {
    // 1000 lines, and 10 of them change.
    let old: String = (0..1000).map(|i| format!("{i}\n")).collect();
    let new: String = (0..1000)
        .map(|i| {
            if i % 100 == 50 {
                format!("x{i}\n")
            } else {
                format!("{i}\n")
            }
        })
        .collect();
    let script = diff_lines(&old, &new);
    let lines: Vec<&str> = script.lines().collect();
    assert_eq!(lines.len(), 1010);
    assert_eq!(lines[49..52], [" 49", "-50", "+x50"]);
    assert_eq!(lines[1009], " 999");
}

#[test]
fn files() {
    let (_, old, new, want) = CASES.iter().find(|c| c.0 == "crlf").unwrap();
    let dir = temp_dir("files");
    fs::write(dir.join("old.txt"), old).unwrap();
    fs::write(dir.join("new.txt"), new).unwrap();
    assert_eq!(
        diff_files(dir.join("old.txt"), dir.join("new.txt")).unwrap(),
        *want
    );
    fs::remove_dir_all(dir).unwrap();
}

#[test]
fn not_found() {
    let dir = temp_dir("not-found");
    fs::write(dir.join("new.txt"), "a\n").unwrap();
    let err = diff_files(dir.join("missing.txt"), dir.join("new.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    fs::remove_dir_all(dir).unwrap();
}
