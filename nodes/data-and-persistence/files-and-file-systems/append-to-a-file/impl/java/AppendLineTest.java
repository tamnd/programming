// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

class AppendLineTest {
  @TempDir Path dir;

  // name, old content or null, line, content after
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("new-file", null, "first", "first\n"),
        Arguments.of("append-existing", "one\n", "two", "one\ntwo\n"),
        Arguments.of("new-file-empty-line", null, "", "\n"),
        Arguments.of("empty-line", "a\n", "", "a\n\n"),
        Arguments.of("empty-file", "", "x", "x\n"),
        Arguments.of("no-final-newline", "one", "two", "onetwo\n"),
        Arguments.of("crlf-file", "one\r\n", "two", "one\r\ntwo\n"),
        Arguments.of("whitespace", "", "  two spaces\ttab ", "  two spaces\ttab \n"),
        Arguments.of("non-ascii", "naïve\n", "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void appends(String name, String old, String line, String want) throws IOException {
    Path path = dir.resolve("log.txt");
    if (old != null) Files.writeString(path, old);
    AppendLine.appendLine(path, line);
    assertEquals(want, new String(Files.readAllBytes(path), StandardCharsets.UTF_8));
  }

  @Test
  void missingDirectory() {
    assertThrows(
        NoSuchFileException.class,
        () -> AppendLine.appendLine(dir.resolve("missing").resolve("log.txt"), "x"));
  }

  @ParameterizedTest
  @ValueSource(strings = {"a\nb", "a\rb"})
  void invalidLine(String line) {
    Path path = dir.resolve("log.txt");
    assertThrows(IllegalArgumentException.class, () -> AppendLine.appendLine(path, line));
    assertFalse(Files.exists(path));
  }

  private static String text(int w, int i) {
    return String.format("writer %d line %03d ", w, i) + "x".repeat(40);
  }

  @Test
  void manyWriters() throws Exception {
    Path path = dir.resolve("log.txt");
    int writers = 8, lines = 250;
    List<Thread> threads = new ArrayList<>();
    List<Throwable> errors = Collections.synchronizedList(new ArrayList<>());
    for (int w = 0; w < writers; w++) {
      int id = w;
      threads.add(
          Thread.ofPlatform()
              .start(
                  () -> {
                    try {
                      for (int i = 0; i < lines; i++) AppendLine.appendLine(path, text(id, i));
                    } catch (Throwable e) {
                      errors.add(e);
                    }
                  }));
    }
    for (Thread t : threads) t.join();
    assertEquals(List.of(), errors);
    List<String> want = new ArrayList<>();
    for (int w = 0; w < writers; w++) for (int i = 0; i < lines; i++) want.add(text(w, i));
    List<String> got = new ArrayList<>(Files.readAllLines(path));
    Collections.sort(want);
    Collections.sort(got);
    assertEquals(want, got);
  }
}
