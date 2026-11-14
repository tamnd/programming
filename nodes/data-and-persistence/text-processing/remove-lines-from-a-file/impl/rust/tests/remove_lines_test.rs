// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;
use std::sync::atomic::{AtomicU32, Ordering};

use algo_remove_lines::remove_lines;

/// (name, the bytes of the file, start, count, the result, the bytes after)
type Case<'a> = (&'a str, &'a [u8], i32, i32, i32, &'a [u8]);

/// Makes a new directory for one test and writes the file input.txt in it.
fn temp_file(text: &[u8]) -> (PathBuf, PathBuf) {
    static NEXT: AtomicU32 = AtomicU32::new(0);
    let n = NEXT.fetch_add(1, Ordering::Relaxed);
    let dir = std::env::temp_dir().join(format!("remove-lines-{}-{n}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join("input.txt");
    fs::write(&path, text).unwrap();
    (dir, path)
}

#[test]
fn cases() {
    // 100000 lines with the numbers 0 to 99999. Lines 2 to 99999 go.
    let many: String = (0..100000).map(|i| format!("{i}\n")).collect();
    let cases: [Case; 16] = [
        ("first-two", b"a\nb\nc\nd\n", 1, 2, 2, b"c\nd\n"),
        ("middle", b"a\nb\nc\nd\n", 2, 2, 2, b"a\nd\n"),
        ("last", b"a\nb\nc\n", 3, 1, 1, b"a\nb\n"),
        ("past-the-end", b"a\nb\n", 3, 1, 0, b"a\nb\n"),
        ("partly-past-the-end", b"a\nb\nc\n", 2, 5, 2, b"a\n"),
        ("count-zero", b"a\nb\n", 1, 0, 0, b"a\nb\n"),
        ("all-lines", b"a\nb\n", 1, 2, 2, b""),
        ("empty", b"", 1, 1, 0, b""),
        ("empty-line", b"a\n\nb\n", 2, 1, 1, b"a\nb\n"),
        ("no-final-newline", b"a\nb", 1, 1, 1, b"b"),
        ("remove-incomplete-line", b"a\nb", 2, 1, 1, b"a\n"),
        ("crlf", b"a\r\nb\r\nc\r\n", 2, 1, 1, b"a\r\nc\r\n"),
        ("lone-cr", b"a\rb\nc\n", 1, 1, 1, b"c\n"),
        (
            "non-ascii",
            "caf\u{e9}\nna\u{ef}ve\nzo\u{eb}\n".as_bytes(),
            2,
            1,
            1,
            "caf\u{e9}\nzo\u{eb}\n".as_bytes(),
        ),
        ("large-count", b"a\nb\n", 2, i32::MAX, 1, b"a\n"),
        (
            "many-lines",
            many.as_bytes(),
            2,
            99998,
            99998,
            b"0\n99999\n",
        ),
    ];
    for (name, text, start, count, want, after) in cases {
        let (dir, path) = temp_file(text);
        assert_eq!(remove_lines(&path, start, count).unwrap(), want, "{name}");
        assert_eq!(fs::read(&path).unwrap(), after, "{name}");
        // No temporary file stays in the directory.
        assert_eq!(fs::read_dir(&dir).unwrap().count(), 1, "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[cfg(unix)]
#[test]
fn keeps_the_permissions() {
    use std::os::unix::fs::PermissionsExt;
    let (dir, path) = temp_file(b"a\nb\n");
    fs::set_permissions(&path, fs::Permissions::from_mode(0o640)).unwrap();
    assert_eq!(remove_lines(&path, 1, 1).unwrap(), 1);
    assert_eq!(
        fs::metadata(&path).unwrap().permissions().mode() & 0o777,
        0o640
    );
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn not_found() {
    let path = std::env::temp_dir().join("remove-lines-missing.txt");
    assert_eq!(
        remove_lines(&path, 1, 1).unwrap_err().kind(),
        ErrorKind::NotFound
    );
}

#[test]
fn bad_range() {
    for (start, count) in [(0, 1), (1, -1)] {
        let (dir, path) = temp_file(b"a\nb\n");
        assert_eq!(
            remove_lines(&path, start, count).unwrap_err().kind(),
            ErrorKind::InvalidInput
        );
        assert_eq!(fs::read(&path).unwrap(), b"a\nb\n");
        fs::remove_dir_all(&dir).unwrap();
    }
}
