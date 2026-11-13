// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

import java.util.Arrays;

final class PartitionOdd {
  private PartitionOdd() {}

  /** Returns two arrays: the odd items and the even items, each in input order. */
  static int[][] partitionOdd(int[] items) {
    int[] odd = Arrays.stream(items).filter(x -> x % 2 != 0).toArray();
    int[] even = Arrays.stream(items).filter(x -> x % 2 == 0).toArray();
    return new int[][] {odd, even};
  }
}
