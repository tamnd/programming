// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

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

class WordFrequenciesTest {
  @TempDir Path dir;

  private static WordFrequencies.WordCount wc(String word, long count) {
    return new WordFrequencies.WordCount(word, count);
  }

  private static byte[] utf8(String s) {
    return s.getBytes(StandardCharsets.UTF_8);
  }

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("simple", utf8("the cat and the hat\n"), 2, List.of(wc("the", 2), wc("and", 1))),
        Arguments.of("case", utf8("The THE the tHe\n"), 5, List.of(wc("the", 4))),
        Arguments.of("punctuation", utf8("it's well-dressed, isn't it?\n"), 10,
            List.of(wc("it", 2), wc("dressed", 1), wc("isn", 1), wc("s", 1), wc("t", 1), wc("well", 1))),
        Arguments.of("empty", utf8(""), 3, List.of()),
        Arguments.of("no-letters", utf8("123 ... 42\n-- !!\n"), 3, List.of()),
        Arguments.of("ties", utf8("b a c b a c\n"), 2, List.of(wc("a", 2), wc("b", 2))),
        Arguments.of("digits", utf8("abc123def 42 x9\n"), 10, List.of(wc("abc", 1), wc("def", 1), wc("x", 1))),
        Arguments.of("n-zero", utf8("the cat and the hat\n"), 0, List.of()),
        Arguments.of("n-above-count", utf8("x y x\n"), 10, List.of(wc("x", 2), wc("y", 1))),
        Arguments.of("crlf", utf8("one two\r\ntwo\r\n"), 5, List.of(wc("two", 2), wc("one", 1))),
        Arguments.of("non-ascii", utf8("caf\u00e9 na\u00efve cafe\n"), 10,
            List.of(wc("caf", 1), wc("cafe", 1), wc("na", 1), wc("ve", 1))),
        Arguments.of("leading-separators", utf8("  ...hello, world. Hello\n"), 5,
            List.of(wc("hello", 2), wc("world", 1))),
        Arguments.of("no-final-newline", utf8("dog cat dog"), 5, List.of(wc("dog", 2), wc("cat", 1))),
        Arguments.of("not-utf8", new byte[] {'o', 'k', ' ', (byte) 0xff, (byte) 0xfe, ' ', 'o', 'k', '\n'}, 5,
            List.of(wc("ok", 2))),
        Arguments.of("many-lines", utf8("a b a\n".repeat(100000)), 3, List.of(wc("a", 200000), wc("b", 100000))),
        // One line of 100000 words: longer than the buffer of 64 KiB.
        Arguments.of("long-line", utf8("word ".repeat(100000)), 3, List.of(wc("word", 100000))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void wordFrequencies(String name, byte[] data, int n, List<WordFrequencies.WordCount> want)
      throws IOException {
    Path path = dir.resolve(name + ".txt");
    Files.write(path, data);
    assertEquals(want, WordFrequencies.of(path, n));
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class, () -> WordFrequencies.of(dir.resolve("missing.txt"), 3));
  }
}
