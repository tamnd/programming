// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

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

class ReadTextTest {
  @TempDir Path dir;

  private Path write(byte[] data) throws IOException {
    return Files.write(dir.resolve("input.txt"), data);
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("lf", "alpha\nbeta\n"),
        Arguments.of("paragraphs", "first line\n\nsecond paragraph\n"),
        Arguments.of("non-ascii", "naïve café\n日本語\n🙂\n"),
        Arguments.of("empty", ""),
        Arguments.of("no-final-newline", "one\ntwo"),
        Arguments.of("trailing-newlines", "end\n\n\n"),
        Arguments.of("crlf", "one\r\ntwo\r\n"),
        Arguments.of("mixed-endings", "lf\ncrlf\r\ncr\rend"),
        Arguments.of("bom", "\uFEFFhead\n"),
        Arguments.of("whitespace", "  two spaces\ttab \n"),
        Arguments.of("large", "0123456789abcdef".repeat(65536)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void readsText(String name, String text) throws IOException {
    assertEquals(text, ReadText.readText(write(text.getBytes(StandardCharsets.UTF_8))));
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> ReadText.readText(dir.resolve("missing.txt")));
  }

  @Test
  void directory() {
    assertThrows(IOException.class, () -> ReadText.readText(dir));
  }

  static Stream<Arguments> invalidUtf8() {
    return Stream.of(
        Arguments.of("invalid-byte", new byte[] {'o', 'k', '\n', (byte) 0xff, (byte) 0xfe, '\n'}),
        Arguments.of("truncated-at-end", new byte[] {'o', 'k', '\n', 'c', 'a', 'f', (byte) 0xc3}),
        Arguments.of(
            "encoded-surrogate", new byte[] {'a', (byte) 0xed, (byte) 0xa0, (byte) 0x80, 'b', '\n'}),
        Arguments.of("overlong", new byte[] {(byte) 0xc0, (byte) 0xaf, '\n'}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("invalidUtf8")
  void rejectsInvalidUtf8(String name, byte[] data) {
    assertThrows(MalformedInputException.class, () -> ReadText.readText(write(data)));
  }
}
