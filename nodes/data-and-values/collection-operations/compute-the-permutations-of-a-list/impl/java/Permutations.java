// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

import java.util.ArrayList;
import java.util.List;

final class Permutations {
  private Permutations() {}

  /** Returns each order of the items, in the lexicographic order of the positions. */
  static int[][] permutations(int[] items) {
    List<int[]> out = new ArrayList<>();
    visit(items, new boolean[items.length], new int[items.length], 0, out);
    return out.toArray(new int[0][]);
  }

  private static void visit(int[] items, boolean[] used, int[] path, int depth, List<int[]> out) {
    if (depth == items.length) {
      // path changes later, so add a copy.
      out.add(path.clone());
      return;
    }
    for (int i = 0; i < items.length; i++) {
      if (used[i]) {
        continue;
      }
      used[i] = true;
      path[depth] = items[i];
      visit(items, used, path, depth + 1, out);
      used[i] = false;
    }
  }
}
