// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

final class ScoreLines {
  private ScoreLines() {}

  /** Returns a line key=value for each entry in key order, then the total of the values. */
  static List<String> scoreLines(Map<String, Integer> scores) {
    List<String> lines = new ArrayList<>();
    // A HashMap gives no order. A TreeMap gives its entries in key order.
    for (Map.Entry<String, Integer> e : new TreeMap<>(scores).entrySet()) {
      lines.add(e.getKey() + "=" + e.getValue());
    }
    int total = 0;
    for (int value : scores.values()) {
      total += value;
    }
    lines.add("total=" + total);
    return lines;
  }
}
