// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

//! Aligns the columns of a text in which a `$` separates the fields.

use std::fmt;
use std::str::FromStr;

/// Where a field goes in its column.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Align {
    Left,
    Right,
    Center,
}

/// The error when a text is not `left`, `right` or `center`.
#[derive(Debug, PartialEq, Eq)]
pub struct BadAlign(pub String);

impl fmt::Display for BadAlign {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "bad align: {:?}", self.0)
    }
}

impl std::error::Error for BadAlign {}

impl FromStr for Align {
    type Err = BadAlign;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "left" => Ok(Align::Left),
            "right" => Ok(Align::Right),
            "center" => Ok(Align::Center),
            _ => Err(BadAlign(s.to_string())),
        }
    }
}

/// Returns `text` with its fields in columns. A line ends at LF. Each `$` in a
/// line separates two fields, and a `$` at the end of a line does not start a
/// new field. An empty line has no fields. The width of a column is the
/// largest number of code points of a field in it. Each field is padded with
/// spaces to the width of its column, on the right (`Left`), on the left
/// (`Right`), or on the two sides (`Center`), with the odd space on the
/// right. One space separates two columns. The padding on the right of the
/// last field of a line is not written. Each line of the result ends with LF.
pub fn align_columns(text: &str, align: Align) -> String {
    // split_terminator does not give an empty last line after a final LF.
    let rows: Vec<Vec<&str>> = text.split_terminator('\n').map(split_fields).collect();
    let mut widths: Vec<usize> = Vec::new();
    for row in &rows {
        for (j, field) in row.iter().enumerate() {
            // len() counts bytes. chars().count() counts code points.
            let width = field.chars().count();
            if j == widths.len() {
                widths.push(width);
            } else {
                widths[j] = widths[j].max(width);
            }
        }
    }
    let mut out = String::with_capacity(text.len());
    for row in &rows {
        for (j, field) in row.iter().enumerate() {
            let pad = widths[j] - field.chars().count();
            let left = match align {
                Align::Left => 0,
                Align::Right => pad,
                Align::Center => pad / 2,
            };
            let right = if j + 1 < row.len() { pad - left } else { 0 };
            if j > 0 {
                out.push(' ');
            }
            out.extend(std::iter::repeat_n(' ', left));
            out.push_str(field);
            out.extend(std::iter::repeat_n(' ', right));
        }
        out.push('\n');
    }
    out
}

/// Returns the fields of a line: none for an empty line.
fn split_fields(line: &str) -> Vec<&str> {
    if line.is_empty() {
        return Vec::new();
    }
    // split_terminator does not give an empty last field after a final $.
    line.split_terminator('$').collect()
}
