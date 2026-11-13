// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

import java.io.IOException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;

final class FileSize {
  private FileSize() {}

  /**
   * Returns the size in bytes of the regular file at path. readAttributes follows a symbolic link,
   * so a link gives the size of its target. A missing file gives NoSuchFileException. Files.size
   * also gives a size for a directory, but it is not the size of a file, so the code throws a
   * FileSystemException.
   */
  static long size(Path path) throws IOException {
    BasicFileAttributes a = Files.readAttributes(path, BasicFileAttributes.class);
    if (a.isDirectory()) {
      throw new FileSystemException(path.toString(), null, "Is a directory");
    }
    return a.size();
  }
}
