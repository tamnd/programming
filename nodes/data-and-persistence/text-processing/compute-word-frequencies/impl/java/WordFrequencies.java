// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/** Finds the most frequent words of a text file. */
public final class WordFrequencies {
  private WordFrequencies() {}

  /** One word and the number of times it occurs. */
  public record WordCount(String word, long count) {}

  /**
   * Returns the n most frequent words of the file at path. A word is a run of ASCII letters,
   * changed to lower case. The result is sorted by count from high to low, then by word. A
   * missing file throws NoSuchFileException.
   */
  public static List<WordCount> of(Path path, int n) throws IOException {
    Map<String, Long> counts = new HashMap<>();
    // The current word stays in word across the reads of the buffer.
    StringBuilder word = new StringBuilder();
    try (InputStream in = new BufferedInputStream(Files.newInputStream(path), 64 * 1024)) {
      for (int b = 0; b != -1; ) {
        b = in.read();
        if (b >= 'A' && b <= 'Z') {
          word.append((char) (b - 'A' + 'a'));
        } else if (b >= 'a' && b <= 'z') {
          word.append((char) b);
        } else if (word.length() > 0) {
          // A byte that is not a letter, or the end of the file, ends the word.
          counts.merge(word.toString(), 1L, Long::sum);
          word.setLength(0);
        }
      }
    }
    List<WordCount> ranked = new ArrayList<>();
    counts.forEach((w, c) -> ranked.add(new WordCount(w, c)));
    // The order of a HashMap is not fixed, so the word is the second key.
    ranked.sort(
        Comparator.comparingLong(WordCount::count).reversed().thenComparing(WordCount::word));
    return List.copyOf(ranked.subList(0, Math.min(n, ranked.size())));
  }
}
