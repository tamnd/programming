// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

use algo_generate_random_numbers::{int_in_range, secure_words, seeded_words, shuffle, token_hex};

type RangeCase = (
    &'static str,
    &'static [u32],
    i64,
    i64,
    usize,
    &'static [i64],
);
type ShuffleCase = (
    &'static str,
    &'static [u32],
    &'static [&'static str],
    &'static [&'static str],
);

const RANGE_CASES: &[RangeCase] = &[
    ("coin", &[5, 2, 7], 0, 1, 3, &[1, 0, 1]),
    ("no-results", &[], 1, 6, 0, &[]),
    ("die", &[7, 12, 1000000, 4294967295], 1, 6, 4, &[2, 1, 5, 4]),
    (
        "negative-range",
        &[100, 2000000000, 4294967294],
        -10,
        10,
        3,
        &[6, -5, -8],
    ),
    ("one-value", &[0, 99], 5, 5, 2, &[5, 5]),
    ("reject-low-words", &[0, 3, 4], 1, 6, 1, &[5]),
    ("accept-least", &[3, 4, 9], 0, 5, 2, &[4, 3]),
    ("power-of-two", &[0, 4294967295], 0, 7, 2, &[0, 7]),
    (
        "worst-case",
        &[2147483646, 2147483647],
        0,
        2147483648,
        1,
        &[2147483647],
    ),
    (
        "full-unsigned-32",
        &[0, 4294967295, 123456789],
        0,
        4294967295,
        3,
        &[0, 4294967295, 123456789],
    ),
    (
        "full-signed-32",
        &[0, 2147483648, 4294967295],
        -2147483648,
        2147483647,
        3,
        &[-2147483648, 0, 2147483647],
    ),
    (
        "near-int64-max",
        &[4294967295, 6],
        9223372036854775798,
        9223372036854775807,
        2,
        &[9223372036854775803, 9223372036854775804],
    ),
    (
        "near-int64-min",
        &[4294967295, 9],
        i64::MIN,
        -9223372036854775799,
        2,
        &[-9223372036854775803, -9223372036854775799],
    ),
    (
        "wide-int64",
        &[4294967295, 0],
        9223372032559808512,
        9223372036854775807,
        2,
        &[9223372036854775807, 9223372032559808512],
    ),
];

const SHUFFLE_CASES: &[ShuffleCase] = &[
    ("empty", &[], &[], &[]),
    ("one-item", &[], &["a"], &["a"]),
    ("two-items-swap", &[0], &["a", "b"], &["b", "a"]),
    ("two-items-keep", &[1], &["a", "b"], &["a", "b"]),
    (
        "five-items",
        &[7, 3, 4, 1],
        &["a", "b", "c", "d", "e"],
        &["a", "e", "b", "d", "c"],
    ),
    (
        "reject-in-shuffle",
        &[0, 2, 0],
        &["a", "b", "c"],
        &["b", "a", "c"],
    ),
    ("duplicates", &[1, 1], &["x", "x", "y"], &["x", "y", "x"]),
    (
        "non-ascii",
        &[4294967295, 4294967294, 0],
        &["é", "日本", "🙂", "a"],
        &["日本", "é", "🙂", "a"],
    ),
];

/// Returns a word source that gives `words` in order and counts the words it gave.
fn fake_words(
    words: &[u32],
) -> (
    impl FnMut() -> u32 + '_,
    std::rc::Rc<std::cell::Cell<usize>>,
) {
    let used = std::rc::Rc::new(std::cell::Cell::new(0));
    let counter = used.clone();
    let source = move || {
        let w = words[counter.get()];
        counter.set(counter.get() + 1);
        w
    };
    (source, used)
}

#[test]
fn int_in_range_cases() {
    for &(name, words, lo, hi, count, want) in RANGE_CASES {
        let (mut source, used) = fake_words(words);
        let got: Vec<i64> = (0..count)
            .map(|_| int_in_range(&mut source, lo, hi))
            .collect();
        assert_eq!(got, want, "{name}");
        assert_eq!(used.get(), words.len(), "{name}");
    }
}

#[test]
fn shuffle_cases() {
    for &(name, words, items, want) in SHUFFLE_CASES {
        let (mut source, used) = fake_words(words);
        let mut got = items.to_vec();
        shuffle(&mut source, &mut got);
        assert_eq!(got, want, "{name}");
        assert_eq!(used.get(), words.len(), "{name}");
    }
}

#[test]
fn bad_range_panics() {
    for (lo, hi) in [(1, 0), (0, 1 << 32), (i64::MIN, i64::MAX)] {
        let result = std::panic::catch_unwind(|| int_in_range(&mut || 0, lo, hi));
        assert!(result.is_err(), "{lo} {hi}");
    }
}

#[test]
fn splitmix64_matches_the_reference_code() {
    let mut source = seeded_words(42);
    let got: Vec<u32> = (0..5).map(|_| source()).collect();
    assert_eq!(
        got,
        [3184996902, 686809907, 1196582743, 1478287871, 163338330]
    );
}

#[test]
fn same_seed_same_words() {
    let (mut a, mut b, mut c) = (seeded_words(7), seeded_words(7), seeded_words(8));
    let first: Vec<u32> = (0..10).map(|_| a()).collect();
    assert_eq!(first, (0..10).map(|_| b()).collect::<Vec<_>>());
    assert_ne!(first, (0..10).map(|_| c()).collect::<Vec<_>>());
}

#[test]
fn secure_words_in_range() {
    let mut source = secure_words().expect("open /dev/urandom");
    for _ in 0..100 {
        assert!((1..=6).contains(&int_in_range(&mut source, 1, 6)));
    }
}

#[test]
fn token_hex_has_the_right_digits() {
    for n in [0, 1, 16, 32] {
        let token = token_hex(n).expect("read /dev/urandom");
        assert_eq!(token.len(), 2 * n);
        assert!(
            token
                .bytes()
                .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
        );
    }
    assert_ne!(token_hex(16).unwrap(), token_hex(16).unwrap());
}
