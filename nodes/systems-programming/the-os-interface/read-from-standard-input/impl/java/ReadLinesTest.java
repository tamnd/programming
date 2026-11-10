// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReadLinesTest {
  private static Arguments text(String name, String data, String... want) {
    return Arguments.of(name, data.getBytes(StandardCharsets.UTF_8), List.of(want));
  }

  static Stream<Arguments> cases() {
    String longLine = "x".repeat(1 << 20);
    return Stream.of(
        text("lf", "alpha\nbeta\ngamma\n", "alpha", "beta", "gamma"),
        text("blank-lines", "first\n\n\nlast line\n", "first", "", "", "last line"),
        text("empty", ""),
        text("only-newline", "\n", ""),
        text("no-final-newline", "one\ntwo\nthree", "one", "two", "three"),
        text("crlf", "one\r\ntwo\r\nthree\r\n", "one", "two", "three"),
        text("crlf-no-final-newline", "one\r\ntwo\r\nthree", "one", "two", "three"),
        text("mixed-endings", "one\r\ntwo\nthree\r\n", "one", "two", "three"),
        text("trailing-blank-line", "last\n\n", "last", ""),
        text("whitespace", "  two spaces\ttab \n", "  two spaces\ttab "),
        text("backslashes", "C:\\temp\\new\n", "C:\\temp\\new"),
        text(
            "non-ascii",
            "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n",
            "naïve café",
            "日本語のテキスト",
            "🙂🙂🙂🙂🙂🙂"),
        text("long-line", longLine + "\nend\n", longLine, "end"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void readsStandardInput(String name, byte[] data, List<String> want) throws IOException {
    InputStream saved = System.in;
    System.setIn(new ByteArrayInputStream(data));
    try {
      assertEquals(want, ReadLines.readStdinLines(), name);
    } finally {
      System.setIn(saved);
    }
  }
}
