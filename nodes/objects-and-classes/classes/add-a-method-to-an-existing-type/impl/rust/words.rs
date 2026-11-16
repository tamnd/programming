// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

/// An extension trait. The trait is local to this crate, so the crate can
/// implement it for `str`, a type of the standard library. Code that imports
/// the trait can call `text.word_count()` on any string slice.
pub trait WordCount {
    /// Returns the number of runs of characters that are not whitespace.
    fn word_count(&self) -> usize;
}

impl WordCount for str {
    fn word_count(&self) -> usize {
        self.split_whitespace().count()
    }
}

pub fn word_count(text: &str) -> usize {
    text.word_count()
}
