// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

import java.util.List;

final class MinMax {
  private MinMax() {}

  /** Java has no tuple type. A record is a named tuple with named components. */
  record Pair(int min, int max) {}

  /** Returns the smallest and the largest of a list with at least one number. */
  static Pair minMax(List<Integer> numbers) {
    int lo = numbers.get(0);
    int hi = lo;
    for (int n : numbers) {
      lo = Math.min(lo, n);
      hi = Math.max(hi, n);
    }
    return new Pair(lo, hi);
  }
}
