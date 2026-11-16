// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

/**
 * Returns the letter grade for a score from 0 to 100.
 * @param {number} score
 * @returns {string}
 */
export function grade(score) {
  if (score >= 90) {
    return "A";
  } else if (score >= 80) {
    return "B";
  } else if (score >= 70) {
    return "C";
  } else if (score >= 60) {
    return "D";
  } else {
    return "F";
  }
}
