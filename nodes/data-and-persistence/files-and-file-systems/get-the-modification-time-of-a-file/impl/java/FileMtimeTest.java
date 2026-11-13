// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.FileTime;
import java.util.concurrent.TimeUnit;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class FileMtimeTest {
  @TempDir Path dir;

  @ParameterizedTest(name = "{0}")
  @CsvSource({
    "whole-second, a.txt, 1709296496000000000, 1709296496",
    "sub-second, a.txt, 1709296496500000000, 1709296496",
    "epoch, a.txt, 0, 0",
    "before-1970, a.txt, -86400000000000, -86400",
    "before-1970-sub-second, a.txt, -1500000000, -2",
    "after-2038, a.txt, 4102444800000000000, 4102444800",
    "directory, dir, 1709296496000000000, 1709296496",
    "name-with-spaces, my notes.txt, 1709296496000000000, 1709296496",
  })
  void cases(String name, String rel, long ns, long want) throws IOException {
    Path path = dir.resolve(rel);
    if (rel.equals("dir")) {
      Files.createDirectory(path);
    } else {
      Files.writeString(path, "alpha\n");
    }
    setTime(path, ns);
    assertEquals(want, FileMtime.mtime(path), name);
  }

  // On Linux, the JDK 25 that the tests ran on set a time such as -1.5 s to 0, but it
  // read the time back correctly. So when the time that it reads back is not the time
  // that the test asked for, the test sets the time with GNU touch, and it skips the case
  // when touch cannot set the time either.
  private static void setTime(Path path, long ns) throws IOException {
    Files.setLastModifiedTime(path, FileTime.from(ns, TimeUnit.NANOSECONDS));
    if (Files.getLastModifiedTime(path).to(TimeUnit.NANOSECONDS) == ns) {
      return;
    }
    String stamp = "@" + java.math.BigDecimal.valueOf(ns, 9).toPlainString();
    int status;
    try {
      status = new ProcessBuilder("touch", "-d", stamp, path.toString()).inheritIO().start().waitFor();
    } catch (InterruptedException e) {
      Thread.currentThread().interrupt();
      throw new IOException(e);
    }
    assumeTrue(status == 0 && Files.getLastModifiedTime(path).to(TimeUnit.NANOSECONDS) == ns,
        "this system cannot set the time " + stamp);
  }

  @Test
  void errors() {
    assertThrows(NoSuchFileException.class, () -> FileMtime.mtime(dir.resolve("a.txt")));
  }

  @Test
  void symbolicLink() throws IOException {
    Files.writeString(dir.resolve("a.txt"), "alpha\n");
    Files.setLastModifiedTime(dir.resolve("a.txt"), FileTime.from(1709296496, TimeUnit.SECONDS));
    Files.createSymbolicLink(dir.resolve("link"), Path.of("a.txt"));
    assertEquals(1709296496, FileMtime.mtime(dir.resolve("link")));
  }

  @Test
  void writeSetsThePresentTime() throws IOException {
    Path path = dir.resolve("a.txt");
    Files.writeString(path, "alpha\n");
    Files.setLastModifiedTime(path, FileTime.fromMillis(0));
    long before = System.currentTimeMillis() / 1000 - 1;
    Files.writeString(path, "beta\n", StandardOpenOption.APPEND);
    assertTrue(FileMtime.mtime(path) >= before);
  }
}
