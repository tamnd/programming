// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_read_a_file_line_by_line::line_stats;

/// Writes the bytes to a new file in the temporary directory and returns its path.
fn write(name: &str, data: &[u8]) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("line-stats-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    let path = dir.join(name);
    fs::write(&path, data).unwrap();
    path
}

#[test]
fn cases() {
    let long = [&b"x".repeat(1 << 20)[..], b"\nend\n"].concat();
    let cases: [(&str, &[u8], (usize, usize)); 12] = [
        ("lf", b"alpha\nbeta\ngamma\n", (3, 5)),
        ("blank-lines", b"first\n\n\nlast line\n", (4, 9)),
        ("empty", b"", (0, 0)),
        ("only-newline", b"\n", (1, 0)),
        ("no-final-newline", b"one\ntwo\nthree", (3, 5)),
        ("crlf", b"one\r\ntwo\r\nthree\r\n", (3, 5)),
        ("crlf-no-final-newline", b"one\r\ntwo\r\nthree", (3, 5)),
        ("mixed-endings", b"one\r\ntwo\nthree\r\n", (3, 5)),
        ("whitespace", b"  two spaces\ttab \n", (1, 17)),
        ("backslashes", b"C:\\temp\\new\n", (1, 11)),
        ("long-line", &long, (2, 1 << 20)),
        (
            "non-ascii",
            "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n".as_bytes(),
            (3, 10),
        ),
    ];
    for (name, data, want) in cases {
        let got = line_stats(write(name, data)).unwrap();
        assert_eq!(got, want, "case {name}");
    }
}

#[test]
fn missing_file() {
    let err = line_stats(write("present", b"").with_file_name("missing")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn invalid_utf8() {
    let cases: [(&str, &[u8]); 4] = [
        ("invalid-byte", b"ok\n\xff\xfe\n"),
        ("truncated-at-end", b"ok\ncaf\xc3"),
        ("encoded-surrogate", b"a\xed\xa0\x80b\n"),
        ("overlong", b"\xc0\xaf\n"),
    ];
    for (name, data) in cases {
        let err = line_stats(write(name, data)).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::InvalidData, "case {name}");
    }
}
