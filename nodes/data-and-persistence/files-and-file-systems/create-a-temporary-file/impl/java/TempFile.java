// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;

public final class TempFile {
  private TempFile() {}

  /**
   * Makes a new, empty file in dir whose name starts with prefix, and returns its path.
   *
   * <p>Files.createTempFile makes the file only when no file has the name. Without file
   * attributes, the mode of the file is not given, so the call asks for rw------- itself. The
   * suffix is "", because a null suffix gives the name the end ".tmp".
   */
  public static Path makeTempFile(Path dir, String prefix) throws IOException {
    if (prefix.contains("/")) {
      throw new IllegalArgumentException("invalid prefix: " + prefix);
    }
    return Files.createTempFile(
        dir, prefix, "", PosixFilePermissions.asFileAttribute(PosixFilePermissions.fromString("rw-------")));
  }

  /** Writes data to a new temporary file, reads it back, removes the file, and returns the text. */
  public static String useTempFile(Path dir, String prefix, String data) throws IOException {
    Path path = makeTempFile(dir, prefix);
    try {
      Files.writeString(path, data, StandardCharsets.UTF_8);
      return Files.readString(path, StandardCharsets.UTF_8);
    } finally {
      Files.delete(path);
    }
  }
}
