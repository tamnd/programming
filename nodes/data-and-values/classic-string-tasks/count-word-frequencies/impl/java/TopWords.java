// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Counts the words of a text and returns the most frequent ones. */
public final class TopWords {
  /** A word and the number of times it occurs. */
  public record WordCount(String word, long count) {}

  // A word is a run of ASCII letters, ASCII digits and characters that are not ASCII.
  private static final Pattern WORD = Pattern.compile("[A-Za-z0-9\\x{80}-\\x{10FFFF}]+");

  private TopWords() {}

  /**
   * Returns the n most frequent words of text, by count from high to low, then by word in code
   * point order.
   */
  public static List<WordCount> topWords(String text, int n) {
    Map<String, Long> counts = new HashMap<>();
    Matcher m = WORD.matcher(text);
    while (m.find()) {
      // Locale.ROOT: the default locale can change the mapping, for example in Turkish.
      counts.merge(m.group().toLowerCase(Locale.ROOT), 1L, Long::sum);
    }
    // String.compareTo compares UTF-16 code units. Below U+D800 that is code point order.
    return counts.entrySet().stream()
        .map(e -> new WordCount(e.getKey(), e.getValue()))
        .sorted(
            Comparator.comparingLong(WordCount::count).reversed().thenComparing(WordCount::word))
        .limit(n)
        .toList();
  }
}
