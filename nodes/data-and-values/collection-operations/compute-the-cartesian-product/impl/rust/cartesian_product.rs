// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

/// Returns each tuple that takes one item from each list, in nested-loop order.
pub fn cartesian_product(lists: &[Vec<i32>]) -> Vec<Vec<i32>> {
    // The product of no lists is one empty tuple, so the fold starts there.
    lists.iter().fold(vec![Vec::new()], |tuples, xs| {
        tuples
            .iter()
            .flat_map(|t| {
                xs.iter().map(move |&x| {
                    let mut u = t.clone();
                    u.push(x);
                    u
                })
            })
            .collect()
    })
}
