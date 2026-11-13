// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class MakeSymlinkTest {
  @TempDir Path dir;

  @BeforeEach
  void tree() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Files.createDirectory(dir.resolve("sub"));
    Files.writeString(dir.resolve("sub/b.txt"), "beta\n");
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource(
      delimiter = '|',
      value = {
        "file-target|a.txt|link",
        "directory-target|sub|link",
        "link-in-subdirectory|b.txt|sub/link",
        "dangling-target|missing.txt|link",
        "relative-target|a.txt|sub/link",
        "parent-target|../a.txt|sub/link",
        "name-with-spaces|my notes.txt|my link",
        "non-ascii-name|café 日本.txt|lien 日本",
      })
  void cases(String name, String target, String rel) throws IOException {
    Path link = dir.resolve(rel);
    assertEquals(target, MakeSymlink.makeSymlink(target, link));
    assertTrue(Files.isSymbolicLink(link));
  }

  @ParameterizedTest
  @CsvSource({"b.txt,a.txt", "a.txt,sub"})
  void exists(String target, String rel) {
    assertThrows(
        FileAlreadyExistsException.class, () -> MakeSymlink.makeSymlink(target, dir.resolve(rel)));
    assertFalse(Files.isSymbolicLink(dir.resolve(rel)));
  }

  @Test
  void danglingLinkExists() throws IOException {
    Path link = dir.resolve("link");
    Files.createSymbolicLink(link, Path.of("missing.txt"));
    assertFalse(Files.exists(link));
    assertTrue(Files.exists(link, LinkOption.NOFOLLOW_LINKS));
    assertThrows(FileAlreadyExistsException.class, () -> MakeSymlink.makeSymlink("a.txt", link));
    assertEquals("missing.txt", Files.readSymbolicLink(link).toString());
  }

  @Test
  void missingDirectory() {
    assertThrows(
        NoSuchFileException.class,
        () -> MakeSymlink.makeSymlink("a.txt", dir.resolve("nodir/link")));
  }

  @Test
  void linkReadsTarget() throws IOException {
    MakeSymlink.makeSymlink("a.txt", dir.resolve("link"));
    assertEquals("alpha\n", Files.readString(dir.resolve("link")));
    MakeSymlink.makeSymlink("../a.txt", dir.resolve("sub/up"));
    assertEquals("alpha\n", Files.readString(dir.resolve("sub/up")));
  }

  @Test
  void absoluteTarget() throws IOException {
    String target = dir.resolve("a.txt").toString();
    assertEquals(target, MakeSymlink.makeSymlink(target, dir.resolve("link")));
  }
}
