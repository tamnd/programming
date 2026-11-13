// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;
import org.junit.jupiter.params.provider.ValueSource;

class SetModeTest {
  @TempDir Path dir;

  private Path newFile(String name) throws IOException {
    Path p = Files.writeString(dir.resolve(name), "alpha\n");
    Files.setPosixFilePermissions(p, PosixFilePermissions.fromString("rw-r--r--"));
    return p;
  }

  private static int mode(Path p) throws IOException {
    return (Integer) Files.getAttribute(p, "unix:mode") & 07777;
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource({
    "owner-read-write-group-read, a.txt, 640, 416",
    "owner-only, a.txt, 600, 384",
    "executable, a.txt, 755, 493",
    "same-mode, a.txt, 644, 420",
    "no-permissions, a.txt, 000, 0",
    "leading-zero, a.txt, 0640, 416",
    "set-user-id, a.txt, 4755, 2541",
    "name-with-spaces, my notes.txt, 640, 416",
    "non-ascii-name, café 日本.txt, 640, 416",
  })
  void cases(String name, String rel, String mode, int result) throws IOException {
    Path p = newFile(rel);
    assertEquals(result, SetMode.setMode(p, mode));
    assertEquals(result, mode(p));
  }

  @ParameterizedTest
  @ValueSource(strings = {"648", "64", "10640", "u+x", "", "640\n"})
  void invalidMode(String mode) throws IOException {
    Path p = newFile("a.txt");
    assertThrows(IllegalArgumentException.class, () -> SetMode.setMode(p, mode));
    assertEquals(0644, mode(p));
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> SetMode.setMode(dir.resolve("missing.txt"), "640"));
  }

  @Test
  void symbolicLinkChangesTarget() throws IOException {
    Path target = newFile("a.txt");
    Path link = Files.createSymbolicLink(dir.resolve("link"), Path.of("a.txt"));
    assertEquals(0600, SetMode.setMode(link, "600"));
    assertEquals(0600, mode(target));
  }

  @Test
  void directory() throws IOException {
    Path d = Files.createDirectory(dir.resolve("dir"));
    assertEquals(0700, SetMode.setMode(d, "700"));
  }
}
