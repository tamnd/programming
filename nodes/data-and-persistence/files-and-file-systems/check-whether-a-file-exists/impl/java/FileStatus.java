// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import java.io.IOException;
import java.nio.file.AccessDeniedException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;

public final class FileStatus {
  private FileStatus() {}

  /**
   * Tells what is at path: "file", "directory", "other", "missing" or "broken-link".
   *
   * <p>Files.exists is not used, because it gives false also when it cannot tell. Here each error
   * other than "no such entry" comes out as an IOException.
   */
  public static String of(Path path) throws IOException {
    BasicFileAttributes attrs;
    try {
      attrs = Files.readAttributes(path, BasicFileAttributes.class); // follows links
    } catch (NoSuchFileException e) {
      return isLink(path) ? "broken-link" : "missing";
    } catch (AccessDeniedException e) {
      throw e;
    } catch (FileSystemException e) {
      // Java has no exception type for ENOTDIR. A path through a regular file
      // has no entry, so look for the nearest parent that is there.
      if (throughAFile(path)) {
        return "missing";
      }
      throw e;
    }
    if (attrs.isRegularFile()) {
      return "file";
    }
    if (attrs.isDirectory()) {
      return "directory";
    }
    return "other";
  }

  private static boolean isLink(Path path) {
    try {
      return Files.readAttributes(path, BasicFileAttributes.class, LinkOption.NOFOLLOW_LINKS)
          .isSymbolicLink();
    } catch (IOException e) {
      return false;
    }
  }

  private static boolean throughAFile(Path path) {
    for (Path p = path.getParent(); p != null; p = p.getParent()) {
      if (Files.exists(p)) {
        return !Files.isDirectory(p);
      }
    }
    return false;
  }
}
