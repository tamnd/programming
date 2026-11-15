// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

/**
 * Returns FizzBuzz, Fizz, Buzz or the number for each number from 1 to n.
 * @param {number} n
 * @returns {string[]}
 */
export function fizzbuzz(n) {
  const out = [];
  for (let i = 1; i <= n; i++) {
    // Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
    if (i % 15 === 0) {
      out.push("FizzBuzz");
    } else if (i % 3 === 0) {
      out.push("Fizz");
    } else if (i % 5 === 0) {
      out.push("Buzz");
    } else {
      out.push(String(i));
    }
  }
  return out;
}
