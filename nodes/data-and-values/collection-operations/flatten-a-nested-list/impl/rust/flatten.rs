// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

/// Returns a new vector with the items of the inner vectors, in order.
pub fn flatten(lists: &[Vec<i32>]) -> Vec<i32> {
    lists.concat()
}
