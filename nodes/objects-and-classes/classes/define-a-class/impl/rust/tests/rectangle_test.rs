// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

use algo_define_a_class::{rectangle_report, Rectangle};

#[test]
fn cases() {
    let cases: Vec<(&str, (u32, u32, u32), Vec<u32>)> = vec![
        ("three-by-four", (3, 4, 2), vec![12, 14, 48, 28]),
        ("square", (5, 5, 3), vec![25, 20, 225, 60]),
        ("zero-side", (0, 7, 2), vec![0, 14, 0, 28]),
        ("zero-factor", (3, 4, 0), vec![12, 14, 0, 0]),
        ("identity", (3, 4, 1), vec![12, 14, 12, 14]),
        ("limit", (1000, 1000, 46), vec![1000000, 4000, 2116000000, 184000]),
    ];
    for (name, (w, h, k), want) in cases {
        assert_eq!(rectangle_report(w, h, k), want, "{name}");
    }
}

#[test]
fn instances_have_their_own_fields() {
    let mut a = Rectangle::new(1, 2);
    let b = Rectangle::new(1, 2);
    a.scale(3);
    assert_eq!(a.to_string(), "Rectangle(3, 6)");
    assert_eq!(b.to_string(), "Rectangle(1, 2)");
}
