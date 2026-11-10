// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

import java.util.Arrays;
import java.util.List;
import java.util.regex.Pattern;

/** Splits strings on a literal separator, on white space and on a regular expression. */
public final class Split {
  private static final Pattern WHITESPACE = Pattern.compile("\\s+");

  private Split() {}

  /**
   * Splits s on the literal separator sep and keeps the empty fields. With a limit n > 0 the
   * result has at most n fields, and the last field holds the rest of s. A limit of 0 means no
   * limit.
   */
  public static List<String> split(String s, String sep, int limit) {
    // String.split takes a regular expression, so quote the separator. A negative limit keeps
    // the trailing empty strings, which a limit of 0 removes.
    return List.of(s.split(Pattern.quote(sep), limit > 0 ? limit : -1));
  }

  /** Splits s on runs of white space. The result has no empty fields. */
  public static List<String> splitWhitespace(String s) {
    return Arrays.stream(WHITESPACE.split(s)).filter(field -> !field.isEmpty()).toList();
  }

  /** Splits s on each match of the regular expression pattern. */
  public static List<String> splitRegex(String s, String pattern) {
    return List.of(Pattern.compile(pattern).split(s, -1));
  }
}
