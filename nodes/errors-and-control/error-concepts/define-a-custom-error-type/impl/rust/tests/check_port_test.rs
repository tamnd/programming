// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

use algo_define_a_custom_error_type::{check_port, parse_port, PortError, Reason};

#[test]
fn cases() {
    let cases: &[(&str, &str, &str)] = &[
        ("valid", "8080", "port 8080"),
        ("not-a-number", "x80", "invalid port 'x80': not a number"),
        ("too-large", "65536", "invalid port '65536': out of range"),
        ("empty", "", "invalid port '': empty"),
        ("zero", "0", "invalid port '0': out of range"),
        ("one", "1", "port 1"),
        ("largest", "65535", "port 65535"),
        ("leading-zeros", "0080", "port 80"),
        ("negative", "-1", "invalid port '-1': not a number"),
        ("leading-space", " 80", "invalid port ' 80': not a number"),
        ("overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range"),
    ];
    for &(name, text, want) in cases {
        assert_eq!(check_port(text), want, "{name}");
    }
}

#[test]
fn fields() {
    let want = PortError { text: "x80".to_string(), reason: Reason::NotANumber };
    assert_eq!(parse_port("x80"), Err(want));
}
