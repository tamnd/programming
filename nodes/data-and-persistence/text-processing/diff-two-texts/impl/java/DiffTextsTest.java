// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

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

class DiffTextsTest {
  @TempDir Path dir;

  // name, the old text, the new text, the edit script
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("simple", "a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"),
        Arguments.of("change-a-line", "one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n"),
        Arguments.of("identical", "a\nb\n", "a\nb\n", " a\n b\n"),
        Arguments.of("lines-of-a-poem", "The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n"),
        Arguments.of("both-empty", "", "", ""),
        Arguments.of("old-empty", "", "a\nb\n", "+a\n+b\n"),
        Arguments.of("new-empty", "a\nb\n", "", "-a\n-b\n"),
        Arguments.of("all-different", "a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"),
        Arguments.of("insert-at-start", "b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"),
        Arguments.of("delete-at-end", "a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"),
        Arguments.of("swap", "a\nb\n", "b\na\n", "-a\n b\n+a\n"),
        Arguments.of("repeated-lines", "a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n"),
        Arguments.of("empty-line", "a\n\nb\n", "a\nb\n", " a\n-\n b\n"),
        Arguments.of("no-final-newline", "a\nb", "a\nb\n", " a\n b\n"),
        Arguments.of("crlf", "a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"),
        Arguments.of("trailing-space", "a \nb\n", "a\nb\n", "-a \n+a\n b\n"),
        Arguments.of("normalization", "caf\u00e9\n", "cafe\u0301\n", "-caf\u00e9\n+cafe\u0301\n"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void lines(String name, String oldText, String newText, String want) {
    assertEquals(want, DiffTexts.lines(oldText, newText), name);
  }

  @Test
  void manyLines() {
    // 1000 lines, and 10 of them change.
    StringBuilder oldText = new StringBuilder();
    StringBuilder newText = new StringBuilder();
    for (int i = 0; i < 1000; i++) {
      oldText.append(i).append('\n');
      newText.append(i % 100 == 50 ? "x" + i : Integer.toString(i)).append('\n');
    }
    List<String> lines = DiffTexts.splitLines(DiffTexts.lines(oldText.toString(), newText.toString()));
    assertEquals(1010, lines.size());
    assertEquals(List.of(" 49", "-50", "+x50"), lines.subList(49, 52));
    assertEquals(" 999", lines.get(1009));
  }

  @Test
  void files() throws IOException {
    Files.writeString(dir.resolve("old.txt"), "a\r\nb\n", StandardCharsets.UTF_8);
    Files.writeString(dir.resolve("new.txt"), "a\nb\n", StandardCharsets.UTF_8);
    assertEquals("-a\r\n+a\n b\n", DiffTexts.files(dir.resolve("old.txt"), dir.resolve("new.txt")));
  }

  @Test
  void notFound() throws IOException {
    Files.writeString(dir.resolve("new.txt"), "a\n", StandardCharsets.UTF_8);
    assertThrows(
        NoSuchFileException.class,
        () -> DiffTexts.files(dir.resolve("missing.txt"), dir.resolve("new.txt")));
  }
}
