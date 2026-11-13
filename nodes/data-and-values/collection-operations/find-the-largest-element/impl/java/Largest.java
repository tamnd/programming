// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

import java.util.Arrays;

final class Largest {
  private Largest() {}

  /** Returns the largest item. It throws NoSuchElementException for an empty array. */
  static int largest(int[] items) {
    return Arrays.stream(items).max().orElseThrow();
  }
}
