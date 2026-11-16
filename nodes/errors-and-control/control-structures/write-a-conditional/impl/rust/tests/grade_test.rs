// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

use algo_write_a_conditional::grade;

#[test]
fn cases() {
    let cases: &[(&str, i32, &str)] = &[
        ("ninety-five", 95, "A"),
        ("eighty-five", 85, "B"),
        ("seventy-five", 75, "C"),
        ("sixty-five", 65, "D"),
        ("forty", 40, "F"),
        ("hundred", 100, "A"),
        ("zero", 0, "F"),
        ("ninety", 90, "A"),
        ("eighty-nine", 89, "B"),
        ("sixty", 60, "D"),
        ("fifty-nine", 59, "F"),
    ];
    for &(name, score, want) in cases {
        assert_eq!(grade(score), want, "{name}");
    }
}
