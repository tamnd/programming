// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
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

class LineCountTest {
  @TempDir Path dir;

  private static Arguments text(String name, String data, long newlines, long lines) {
    return Arguments.of(name, data.getBytes(StandardCharsets.UTF_8), newlines, lines);
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        text("lf", "alpha\nbeta\ngamma\n", 3, 3),
        text("blank-lines", "first\n\n\nlast\n", 4, 4),
        text("empty", "", 0, 0),
        text("only-newline", "\n", 1, 1),
        text("no-final-newline", "one\ntwo\nthree", 2, 3),
        text("one-line-no-newline", "x", 0, 1),
        text("crlf", "one\r\ntwo\r\n", 2, 2),
        text("crlf-no-final-newline", "one\r\ntwo", 1, 2),
        text("cr-only", "one\rtwo\r", 0, 1),
        text("other-breaks", "a\u000bb\fc\u0085d\u2028e\n", 1, 1),
        Arguments.of("invalid-utf8", new byte[] {'o', 'k', '\n', (byte) 0xff, (byte) 0xfe, '\n'}, 2L, 2L),
        text("nul-bytes", "a\0b\n\0\n", 2, 2),
        // 100000 lines and a last line with no LF: more than one buffer of 64 KiB.
        text("many-chunks", "x\n".repeat(100000) + "end", 100000, 100001));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countLines(String name, byte[] data, long newlines, long lines) throws IOException {
    Path path = Files.write(dir.resolve("input.txt"), data);
    assertEquals(new LineCount(newlines, lines), LineCount.of(path));
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> LineCount.of(dir.resolve("missing.txt")));
  }
}
