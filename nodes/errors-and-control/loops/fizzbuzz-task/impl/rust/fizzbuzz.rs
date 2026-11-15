// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

/// Returns FizzBuzz, Fizz, Buzz or the number for each number from 1 to n.
pub fn fizzbuzz(n: u32) -> Vec<String> {
    let mut out = Vec::new();
    for i in 1..=n {
        // Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
        if i % 15 == 0 {
            out.push("FizzBuzz".to_string());
        } else if i % 3 == 0 {
            out.push("Fizz".to_string());
        } else if i % 5 == 0 {
            out.push("Buzz".to_string());
        } else {
            out.push(i.to_string());
        }
    }
    out
}
