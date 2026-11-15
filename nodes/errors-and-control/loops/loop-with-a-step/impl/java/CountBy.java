// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

import java.util.ArrayList;
import java.util.List;

final class CountBy {
  private CountBy() {}

  /** Returns start, start + step, ... while the value has not passed stop. */
  static List<Integer> countBy(int start, int stop, int step) {
    List<Integer> out = new ArrayList<>();
    // A long counter: with an int, i + step can wrap past Integer.MAX_VALUE.
    for (long i = start; step > 0 ? i <= stop : i >= stop; i += step) {
      out.add((int) i);
    }
    return out;
  }
}
