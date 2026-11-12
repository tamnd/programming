// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

// The replacement is a function, so the patterns such as $& in nw have no
// special meaning.

/** Returns s with each match of old replaced by nw. */
export function replace(s, old, nw) {
  return s.replaceAll(old, () => nw);
}

/** Returns s with the first match of old replaced by nw. */
export function replaceFirst(s, old, nw) {
  // With a string pattern, replace changes only the first match.
  return s.replace(old, () => nw);
}
