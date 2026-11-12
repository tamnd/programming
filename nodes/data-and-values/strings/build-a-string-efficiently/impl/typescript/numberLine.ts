// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

/** Returns the numbers 1 to n, separated by commas. */
export function numberLine(n: number): string {
  // Collect the parts in an array and join them once at the end.
  const parts: string[] = [];
  for (let i = 1; i <= n; i++) {
    parts.push(String(i));
  }
  return parts.join(",");
}
