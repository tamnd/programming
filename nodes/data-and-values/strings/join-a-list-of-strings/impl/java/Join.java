// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

/** Joins a list of strings, and a list of numbers, with a separator. */
public final class Join {
  private Join() {}

  /** Returns the items with sep between each two. */
  public static String join(List<String> items, String sep) {
    return String.join(sep, items);
  }

  /** Returns the decimal form of each item, with sep between each two. */
  public static String joinNumbers(int[] items, String sep) {
    // String.join takes only CharSequence items, so the stream converts each
    // int first.
    return Arrays.stream(items).mapToObj(Integer::toString).collect(Collectors.joining(sep));
  }
}
