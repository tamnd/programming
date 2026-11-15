// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

import java.util.ArrayList;
import java.util.List;

final class Positions {
  private Positions() {}

  /** Returns the index of each item that equals target. */
  static List<Integer> positions(String[] items, String target) {
    List<Integer> out = new ArrayList<>();
    for (int i = 0; i < items.length; i++) {
      // equals compares the characters. == would compare the references.
      if (items[i].equals(target)) {
        out.add(i);
      }
    }
    return out;
  }
}
