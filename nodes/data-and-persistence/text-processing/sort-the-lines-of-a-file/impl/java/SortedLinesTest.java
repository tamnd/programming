// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SortedLinesTest {
  @TempDir Path dir;

  private static List<String> aThenB(int n) {
    List<String> lines = new ArrayList<>(Collections.nCopies(n, "a"));
    lines.addAll(Collections.nCopies(n, "b"));
    return lines;
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("simple", "pear\napple\nfig\n", List.of("apple", "fig", "pear")),
        Arguments.of("case", "b\nB\na\nA\n", List.of("A", "B", "a", "b")),
        Arguments.of("numbers", "10\n9\n100\n", List.of("10", "100", "9")),
        Arguments.of("empty", "", List.of()),
        Arguments.of("one-empty-line", "\n", List.of("")),
        Arguments.of("empty-lines", "b\n\na\n\n", List.of("", "", "a", "b")),
        Arguments.of("duplicates", "b\na\nb\n", List.of("a", "b", "b")),
        Arguments.of("no-final-newline", "b\na", List.of("a", "b")),
        Arguments.of("crlf", "b\r\na\r\n", List.of("a\r", "b\r")),
        Arguments.of("prefix", "ab\na\n", List.of("a", "ab")),
        Arguments.of("whitespace", "b\n a\n\tc\n", List.of("\tc", " a", "b")),
        Arguments.of("non-ascii", "\u00e9\nz\ne\n", List.of("e", "z", "\u00e9")),
        // U+FF61 is below U+1F600, but its UTF-16 char is above 0xD83D.
        Arguments.of("supplementary", "\ud83d\ude00\n\uff61\n", List.of("\uff61", "\ud83d\ude00")),
        Arguments.of("other-line-breaks", "b\u2028a\nb\u001ca\nb\fa\n", List.of("b\fa", "b\u001ca", "b\u2028a")),
        Arguments.of("many-lines", "b\na\n".repeat(50000), aThenB(50000)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void read(String name, String text, List<String> want) throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, text, StandardCharsets.UTF_8);
    assertEquals(want, SortedLines.read(path), name);
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class, () -> SortedLines.read(dir.resolve("missing.txt")));
  }
}
