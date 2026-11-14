// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

final class BuildMap {
  private BuildMap() {}

  /** Puts the pairs in a map, removes the keys in remove, and returns the entries sorted by key. */
  static List<Map.Entry<String, Integer>> buildMap(List<Map.Entry<String, Integer>> pairs, List<String> remove) {
    Map<String, Integer> map = new HashMap<>();
    for (var pair : pairs) {
      // put adds the key, or replaces the value of a key that is there.
      map.put(pair.getKey(), pair.getValue());
    }
    for (var key : remove) {
      // remove returns null for a key that is not there.
      map.remove(key);
    }
    // A TreeMap orders String keys by compareTo, which compares the chars.
    return new TreeMap<>(map).entrySet().stream()
        .map(e -> Map.entry(e.getKey(), e.getValue()))
        .toList();
  }
}
