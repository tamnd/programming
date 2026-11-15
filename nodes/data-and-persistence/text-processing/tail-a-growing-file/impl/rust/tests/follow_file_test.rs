// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

use std::cell::Cell;
use std::fs::{self, OpenOptions};
use std::io::{ErrorKind, Write};
use std::path::PathBuf;
use std::sync::atomic::{AtomicU32, Ordering};
use std::time::Duration;

use algo_follow_file::{follow_file, Follower};

/// (name, initial, steps, the lines of each poll)
type Case<'a> = (&'a str, &'a str, &'a [&'a str], &'a [&'a [&'a str]]);

/// Makes a new directory for one test.
fn temp_dir() -> PathBuf {
    static NEXT: AtomicU32 = AtomicU32::new(0);
    let n = NEXT.fetch_add(1, Ordering::Relaxed);
    let dir = std::env::temp_dir().join(format!("follow-file-{}-{n}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    dir
}

const CASES: &[Case] = &[
    (
        "append-lines",
        "old\n",
        &["append:a\nb\n", "poll", "append:c\n", "poll"],
        &[&["a", "b"], &["c"]],
    ),
    (
        "partial-line",
        "",
        &[
            "append:ab",
            "poll",
            "append:c\nd",
            "poll",
            "append:\n",
            "poll",
        ],
        &[&[], &["abc"], &["d"]],
    ),
    (
        "rotate",
        "1\n",
        &["append:2\n", "poll", "rotate", "append:3\n", "poll"],
        &[&["2"], &["3"]],
    ),
    (
        "truncate",
        "",
        &[
            "append:a\nb\n",
            "poll",
            "truncate:",
            "poll",
            "append:c\n",
            "poll",
        ],
        &[&["a", "b"], &[], &["c"]],
    ),
    ("no-change", "x\n", &["poll", "poll"], &[&[], &[]]),
    (
        "empty-line",
        "",
        &["append:\n\na\n", "poll"],
        &[&["", "", "a"]],
    ),
    (
        "crlf",
        "",
        &["append:a\r\nb\r\n", "poll"],
        &[&["a\r", "b\r"]],
    ),
    (
        "truncate-and-write",
        "",
        &["append:aaaa\n", "poll", "truncate:b\n", "poll"],
        &[&["aaaa"], &["b"]],
    ),
    (
        "truncate-partial",
        "",
        &["append:abc", "poll", "truncate:d\n", "poll"],
        &[&[], &["abc", "d"]],
    ),
    (
        "rotate-late-writes",
        "",
        &[
            "append:1\n",
            "poll",
            "rotate",
            "append-old:2\n",
            "append:3\n",
            "poll",
        ],
        &[&["1"], &["2", "3"]],
    ),
    (
        "rotate-partial",
        "",
        &["append:a", "poll", "rotate", "append:b\n", "poll"],
        &[&[], &["a", "b"]],
    ),
    (
        "rotate-twice",
        "",
        &[
            "append:1\n",
            "poll",
            "rotate",
            "append:2\n",
            "poll",
            "rotate",
            "append:3\n",
            "poll",
        ],
        &[&["1"], &["2"], &["3"]],
    ),
    (
        "delete",
        "",
        &["append:1\n", "poll", "delete", "poll", "poll"],
        &[&["1"], &[], &[]],
    ),
    (
        "delete-partial",
        "",
        &["append:x", "poll", "delete", "poll"],
        &[&[], &["x"]],
    ),
    (
        "delete-and-create",
        "",
        &["append:1\n", "poll", "delete", "poll", "create:2\n", "poll"],
        &[&["1"], &[], &["2"]],
    ),
    (
        "delete-and-create-between-polls",
        "",
        &["append:1\n", "poll", "delete", "create:2\n", "poll"],
        &[&["1"], &["2"]],
    ),
    (
        "start-mid-line",
        "abc",
        &["append:d\ne\n", "poll"],
        &[&["d", "e"]],
    ),
    (
        "non-ascii",
        "",
        &["append:caf\u{e9}\nna\u{ef}ve\n", "poll"],
        &[&["caf\u{e9}", "na\u{ef}ve"]],
    ),
];

#[test]
fn cases() {
    for (name, initial, steps, want) in CASES {
        let dir = temp_dir();
        let got = follow_file(dir.join("followed.txt"), initial, steps).unwrap();
        assert_eq!(got, *want, "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn not_found() {
    let dir = temp_dir();
    let err = follow_file(dir.join("missing").join("followed.txt"), "", &["poll"]).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    let err = Follower::new(dir.join("missing.txt")).err().unwrap();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn many_lines() {
    let dir = temp_dir();
    let text: String = (0..100000).map(|i| format!("{i}\n")).collect();
    let step = format!("append:{text}");
    let got = follow_file(dir.join("followed.txt"), "", &[&step, "poll"]).unwrap();
    assert_eq!(got[0].len(), 100000);
    assert_eq!(got[0][0], "0");
    assert_eq!(got[0][99999], "99999");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn character_across_polls() {
    let dir = temp_dir();
    let path = dir.join("followed.txt");
    fs::write(&path, "").unwrap();
    let mut follower = Follower::new(&path).unwrap();
    let mut file = OpenOptions::new().append(true).open(&path).unwrap();
    file.write_all(b"caf\xc3").unwrap();
    assert!(follower.poll().unwrap().is_empty());
    file.write_all(b"\xa9\n").unwrap();
    assert_eq!(follower.poll().unwrap(), ["caf\u{e9}"]);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn follow() {
    let dir = temp_dir();
    let path = dir.join("followed.txt");
    fs::write(&path, "old\n").unwrap();
    let mut follower = Follower::new(&path).unwrap();
    // Write the lines first, so that the test ends on its own.
    OpenOptions::new()
        .append(true)
        .open(&path)
        .unwrap()
        .write_all(b"a\nb\n")
        .unwrap();
    // Stop after 2 lines, or after 500 polls (5 seconds) if a line is lost.
    let count = Cell::new(0);
    let mut polls = 0;
    let mut lines = Vec::new();
    let stop = || {
        polls += 1;
        count.get() >= 2 || polls > 500
    };
    let emit = |line| {
        lines.push(line);
        count.set(count.get() + 1);
    };
    follower
        .follow(Duration::from_millis(10), stop, emit)
        .unwrap();
    assert_eq!(lines, ["a", "b"]);
    fs::remove_dir_all(&dir).unwrap();
}
