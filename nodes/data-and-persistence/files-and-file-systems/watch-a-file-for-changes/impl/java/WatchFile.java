// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.attribute.FileTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.TimeUnit;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;

/** Watch a file for changes: poll its size and modification time. */
public final class WatchFile {
  /** Seconds; the cases set the mtime to BASE_MTIME + step. */
  static final long BASE_MTIME = 1_700_000_000L;

  private WatchFile() {}

  /** The size and the mtime of a file. */
  record Signature(long size, FileTime mtime) {}

  /** The signature of the file, or null when it does not exist. */
  static Signature signature(Path path) throws IOException {
    BasicFileAttributes a;
    try {
      a = Files.readAttributes(path, BasicFileAttributes.class);
    } catch (NoSuchFileException e) {
      return null;
    }
    if (a.isDirectory()) {
      throw new FileSystemException(path.toString(), null, "Is a directory");
    }
    return new Signature(a.size(), a.lastModifiedTime());
  }

  /** Remembers the last signature of a file and tells what changed since then. */
  public static final class FileWatcher {
    private final Path path;
    private Signature last;

    public FileWatcher(Path path) throws IOException {
      this.path = path;
      this.last = signature(path);
    }

    /** created, deleted, modified or none, for the time since the last poll. */
    public String poll() throws IOException {
      Signature now = signature(path);
      Signature before = last;
      last = now;
      if (before == null && now == null) {
        return "none";
      }
      if (before == null) {
        return "created";
      }
      if (now == null) {
        return "deleted";
      }
      return Objects.equals(before, now) ? "none" : "modified";
    }
  }

  /** Calls onChange once for each burst of changes, after the file is quiet. */
  public static void watch(
      Path path, Consumer<Path> onChange, long intervalMillis, long quietMillis, BooleanSupplier stop)
      throws IOException, InterruptedException {
    FileWatcher watcher = new FileWatcher(path);
    long pending = -1; // the time of the last change that onChange has not seen
    while (!stop.getAsBoolean()) {
      long now = System.nanoTime();
      if (!watcher.poll().equals("none")) {
        pending = now;
      } else if (pending >= 0 && now - pending >= TimeUnit.MILLISECONDS.toNanos(quietMillis)) {
        pending = -1;
        onChange.accept(path);
      }
      Thread.sleep(intervalMillis);
    }
  }

  /** Makes the change of one step of a case (the entry of the cases uses it). */
  static void applyStep(Path path, String step, int number) throws IOException {
    int colon = step.indexOf(':');
    String kind = colon < 0 ? step : step.substring(0, colon);
    byte[] text = (colon < 0 ? "" : step.substring(colon + 1)).getBytes(StandardCharsets.UTF_8);
    switch (kind) {
      case "create", "write" -> Files.write(path, text);
      case "append" -> Files.write(path, text, StandardOpenOption.APPEND);
      case "replace" -> {
        Path tmp = path.resolveSibling(path.getFileName() + ".tmp");
        Files.write(tmp, text);
        Files.move(tmp, path, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.ATOMIC_MOVE);
      }
      case "recreate" -> {
        Files.delete(path);
        Files.write(path, text);
      }
      case "delete" -> Files.delete(path);
      case "mkdir" -> Files.createDirectory(path);
      case "touch", "none" -> {}
      default -> throw new IllegalArgumentException("unknown step " + step);
    }
    if (!kind.equals("none") && Files.isRegularFile(path)) {
      Files.setLastModifiedTime(path, FileTime.from(BASE_MTIME + number, TimeUnit.SECONDS));
    }
  }

  /** The entry of the cases: polls after each step and returns the results. */
  public static List<String> watchChanges(Path path, List<String> steps) throws IOException {
    FileWatcher watcher = new FileWatcher(path);
    List<String> results = new ArrayList<>();
    for (int i = 0; i < steps.size(); i++) {
      applyStep(path, steps.get(i), i + 1);
      results.add(watcher.poll());
    }
    return results;
  }
}
