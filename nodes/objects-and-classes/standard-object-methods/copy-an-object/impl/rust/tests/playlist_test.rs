// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

use std::rc::Rc;

use algo_copy_an_object::{copy_and_change, Playlist};

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<&str>, usize, &str, Vec<Vec<&str>>)> = vec![
        (
            "middle",
            vec!["Intro", "Song", "Outro"],
            1,
            "Hit",
            vec![vec!["Intro", "Hit", "Outro"], vec!["Intro", "Hit", "Outro"], vec!["Intro", "Song", "Outro"]],
        ),
        ("two", vec!["A", "B"], 0, "C", vec![vec!["C", "B"], vec!["C", "B"], vec!["A", "B"]]),
        ("single", vec!["Solo"], 0, "Duet", vec![vec!["Duet"], vec!["Duet"], vec!["Solo"]]),
        ("first", vec!["a", "b", "c"], 0, "z", vec![vec!["z", "b", "c"], vec!["z", "b", "c"], vec!["a", "b", "c"]]),
        ("last", vec!["a", "b", "c"], 2, "z", vec![vec!["a", "b", "z"], vec!["a", "b", "z"], vec!["a", "b", "c"]]),
        ("same-title", vec!["a", "b"], 1, "b", vec![vec!["a", "b"], vec!["a", "b"], vec!["a", "b"]]),
        ("duplicates", vec!["x", "x", "x"], 1, "y", vec![vec!["x", "y", "x"], vec!["x", "y", "x"], vec!["x", "x", "x"]]),
    ];
    for (name, songs, index, title, want) in cases {
        let want: Vec<Vec<String>> = want.iter().map(|v| v.iter().map(|s| s.to_string()).collect()).collect();
        assert_eq!(copy_and_change(&songs, index, title), Some(want), "case {name}");
    }
}

#[test]
fn copies_differ() {
    let original = Playlist::new("mix", vec!["a".to_string()]);
    let mut shallow = original.shallow_copy();
    let deep = original.deep_copy();
    assert!(Rc::ptr_eq(&original.songs, &shallow.songs));
    assert!(!Rc::ptr_eq(&original.songs, &deep.songs));
    shallow.name = "other".to_string();
    assert_eq!(original.name, "mix");
}

#[test]
fn bad_index() {
    assert_eq!(copy_and_change(&["a"], 1, "b"), None);
}
