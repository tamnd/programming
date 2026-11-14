// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

import java.util.List;
import java.util.Map;
import java.util.TreeMap;

final class MergeMaps {
  private MergeMaps() {}

  /** Returns the entries of both maps in key order; the value of override wins. */
  static List<Map.Entry<String, Integer>> mergeMaps(Map<String, Integer> base, Map<String, Integer> override) {
    // The copy keeps base as it is. putAll replaces the values of keys that are there.
    Map<String, Integer> merged = new TreeMap<>(base);
    merged.putAll(override);
    return merged.entrySet().stream()
        .map(e -> Map.entry(e.getKey(), e.getValue()))
        .toList();
  }
}
