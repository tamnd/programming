// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

use algo_remove_duplicates_from_a_list::{User, unique, unique_by_email};

#[test]
fn cases() {
    let cases: &[(&str, &[&str], &[&str])] = &[
        (
            "repeats",
            &["apple", "pear", "apple", "fig", "pear"],
            &["apple", "pear", "fig"],
        ),
        (
            "words",
            &["to", "be", "or", "not", "to", "be"],
            &["to", "be", "or", "not"],
        ),
        ("empty", &[], &[]),
        ("single", &["a"], &["a"]),
        ("all-same", &["x", "x", "x"], &["x"]),
        ("no-duplicates", &["c", "a", "b"], &["c", "a", "b"]),
        ("not-adjacent", &["a", "b", "a", "b"], &["a", "b"]),
        ("adjacent", &["a", "a", "b", "b", "a"], &["a", "b"]),
        ("empty-string", &["", "a", "", "b"], &["", "a", "b"]),
        ("case", &["a", "A", "a"], &["a", "A"]),
        (
            "normalization",
            &["\u{e9}", "e\u{301}", "\u{e9}"],
            &["\u{e9}", "e\u{301}"],
        ),
        ("whitespace", &[" a", "a", "a ", " a"], &[" a", "a", "a "]),
        ("number-like", &["1", "01", "1.0", "1"], &["1", "01", "1.0"]),
        (
            "non-ascii",
            &["日本", "🙂", "日本", "🙂", "ü"],
            &["日本", "🙂", "ü"],
        ),
    ];
    for &(name, given, want) in cases {
        let given: Vec<String> = given.iter().map(|s| s.to_string()).collect();
        assert_eq!(unique(&given), want, "case {name}");
    }
}

type Row = (&'static str, &'static str);

fn users(rows: &[Row]) -> Vec<User> {
    rows.iter()
        .map(|&(name, email)| User {
            name: name.to_string(),
            email: email.to_string(),
        })
        .collect()
}

/// The cases of cases/by-key.jsonl.
#[test]
fn by_key() {
    let cases: &[(&str, &[Row], &[Row])] = &[
        (
            "case-insensitive",
            &[
                ("Ada", "ada@example.com"),
                ("Bob", "bob@example.com"),
                ("Ada L.", "ADA@Example.com"),
            ],
            &[("Ada", "ada@example.com"), ("Bob", "bob@example.com")],
        ),
        ("empty", &[], &[]),
        (
            "same-name-other-email",
            &[("Sam", "sam@a.example"), ("Sam", "sam@b.example")],
            &[("Sam", "sam@a.example"), ("Sam", "sam@b.example")],
        ),
        (
            "first-record-wins",
            &[("Old", "x@example.com"), ("New", "X@EXAMPLE.COM")],
            &[("Old", "x@example.com")],
        ),
        (
            "dots-differ",
            &[("Al", "a.b@example.com"), ("Bo", "ab@example.com")],
            &[("Al", "a.b@example.com"), ("Bo", "ab@example.com")],
        ),
        (
            "not-adjacent",
            &[
                ("A", "a@example.com"),
                ("B", "b@example.com"),
                ("C", "A@example.com"),
                ("D", "B@EXAMPLE.com"),
            ],
            &[("A", "a@example.com"), ("B", "b@example.com")],
        ),
    ];
    for &(name, given, want) in cases {
        assert_eq!(unique_by_email(&users(given)), users(want), "case {name}");
    }
}
