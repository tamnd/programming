// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

use algo_log_an_error::read_ports;

#[test]
fn cases() {
    let cases: &[(&str, &[&str], &[u16], &str)] = &[
        ("one-bad", &["8080", "x", "443"], &[8080, 443], "level=ERROR msg=\"read port failed\" line=2 err=\"not a number: 'x'\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n"),
        ("all-good", &["80", "443"], &[80, 443], "level=INFO msg=\"ports read\" ok=2 total=2\n"),
        ("out-of-range", &["70000"], &[], "level=ERROR msg=\"read port failed\" line=1 err=\"out of range: 70000\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        ("empty-list", &[], &[], "level=INFO msg=\"ports read\" ok=0 total=0\n"),
        ("empty-text", &[""], &[], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: ''\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        ("boundary", &["1", "65535", "0"], &[1, 65535], "level=ERROR msg=\"read port failed\" line=3 err=\"out of range: 0\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n"),
        ("newline", &["80\nlevel=INFO msg=\"all good\""], &[], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\nlevel=INFO msg=\\\"all good\\\"'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        ("backslash", &["C:\\port"], &[], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: 'C:\\\\port'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        ("carriage-return", &["80\r"], &[], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\r'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
        ("tab", &["8\t0"], &[], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '8\\t0'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"),
    ];
    for &(name, texts, want_ports, want_log) in cases {
        let mut log = Vec::new();
        let ports = read_ports(texts, &mut log).unwrap();
        assert_eq!(ports, want_ports, "{name}");
        assert_eq!(String::from_utf8(log).unwrap(), want_log, "{name}");
    }
}
