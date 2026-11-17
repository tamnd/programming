// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

/// Returns how many items are greater than limit.
pub fn count_greater<T: PartialOrd>(items: &[T], limit: &T) -> usize {
    items.iter().filter(|item| *item > limit).count()
}
