// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

import java.util.Arrays;
import java.util.stream.IntStream;

final class Take {
  private Take() {}

  /** Returns a copy of the first n items; a shorter array gives all its items. */
  static int[] take(int[] items, int n) {
    // copyOf pads with zeros when the length is larger, so use the smaller length.
    return Arrays.copyOf(items, Math.min(n, items.length));
  }

  /** Returns the first n items of the infinite sequence start, start + 1, ... */
  static int[] takeNaturals(int start, int n) {
    // iterate makes an infinite lazy stream; limit stops it after n items.
    return IntStream.iterate(start, i -> i + 1).limit(n).toArray();
  }
}
