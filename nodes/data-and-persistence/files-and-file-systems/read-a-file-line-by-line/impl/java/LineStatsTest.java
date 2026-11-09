// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.MalformedInputException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LineStatsTest {
  @TempDir Path dir;

  private Path write(byte[] data) throws IOException {
    return Files.write(dir.resolve("input.txt"), data);
  }

  private static Arguments text(String name, String data, long lines, long longest) {
    return Arguments.of(name, data.getBytes(StandardCharsets.UTF_8), lines, longest);
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        text("lf", "alpha\nbeta\ngamma\n", 3, 5),
        text("blank-lines", "first\n\n\nlast line\n", 4, 9),
        text("empty", "", 0, 0),
        text("only-newline", "\n", 1, 0),
        text("no-final-newline", "one\ntwo\nthree", 3, 5),
        text("crlf", "one\r\ntwo\r\nthree\r\n", 3, 5),
        text("crlf-no-final-newline", "one\r\ntwo\r\nthree", 3, 5),
        text("mixed-endings", "one\r\ntwo\nthree\r\n", 3, 5),
        text("whitespace", "  two spaces\ttab \n", 1, 17),
        text("backslashes", "C:\\temp\\new\n", 1, 11),
        text("long-line", "x".repeat(1 << 20) + "\nend\n", 2, 1 << 20),
        text("non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", 3, 10));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void lineStats(String name, byte[] data, long lines, long longest) throws IOException {
    assertEquals(new LineStats(lines, longest), LineStats.of(write(data)));
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> LineStats.of(dir.resolve("missing.txt")));
  }

  static Stream<Arguments> invalid() {
    return Stream.of(
        Arguments.of("invalid-byte", new byte[] {'o', 'k', '\n', (byte) 0xff, (byte) 0xfe, '\n'}),
        Arguments.of("truncated-at-end", new byte[] {'o', 'k', '\n', 'c', 'a', 'f', (byte) 0xc3}),
        Arguments.of(
            "encoded-surrogate", new byte[] {'a', (byte) 0xed, (byte) 0xa0, (byte) 0x80, 'b', '\n'}),
        Arguments.of("overlong", new byte[] {(byte) 0xc0, (byte) 0xaf, '\n'}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("invalid")
  void invalidUtf8(String name, byte[] data) throws IOException {
    Path path = write(data);
    assertThrows(MalformedInputException.class, () -> LineStats.of(path));
  }
}
