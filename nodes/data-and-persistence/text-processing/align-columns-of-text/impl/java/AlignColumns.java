// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

import java.util.ArrayList;
import java.util.List;

/** Aligns the fields of a text in columns. */
public final class AlignColumns {
  private AlignColumns() {}

  /**
   * Aligns the fields of the text in columns. A $ separates two fields, and align is "left",
   * "right" or "center".
   *
   * @throws IllegalArgumentException if align is not one of these words
   */
  public static String align(String text, String align) {
    if (!align.equals("left") && !align.equals("right") && !align.equals("center")) {
      throw new IllegalArgumentException("align must be left, right or center: " + align);
    }
    List<List<String>> rows = new ArrayList<>();
    List<Integer> widths = new ArrayList<>();
    for (String line : split(text, '\n')) {
      List<String> fields = split(line, '$');
      rows.add(fields);
      for (int j = 0; j < fields.size(); j++) {
        int width = width(fields.get(j));
        if (j == widths.size()) {
          widths.add(width);
        } else if (width > widths.get(j)) {
          widths.set(j, width);
        }
      }
    }
    StringBuilder out = new StringBuilder();
    for (List<String> fields : rows) {
      for (int j = 0; j < fields.size(); j++) {
        String field = fields.get(j);
        int pad = widths.get(j) - width(field);
        int left =
            switch (align) {
              case "left" -> 0;
              case "right" -> pad;
              default -> pad / 2;
            };
        if (j > 0) {
          out.append(' ');
        }
        out.append(" ".repeat(left)).append(field);
        if (j + 1 < fields.size()) {
          out.append(" ".repeat(pad - left));
        }
      }
      out.append('\n');
    }
    return out.toString();
  }

  /** Returns the number of code points in s. String.length() counts UTF-16 code units. */
  private static int width(String s) {
    return s.codePointCount(0, s.length());
  }

  /** Splits s at each sep. A sep at the end does not start a new part. */
  private static List<String> split(String s, char sep) {
    List<String> parts = new ArrayList<>();
    int start = 0;
    while (start < s.length()) {
      int at = s.indexOf(sep, start);
      if (at < 0) {
        at = s.length();
      }
      parts.add(s.substring(start, at));
      start = at + 1;
    }
    return parts;
  }
}
