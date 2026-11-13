// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileSystemException;
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

class WriteSyncedTest {
  @TempDir Path dir;

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("new-file", "alpha\nbeta\n", null),
        Arguments.of("replace-longer", "short\n", "a much longer old line\n"),
        Arguments.of("many-lines", "one\ntwo\nthree\nfour\n", null),
        Arguments.of("empty-replace", "", "old\n"),
        Arguments.of("empty-new", "", null),
        Arguments.of("no-final-newline", "last line", null),
        Arguments.of("non-ascii", "naïve café\n日本語\n🙂\n", null),
        Arguments.of("crlf", "one\r\ntwo\r\n", null));
  }

  private List<String> names() throws IOException {
    try (Stream<Path> s = Files.list(dir)) {
      return s.map(p -> p.getFileName().toString()).sorted().toList();
    }
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void writes(String name, String text, String before) throws IOException {
    Path path = dir.resolve("out.txt");
    if (before != null) {
      Files.write(path, before.getBytes(StandardCharsets.UTF_8));
    }
    WriteSynced.writeSynced(path, text);
    assertArrayEquals(text.getBytes(StandardCharsets.UTF_8), Files.readAllBytes(path), name);
    assertEquals(List.of("out.txt"), names());
  }

  @Test
  void syncsADirectory() throws IOException {
    WriteSynced.syncDirectory(dir);
  }

  @Test
  void missingDirectory() throws IOException {
    assertThrows(
        NoSuchFileException.class,
        () -> WriteSynced.writeSynced(dir.resolve("missing").resolve("out.txt"), "x\n"));
    assertEquals(List.of(), names());
  }

  @Test
  void ontoDirectory() throws IOException {
    Path path = dir.resolve("out.txt");
    Files.createDirectory(path);
    assertThrows(FileSystemException.class, () -> WriteSynced.writeSynced(path, "x\n"));
    assertTrue(Files.isDirectory(path));
  }
}
