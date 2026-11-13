// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

import java.util.stream.IntStream;

final class ZipPairs {
  private ZipPairs() {}

  /** Returns the pairs {a[i], b[i]} up to the end of the shorter array. */
  static int[][] zipPairs(int[] a, int[] b) {
    int n = Math.min(a.length, b.length);
    return IntStream.range(0, n).mapToObj(i -> new int[] {a[i], b[i]}).toArray(int[][]::new);
  }
}
