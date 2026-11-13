// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

import java.io.IOException;
import java.nio.file.AccessDeniedException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;

public final class DeleteFile {
  private DeleteFile() {}

  /**
   * Deletes the file at path. Gives true if a file was there, false if not.
   *
   * <p>Files.delete also deletes an empty directory, so the code looks at the entry first, with
   * no link followed, and does not delete a directory. A symbolic link is deleted, not its target.
   * Each error other than "no such entry" comes out as an IOException.
   */
  public static boolean delete(Path path) throws IOException {
    BasicFileAttributes attrs;
    try {
      attrs = Files.readAttributes(path, BasicFileAttributes.class, LinkOption.NOFOLLOW_LINKS);
    } catch (NoSuchFileException e) {
      return false;
    } catch (AccessDeniedException e) {
      throw e;
    } catch (FileSystemException e) {
      // Java has no exception type for ENOTDIR. A path through a regular file
      // has no entry, so look for the nearest parent that is there.
      if (throughAFile(path)) {
        return false;
      }
      throw e;
    }
    if (attrs.isDirectory()) {
      throw new FileSystemException(path.toString(), null, "Is a directory");
    }
    try {
      Files.delete(path);
    } catch (NoSuchFileException e) {
      return false; // another program deleted it first
    }
    return true;
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
