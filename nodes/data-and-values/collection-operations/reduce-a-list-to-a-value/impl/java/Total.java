// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

import java.util.Arrays;

final class Total {
  private Total() {}

  /** Returns the sum of items. The sum is in a long, so a partial sum does not overflow. */
  static int total(int[] items) {
    long sum = Arrays.stream(items).asLongStream().reduce(0L, Long::sum);
    return Math.toIntExact(sum);
  }
}
