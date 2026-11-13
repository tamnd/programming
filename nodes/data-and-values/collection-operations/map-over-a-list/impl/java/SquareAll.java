// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

import java.util.Arrays;

final class SquareAll {
  private SquareAll() {}

  /** Returns a new array with the square of each item, in the same order. */
  static int[] squareAll(int[] items) {
    return Arrays.stream(items).map(x -> x * x).toArray();
  }
}
