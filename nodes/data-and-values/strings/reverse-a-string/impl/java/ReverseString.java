// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.MatchResult;
import java.util.regex.Pattern;

public final class ReverseString {
  private ReverseString() {}

  /**
   * Returns the code points of s in the opposite order. StringBuilder.reverse treats a surrogate
   * pair as one character, so the pair keeps its order.
   */
  public static String reverseCodePoints(String s) {
    return new StringBuilder(s).reverse().toString();
  }

  // \X matches one extended grapheme cluster.
  private static final Pattern GRAPHEME = Pattern.compile("\\X");

  /** Returns the extended grapheme clusters of s in the opposite order. */
  public static String reverseGraphemes(String s) {
    List<String> parts = new ArrayList<>(GRAPHEME.matcher(s).results().map(MatchResult::group).toList());
    Collections.reverse(parts);
    return String.join("", parts);
  }
}
