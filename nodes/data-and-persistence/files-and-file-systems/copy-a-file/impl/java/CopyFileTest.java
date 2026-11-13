// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class CopyFileTest {
  @TempDir Path dir;

  private static byte[] text(String s) {
    String t = switch (s) {
      case "EMPTY" -> "";
      case "BINARY" -> "\0\r\nÿ\0";
      default -> s + "\n";
    };
    return t.getBytes(StandardCharsets.UTF_8);
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource(
      delimiter = '|',
      nullValues = "NULL",
      value = {
        "copy | a.txt | b.txt | alpha | NULL",
        "replace-existing | a.txt | b.txt | alpha | beta, a longer text",
        "empty-file | a.txt | b.txt | EMPTY | beta",
        "other-directory | a.txt | sub/a.txt | alpha | NULL",
        "name-with-spaces | my notes.txt | copy of my notes.txt | alpha | NULL",
        "non-ascii-name | a.txt | café 日本.txt | alpha | NULL",
        "binary-data | a.bin | b.bin | BINARY | NULL",
      })
  void cases(String name, String srcRel, String dstRel, String srcText, String dstText)
      throws IOException {
    Path src = dir.resolve(srcRel);
    Path dst = dir.resolve(dstRel);
    Files.createDirectories(dst.getParent());
    if (dstText != null) {
      Files.write(dst, text(dstText));
    }
    Files.write(src, text(srcText));
    CopyFile.copy(src, dst);
    assertArrayEquals(text(srcText), Files.readAllBytes(dst), name);
    assertArrayEquals(text(srcText), Files.readAllBytes(src), name);
  }

  @Test
  void missing() throws IOException {
    assertThrows(NoSuchFileException.class, () -> CopyFile.copy(dir.resolve("a.txt"), dir.resolve("b.txt")));
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    assertThrows(
        NoSuchFileException.class,
        () -> CopyFile.copy(dir.resolve("a.txt"), dir.resolve("no-such-dir/b.txt")));
  }

  @Test
  void directories() throws IOException {
    Path sub = Files.createDirectory(dir.resolve("dir"));
    Path src = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    FileSystemException e = assertThrows(FileSystemException.class, () -> CopyFile.copy(sub, dir.resolve("b.txt")));
    assertEquals("Is a directory", e.getReason());
    e = assertThrows(FileSystemException.class, () -> CopyFile.copy(src, sub));
    assertEquals("Is a directory", e.getReason());
    assertTrue(Files.isDirectory(sub));
    try (var entries = Files.list(sub)) {
      assertEquals(0, entries.count());
    }
    assertFalse(Files.exists(dir.resolve("b.txt")));
  }

  @Test
  void sameFile() throws IOException {
    Path src = Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Path link = Files.createLink(dir.resolve("b.txt"), src);
    for (Path dst : new Path[] {src, link}) {
      FileSystemException e = assertThrows(FileSystemException.class, () -> CopyFile.copy(src, dst));
      assertEquals("Same file", e.getReason());
      assertEquals("alpha\n", Files.readString(src));
    }
  }

  @Test
  void mode() throws IOException {
    Path src = Files.writeString(dir.resolve("a.sh"), "echo alpha\n");
    Files.setPosixFilePermissions(src, PosixFilePermissions.fromString("rwxr-x---"));
    CopyFile.copy(src, dir.resolve("b.sh"));
    assertEquals("rwxr-x---", PosixFilePermissions.toString(Files.getPosixFilePermissions(dir.resolve("b.sh"))));
    Path other = Files.writeString(dir.resolve("c.sh"), "beta\n");
    Files.setPosixFilePermissions(other, PosixFilePermissions.fromString("rw-r-----"));
    CopyFile.copy(src, other);
    assertEquals("rwxr-x---", PosixFilePermissions.toString(Files.getPosixFilePermissions(other)));
  }

  @Test
  void largeFile() throws IOException {
    byte[] data = new byte[5_000_003];
    for (int i = 0; i < data.length; i++) {
      data[i] = (byte) ((long) i * 7 % 251);
    }
    Path src = Files.write(dir.resolve("big.bin"), data);
    CopyFile.copy(src, dir.resolve("copy.bin"));
    assertArrayEquals(data, Files.readAllBytes(dir.resolve("copy.bin")));
  }

  @Test
  void linkAtSrc() throws IOException {
    Files.writeString(dir.resolve("target.txt"), "alpha\n");
    Path link = Files.createSymbolicLink(dir.resolve("link.txt"), Path.of("target.txt"));
    CopyFile.copy(link, dir.resolve("b.txt"));
    assertFalse(Files.isSymbolicLink(dir.resolve("b.txt")));
    assertEquals("alpha\n", Files.readString(dir.resolve("b.txt")));
  }
}
