// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

use std::io::ErrorKind;

use algo_run_a_subprocess::run;

#[test]
fn cases() {
    let cases: [(&str, &[&str], i32, &str, &str); 14] = [
        ("hello", &["printf", "hello"], 0, "hello", ""),
        ("two-arguments", &["echo", "one", "two"], 0, "one two\n", ""),
        ("exit-status", &["sh", "-c", "exit 3"], 3, "", ""),
        (
            "both-streams",
            &["sh", "-c", "echo out; echo err >&2; exit 2"],
            2,
            "out\n",
            "err\n",
        ),
        (
            "metacharacters",
            &["printf", "%s|", "a b", "$HOME", "*", "x;y", "\"q\"", "'s'"],
            0,
            "a b|$HOME|*|x;y|\"q\"|'s'|",
            "",
        ),
        ("empty-argument", &["printf", "[%s]", ""], 0, "[]", ""),
        (
            "newline-argument",
            &["printf", "%s", "line one\nline two"],
            0,
            "line one\nline two",
            "",
        ),
        ("trailing-newlines", &["printf", r"a\n\n"], 0, "a\n\n", ""),
        (
            "non-ascii",
            &["printf", r"%s\n", "xin chào, 日本語, 🙂"],
            0,
            "xin chào, 日本語, 🙂\n",
            "",
        ),
        ("stderr-only", &["sh", "-c", "echo warning >&2"], 0, "", "warning\n"),
        ("no-output", &["true"], 0, "", ""),
        ("false", &["false"], 1, "", ""),
        ("status-255", &["sh", "-c", "exit 255"], 255, "", ""),
        (
            "empty-stdin",
            &["sh", "-c", "if read -r line; then echo got; else echo eof; fi"],
            0,
            "eof\n",
            "",
        ),
    ];
    for (name, argv, status, stdout, stderr) in cases {
        let got = run(argv).unwrap_or_else(|e| panic!("{name}: {e}"));
        assert_eq!(
            got,
            (status, stdout.to_string(), stderr.to_string()),
            "{name}"
        );
    }
}

#[test]
fn large_output_on_both_streams() {
    let script = r"head -c 1048576 /dev/zero | tr '\0' x; head -c 1048576 /dev/zero | tr '\0' y >&2";
    let (status, stdout, stderr) = run(&["sh", "-c", script]).unwrap();
    assert_eq!(status, 0);
    assert_eq!(stdout, "x".repeat(1048576));
    assert_eq!(stderr, "y".repeat(1048576));
}

#[test]
fn missing_program() {
    let err = run(&["no-such-program-7f3a"]).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn signal() {
    assert!(run(&["sh", "-c", "kill -9 $$"]).is_err());
}
