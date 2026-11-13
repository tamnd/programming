// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

import java.util.Arrays;
import java.util.Objects;

final class SliceList {
  private SliceList() {}

  /** Returns a new array of items[start] up to, but not including, items[stop]. */
  static int[] slice(int[] items, int start, int stop) {
    // copyOfRange pads with zeros when stop is past the end, so check first.
    Objects.checkFromToIndex(start, stop, items.length);
    return Arrays.copyOfRange(items, start, stop);
  }
}
