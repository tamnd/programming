// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

//! A color with a display form (`Display`) and a debug form (`Debug`).

use std::fmt;

/// A color with three channels of 0 to 255. `Debug` is derived.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Color {
    r: u8,
    g: u8,
    b: u8,
}

impl Color {
    pub fn new(r: u8, g: u8, b: u8) -> Self {
        Color { r, g, b }
    }
}

/// The display form. `{}`, `format!`, `println!` and `to_string` use it.
impl fmt::Display for Color {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "#{:02X}{:02X}{:02X}", self.r, self.g, self.b)
    }
}

/// Returns the display form of each color, or None when the slices differ in length.
pub fn format_colors(reds: &[u8], greens: &[u8], blues: &[u8]) -> Option<Vec<String>> {
    if reds.len() != greens.len() || reds.len() != blues.len() {
        return None;
    }
    Some(
        (0..reds.len())
            .map(|i| Color::new(reds[i], greens[i], blues[i]).to_string())
            .collect(),
    )
}
