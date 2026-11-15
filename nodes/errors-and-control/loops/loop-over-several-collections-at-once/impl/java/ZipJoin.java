// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

import java.util.ArrayList;
import java.util.List;

final class ZipJoin {
  private ZipJoin() {}

  /** Joins the items at each index, up to the end of the shortest list. */
  static List<String> zipJoin(String[] lower, String[] upper, int[] digits) {
    List<String> out = new ArrayList<>();
    // An index past the end of a shorter array would throw.
    int n = Math.min(lower.length, Math.min(upper.length, digits.length));
    for (int i = 0; i < n; i++) {
      out.add(lower[i] + upper[i] + digits[i]);
    }
    return out;
  }
}
