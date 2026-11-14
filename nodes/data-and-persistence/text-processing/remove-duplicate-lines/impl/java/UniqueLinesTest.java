// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class UniqueLinesTest {
  @TempDir Path dir;

  // 100000 lines with 1000 different lines.
  private static String manyLines() {
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < 100000; i++) {
      b.append(i % 1000).append('\n');
    }
    return b.toString();
  }

  private static List<String> numbers(int n) {
    List<String> lines = new ArrayList<>(n);
    for (int i = 0; i < n; i++) {
      lines.add(Integer.toString(i));
    }
    return lines;
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("simple", "b\na\nb\nc\na\n", List.of("b", "a", "c")),
        Arguments.of("adjacent", "a\na\nb\n", List.of("a", "b")),
        Arguments.of("no-duplicates", "c\nb\na\n", List.of("c", "b", "a")),
        Arguments.of("empty", "", List.of()),
        Arguments.of("empty-lines", "\na\n\n\n", List.of("", "a")),
        Arguments.of("all-same", "x\nx\nx\n", List.of("x")),
        Arguments.of("no-final-newline", "a\nb\na", List.of("a", "b")),
        Arguments.of("crlf", "a\r\na\n", List.of("a\r", "a")),
        Arguments.of("case", "A\na\nA\n", List.of("A", "a")),
        Arguments.of("whitespace", "a \na\n a\na \n", List.of("a ", "a", " a")),
        // U+00E9 and e with U+0301 look the same, but their code points are not equal.
        Arguments.of("normalization", "\u00e9\ne\u0301\n\u00e9\n", List.of("\u00e9", "e\u0301")),
        Arguments.of("many-lines", manyLines(), numbers(1000)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void read(String name, String text, List<String> want) throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, text, StandardCharsets.UTF_8);
    assertEquals(want, UniqueLines.read(path), name);
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class, () -> UniqueLines.read(dir.resolve("missing.txt")));
  }
}
