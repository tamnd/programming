// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

import java.util.List;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;

final class GroupByLength {
  private GroupByLength() {}

  /** Returns the words of each length, in ascending order of the length. */
  static SortedMap<Integer, List<String>> groupByLength(List<String> words) {
    // A TreeMap keeps the keys in order. toList keeps the order of the words.
    return words.stream()
        .collect(Collectors.groupingBy(String::length, TreeMap::new, Collectors.toList()));
  }
}
