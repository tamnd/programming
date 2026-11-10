// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

use std::io::{Cursor, Write};
use std::process::{Command, Stdio};

use algo_read_from_standard_input::{read_lines, read_stdin_lines};

fn case(name: &'static str, data: &str, want: &[&str]) -> (&'static str, Vec<u8>, Vec<String>) {
    (name, data.as_bytes().to_vec(), want.iter().map(|s| s.to_string()).collect())
}

fn cases() -> Vec<(&'static str, Vec<u8>, Vec<String>)> {
    let long = "x".repeat(1 << 20);
    vec![
        case("lf", "alpha\nbeta\ngamma\n", &["alpha", "beta", "gamma"]),
        case("blank-lines", "first\n\n\nlast line\n", &["first", "", "", "last line"]),
        case("empty", "", &[]),
        case("only-newline", "\n", &[""]),
        case("no-final-newline", "one\ntwo\nthree", &["one", "two", "three"]),
        case("crlf", "one\r\ntwo\r\nthree\r\n", &["one", "two", "three"]),
        case("crlf-no-final-newline", "one\r\ntwo\r\nthree", &["one", "two", "three"]),
        case("mixed-endings", "one\r\ntwo\nthree\r\n", &["one", "two", "three"]),
        case("trailing-blank-line", "last\n\n", &["last", ""]),
        case("whitespace", "  two spaces\ttab \n", &["  two spaces\ttab "]),
        case("backslashes", "C:\\temp\\new\n", &["C:\\temp\\new"]),
        case(
            "non-ascii",
            "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n",
            &["naïve café", "日本語のテキスト", "🙂🙂🙂🙂🙂🙂"],
        ),
        case("long-line", &format!("{long}\nend\n"), &[&long, "end"]),
    ]
}

#[test]
fn reader_cases() {
    for (name, data, want) in cases() {
        let got = read_lines(Cursor::new(data)).unwrap();
        assert!(got == want, "case {name}");
    }
}

/// The child mode of this test binary: it reads its real standard input and prints the lines.
#[test]
fn child_reads_stdin() {
    if std::env::var_os("READ_LINES_CHILD").is_none() {
        return;
    }
    let lines = read_stdin_lines().unwrap();
    println!("lines={lines:?}");
}

/// Runs this test binary again, with each case on its standard input.
#[test]
fn stdin_cases() {
    let exe = std::env::current_exe().unwrap();
    for (name, data, want) in cases() {
        let mut child = Command::new(&exe)
            .args(["--exact", "child_reads_stdin", "--nocapture", "--test-threads=1"])
            .env("READ_LINES_CHILD", "1")
            .stdin(Stdio::piped())
            .stdout(Stdio::piped())
            .spawn()
            .unwrap();
        let mut stdin = child.stdin.take().unwrap();
        // A thread writes the input, so a full pipe cannot block the test.
        let writer = std::thread::spawn(move || stdin.write_all(&data));
        let out = child.wait_with_output().unwrap();
        writer.join().unwrap().unwrap();
        assert!(out.status.success(), "case {name}");
        let stdout = String::from_utf8(out.stdout).unwrap();
        assert!(stdout.contains(&format!("lines={want:?}\n")), "case {name}");
    }
}
