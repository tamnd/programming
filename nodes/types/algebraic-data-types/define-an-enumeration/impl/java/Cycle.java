// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

final class Cycle {
  private Cycle() {}

  /** The days of the week. values() gives them in this order. */
  enum Day {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY;

    String label() {
      return name().toLowerCase(Locale.ROOT);
    }
  }

  static List<String> cycle(String start, int steps) {
    Day[] days = Day.values();
    for (Day d : days) {
      if (d.label().equals(start)) {
        List<String> out = new ArrayList<>();
        for (int k = 0; k < steps; k++) {
          out.add(days[(d.ordinal() + k) % days.length].label());
        }
        return out;
      }
    }
    return List.of();
  }
}
