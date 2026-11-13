// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assumptions.assumeFalse;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.AccessDeniedException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class DeleteFileTest {
  @TempDir Path dir;

  private boolean lexists(Path p) {
    return Files.exists(p, LinkOption.NOFOLLOW_LINKS);
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource(
      delimiter = '|',
      nullValues = "NULL",
      emptyValue = "EMPTY",
      value = {
        "delete | a.txt | alpha | true",
        "missing | a.txt | NULL | false",
        "empty-file | a.txt | EMPTY | true",
        "missing-parent | no-such-dir/a.txt | NULL | false",
        "name-with-spaces | my notes.txt | alpha | true",
        "non-ascii-name | café 日本.txt | alpha | true",
      })
  void cases(String name, String rel, String content, boolean want) throws IOException {
    Path path = dir.resolve(rel);
    if (content != null) {
      Files.writeString(path, "EMPTY".equals(content) ? "" : content + "\n");
    }
    assertEquals(want, DeleteFile.delete(path));
    assertFalse(lexists(path));
  }

  @Test
  void throughAFile() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    assertFalse(DeleteFile.delete(dir.resolve("a.txt").resolve("b.txt")));
    assertTrue(Files.exists(dir.resolve("a.txt")));
  }

  @Test
  void directory() throws IOException {
    Files.createDirectory(dir.resolve("dir"));
    assertThrows(FileSystemException.class, () -> DeleteFile.delete(dir.resolve("dir")));
    assertTrue(Files.isDirectory(dir.resolve("dir")));
  }

  @Test
  void links() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Files.createDirectory(dir.resolve("dir"));
    Files.createSymbolicLink(dir.resolve("to-file"), Path.of("a.txt"));
    Files.createSymbolicLink(dir.resolve("to-dir"), Path.of("dir"));
    Files.createSymbolicLink(dir.resolve("broken"), Path.of("missing.txt"));
    for (String link : new String[] {"to-file", "to-dir", "broken"}) {
      assertTrue(DeleteFile.delete(dir.resolve(link)), link);
      assertFalse(lexists(dir.resolve(link)), link);
    }
    assertEquals("alpha\n", Files.readString(dir.resolve("a.txt")));
    assertTrue(Files.isDirectory(dir.resolve("dir")));
  }

  @Test
  void readOnlyFile() throws IOException {
    Path path = dir.resolve("a.txt");
    Files.writeString(path, "alpha\n");
    Files.setPosixFilePermissions(path, PosixFilePermissions.fromString("r--r--r--"));
    assertTrue(DeleteFile.delete(path));
    assertFalse(lexists(path));
  }

  @Test
  void directoryWithNoWritePermission() throws IOException {
    assumeFalse("root".equals(System.getProperty("user.name")));
    Path locked = Files.createDirectory(dir.resolve("locked"));
    Path path = Files.writeString(locked.resolve("a.txt"), "alpha\n");
    Files.setPosixFilePermissions(locked, PosixFilePermissions.fromString("r-x------"));
    try {
      assertThrows(AccessDeniedException.class, () -> DeleteFile.delete(path));
      assertTrue(Files.exists(path));
    } finally {
      Files.setPosixFilePermissions(locked, PosixFilePermissions.fromString("rwx------"));
    }
  }

  @Test
  void openFile() throws IOException {
    Path path = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    try (InputStream in = Files.newInputStream(path)) {
      assertTrue(DeleteFile.delete(path));
      assertFalse(lexists(path));
      // On Unix the data stays until the close.
      assertEquals("alpha\n", new String(in.readAllBytes(), StandardCharsets.UTF_8));
    }
  }
}
