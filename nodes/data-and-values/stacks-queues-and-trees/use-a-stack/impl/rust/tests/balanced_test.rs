// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

use algo_use_a_stack::balanced;

#[test]
fn cases() {
    let deep = "(".repeat(1000) + &")".repeat(1000);
    let cases: Vec<(&str, &str, bool)> = vec![
        ("pair", "()", true),
        ("nested", "{[()()]}", true),
        ("code", "f(a[0], {b: 1})", true),
        ("crossed", "([)]", false),
        ("empty", "", true),
        ("no-brackets", "abc", true),
        ("unclosed", "((", false),
        ("extra-close", "())", false),
        ("close-first", ")(", false),
        ("wrong-kind", "(]", false),
        ("deep", &deep, true),
    ];
    for (name, text, want) in cases {
        assert_eq!(balanced(text), want, "case {name}");
    }
}
