// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

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
import org.junit.jupiter.params.provider.EnumSource;
import org.junit.jupiter.params.provider.MethodSource;

class WriteTextTest {
  @TempDir Path dir;

  static Stream<Arguments> cases() {
    String nonAscii = "naïve café\n日本語\n🙂\n";
    return Stream.of(
        Arguments.of("new-file", "alpha\nbeta\n", "TRUNCATE", null, "alpha\nbeta\n"),
        Arguments.of("append-existing", "two\n", "APPEND", "one\n", "one\ntwo\n"),
        Arguments.of("atomic-replace", "new\n", "ATOMIC", "old\n", "new\n"),
        Arguments.of(
            "truncate-longer", "short\n", "TRUNCATE", "a much longer old line\n", "short\n"),
        Arguments.of("empty-truncate", "", "TRUNCATE", "old\n", ""),
        Arguments.of("empty-new", "", "TRUNCATE", null, ""),
        Arguments.of("append-missing", "first\n", "APPEND", null, "first\n"),
        Arguments.of("append-no-newline", "two", "APPEND", "one", "onetwo"),
        Arguments.of("non-ascii", nonAscii, "TRUNCATE", null, nonAscii),
        Arguments.of("crlf", "one\r\ntwo\r\n", "TRUNCATE", null, "one\r\ntwo\r\n"),
        Arguments.of(
            "mixed-endings", "lf\ncrlf\r\ncr\rend", "TRUNCATE", null, "lf\ncrlf\r\ncr\rend"),
        Arguments.of("backslashes", "C:\\temp\\new\n", "TRUNCATE", null, "C:\\temp\\new\n"),
        Arguments.of("format-chars", "-n 100% %s\n", "TRUNCATE", null, "-n 100% %s\n"),
        Arguments.of("atomic-new", "new\n", "ATOMIC", null, "new\n"),
        Arguments.of("atomic-empty", "", "ATOMIC", "old\n", ""),
        Arguments.of("atomic-non-ascii", nonAscii, "ATOMIC", "old\n", nonAscii));
  }

  private List<String> names() throws IOException {
    try (Stream<Path> files = Files.list(dir)) {
      return files.map(p -> p.getFileName().toString()).toList();
    }
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void writesTheBytes(String name, String text, String mode, String before, String want)
      throws IOException {
    Path path = dir.resolve("out.txt");
    if (before != null) {
      Files.write(path, before.getBytes(StandardCharsets.UTF_8));
    }
    WriteText.writeText(path, text, WriteText.Mode.valueOf(mode));
    // The exact bytes: UTF-8, no byte order mark, no change to the line ends.
    assertArrayEquals(want.getBytes(StandardCharsets.UTF_8), Files.readAllBytes(path), name);
    assertEquals(List.of("out.txt"), names(), name);
  }

  @ParameterizedTest
  @EnumSource(WriteText.Mode.class)
  void missingDirectory(WriteText.Mode mode) throws IOException {
    Path path = dir.resolve("missing").resolve("out.txt");
    assertThrows(NoSuchFileException.class, () -> WriteText.writeText(path, "x\n", mode));
    assertEquals(List.of(), names());
  }

  @Test
  void atomicOntoDirectory() throws IOException {
    Path path = Files.createDirectory(dir.resolve("out.txt"));
    assertThrows(IOException.class, () -> WriteText.writeText(path, "x\n", WriteText.Mode.ATOMIC));
    assertTrue(Files.isDirectory(path));
    assertEquals(List.of("out.txt"), names());
  }
}
