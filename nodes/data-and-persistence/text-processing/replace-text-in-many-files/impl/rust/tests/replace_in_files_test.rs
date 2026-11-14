// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;
use std::sync::atomic::{AtomicU32, Ordering};

use algo_replace_in_files::replace_in_files;

/// (file name, the text before, the text after)
type File<'a> = (&'a str, &'a str, &'a str);

/// (name, old, new, the files, the result)
type Case<'a> = (&'a str, &'a str, &'a str, &'a [File<'a>], &'a [usize]);

/// Makes a new directory for one test.
fn temp_dir() -> PathBuf {
    static NEXT: AtomicU32 = AtomicU32::new(0);
    let n = NEXT.fetch_add(1, Ordering::Relaxed);
    let dir = std::env::temp_dir().join(format!("replace-in-files-{}-{n}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn cases() {
    let cases: [Case; 19] = [
        (
            "rosetta",
            "Goodbye London!",
            "Hello New York!",
            &[
                ("a.txt", "Goodbye London!\n", "Hello New York!\n"),
                (
                    "b.txt",
                    "Goodbye London! Goodbye London!\nGoodbye\n",
                    "Hello New York! Hello New York!\nGoodbye\n",
                ),
            ],
            &[1, 2],
        ),
        (
            "some-files-match",
            "dog",
            "fox",
            &[
                ("a.txt", "cat\n", "cat\n"),
                ("b.txt", "dog\ncat dog\n", "fox\ncat fox\n"),
            ],
            &[0, 2],
        ),
        (
            "one-file",
            "1",
            "2",
            &[("a.txt", "x = 1\ny = 1\n", "x = 2\ny = 2\n")],
            &[2],
        ),
        ("no-files", "a", "b", &[], &[]),
        ("no-match", "x", "y", &[("a.txt", "abc\n", "abc\n")], &[0]),
        ("empty-file", "a", "b", &[("a.txt", "", "")], &[0]),
        ("delete", "-", "", &[("a.txt", "a-b-c\n", "abc\n")], &[2]),
        ("whole-file", "abc", "", &[("a.txt", "abc", "")], &[1]),
        (
            "new-contains-old",
            "a",
            "aa",
            &[("a.txt", "a a\n", "aa aa\n")],
            &[2],
        ),
        (
            "overlapping",
            "aa",
            "b",
            &[("a.txt", "aaa\n", "ba\n")],
            &[1],
        ),
        ("adjacent", "aa", "b", &[("a.txt", "aaaa\n", "bb\n")], &[2]),
        (
            "special-chars",
            "1.5",
            r"\&$1 $& [x]",
            &[("a.txt", "1.5 and 125\n", "\\&$1 $& [x] and 125\n")],
            &[1],
        ),
        (
            "glob-chars",
            "*.txt",
            "?",
            &[("a.txt", "a.txt *.txt\n", "a.txt ?\n")],
            &[1],
        ),
        (
            "across-lines",
            "end\nstart",
            "end start",
            &[("a.txt", "end\nstart\n", "end start\n")],
            &[1],
        ),
        (
            "lf-to-crlf",
            "\n",
            "\r\n",
            &[("a.txt", "a\nb\n", "a\r\nb\r\n")],
            &[2],
        ),
        (
            "crlf",
            "a",
            "c",
            &[("a.txt", "a\r\nb\r\n", "c\r\nb\r\n")],
            &[1],
        ),
        (
            "no-final-newline",
            "b",
            "c",
            &[("a.txt", "a b", "a c")],
            &[1],
        ),
        (
            "non-ascii",
            "\u{e9}",
            "e",
            &[("a.txt", "caf\u{e9}, cafe\n", "cafe, cafe\n")],
            &[1],
        ),
        (
            "case-sensitive",
            "hello",
            "bye",
            &[("a.txt", "Hello hello\n", "Hello bye\n")],
            &[1],
        ),
    ];
    for (name, old, new, files, want) in cases {
        let dir = temp_dir();
        let paths: Vec<PathBuf> = files.iter().map(|f| dir.join(f.0)).collect();
        for (f, p) in files.iter().zip(&paths) {
            fs::write(p, f.1).unwrap();
        }
        assert_eq!(replace_in_files(&paths, old, new).unwrap(), want, "{name}");
        for (f, p) in files.iter().zip(&paths) {
            assert_eq!(fs::read_to_string(p).unwrap(), f.2, "{name} after");
        }
        // No temporary file stays in the directory.
        assert_eq!(
            fs::read_dir(&dir).unwrap().count(),
            files.len(),
            "{name} files"
        );
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn many_lines() {
    let lines = |prefix: &str| {
        (0..100000)
            .map(|i| format!("{prefix}{i}\n"))
            .collect::<String>()
    };
    let dir = temp_dir();
    let path = dir.join("many.txt");
    fs::write(&path, lines("line ")).unwrap();
    assert_eq!(replace_in_files(&[&path], "line ", "L").unwrap(), [100000]);
    assert_eq!(fs::read_to_string(&path).unwrap(), lines("L"));
    fs::remove_dir_all(&dir).unwrap();
}

#[cfg(unix)]
#[test]
fn keeps_the_permissions() {
    use std::os::unix::fs::PermissionsExt;
    let dir = temp_dir();
    let path = dir.join("a.txt");
    fs::write(&path, "a\n").unwrap();
    fs::set_permissions(&path, fs::Permissions::from_mode(0o640)).unwrap();
    assert_eq!(replace_in_files(&[&path], "a", "b").unwrap(), [1]);
    assert_eq!(
        fs::metadata(&path).unwrap().permissions().mode() & 0o777,
        0o640
    );
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn errors() {
    let dir = temp_dir();
    let err = replace_in_files(&[dir.join("missing.txt")], "a", "b").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    let path = dir.join("a.txt");
    fs::write(&path, "a\n").unwrap();
    let err = replace_in_files(&[&path], "", "b").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::InvalidInput);
    assert_eq!(fs::read_to_string(&path).unwrap(), "a\n");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn stop() {
    let dir = temp_dir();
    fs::write(dir.join("a.txt"), "a\n").unwrap();
    fs::write(dir.join("c.txt"), "a\n").unwrap();
    let paths = [
        dir.join("a.txt"),
        dir.join("missing.txt"),
        dir.join("c.txt"),
    ];
    let err = replace_in_files(&paths, "a", "b").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    assert_eq!(fs::read_to_string(dir.join("a.txt")).unwrap(), "b\n");
    assert_eq!(fs::read_to_string(dir.join("c.txt")).unwrap(), "a\n");
    fs::remove_dir_all(&dir).unwrap();
}
