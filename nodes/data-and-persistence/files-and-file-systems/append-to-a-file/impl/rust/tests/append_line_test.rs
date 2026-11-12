// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;
use std::thread;

use algo_append_to_a_file::append_line;

/// Returns a new empty directory for one test.
fn temp_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("append-line-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn cases() {
    // name, old content or None, line, content after
    let cases: [(&str, Option<&str>, &str, &str); 9] = [
        ("new-file", None, "first", "first\n"),
        ("append-existing", Some("one\n"), "two", "one\ntwo\n"),
        ("new-file-empty-line", None, "", "\n"),
        ("empty-line", Some("a\n"), "", "a\n\n"),
        ("empty-file", Some(""), "x", "x\n"),
        ("no-final-newline", Some("one"), "two", "onetwo\n"),
        ("crlf-file", Some("one\r\n"), "two", "one\r\ntwo\n"),
        ("whitespace", Some(""), "  two spaces\ttab ", "  two spaces\ttab \n"),
        ("non-ascii", Some("naïve\n"), "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n"),
    ];
    for (name, old, line, want) in cases {
        let path = temp_dir(name).join("log.txt");
        if let Some(old) = old {
            fs::write(&path, old).unwrap();
        }
        append_line(&path, line).unwrap();
        assert_eq!(fs::read_to_string(&path).unwrap(), want, "{name}");
    }
}

#[test]
fn missing_dir() {
    let err = append_line(temp_dir("missing").join("missing").join("log.txt"), "x").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn invalid_line() {
    for (name, line) in [("newline", "a\nb"), ("cr", "a\rb")] {
        let path = temp_dir(name).join("log.txt");
        let err = append_line(&path, line).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::InvalidInput);
        assert!(!path.exists());
    }
}

fn text(w: usize, i: usize) -> String {
    format!("writer {w} line {i:03} {}", "x".repeat(40))
}

#[test]
fn many_writers() {
    let path = temp_dir("many").join("log.txt");
    let (writers, lines) = (8, 250);
    let handles: Vec<_> = (0..writers)
        .map(|w| {
            let path = path.clone();
            thread::spawn(move || {
                for i in 0..lines {
                    append_line(&path, &text(w, i)).unwrap();
                }
            })
        })
        .collect();
    for h in handles {
        h.join().unwrap();
    }
    let mut got: Vec<String> = fs::read_to_string(&path).unwrap().lines().map(String::from).collect();
    let mut want: Vec<String> = (0..writers).flat_map(|w| (0..lines).map(move |i| text(w, i))).collect();
    got.sort();
    want.sort();
    assert_eq!(got, want);
}
