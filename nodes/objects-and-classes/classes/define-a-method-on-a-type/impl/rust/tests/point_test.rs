// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

use algo_define_a_method_on_a_type::{point_report, Point};

#[test]
fn cases() {
    let cases: Vec<(&str, (i32, i32, i32, i32), Vec<i32>)> = vec![
        ("right-and-up", (1, 2, 3, 4), vec![4, 6, 7, 10]),
        ("mixed-signs", (5, -1, -2, 3), vec![3, 2, 5, 5]),
        ("zero-move", (7, -3, 0, 0), vec![7, -3, 0, 10]),
        ("negative", (-2, -5, -3, -1), vec![-5, -6, 4, 11]),
        ("origin", (2, 3, -2, -3), vec![0, 0, 5, 0]),
        (
            "limit",
            (100000000, -100000000, 100000000, -100000000),
            vec![200000000, -200000000, 200000000, 400000000],
        ),
    ];
    for (name, (x, y, dx, dy), want) in cases {
        assert_eq!(point_report(x, y, dx, dy), want, "{name}");
    }
}

#[test]
fn translate_returns_another_point() {
    let p = Point::new(1, 2);
    let q = p.translate(3, 4);
    assert_eq!(p, Point::new(1, 2));
    assert_eq!(q, Point::new(4, 6));
}
