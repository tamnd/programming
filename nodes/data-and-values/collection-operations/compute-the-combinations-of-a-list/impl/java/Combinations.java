// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

import java.util.ArrayList;
import java.util.List;

final class Combinations {
  private Combinations() {}

  /** Returns each choice of k items, in the lexicographic order of the positions. */
  static int[][] combinations(int[] items, int k) {
    List<int[]> out = new ArrayList<>();
    if (k <= items.length) {
      visit(items, new int[k], 0, 0, out);
    }
    return out.toArray(new int[0][]);
  }

  private static void visit(int[] items, int[] path, int depth, int start, List<int[]> out) {
    if (depth == path.length) {
      // path changes later, so add a copy.
      out.add(path.clone());
      return;
    }
    // Stop when too few items are left to fill path.
    for (int i = start; i <= items.length - (path.length - depth); i++) {
      path[depth] = items[i];
      visit(items, path, depth + 1, i + 1, out);
    }
  }
}
