// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.Optional;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SpecificLineTest {
  @TempDir Path dir;

  // 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
  private static String numbers() {
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < 100000; i++) {
      b.append(i).append('\n');
    }
    return b.toString();
  }

  static Stream<Arguments> cases() {
    String many = numbers();
    return Stream.of(
        Arguments.of("first", "a\nb\nc\n", 1, Optional.of("a")),
        Arguments.of("middle", "a\nb\nc\n", 2, Optional.of("b")),
        Arguments.of("last", "a\nb\nc\n", 3, Optional.of("c")),
        Arguments.of("past-the-end", "a\nb\n", 3, Optional.empty()),
        Arguments.of("zero", "a\n", 0, Optional.empty()),
        Arguments.of("negative", "a\n", -1, Optional.empty()),
        Arguments.of("empty", "", 1, Optional.empty()),
        Arguments.of("empty-line", "a\n\nb\n", 2, Optional.of("")),
        Arguments.of("only-newline", "\n", 1, Optional.of("")),
        Arguments.of("no-final-newline", "a\nb", 2, Optional.of("b")),
        Arguments.of("crlf", "a\r\nb\r\n", 1, Optional.of("a\r")),
        Arguments.of("lone-cr", "a\rb\nc\n", 2, Optional.of("c")),
        Arguments.of("non-ascii", "caf\u00e9\nzo\u00eb\n", 2, Optional.of("zo\u00eb")),
        Arguments.of("many-lines", many, 100000, Optional.of("99999")),
        Arguments.of("many-lines-past-the-end", many, 100001, Optional.empty()));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void read(String name, String text, int number, Optional<String> want) throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, text, StandardCharsets.UTF_8);
    assertEquals(want, SpecificLine.read(path, number), name);
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class, () -> SpecificLine.read(dir.resolve("missing.txt"), 1));
  }
}
