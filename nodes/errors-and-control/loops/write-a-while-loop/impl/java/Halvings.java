// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

import java.util.ArrayList;
import java.util.List;

final class Halvings {
  private Halvings() {}

  /** Returns n, n / 2, n / 4, ... while the value is greater than 0. */
  static List<Integer> halvings(int n) {
    List<Integer> out = new ArrayList<>();
    while (n > 0) {
      out.add(n);
      n /= 2;
    }
    return out;
  }
}
