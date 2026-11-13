// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

/// The text of lines.txt. The compiler reads the file, relative to this
/// source file, and puts it in the program as a `&'static str`.
pub const LINES: &str = include_str!("lines.txt");

/// The same file as bytes, a `&'static [u8; N]`.
pub const BYTES: &[u8] = include_bytes!("lines.txt");

/// Returns line `n` of lines.txt (1 is the line after the license header), with no line end, or
/// `None` when there is no such line.
pub fn embedded_line(n: i64) -> Option<String> {
    if n < 1 {
        return None;
    }
    let index = usize::try_from(n - 1).ok()?;
    // The first line is the license header of the file.
    LINES.lines().skip(1).nth(index).map(str::to_string)
}
