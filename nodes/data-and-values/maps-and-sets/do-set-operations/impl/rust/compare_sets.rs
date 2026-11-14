// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

use std::collections::BTreeSet;

/// The results of the set operations. Each vector is in ascending order.
#[derive(Debug, PartialEq)]
pub struct SetOps<'a> {
    pub union: Vec<&'a str>,
    pub intersection: Vec<&'a str>,
    pub difference: Vec<&'a str>,
    pub symmetric_difference: Vec<&'a str>,
    pub is_subset: bool,
}

/// Returns the union, intersection, difference and symmetric difference of the
/// sets of a and b, and whether the set of a is a subset of the set of b.
pub fn compare_sets<'a>(a: &[&'a str], b: &[&'a str]) -> SetOps<'a> {
    // A BTreeSet visits its items in ascending order, so no sort is needed.
    let sa: BTreeSet<&'a str> = a.iter().copied().collect();
    let sb: BTreeSet<&'a str> = b.iter().copied().collect();
    SetOps {
        union: sa.union(&sb).copied().collect(),
        intersection: sa.intersection(&sb).copied().collect(),
        difference: sa.difference(&sb).copied().collect(),
        symmetric_difference: sa.symmetric_difference(&sb).copied().collect(),
        is_subset: sa.is_subset(&sb),
    }
}
