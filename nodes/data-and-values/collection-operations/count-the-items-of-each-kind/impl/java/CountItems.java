// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

import java.util.Map;
import java.util.TreeMap;

final class CountItems {
  private CountItems() {}

  /** Returns one {item, count} pair for each distinct item, in ascending order of the item. */
  static int[][] countItems(int[] items) {
    // A TreeMap keeps its keys in ascending order.
    Map<Integer, Integer> counts = new TreeMap<>();
    for (int x : items) {
      counts.merge(x, 1, Integer::sum);
    }
    int[][] out = new int[counts.size()][];
    int i = 0;
    for (Map.Entry<Integer, Integer> e : counts.entrySet()) {
      out[i++] = new int[] {e.getKey(), e.getValue()};
    }
    return out;
  }
}
