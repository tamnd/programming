// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

//! A shallow copy and a deep copy of a struct with a list field.
//!
//! A derived `Clone` on a struct with a `Vec` field copies the vector, so it is a deep copy.
//! To share the list, the field is an `Rc<RefCell<Vec<String>>>`; cloning an `Rc` copies the
//! pointer, not the list.

use std::cell::RefCell;
use std::rc::Rc;

#[derive(Clone, Debug)]
pub struct Playlist {
    pub name: String,
    pub songs: Rc<RefCell<Vec<String>>>,
}

impl Playlist {
    pub fn new(name: &str, songs: Vec<String>) -> Self {
        Playlist { name: name.to_string(), songs: Rc::new(RefCell::new(songs)) }
    }

    /// A shallow copy: the derived `clone` copies the name and the `Rc` pointer.
    pub fn shallow_copy(&self) -> Self {
        self.clone()
    }

    /// A deep copy: a new `Rc` that holds a copy of the list.
    pub fn deep_copy(&self) -> Self {
        Playlist {
            name: self.name.clone(),
            songs: Rc::new(RefCell::new(self.songs.borrow().clone())),
        }
    }
}

/// Changes one song through the original and returns the songs of the original, of a shallow
/// copy and of a deep copy, or None when the index is out of range.
pub fn copy_and_change(songs: &[&str], index: usize, title: &str) -> Option<Vec<Vec<String>>> {
    if index >= songs.len() {
        return None;
    }
    let original = Playlist::new("mix", songs.iter().map(|s| s.to_string()).collect());
    let shallow = original.shallow_copy();
    let deep = original.deep_copy();
    original.songs.borrow_mut()[index] = title.to_string();
    let songs_of = |p: &Playlist| p.songs.borrow().clone();
    Some(vec![songs_of(&original), songs_of(&shallow), songs_of(&deep)])
}
