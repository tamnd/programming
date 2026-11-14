// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

export function toggle(words: readonly string[]): string[] {
  const seen = new Set<string>();
  for (const word of words) {
    // delete returns true if the set had the word.
    if (!seen.delete(word)) {
      seen.add(word);
    }
  }
  // The default sort compares UTF-16 code units, which is the byte order for ASCII.
  return [...seen].sort();
}
