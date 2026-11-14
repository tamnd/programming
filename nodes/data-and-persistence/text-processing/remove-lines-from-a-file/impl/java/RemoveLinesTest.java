// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFileAttributeView;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.CsvSource;
import org.junit.jupiter.params.provider.MethodSource;

class RemoveLinesTest {
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
        Arguments.of("first-two", "a\nb\nc\nd\n", 1, 2, 2, "c\nd\n"),
        Arguments.of("middle", "a\nb\nc\nd\n", 2, 2, 2, "a\nd\n"),
        Arguments.of("last", "a\nb\nc\n", 3, 1, 1, "a\nb\n"),
        Arguments.of("past-the-end", "a\nb\n", 3, 1, 0, "a\nb\n"),
        Arguments.of("partly-past-the-end", "a\nb\nc\n", 2, 5, 2, "a\n"),
        Arguments.of("count-zero", "a\nb\n", 1, 0, 0, "a\nb\n"),
        Arguments.of("all-lines", "a\nb\n", 1, 2, 2, ""),
        Arguments.of("empty", "", 1, 1, 0, ""),
        Arguments.of("empty-line", "a\n\nb\n", 2, 1, 1, "a\nb\n"),
        Arguments.of("no-final-newline", "a\nb", 1, 1, 1, "b"),
        Arguments.of("remove-incomplete-line", "a\nb", 2, 1, 1, "a\n"),
        Arguments.of("crlf", "a\r\nb\r\nc\r\n", 2, 1, 1, "a\r\nc\r\n"),
        Arguments.of("lone-cr", "a\rb\nc\n", 1, 1, 1, "c\n"),
        Arguments.of("non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 2, 1, 1, "caf\u00e9\nzo\u00eb\n"),
        Arguments.of("large-count", "a\nb\n", 2, Integer.MAX_VALUE, 1, "a\n"),
        // Lines 2 to 99999 go.
        Arguments.of("many-lines", numbers(), 2, 99998, 99998, "0\n99999\n"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void remove(String name, String text, int start, int count, int want, String after)
      throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, text, StandardCharsets.UTF_8);
    assertEquals(want, RemoveLines.remove(path, start, count), name);
    assertEquals(after, Files.readString(path, StandardCharsets.UTF_8), name);
    // No temporary file stays in the directory.
    try (Stream<Path> files = Files.list(dir)) {
      assertEquals(1, files.count(), name);
    }
  }

  @Test
  void keepsThePermissions() throws IOException {
    assumeTrue(Files.getFileAttributeView(dir, PosixFileAttributeView.class) != null);
    Path path = dir.resolve("input.txt");
    Files.writeString(path, "a\nb\n");
    Files.setPosixFilePermissions(path, PosixFilePermissions.fromString("rw-r-----"));
    assertEquals(1, RemoveLines.remove(path, 1, 1));
    assertEquals("rw-r-----", PosixFilePermissions.toString(Files.getPosixFilePermissions(path)));
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class, () -> RemoveLines.remove(dir.resolve("missing.txt"), 1, 1));
  }

  @ParameterizedTest(name = "start {0}, count {1}")
  @CsvSource({"0, 1", "1, -1"})
  void badRange(int start, int count) throws IOException {
    Path path = dir.resolve("input.txt");
    Files.writeString(path, "a\nb\n");
    assertThrows(IllegalArgumentException.class, () -> RemoveLines.remove(path, start, count));
    assertArrayEquals("a\nb\n".getBytes(StandardCharsets.UTF_8), Files.readAllBytes(path));
  }
}
