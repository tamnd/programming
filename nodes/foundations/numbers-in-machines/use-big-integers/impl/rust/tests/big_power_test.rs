// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

use algo_use_big_integers::big_power;

#[test]
fn cases() {
    let cases: Vec<(&str, i32, u32, String)> = vec![
        ("small", 2, 10, "1024".into()),
        ("two-to-the-100", 2, 100, "1267650600228229401496703205376".into()),
        ("ten-to-the-30", 10, 30, format!("1{}", "0".repeat(30))),
        ("zero-exponent", 7, 0, "1".into()),
        ("zero-to-the-zero", 0, 0, "1".into()),
        ("zero-base", 0, 5, "0".into()),
        ("minus-one-odd", -1, 199, "-1".into()),
        ("negative-odd", -3, 41, "-36472996377170786403".into()),
        ("negative-even", -3, 40, "12157665459056928801".into()),
        ("int64-max-plus-one", 2, 63, "9223372036854775808".into()),
        ("uint64-max-plus-one", 2, 64, "18446744073709551616".into()),
        ("two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376".into()),
        ("largest", 1000, 200, format!("1{}", "0".repeat(600))),
        ("smallest", -1000, 199, format!("-1{}", "0".repeat(597))),
    ];
    for (name, base, exp, want) in cases {
        assert_eq!(big_power(base, exp), want, "{name}");
    }
}

#[test]
fn fixed_width() {
    assert_eq!(2i64.checked_pow(63), None);
    assert_eq!(2u128.pow(64).to_string(), big_power(2, 64));
    assert_eq!(2u128.pow(127).to_string(), big_power(2, 127));
    assert!(u64::try_from(2u128.pow(64)).is_err());
    assert!(i64::try_from(2u128.pow(63)).is_err());
}
