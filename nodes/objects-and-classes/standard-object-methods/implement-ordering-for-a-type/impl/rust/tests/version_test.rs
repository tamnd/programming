// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

use std::cmp::Ordering;
use std::collections::BTreeSet;

use algo_implement_ordering_for_a_type::{sort_versions, Version};

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<&str>, Vec<&str>)> = vec![
        ("simple", vec!["1.2.0", "1.0.0", "1.1.0"], vec!["1.0.0", "1.1.0", "1.2.0"]),
        ("fields-in-order", vec!["2.0.0", "1.9.9", "1.10.0"], vec!["1.9.9", "1.10.0", "2.0.0"]),
        ("patch", vec!["0.1.10", "0.1.2", "0.1.1"], vec!["0.1.1", "0.1.2", "0.1.10"]),
        ("empty", vec![], vec![]),
        ("single", vec!["3.4.5"], vec!["3.4.5"]),
        ("numeric-not-text", vec!["10.0.0", "9.0.0", "1.0.0"], vec!["1.0.0", "9.0.0", "10.0.0"]),
        ("duplicates", vec!["1.0.0", "0.9.0", "1.0.0"], vec!["0.9.0", "1.0.0", "1.0.0"]),
        ("zero", vec!["0.0.1", "0.0.0"], vec!["0.0.0", "0.0.1"]),
        ("limit", vec!["1000000.0.0", "999999.1000000.1000000"], vec!["999999.1000000.1000000", "1000000.0.0"]),
    ];
    for (name, versions, want) in cases {
        assert_eq!(sort_versions(&versions), want, "case {name}");
    }
}

#[test]
fn order_rules() {
    let a = Version::new(1, 2, 3);
    let b = Version::new(1, 10, 0);
    assert_eq!(a.cmp(&b), Ordering::Less);
    assert_eq!(b.cmp(&a), Ordering::Greater);
    assert_eq!(a.cmp(&Version::new(1, 2, 3)), Ordering::Equal);
    assert!(a < b && a <= a && b > a);
    assert_eq!(std::cmp::max(a, b), b);
    assert_eq!(Version::parse("1.2"), None);
    assert_eq!(Version::parse("1.2.3.4"), None);
}

#[test]
fn sorted_set() {
    let set: BTreeSet<Version> = [Version::new(2, 0, 0), Version::new(1, 0, 0), Version::new(1, 0, 0)].into();
    assert_eq!(set.len(), 2);
    assert_eq!(set.first(), Some(&Version::new(1, 0, 0)));
}
