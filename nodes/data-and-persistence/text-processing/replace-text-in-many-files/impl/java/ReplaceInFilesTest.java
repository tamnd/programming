// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

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
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReplaceInFilesTest {
  @TempDir Path dir;

  /** One file: its name, the text before and the text after. */
  record F(String name, String before, String after) {}

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("rosetta", "Goodbye London!", "Hello New York!",
            List.of(new F("a.txt", "Goodbye London!\n", "Hello New York!\n"),
                new F("b.txt", "Goodbye London! Goodbye London!\nGoodbye\n",
                    "Hello New York! Hello New York!\nGoodbye\n")),
            new int[] {1, 2}),
        Arguments.of("some-files-match", "dog", "fox",
            List.of(new F("a.txt", "cat\n", "cat\n"), new F("b.txt", "dog\ncat dog\n", "fox\ncat fox\n")),
            new int[] {0, 2}),
        Arguments.of("one-file", "1", "2", List.of(new F("a.txt", "x = 1\ny = 1\n", "x = 2\ny = 2\n")), new int[] {2}),
        Arguments.of("no-files", "a", "b", List.of(), new int[] {}),
        Arguments.of("no-match", "x", "y", List.of(new F("a.txt", "abc\n", "abc\n")), new int[] {0}),
        Arguments.of("empty-file", "a", "b", List.of(new F("a.txt", "", "")), new int[] {0}),
        Arguments.of("delete", "-", "", List.of(new F("a.txt", "a-b-c\n", "abc\n")), new int[] {2}),
        Arguments.of("whole-file", "abc", "", List.of(new F("a.txt", "abc", "")), new int[] {1}),
        Arguments.of("new-contains-old", "a", "aa", List.of(new F("a.txt", "a a\n", "aa aa\n")), new int[] {2}),
        Arguments.of("overlapping", "aa", "b", List.of(new F("a.txt", "aaa\n", "ba\n")), new int[] {1}),
        Arguments.of("adjacent", "aa", "b", List.of(new F("a.txt", "aaaa\n", "bb\n")), new int[] {2}),
        Arguments.of("special-chars", "1.5", "\\&$1 $& [x]",
            List.of(new F("a.txt", "1.5 and 125\n", "\\&$1 $& [x] and 125\n")), new int[] {1}),
        Arguments.of("glob-chars", "*.txt", "?", List.of(new F("a.txt", "a.txt *.txt\n", "a.txt ?\n")), new int[] {1}),
        Arguments.of("across-lines", "end\nstart", "end start",
            List.of(new F("a.txt", "end\nstart\n", "end start\n")), new int[] {1}),
        Arguments.of("lf-to-crlf", "\n", "\r\n", List.of(new F("a.txt", "a\nb\n", "a\r\nb\r\n")), new int[] {2}),
        Arguments.of("crlf", "a", "c", List.of(new F("a.txt", "a\r\nb\r\n", "c\r\nb\r\n")), new int[] {1}),
        Arguments.of("no-final-newline", "b", "c", List.of(new F("a.txt", "a b", "a c")), new int[] {1}),
        Arguments.of("non-ascii", "\u00e9", "e", List.of(new F("a.txt", "caf\u00e9, cafe\n", "cafe, cafe\n")), new int[] {1}),
        Arguments.of("case-sensitive", "hello", "bye",
            List.of(new F("a.txt", "Hello hello\n", "Hello bye\n")), new int[] {1}));
  }

  private static void write(Path path, String text) throws IOException {
    Files.write(path, text.getBytes(StandardCharsets.UTF_8));
  }

  private static String read(Path path) throws IOException {
    return new String(Files.readAllBytes(path), StandardCharsets.UTF_8);
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void replace(String name, String oldText, String newText, List<F> files, int[] want)
      throws IOException {
    List<Path> paths = new ArrayList<>();
    for (F f : files) {
      paths.add(dir.resolve(f.name()));
      write(paths.getLast(), f.before());
    }
    assertArrayEquals(want, ReplaceInFiles.replace(paths, oldText, newText));
    for (F f : files) {
      assertEquals(f.after(), read(dir.resolve(f.name())));
    }
    // No temporary file stays in the directory.
    try (Stream<Path> s = Files.list(dir)) {
      assertEquals(files.size(), s.count());
    }
  }

  private static String lines(String prefix) {
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < 100000; i++) {
      b.append(prefix).append(i).append('\n');
    }
    return b.toString();
  }

  @Test
  void manyLines() throws IOException {
    Path path = dir.resolve("many.txt");
    write(path, lines("line "));
    assertArrayEquals(new int[] {100000}, ReplaceInFiles.replace(List.of(path), "line ", "L"));
    assertEquals(lines("L"), read(path));
  }

  @Test
  void keepsThePermissions() throws IOException {
    assumeTrue(Files.getFileAttributeView(dir, PosixFileAttributeView.class) != null);
    Path path = dir.resolve("a.txt");
    write(path, "a\n");
    Files.setPosixFilePermissions(path, PosixFilePermissions.fromString("rw-r-----"));
    assertArrayEquals(new int[] {1}, ReplaceInFiles.replace(List.of(path), "a", "b"));
    assertEquals("rw-r-----", PosixFilePermissions.toString(Files.getPosixFilePermissions(path)));
  }

  @Test
  void notFound() {
    assertThrows(NoSuchFileException.class,
        () -> ReplaceInFiles.replace(List.of(dir.resolve("missing.txt")), "a", "b"));
  }

  @Test
  void emptyOld() throws IOException {
    Path path = dir.resolve("a.txt");
    write(path, "a\n");
    assertThrows(IllegalArgumentException.class, () -> ReplaceInFiles.replace(List.of(path), "", "b"));
    assertEquals("a\n", read(path));
  }

  @Test
  void stop() throws IOException {
    write(dir.resolve("a.txt"), "a\n");
    write(dir.resolve("c.txt"), "a\n");
    List<Path> paths = List.of(dir.resolve("a.txt"), dir.resolve("missing.txt"), dir.resolve("c.txt"));
    assertThrows(NoSuchFileException.class, () -> ReplaceInFiles.replace(paths, "a", "b"));
    assertEquals("b\n", read(dir.resolve("a.txt")));
    assertEquals("a\n", read(dir.resolve("c.txt")));
  }
}
