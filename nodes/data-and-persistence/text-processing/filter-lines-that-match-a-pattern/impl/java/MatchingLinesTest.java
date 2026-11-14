// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class MatchingLinesTest {
  @TempDir Path dir;

  // 100000 lines with the numbers 0 to 99999.
  private static String numbers() {
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < 100000; i++) {
      b.append(i).append('\n');
    }
    return b.toString();
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("literal", "apple\nbanana\ncherry\n", "an", List.of("banana")),
        Arguments.of("several", "cat\ndog\ncatalog\n", "cat", List.of("cat", "catalog")),
        Arguments.of("start", "ab\nba\n", "^a", List.of("ab")),
        Arguments.of("end", "ab\nba\n", "a$", List.of("ba")),
        Arguments.of("dot", "abc\nac\naxc\n", "a.c", List.of("abc", "axc")),
        Arguments.of("star", "ac\nabc\nabbc\nadc\n", "ab*c", List.of("ac", "abc", "abbc")),
        Arguments.of("whole-line", "a\nab\nba\n", "^a$", List.of("a")),
        Arguments.of("empty", "", "a", List.of()),
        Arguments.of("no-match", "abc\n", "x", List.of()),
        Arguments.of("empty-lines", "a\n\nb\n\n", "^$", List.of("", "")),
        Arguments.of("match-all", "a\n\nb\n", ".*", List.of("a", "", "b")),
        Arguments.of("duplicates", "a\nb\na\n", "a", List.of("a", "a")),
        Arguments.of("no-final-newline", "a\nab", "b", List.of("ab")),
        Arguments.of("crlf-end", "ab\r\nab\n", "b$", List.of("ab")),
        Arguments.of("crlf-dot", "a\r\n", "^a.$", List.of("a\r")),
        Arguments.of("case", "Apple\napple\n", "apple", List.of("apple")),
        Arguments.of("non-ascii", "caf\u00e9\ncafe\n", "\u00e9", List.of("caf\u00e9")),
        // Five of the numbers hold only nines.
        Arguments.of("many-lines", numbers(), "^9*$", List.of("9", "99", "999", "9999", "99999")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void read(String name, String text, String pattern, List<String> want) throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, text, StandardCharsets.UTF_8);
    assertEquals(want, MatchingLines.read(path, pattern), name);
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class, () -> MatchingLines.read(dir.resolve("missing.txt"), "a"));
  }
}
