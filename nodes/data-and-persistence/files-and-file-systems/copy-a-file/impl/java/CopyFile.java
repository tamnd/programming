// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

import java.io.IOException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.attribute.PosixFileAttributeView;

public final class CopyFile {
  private CopyFile() {}

  /**
   * Copies the bytes and the permission bits of the file at src to dst, and replaces a file at dst.
   *
   * <p>Files.copy copies a directory as a new empty directory, it replaces an empty directory at
   * dst, and it does nothing when src and dst are the same file. So the method checks these cases
   * first and throws a FileSystemException. With no COPY_ATTRIBUTES, Files.copy does not have to
   * copy the permission bits, so the method sets them after the copy.
   */
  public static void copy(Path src, Path dst) throws IOException {
    BasicFileAttributes s = Files.readAttributes(src, BasicFileAttributes.class);
    if (s.isDirectory()) {
      throw new FileSystemException(src.toString(), null, "Is a directory");
    }
    if (Files.isDirectory(dst)) {
      throw new FileSystemException(dst.toString(), null, "Is a directory");
    }
    if (Files.exists(dst) && Files.isSameFile(src, dst)) {
      throw new FileSystemException(src.toString(), dst.toString(), "Same file");
    }
    Files.copy(src, dst, StandardCopyOption.REPLACE_EXISTING);
    if (Files.getFileStore(dst).supportsFileAttributeView(PosixFileAttributeView.class)) {
      Files.setPosixFilePermissions(dst, Files.getPosixFilePermissions(src));
    }
  }
}
