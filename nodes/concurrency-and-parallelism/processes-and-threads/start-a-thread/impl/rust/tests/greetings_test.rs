// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

use algo_start_a_thread::thread_greetings;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<&str>, Vec<&str>)> = vec![
        ("two", vec!["Ada", "Linus"], vec!["hello from Ada", "hello from Linus"]),
        ("three", vec!["main", "worker", "logger"], vec!["hello from main", "hello from worker", "hello from logger"]),
        ("empty", vec![], vec![]),
        ("single", vec!["solo"], vec!["hello from solo"]),
        ("empty-name", vec![""], vec!["hello from "]),
        ("duplicate", vec!["twin", "twin"], vec!["hello from twin", "hello from twin"]),
        ("unicode", vec!["Zoë", "李雷"], vec!["hello from Zoë", "hello from 李雷"]),
    ];
    for (name, names, want) in cases {
        assert_eq!(thread_greetings(&names), want, "case {name}");
    }
}

#[test]
fn many() {
    let many: Vec<String> = (0..32).map(|i| format!("t{i}")).collect();
    let names: Vec<&str> = many.iter().map(String::as_str).collect();
    let want: Vec<String> = many.iter().map(|n| format!("hello from {n}")).collect();
    assert_eq!(thread_greetings(&names), want);
}
