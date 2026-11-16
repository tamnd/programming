// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

import java.util.ArrayList;
import java.util.List;

/** A color whose string form is the hexadecimal notation, such as #FF8000. */
public record Color(int r, int g, int b) {
  public Color {
    for (int v : new int[] {r, g, b}) {
      if (v < 0 || v > 255) {
        throw new IllegalArgumentException("not a channel value: " + v);
      }
    }
  }

  /** The display form. String concatenation, String.valueOf and println call it. */
  @Override
  public String toString() {
    return String.format("#%02X%02X%02X", r, g, b);
  }

  public static List<String> formatColors(List<Integer> reds, List<Integer> greens, List<Integer> blues) {
    if (reds.size() != greens.size() || reds.size() != blues.size()) {
      throw new IllegalArgumentException("the lists must have the same length");
    }
    List<String> out = new ArrayList<>(reds.size());
    for (int i = 0; i < reds.size(); i++) {
      out.add(String.valueOf(new Color(reds.get(i), greens.get(i), blues.get(i))));
    }
    return out;
  }
}
