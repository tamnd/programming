// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

use algo_compose_functions::{compose, compose_all};

fn times<'a>(names: &[&'a str], n: usize) -> Vec<&'a str> {
    names.repeat(n)
}

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<&str>, i32, i32)> = vec![
        ("inc-after-double", vec!["inc", "double"], 3, 7),
        ("double-after-inc", vec!["double", "inc"], 3, 8),
        ("three-functions", vec!["negate", "dec", "double"], 5, -9),
        ("empty", vec![], 42, 42),
        ("single-zero", vec!["negate"], 0, 0),
        ("negative", vec!["dec", "negate"], -5, 4),
        ("low", vec!["inc"], -1000, -999),
        ("duplicates", vec!["inc", "inc", "inc"], 7, 10),
        ("limit-high", times(&["double"], 20), 1000, 1048576000),
        ("limit-low", times(&["double"], 20), -1000, -1048576000),
        ("mixed-20", times(&["double", "inc"], 10), 1000, 1026046),
        ("mixed-20-swapped", times(&["inc", "double"], 10), 1000, 1025023),
    ];
    for (name, ops, x, want) in cases {
        assert_eq!(compose_all(&ops, x), want, "{name}");
    }
}

#[test]
fn compose_is_generic() {
    let fs: Vec<algo_compose_functions::Func<String>> = vec![
        Box::new(|s: String| s + "!"),
        Box::new(|s: String| s.to_uppercase()),
        Box::new(|s: String| s.trim().to_string()),
    ];
    assert_eq!(compose(fs)("  hello ".to_string()), "HELLO!");
}

#[test]
fn compose_of_nothing_is_the_identity() {
    assert_eq!(compose::<i32>(Vec::new())(5), 5);
}
