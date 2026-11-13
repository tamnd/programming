// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.AtomicMoveNotSupportedException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class RenameFileTest {
  @TempDir Path dir;

  private static String text(String s) {
    return "EMPTY".equals(s) ? "" : s + "\n";
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource(
      delimiter = '|',
      nullValues = "NULL",
      value = {
        "rename | a.txt | b.txt | alpha | NULL | alpha",
        "replace-existing | a.txt | b.txt | alpha | beta | alpha",
        "same-path | a.txt | a.txt | alpha | alpha | alpha",
        "empty-file | a.txt | b.txt | EMPTY | beta | EMPTY",
        "other-directory | a.txt | sub/a.txt | alpha | NULL | alpha",
        "name-with-spaces | my notes.txt | old notes.txt | alpha | NULL | alpha",
        "non-ascii-name | a.txt | café 日本.txt | alpha | NULL | alpha",
      })
  void cases(String name, String srcRel, String dstRel, String srcText, String dstText, String want)
      throws IOException {
    Path src = dir.resolve(srcRel);
    Path dst = dir.resolve(dstRel);
    Files.createDirectories(dst.getParent());
    if (dstText != null) {
      Files.writeString(dst, text(dstText));
    }
    Files.writeString(src, text(srcText));
    RenameFile.rename(src, dst);
    assertEquals(text(want), Files.readString(dst));
    assertTrue(src.equals(dst) || !Files.exists(src, LinkOption.NOFOLLOW_LINKS));
  }

  @Test
  void missingSrc() {
    assertThrows(NoSuchFileException.class, () -> RenameFile.rename(dir.resolve("a.txt"), dir.resolve("b.txt")));
  }

  @Test
  void missingDstDir() throws IOException {
    Path src = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    assertThrows(NoSuchFileException.class, () -> RenameFile.rename(src, dir.resolve("no-such-dir/b.txt")));
    assertEquals("alpha\n", Files.readString(src));
  }

  @Test
  void dstIsADirectory() throws IOException {
    Path src = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Path sub = Files.createDirectory(dir.resolve("dir"));
    assertThrows(IOException.class, () -> RenameFile.rename(src, sub));
    assertTrue(Files.isDirectory(sub));
    assertEquals("alpha\n", Files.readString(src));
  }

  @Test
  void link() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Files.createSymbolicLink(dir.resolve("link"), Path.of("a.txt"));
    RenameFile.rename(dir.resolve("link"), dir.resolve("new-link"));
    assertEquals(Path.of("a.txt"), Files.readSymbolicLink(dir.resolve("new-link")));
    assertFalse(Files.exists(dir.resolve("link"), LinkOption.NOFOLLOW_LINKS));
  }

  @Test
  void openDstKeepsOldData() throws IOException {
    Path src = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Path dst = Files.writeString(dir.resolve("b.txt"), "beta\n");
    try (InputStream old = Files.newInputStream(dst)) {
      RenameFile.rename(src, dst);
      assertEquals("beta\n", new String(old.readAllBytes(), StandardCharsets.UTF_8));
    }
    assertEquals("alpha\n", Files.readString(dst));
  }

  @Test
  void crossDevice() throws IOException {
    Path shm = Path.of("/dev/shm");
    assumeTrue(Files.isDirectory(shm));
    assumeTrue(!Files.getFileStore(shm).equals(Files.getFileStore(dir)));
    Path src = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Path dst = shm.resolve("rename-file-" + ProcessHandle.current().pid() + ".txt");
    assertThrows(AtomicMoveNotSupportedException.class, () -> RenameFile.rename(src, dst));
    assertEquals("alpha\n", Files.readString(src));
    assertFalse(Files.exists(dst));
  }
}
