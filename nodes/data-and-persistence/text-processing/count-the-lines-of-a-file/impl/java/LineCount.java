// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;

/**
 * The lines of a file. {@code newlines} is the number of LF bytes, the number that {@code wc -l}
 * prints. {@code lines} also counts a last line that has no LF.
 */
public record LineCount(long newlines, long lines) {

  /**
   * Counts the lines of the file at path. It reads the bytes and does not decode them.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   * @throws IOException if the file cannot be read
   */
  public static LineCount of(Path path) throws IOException {
    byte[] buf = new byte[64 * 1024];
    long newlines = 0;
    byte last = '\n'; // an empty file has no line without an LF
    try (InputStream in = Files.newInputStream(path)) {
      int n;
      while ((n = in.read(buf)) != -1) {
        for (int i = 0; i < n; i++) {
          if (buf[i] == '\n') {
            newlines++;
          }
        }
        if (n > 0) {
          last = buf[n - 1];
        }
      }
    }
    return new LineCount(newlines, newlines + (last != '\n' ? 1 : 0));
  }
}
