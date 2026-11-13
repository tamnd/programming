// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.FileTime;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class WatchFileTest {
  @TempDir Path dir;

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("create", List.of("create:hello\n"), List.of("created")),
        Arguments.of(
            "create-then-write",
            List.of("create:a\n", "write:a much longer line\n"),
            List.of("created", "modified")),
        Arguments.of(
            "create-then-delete", List.of("create:a\n", "delete"), List.of("created", "deleted")),
        Arguments.of(
            "append",
            List.of("create:a\n", "append:b\n", "append:c\n"),
            List.of("created", "modified", "modified")),
        Arguments.of(
            "no-change", List.of("create:a\n", "none", "none"), List.of("created", "none", "none")),
        Arguments.of(
            "same-size", List.of("create:abc\n", "write:xyz\n"), List.of("created", "modified")),
        Arguments.of("touch-only", List.of("create:a\n", "touch"), List.of("created", "modified")),
        Arguments.of(
            "replace-by-rename",
            List.of("create:old\n", "replace:new\n"),
            List.of("created", "modified")),
        Arguments.of(
            "delete-and-create",
            List.of("create:a\n", "recreate:b\n"),
            List.of("created", "modified")),
        Arguments.of("never-created", List.of("none"), List.of("none")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void cases(String name, List<String> steps, List<String> expect) throws Exception {
    assertEquals(expect, WatchFile.watchChanges(dir.resolve("watched.txt"), steps));
  }

  @Test
  void directoryIsAnError() {
    assertThrows(
        FileSystemException.class,
        () -> WatchFile.watchChanges(dir.resolve("watched.txt"), List.of("mkdir")));
  }

  @Test
  void sameSizeAndSameTimeHideAChange() throws Exception {
    Path p = dir.resolve("watched.txt");
    FileTime t = FileTime.from(5, TimeUnit.SECONDS);
    Files.writeString(p, "abc\n");
    Files.setLastModifiedTime(p, t);
    WatchFile.FileWatcher watcher = new WatchFile.FileWatcher(p);
    Files.writeString(p, "xyz\n");
    Files.setLastModifiedTime(p, t);
    assertEquals("none", watcher.poll());
    Files.writeString(p, "xyz!\n");
    assertEquals("modified", watcher.poll());
  }

  @Test
  void watchCallsOnceForABurst() throws Exception {
    Path p = dir.resolve("watched.txt");
    List<Path> calls = new CopyOnWriteArrayList<>();
    AtomicBoolean done = new AtomicBoolean();
    Thread t =
        new Thread(
            () -> {
              try {
                WatchFile.watch(p, calls::add, 10, 500, done::get);
              } catch (Exception e) {
                throw new RuntimeException(e);
              }
            });
    t.start();
    for (int i = 1; i <= 5; i++) {
      Files.writeString(p, "x".repeat(i));
      Thread.sleep(20);
    }
    long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
    while (calls.isEmpty() && System.nanoTime() < deadline) {
      Thread.sleep(10);
    }
    Thread.sleep(600);
    done.set(true);
    t.join();
    assertEquals(List.of(p), calls);
  }
}
