// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

//! Removes duplicates from lists and keeps the input order.

use std::collections::HashSet;
use std::hash::Hash;

/// A record of a user.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct User {
    pub name: String,
    pub email: String,
}

/// Returns the items without later copies. The first copies keep their order.
pub fn unique<T: Eq + Hash + Clone>(items: &[T]) -> Vec<T> {
    // The set holds references to the items, so it copies no item.
    let mut seen = HashSet::with_capacity(items.len());
    // insert returns true only if the set did not have the value.
    items.iter().filter(|&x| seen.insert(x)).cloned().collect()
}

/// Returns the first item for each key, in input order.
pub fn unique_by<T: Clone, K: Eq + Hash>(items: &[T], mut key: impl FnMut(&T) -> K) -> Vec<T> {
    let mut seen = HashSet::with_capacity(items.len());
    items
        .iter()
        .filter(|&x| seen.insert(key(x)))
        .cloned()
        .collect()
}

/// Returns the first user for each email. Case does not matter in the emails.
pub fn unique_by_email(users: &[User]) -> Vec<User> {
    unique_by(users, |u| u.email.to_ascii_lowercase())
}
