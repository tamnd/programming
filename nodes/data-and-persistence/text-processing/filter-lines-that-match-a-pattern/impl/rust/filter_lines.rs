// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

//! Selects the lines of a text file that match a small regular expression.

use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;

/// Returns the lines of the UTF-8 file at `path` that match `pattern`. A line
/// matches if the pattern matches some part of it. A line ends at LF, and the
/// LF is not part of it. A last line with no LF is a line too. A missing file
/// gives an error of the kind `ErrorKind::NotFound`.
///
/// The standard library has no regular expressions, so the pattern is the
/// small set that `matches` knows: `c` matches the character c, `.` matches
/// any character, `^` at the start matches the start of the line, `$` at the
/// end matches the end of the line, and `*` matches zero or more of the
/// character before it.
pub fn filter_lines(path: impl AsRef<Path>, pattern: &str) -> io::Result<Vec<String>> {
    let pattern: Vec<char> = pattern.chars().collect();
    let mut reader = BufReader::new(File::open(path)?);
    let mut lines = Vec::new();
    let mut line = String::new();
    // read_line reads up to and with the next LF, so a CR stays in the line.
    // It returns 0 at the end of the file.
    while reader.read_line(&mut line)? > 0 {
        if line.ends_with('\n') {
            line.pop();
        }
        let text: Vec<char> = line.chars().collect();
        if matches(&pattern, &text) {
            lines.push(line.clone());
        }
        line.clear();
    }
    Ok(lines)
}

/// Reports whether `re` matches some part of `text`.
fn matches(re: &[char], text: &[char]) -> bool {
    if let ['^', rest @ ..] = re {
        return match_here(rest, text);
    }
    // Try each start, also the end of the text, where only an empty match is
    // possible.
    (0..=text.len()).any(|i| match_here(re, &text[i..]))
}

/// Reports whether `re` matches the start of `text`.
fn match_here(re: &[char], text: &[char]) -> bool {
    match re {
        [] => true,
        [c, '*', rest @ ..] => match_star(*c, rest, text),
        ['$'] => text.is_empty(),
        [c, rest @ ..] => match text {
            [t, text @ ..] if *c == '.' || c == t => match_here(rest, text),
            _ => false,
        },
    }
}

/// Reports whether zero or more `c`, then `re`, match the start of `text`.
fn match_star(c: char, re: &[char], text: &[char]) -> bool {
    let mut i = 0;
    loop {
        if match_here(re, &text[i..]) {
            return true;
        }
        if i < text.len() && (c == '.' || text[i] == c) {
            i += 1;
        } else {
            return false;
        }
    }
}
