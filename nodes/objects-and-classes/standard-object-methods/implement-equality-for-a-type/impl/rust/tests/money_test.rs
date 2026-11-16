// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

use std::collections::hash_map::DefaultHasher;
use std::collections::HashMap;
use std::hash::{Hash, Hasher};

use algo_implement_equality_for_a_type::{count_distinct, Money};

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i64>, Vec<&str>, usize)> = vec![
        ("two-equal", vec![1250, 1250], vec!["EUR", "EUR"], 1),
        ("different-amounts", vec![100, 200, 100], vec!["USD", "USD", "USD"], 2),
        ("mixed", vec![500, 500, 500, 700], vec!["EUR", "USD", "EUR", "EUR"], 3),
        ("empty", vec![], vec![], 0),
        ("case", vec![999, 999, 999], vec!["gbp", "GBP", "Gbp"], 1),
        ("other-currency", vec![0, 0, 0], vec!["EUR", "USD", "JPY"], 3),
        ("negative", vec![-5, 5, -5], vec!["EUR", "EUR", "EUR"], 2),
        ("limit", vec![1_000_000_000_000, -1_000_000_000_000, 1_000_000_000_000], vec!["usd", "USD", "USD"], 2),
    ];
    for (name, amounts, currencies, want) in cases {
        assert_eq!(count_distinct(&amounts, &currencies), want, "{name}");
    }
}

fn hash_of(m: &Money) -> u64 {
    let mut h = DefaultHasher::new();
    m.hash(&mut h);
    h.finish()
}

#[test]
fn equality_rules() {
    let a = Money::new(1250, "EUR");
    let b = Money::new(1250, "eur");
    assert_eq!(a, a.clone());
    assert!(a == b && b == a);
    assert_eq!(hash_of(&a), hash_of(&b));
    assert_ne!(a, Money::new(1251, "EUR"));
    assert_ne!(a, Money::new(1250, "USD"));
    let names = HashMap::from([(Money::new(500, "USD"), "lunch")]);
    assert_eq!(names[&Money::new(500, "usd")], "lunch");
}
