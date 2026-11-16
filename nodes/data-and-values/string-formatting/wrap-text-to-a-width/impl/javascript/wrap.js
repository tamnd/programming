// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

/**
 * Returns the lines of text, each at most width characters long, with the
 * greedy method. A word longer than width is a line of its own.
 */
export function wrap(text, width) {
  const lines = [];
  let line = "";
  for (const word of text.split(/\s+/).filter(Boolean)) {
    if (line === "") {
      line = word;
    } else if (line.length + 1 + word.length <= width) {
      line += " " + word;
    } else {
      lines.push(line);
      line = word;
    }
  }
  if (line !== "") {
    lines.push(line);
  }
  return lines;
}
