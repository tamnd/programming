// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

use algo_use_a_text_template::substitute;

const CASES: &[(&str, &str, &[&str], &[&str], &str)] = &[
    ("greeting", "Hello, $name!", &["name"], &["Ada"], "Hello, Ada!"),
    ("braces", "${item}s cost $$${price}", &["item", "price"], &["Apple", "3"], "Apples cost $3"),
    ("two-uses", "$a and $a again", &["a"], &["x"], "x and x again"),
    ("no-placeholders", "plain text", &[], &[], "plain text"),
    ("empty-template", "", &["x"], &["1"], ""),
    ("dollar-escape", "$$name", &["name"], &["Ada"], "$name"),
    ("adjacent-text", "${n}th", &["n"], &["4"], "4th"),
    ("value-with-dollar", "$v", &["v"], &["$x and ${y}"], "$x and ${y}"),
    ("empty-value", "[$v]", &["v"], &[""], "[]"),
    ("underscore-name", "$first_name $last_name", &["first_name", "last_name"], &["Grace", "Hopper"], "Grace Hopper"),
    ("name-ends-at-punct", "$a.$b", &["a", "b"], &["1", "2"], "1.2"),
    ("unused-value", "$a", &["a", "b"], &["1", "2"], "1"),
];

#[test]
fn cases() {
    for &(name, template, names, values, want) in CASES {
        assert_eq!(substitute(template, names, values).as_deref(), Ok(want), "{name}");
    }
}

#[test]
fn unknown_name() {
    assert!(substitute("Hi $who", &["name"], &["Ada"]).is_err());
}
