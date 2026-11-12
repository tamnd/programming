// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

use algo_build_a_string_efficiently::number_line;

#[test]
fn exact_cases() {
    let cases = [
        ("zero", 0, ""),
        ("one", 1, "1"),
        ("three", 3, "1,2,3"),
        ("five", 5, "1,2,3,4,5"),
        ("seven", 7, "1,2,3,4,5,6,7"),
        ("nine", 9, "1,2,3,4,5,6,7,8,9"),
        ("ten", 10, "1,2,3,4,5,6,7,8,9,10"),
    ];
    for (name, n, want) in cases {
        assert_eq!(number_line(n), want, "{name}");
    }
}

// Long results: the length, the start and the end.
#[test]
fn long_cases() {
    let cases = [
        ("hundred", 100, 291, ",99,100"),
        ("hundred-and-one", 101, 295, ",100,101"),
        ("thousand", 1000, 3892, ",999,1000"),
        ("large", 100000, 588894, ",99999,100000"),
    ];
    for (name, n, size, end) in cases {
        let got = number_line(n);
        assert_eq!(got.len(), size, "{name}");
        assert!(got.starts_with("1,2,3,"), "{name}");
        assert!(got.ends_with(end), "{name}");
    }
}
