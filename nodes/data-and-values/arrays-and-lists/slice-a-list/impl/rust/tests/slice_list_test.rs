// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

use algo_slice_a_list::slice;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], usize, usize, &[i32])] = &[
        ("middle", &[1, 2, 3, 4, 5], 1, 3, &[2, 3]),
        ("prefix", &[1, 2, 3], 0, 2, &[1, 2]),
        ("suffix", &[1, 2, 3], 1, 3, &[2, 3]),
        ("empty-list", &[], 0, 0, &[]),
        ("empty-slice", &[1, 2, 3], 2, 2, &[]),
        ("whole", &[1, 2, 3], 0, 3, &[1, 2, 3]),
        ("single", &[7], 0, 1, &[7]),
        ("last-one", &[1, 2, 3], 2, 3, &[3]),
        ("at-end", &[1, 2, 3], 3, 3, &[]),
        ("limit", &[2147483647, i32::MIN, 0], 0, 2, &[2147483647, i32::MIN]),
    ];
    for &(name, items, start, stop, want) in cases {
        assert_eq!(slice(items, start, stop), want, "{name}");
    }
}

#[test]
#[should_panic]
fn stop_past_the_end() {
    slice(&[1, 2, 3], 0, 4);
}
