// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.CharacterCodingException;
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

class WordCountTest {
  @TempDir Path dir;

  private static final int CHUNK = 64 * 1024;

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("simple", "one two three\n", new WordCount(1, 3, 14)),
        Arguments.of("multiple-spaces", "  a   b  \n", new WordCount(1, 2, 10)),
        Arguments.of("tabs-and-lines", "a\tb\nc\n", new WordCount(2, 3, 6)),
        Arguments.of("empty", "", new WordCount(0, 0, 0)),
        Arguments.of("only-white-space", " \n\t\n", new WordCount(2, 0, 4)),
        Arguments.of("no-final-newline", "one two", new WordCount(0, 2, 7)),
        Arguments.of("crlf", "a b\r\nc\r\n", new WordCount(2, 3, 8)),
        Arguments.of("non-ascii", "na\u00efve caf\u00e9\n", new WordCount(1, 2, 13)),
        Arguments.of("unicode-spaces", "a\u00a0b\u3000c\u2028d\n", new WordCount(1, 4, 13)),
        Arguments.of("not-white-space", "a\u200bb\u00adc\ufeffd\n", new WordCount(1, 1, 13)),
        Arguments.of("control-separators", "a\u001cb\u001fc\n", new WordCount(1, 1, 6)),
        Arguments.of("nel", "a\u0085b\n", new WordCount(1, 2, 5)),
        // 100000 lines: more than one buffer of 64 KiB.
        Arguments.of("many-lines", "a b\n".repeat(100000), new WordCount(100000, 200000, 400000)),
        // A long line, and U+3000 at the end of the first 64 KiB.
        Arguments.of("chunk-boundary", "x".repeat(CHUNK - 1) + "\u3000y\n", new WordCount(1, 2, CHUNK + 4)));
  }

  static Stream<Arguments> invalid() {
    return Stream.of(
        Arguments.of("invalid-bytes", new byte[] {'o', 'k', (byte) 0xff, (byte) 0xfe, '\n'}),
        Arguments.of("truncated", new byte[] {'o', 'k', ' ', (byte) 0xe2, (byte) 0x82}),
        Arguments.of("surrogate", new byte[] {'a', (byte) 0xed, (byte) 0xa0, (byte) 0x80, 'b', '\n'}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void counts(String name, String text, WordCount want) throws IOException {
    Path path = Files.write(dir.resolve("input.txt"), text.getBytes(StandardCharsets.UTF_8));
    assertEquals(want, WordCount.of(path));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("invalid")
  void invalidUtf8(String name, byte[] data) throws IOException {
    Path path = Files.write(dir.resolve("input.txt"), data);
    assertThrows(CharacterCodingException.class, () -> WordCount.of(path));
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> WordCount.of(dir.resolve("missing.txt")));
  }
}
