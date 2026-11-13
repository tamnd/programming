// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

const VOWELS: [char; 10] = ['A', 'E', 'I', 'O', 'U', 'a', 'e', 'i', 'o', 'u'];
const MARKS: [char; 4] = ['\u{300}', '\u{301}', '\u{302}', '\u{308}'];

/// `COMPOSITES[i][j]` is the composite of `VOWELS[i]` in upper case and `MARKS[j]`.
const COMPOSITES: [[u32; 4]; 5] = [
    [0xC0, 0xC1, 0xC2, 0xC4],
    [0xC8, 0xC9, 0xCA, 0xCB],
    [0xCC, 0xCD, 0xCE, 0xCF],
    [0xD2, 0xD3, 0xD4, 0xD6],
    [0xD9, 0xDA, 0xDB, 0xDC],
];

fn composite(i: usize, j: usize) -> char {
    let c = if i >= 5 { COMPOSITES[i - 5][j] + 0x20 } else { COMPOSITES[i][j] };
    char::from_u32(c).unwrap_or('\u{fffd}')
}

/// Returns the composite of `v` and `m`, if the table has the pair.
fn compose(v: char, m: char) -> Option<char> {
    let i = VOWELS.iter().position(|&x| x == v)?;
    let j = MARKS.iter().position(|&x| x == m)?;
    Some(composite(i, j))
}

/// Returns the vowel and the mark of `c`, if `c` is in the table.
fn decompose(c: char) -> Option<(char, char)> {
    (0..10).find_map(|i| (0..4).find(|&j| composite(i, j) == c).map(|j| (VOWELS[i], MARKS[j])))
}

/// Returns `s` in the form NFC or NFD, for the code points of the contract.
/// The 2 forms of the same text are different strings, so `==` is true only
/// after this.
pub fn normalize(s: &str, form: &str) -> String {
    let mut out: Vec<char> = Vec::new();
    for c in s.chars() {
        if form == "NFD" {
            if let Some((v, m)) = decompose(c) {
                out.push(v);
                out.push(m);
                continue;
            }
        } else if let Some(last) = out.last_mut() {
            if let Some(p) = compose(*last, c) {
                *last = p;
                continue;
            }
        }
        out.push(c);
    }
    out.into_iter().collect()
}
