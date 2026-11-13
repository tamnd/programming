// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

import java.util.Comparator;
import java.util.List;

final class SortByLength {
  private SortByLength() {}

  /** Returns the words in order of the length, then of the word. */
  static List<String> sortByLength(List<String> words) {
    return words.stream()
        .sorted(Comparator.comparingInt(String::length).thenComparing(Comparator.naturalOrder()))
        .toList();
  }
}
