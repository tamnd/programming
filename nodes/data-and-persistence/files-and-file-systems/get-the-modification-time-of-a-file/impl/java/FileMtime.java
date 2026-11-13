// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

final class FileMtime {
  private FileMtime() {}

  /**
   * Returns the modification time of the file at path, in whole seconds since
   * 1970-01-01T00:00:00Z. getLastModifiedTime follows a symbolic link. Instant.getEpochSecond gives
   * the floor, also before 1970. A missing file gives NoSuchFileException.
   */
  static long mtime(Path path) throws IOException {
    return Files.getLastModifiedTime(path).toInstant().getEpochSecond();
  }
}
