// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

/// Returns true if the brackets of text match and nest correctly.
/// A Vec is the stack: push adds to the end, and pop removes from the end.
pub fn balanced(text: &str) -> bool {
    let mut stack: Vec<char> = Vec::new();
    for c in text.chars() {
        let want = match c {
            '(' | '[' | '{' => {
                stack.push(c);
                continue;
            }
            ')' => '(',
            ']' => '[',
            '}' => '{',
            _ => continue,
        };
        // pop returns None on an empty Vec, so an extra closing bracket
        // does not match.
        if stack.pop() != Some(want) {
            return false;
        }
    }
    stack.is_empty()
}
