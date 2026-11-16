// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

use algo_define_an_enumeration::{Day, cycle};

#[test]
fn cases() {
    let cases: Vec<(&str, &str, usize, Vec<&str>)> = vec![
        ("weekdays", "monday", 3, vec!["monday", "tuesday", "wednesday"]),
        ("wrap", "friday", 4, vec!["friday", "saturday", "sunday", "monday"]),
        ("zero-steps", "monday", 0, vec![]),
        ("more-than-a-week", "sunday", 9, vec!["sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday"]),
        ("unknown", "funday", 3, vec![]),
        ("wrong-case", "Friday", 2, vec![]),
        ("number", "3", 2, vec![]),
        ("empty-name", "", 1, vec![]),
    ];
    for (name, start, steps, want) in cases {
        assert_eq!(cycle(start, steps), want, "case {name}");
    }
}

#[test]
fn to_and_from_int() {
    assert_eq!(Day::Friday as usize, 4);
    assert_eq!(Day::ALL[4], Day::Friday);
    assert_eq!(Day::parse("friday"), Some(Day::Friday));
}
