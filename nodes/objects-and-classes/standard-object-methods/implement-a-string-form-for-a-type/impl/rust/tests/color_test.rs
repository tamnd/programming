// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

use algo_implement_a_string_form_for_a_type::{format_colors, Color};

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<u8>, Vec<u8>, Vec<u8>, Vec<&str>)> = vec![
        ("orange", vec![255], vec![128], vec![0], vec!["#FF8000"]),
        (
            "three",
            vec![255, 0, 0],
            vec![0, 255, 0],
            vec![0, 0, 255],
            vec!["#FF0000", "#00FF00", "#0000FF"],
        ),
        ("mixed", vec![18, 171], vec![52, 205], vec![86, 239], vec!["#123456", "#ABCDEF"]),
        ("empty", vec![], vec![], vec![], vec![]),
        ("single", vec![1], vec![2], vec![3], vec!["#010203"]),
        ("zero", vec![0], vec![0], vec![0], vec!["#000000"]),
        ("max", vec![255], vec![255], vec![255], vec!["#FFFFFF"]),
        ("padding", vec![10, 0], vec![11, 15], vec![12, 16], vec!["#0A0B0C", "#000F10"]),
    ];
    for (name, reds, greens, blues, want) in cases {
        let want: Vec<String> = want.iter().map(|s| s.to_string()).collect();
        assert_eq!(format_colors(&reds, &greens, &blues), Some(want), "case {name}");
    }
}

#[test]
fn forms() {
    let c = Color::new(255, 128, 0);
    assert_eq!(format!("{c}"), "#FF8000");
    assert_eq!(c.to_string(), "#FF8000");
    assert_eq!(format!("{c:?}"), "Color { r: 255, g: 128, b: 0 }");
    assert_eq!(format!("{:?}", vec![c]), "[Color { r: 255, g: 128, b: 0 }]");
}

#[test]
fn lengths_differ() {
    assert_eq!(format_colors(&[1], &[], &[]), None);
}
