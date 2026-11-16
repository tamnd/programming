// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

use algo_define_a_record_type::{Person, birthday};

fn people(v: &[(&str, u32)]) -> Vec<Person> {
    v.iter().map(|&(name, age)| Person { name: name.to_string(), age }).collect()
}

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<(&str, u32)>, &str, Vec<(&str, u32)>)> = vec![
        ("first", vec![("Ada", 36), ("Alan", 41)], "Ada", vec![("Ada", 37), ("Alan", 41)]),
        ("second", vec![("Ada", 36), ("Alan", 41)], "Alan", vec![("Ada", 36), ("Alan", 42)]),
        ("empty", vec![], "Ada", vec![]),
        ("not-found", vec![("Ada", 36)], "Grace", vec![("Ada", 36)]),
        ("duplicate-name", vec![("Ada", 36), ("Alan", 41), ("Ada", 9)], "Ada", vec![("Ada", 37), ("Alan", 41), ("Ada", 10)]),
        ("zero-age", vec![("Baby", 0)], "Baby", vec![("Baby", 1)]),
        ("non-ascii", vec![("Zoë", 20), ("Zoe", 30)], "Zoë", vec![("Zoë", 21), ("Zoe", 30)]),
        ("case", vec![("ada", 36)], "Ada", vec![("ada", 36)]),
    ];
    for (name, input, who, want) in cases {
        let input = people(&input);
        let before = input.clone();
        assert_eq!(birthday(&input, who), people(&want), "case {name}");
        assert_eq!(input, before, "case {name}: the input changed");
    }
}

#[test]
fn equality_and_printing() {
    let a = Person { name: "Ada".to_string(), age: 36 };
    assert_eq!(a, a.clone());
    assert_ne!(a, Person { age: 37, ..a.clone() });
    assert_eq!(format!("{a:?}"), r#"Person { name: "Ada", age: 36 }"#);
}
