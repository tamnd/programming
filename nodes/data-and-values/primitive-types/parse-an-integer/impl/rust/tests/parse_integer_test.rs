// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

use std::num::IntErrorKind;

use algo_parse_an_integer::parse_integer;

#[test]
fn parses() {
    let cases: Vec<(&str, String, u32, i64)> = vec![
        ("decimal", "42".to_string(), 10, 42),
        ("negative", "-17".to_string(), 10, -17),
        ("hex", "ff".to_string(), 16, 255),
        ("binary", "101010".to_string(), 2, 42),
        ("base-36", "zz".to_string(), 36, 1295),
        ("plus-sign", "+5".to_string(), 10, 5),
        ("negative-zero", "-0".to_string(), 10, 0),
        ("zero", "0".to_string(), 10, 0),
        ("leading-zeros", "007".to_string(), 10, 7),
        ("many-leading-zeros", "0".repeat(5000) + "42", 10, 42),
        ("max", "9223372036854775807".to_string(), 10, 9223372036854775807),
        ("min", "-9223372036854775808".to_string(), 10, i64::MIN),
        ("max-hex", "7fffffffffffffff".to_string(), 16, 9223372036854775807),
        ("min-binary", "-1000000000000000000000000000000000000000000000000000000000000000".to_string(), 2, i64::MIN),
        ("upper-case", "FF".to_string(), 16, 255),
        ("mixed-case", "Zz".to_string(), 36, 1295),
        ("above-safe-integer", "9007199254740993".to_string(), 10, 9007199254740993),
    ];
    for (name, s, radix, want) in cases {
        assert_eq!(parse_integer(&s, radix), Ok(want), "case {name}");
    }
}

#[test]
fn rejects_invalid() {
    let cases: Vec<(&str, String, u32)> = vec![
        ("empty", "".to_string(), 10),
        ("sign-only", "-".to_string(), 10),
        ("plus-only", "+".to_string(), 10),
        ("double-sign", "+-5".to_string(), 10),
        ("leading-space", " 42".to_string(), 10),
        ("trailing-space", "42 ".to_string(), 10),
        ("trailing-newline", "42\n".to_string(), 10),
        ("trailing-garbage", "42px".to_string(), 10),
        ("underscore", "1_000".to_string(), 10),
        ("thousands-separator", "1,000".to_string(), 10),
        ("decimal-point", "4.0".to_string(), 10),
        ("exponent", "1e3".to_string(), 10),
        ("hex-prefix", "0x1A".to_string(), 16),
        ("binary-prefix", "0b101".to_string(), 2),
        ("digit-outside-base", "102".to_string(), 2),
        ("letter-outside-base", "fg".to_string(), 16),
        ("arabic-indic-digits", "\u{661}\u{662}".to_string(), 10),
        ("fullwidth-digits", "\u{ff11}\u{ff12}".to_string(), 10),
        ("kelvin-sign", "\u{212a}".to_string(), 36),
    ];
    for (name, s, radix) in cases {
        let kind = parse_integer(&s, radix).map_err(|e| e.kind().clone());
        assert!(
            matches!(kind, Err(IntErrorKind::Empty | IntErrorKind::InvalidDigit)),
            "case {name}: {kind:?}"
        );
    }
}

#[test]
fn rejects_out_of_range() {
    let cases: Vec<(&str, String, u32)> = vec![
        ("max-plus-one", "9223372036854775808".to_string(), 10),
        ("min-minus-one", "-9223372036854775809".to_string(), 10),
        ("two-to-the-64", "18446744073709551616".to_string(), 10),
        ("hex-sign-bit", "8000000000000000".to_string(), 16),
        ("hex-all-ones", "ffffffffffffffff".to_string(), 16),
        ("min-times-ten", "-92233720368547758080".to_string(), 10),
        ("many-digits", "9".repeat(5000), 10),
    ];
    for (name, s, radix) in cases {
        let kind = parse_integer(&s, radix).map_err(|e| e.kind().clone());
        assert!(
            matches!(kind, Err(IntErrorKind::PosOverflow | IntErrorKind::NegOverflow)),
            "case {name}: {kind:?}"
        );
    }
}
