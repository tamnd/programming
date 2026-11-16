// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

/// Returns the lines of text, each at most width bytes long, with the
/// greedy method. A word longer than width is a line of its own.
pub fn wrap(text: &str, width: usize) -> Vec<String> {
    let mut lines = Vec::new();
    let mut line = String::new();
    for word in text.split_whitespace() {
        if line.is_empty() {
            line.push_str(word);
        } else if line.len() + 1 + word.len() <= width {
            line.push(' ');
            line.push_str(word);
        } else {
            lines.push(std::mem::take(&mut line));
            line.push_str(word);
        }
    }
    if !line.is_empty() {
        lines.push(line);
    }
    lines
}
