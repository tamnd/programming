// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

import java.util.List;
import java.util.Map;

final class Lookup {
  private Lookup() {}

  /** Returns the value of each key in table, or fallback for a key not in table. */
  static List<Integer> lookup(Map<String, Integer> table, List<String> keys, int fallback) {
    // getOrDefault does not add the key. get would return null for a missing key.
    return keys.stream().map(key -> table.getOrDefault(key, fallback)).toList();
  }
}
