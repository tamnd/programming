// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

use algo_parse_command_line_arguments::{Args, parse_args};

fn strings(v: &[&str]) -> Vec<String> {
    v.iter().map(|s| s.to_string()).collect()
}

#[test]
fn cases() {
    let cases: [(&str, &[&str], (bool, u32, &[&str])); 15] = [
        ("files-only", &["a.txt", "b.txt"], (false, 10, &["a.txt", "b.txt"])),
        ("short-options", &["-v", "-n", "3", "a.txt"], (true, 3, &["a.txt"])),
        ("long-options", &["--verbose", "--limit", "5", "a.txt", "b.txt"], (true, 5, &["a.txt", "b.txt"])),
        ("no-args", &[], (false, 10, &[])),
        ("long-equals", &["--limit=7", "notes.md"], (false, 7, &["notes.md"])),
        ("repeated", &["-n", "3", "--limit", "5", "a.txt"], (false, 5, &["a.txt"])),
        ("zero", &["-n", "0", "a.txt"], (false, 0, &["a.txt"])),
        ("end-of-options", &["-v", "--", "-n", "--limit=4"], (true, 10, &["-n", "--limit=4"])),
        ("only-terminator", &["--"], (false, 10, &[])),
        ("second-terminator", &["--", "--"], (false, 10, &["--"])),
        ("dash", &["-"], (false, 10, &["-"])),
        ("dash-after-option", &["-n", "2", "-", "b.txt"], (false, 2, &["-", "b.txt"])),
        ("empty-arg", &[""], (false, 10, &[""])),
        ("space", &["-v", "my file.txt"], (true, 10, &["my file.txt"])),
        ("non-ascii", &["--limit=4", "日本語.txt", "café.md"], (false, 4, &["日本語.txt", "café.md"])),
    ];
    for (name, args, (verbose, limit, files)) in cases {
        let want = Args { verbose, limit, files: strings(files) };
        assert_eq!(parse_args(strings(args)), Ok(want), "case {name}");
    }
}

#[test]
fn errors() {
    let errors: [(&str, &[&str]); 8] = [
        ("unknown-short", &["-x", "a.txt"]),
        ("unknown-long", &["--color", "a.txt"]),
        ("missing-value", &["-n"]),
        ("missing-value-long", &["--limit"]),
        ("not-a-number", &["-n", "ten", "a.txt"]),
        ("not-a-number-long", &["--limit=ten"]),
        ("empty-value", &["--limit=", "a.txt"]),
        ("verbose-with-value", &["--verbose=yes"]),
    ];
    for (name, args) in errors {
        assert!(parse_args(strings(args)).is_err(), "case {name}");
    }
}
