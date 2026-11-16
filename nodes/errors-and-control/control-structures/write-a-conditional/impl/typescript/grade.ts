// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

export type Grade = "A" | "B" | "C" | "D" | "F";

export function grade(score: number): Grade {
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
