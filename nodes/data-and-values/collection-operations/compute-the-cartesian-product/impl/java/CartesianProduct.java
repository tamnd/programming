// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

final class CartesianProduct {
  private CartesianProduct() {}

  /** Returns each tuple that takes one item from each array, in nested-loop order. */
  static int[][] cartesianProduct(int[][] lists) {
    List<int[]> tuples = new ArrayList<>();
    tuples.add(new int[0]);
    for (int[] xs : lists) {
      List<int[]> next = new ArrayList<>();
      for (int[] t : tuples) {
        for (int x : xs) {
          // A new array for each tuple, so that no two tuples share one.
          int[] u = Arrays.copyOf(t, t.length + 1);
          u[t.length] = x;
          next.add(u);
        }
      }
      tuples = next;
    }
    return tuples.toArray(new int[0][]);
  }
}
