// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.channels.FileLock;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.concurrent.TimeUnit;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class LockFileTest {
  @TempDir Path dir;

  private Path data() throws IOException {
    Path path = dir.resolve("data.txt");
    Files.writeString(path, "alpha\n");
    return path;
  }

  /** Java locks belong to the JVM, so the held lock is in a child JVM. */
  private static Process hold(Path path, String mode) throws Exception {
    String java = Path.of(System.getProperty("java.home"), "bin", "java").toString();
    // The directory or jar that holds LockFile.class, for the class path of the child.
    String classes =
        Path.of(LockFile.class.getProtectionDomain().getCodeSource().getLocation().toURI())
            .toString();
    Process p =
        new ProcessBuilder(java, "-cp", classes, "LockFile", "hold", path.toString(), mode)
            .redirectError(ProcessBuilder.Redirect.INHERIT)
            .start();
    BufferedReader out =
        new BufferedReader(new InputStreamReader(p.getInputStream(), StandardCharsets.UTF_8));
    assertEquals("locked", out.readLine());
    return p;
  }

  private static void stop(Process p) throws Exception {
    p.getOutputStream().close();
    p.waitFor(30, TimeUnit.SECONDS);
  }

  @ParameterizedTest(name = "{0}")
  @CsvSource({
    "no-lock-exclusive, none, exclusive, true",
    "no-lock-shared, none, shared, true",
    "exclusive-with-exclusive, exclusive, exclusive, false",
    "shared-with-shared, shared, shared, true",
    "shared-with-exclusive, shared, exclusive, false",
    "exclusive-with-shared, exclusive, shared, false",
  })
  void tryLock(String name, String held, String want, boolean expect) throws Exception {
    Path path = data();
    Process holder = held.equals("none") ? null : hold(path, held);
    try {
      FileLock lock = LockFile.tryLockFile(path, want.equals("shared"));
      assertEquals(expect, lock != null, name);
      if (lock != null) {
        LockFile.release(lock);
      }
    } finally {
      if (holder != null) {
        stop(holder);
      }
    }
    assertEquals("alpha\n", Files.readString(path));
  }

  @Test
  void releaseFreesTheLock() throws Exception {
    Path path = data();
    FileLock lock = LockFile.tryLockFile(path, false);
    assertNotNull(lock);
    LockFile.release(lock);
    Process holder = hold(path, "exclusive");
    try {
      assertNull(LockFile.tryLockFile(path, false));
    } finally {
      stop(holder);
    }
    FileLock again = LockFile.tryLockFile(path, false);
    assertNotNull(again);
    LockFile.release(again);
  }

  @Test
  void sameJvmGivesNull() throws IOException {
    Path path = data();
    FileLock first = LockFile.tryLockFile(path, true);
    assertNotNull(first);
    // Two shared locks of one JVM overlap, so the second call gets no lock.
    assertNull(LockFile.tryLockFile(path, true));
    LockFile.release(first);
  }

  @Test
  void missingFile() {
    assertThrows(NoSuchFileException.class, () -> LockFile.tryLockFile(dir.resolve("missing.txt"), false));
  }
}
