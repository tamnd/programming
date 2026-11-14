// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

import java.util.HashSet;
import java.util.List;
import java.util.Set;

final class Toggle {
  private Toggle() {}

  /** Adds each word that is not in the set, and removes each word that is in it. */
  static List<String> toggle(List<String> words) {
    Set<String> seen = new HashSet<>();
    for (String word : words) {
      // remove returns true if the set had the word.
      if (!seen.remove(word)) {
        seen.add(word);
      }
    }
    // String.compareTo compares UTF-16 code units, which is the byte order for ASCII.
    return seen.stream().sorted().toList();
  }
}
