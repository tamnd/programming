// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

final class SetMode {
  private SetMode() {}

  /**
   * Sets the mode of the file at path, and returns the new permission bits. mode is three or four
   * octal digits, as for the chmod command. String.matches checks the whole string, so "640\n" is
   * not valid. The PosixFilePermission set has no setuid, setgid or sticky bit, so the code sets
   * the attribute "unix:mode", which takes all twelve bits. It follows a symbolic link, and the
   * umask does not apply. A missing file gives NoSuchFileException.
   */
  static int setMode(Path path, String mode) throws IOException {
    if (mode == null || !mode.matches("[0-7]{3,4}")) {
      throw new IllegalArgumentException("invalid mode: " + mode);
    }
    Files.setAttribute(path, "unix:mode", Integer.parseInt(mode, 8));
    return (Integer) Files.getAttribute(path, "unix:mode") & 07777;
  }
}
