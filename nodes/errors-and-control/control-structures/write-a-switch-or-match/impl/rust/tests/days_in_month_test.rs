// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

use algo_write_a_switch_or_match::days_in_month;

#[test]
fn cases() {
    let cases: &[(&str, i32, i32)] = &[
        ("january", 1, 31),
        ("february", 2, 28),
        ("april", 4, 30),
        ("july", 7, 31),
        ("november", 11, 30),
        ("december", 12, 31),
        ("zero", 0, 0),
        ("thirteen", 13, 0),
        ("minus-one", -1, 0),
    ];
    for &(name, month, want) in cases {
        assert_eq!(days_in_month(month), want, "{name}");
    }
}
