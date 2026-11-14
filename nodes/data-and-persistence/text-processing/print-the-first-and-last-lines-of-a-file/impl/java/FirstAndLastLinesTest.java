// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FirstAndLastLinesTest {
  @TempDir Path dir;

  // The numbers from and up to to, but not to.
  private static List<String> numbers(int from, int to) {
    return IntStream.range(from, to).mapToObj(Integer::toString).toList();
  }

  private static String text(List<String> lines) {
    return lines.stream().map(l -> l + "\n").collect(Collectors.joining());
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two", "a\nb\nc\nd\ne\n", 2, List.of("a", "b"), List.of("d", "e")),
        Arguments.of("one", "first\nmiddle\nlast\n", 1, List.of("first"), List.of("last")),
        Arguments.of("ten", text(numbers(1, 13)), 10, numbers(1, 11), numbers(3, 13)),
        Arguments.of("empty", "", 2, List.of(), List.of()),
        Arguments.of("zero", "a\nb\n", 0, List.of(), List.of()),
        Arguments.of("short", "a\nb\n", 5, List.of("a", "b"), List.of("a", "b")),
        Arguments.of("exact", "a\nb\n", 2, List.of("a", "b"), List.of("a", "b")),
        Arguments.of("overlap", "a\nb\nc\n", 2, List.of("a", "b"), List.of("b", "c")),
        Arguments.of("one-line", "a\n", 1, List.of("a"), List.of("a")),
        Arguments.of("empty-lines", "\n\na\n\n", 1, List.of(""), List.of("")),
        Arguments.of("only-newline", "\n", 1, List.of(""), List.of("")),
        Arguments.of("no-final-newline", "a\nb\nc", 1, List.of("a"), List.of("c")),
        Arguments.of("crlf", "a\r\nb\r\n", 1, List.of("a\r"), List.of("b\r")),
        Arguments.of(
            "non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 1, List.of("caf\u00e9"), List.of("zo\u00eb")),
        // 100000 lines with the numbers 0 to 99999.
        Arguments.of("many-lines", text(numbers(0, 100000)), 3, numbers(0, 3), numbers(99997, 100000)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void read(String name, String text, int count, List<String> first, List<String> last)
      throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, text, StandardCharsets.UTF_8);
    assertEquals(new FirstAndLastLines.Ends(first, last), FirstAndLastLines.read(path, count), name);
  }

  @Test
  void notFound() {
    assertThrows(
        NoSuchFileException.class, () -> FirstAndLastLines.read(dir.resolve("missing.txt"), 1));
  }
}
