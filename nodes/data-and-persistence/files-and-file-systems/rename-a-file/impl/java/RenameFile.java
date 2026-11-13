// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public final class RenameFile {
  private RenameFile() {}

  /**
   * Gives the file at src the name dst, and replaces a file at dst.
   *
   * <p>ATOMIC_MOVE makes Files.move one rename call. With no ATOMIC_MOVE, Files.move copies the
   * file and deletes the source when the two paths are on different file systems, and a copy is
   * not atomic. With ATOMIC_MOVE, that case gives AtomicMoveNotSupportedException. On Unix the
   * rename replaces a file at dst.
   */
  public static void rename(Path src, Path dst) throws IOException {
    Files.move(src, dst, StandardCopyOption.ATOMIC_MOVE);
  }
}
