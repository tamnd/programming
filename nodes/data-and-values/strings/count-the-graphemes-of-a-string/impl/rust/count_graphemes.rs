// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

#[derive(Clone, Copy, PartialEq)]
enum Kind {
    Cr,
    Lf,
    Control,
    Extend,
    Zwj,
    Ri,
    Pict,
    Other,
}

/// Returns the Grapheme_Cluster_Break kind of `c`, or `Pict` for
/// Extended_Pictographic.
fn kind(c: char) -> Kind {
    match u32::from(c) {
        0x0D => Kind::Cr,
        0x0A => Kind::Lf,
        0x00..=0x1F | 0x7F..=0x9F | 0xAD => Kind::Control,
        0x300..=0x36F | 0xFE00..=0xFE0F | 0x1F3FB..=0x1F3FF => Kind::Extend,
        0x200D => Kind::Zwj,
        0x1F1E6..=0x1F1FF => Kind::Ri,
        0xA9 | 0xAE | 0x2764 | 0x1F400..=0x1F4FD | 0x1F600..=0x1F64F => Kind::Pict,
        _ => Kind::Other,
    }
}

/// Returns the number of graphemes in `s`. `s.chars().count()` counts code
/// points, and `s.len()` counts bytes.
pub fn count_graphemes(s: &str) -> usize {
    let mut n = 0;
    let mut prev: Option<Kind> = None;
    let mut ri_run = 0;
    let mut pict = false;
    let mut zwj_after_pict = false;
    for c in s.chars() {
        let k = kind(c);
        let join = match prev {
            None => false,
            Some(Kind::Cr) if k == Kind::Lf => true, // GB3
            Some(Kind::Cr | Kind::Lf | Kind::Control) => false, // GB4
            Some(_) if matches!(k, Kind::Cr | Kind::Lf | Kind::Control) => false, // GB5
            Some(_) if matches!(k, Kind::Extend | Kind::Zwj) => true, // GB9
            Some(_) if k == Kind::Pict && zwj_after_pict => true, // GB11
            Some(_) if k == Kind::Ri && ri_run % 2 == 1 => true, // GB12, GB13
            Some(_) => false, // GB999
        };
        if !join {
            n += 1;
        }
        zwj_after_pict = k == Kind::Zwj && pict;
        pict = k == Kind::Pict || (k == Kind::Extend && pict);
        ri_run = if k == Kind::Ri { ri_run + 1 } else { 0 };
        prev = Some(k);
    }
    n
}
