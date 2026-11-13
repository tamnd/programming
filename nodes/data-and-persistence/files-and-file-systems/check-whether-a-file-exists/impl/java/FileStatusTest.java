// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

import java.io.IOException;
import java.nio.file.AccessDeniedException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class FileStatusTest {
  @TempDir Path dir;

  @BeforeEach
  void tree() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Files.writeString(dir.resolve("empty.txt"), "");
    Files.createDirectory(dir.resolve("dir"));
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource({
    "a.txt, file",
    "empty.txt, file",
    "dir, directory",
    "dir/, directory",
    "missing.txt, missing",
    "no-such-dir/a.txt, missing",
    "a.txt/b.txt, missing",
  })
  void status(String name, String want) throws IOException {
    assertEquals(want, FileStatus.of(dir.resolve(name)));
  }

  @Test
  void links() throws IOException {
    Files.createSymbolicLink(dir.resolve("to-file"), Path.of("a.txt"));
    Files.createSymbolicLink(dir.resolve("to-dir"), Path.of("dir"));
    Files.createSymbolicLink(dir.resolve("to-nothing"), Path.of("missing.txt"));
    assertEquals("file", FileStatus.of(dir.resolve("to-file")));
    assertEquals("directory", FileStatus.of(dir.resolve("to-dir")));
    assertEquals("broken-link", FileStatus.of(dir.resolve("to-nothing")));
  }

  @Test
  void device() throws IOException {
    assumeTrue(Files.exists(Path.of("/dev/null")));
    assertEquals("other", FileStatus.of(Path.of("/dev/null")));
  }

  @Test
  void namedPipe() throws Exception {
    Path pipe = dir.resolve("pipe");
    assertEquals(0, new ProcessBuilder("mkfifo", pipe.toString()).start().waitFor());
    assertEquals("other", FileStatus.of(pipe));
  }

  @Test
  void permissionError() throws IOException {
    assumeFalse("root".equals(System.getProperty("user.name")));
    Path locked = Files.createDirectory(dir.resolve("locked"));
    Files.writeString(locked.resolve("a.txt"), "alpha\n");
    Files.setPosixFilePermissions(locked, PosixFilePermissions.fromString("---------"));
    try {
      assertThrows(AccessDeniedException.class, () -> FileStatus.of(locked.resolve("a.txt")));
    } finally {
      Files.setPosixFilePermissions(locked, PosixFilePermissions.fromString("rwx------"));
    }
  }
}
