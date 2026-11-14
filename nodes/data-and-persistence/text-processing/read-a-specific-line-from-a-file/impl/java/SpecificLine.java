// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;

/** Reads one line of a text file by its number. */
public final class SpecificLine {
  private SpecificLine() {}

  /**
   * Returns line number of the UTF-8 file at path, or an empty Optional if there is no such line.
   * Line 1 is the first line. A line ends at LF, and the LF is not part of it. A last line with no
   * LF is a line too. The method reads the file only up to the line.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   */
  public static Optional<String> read(Path path, int number) throws IOException {
    // The method reads bytes, not BufferedReader.readLine, because readLine
    // also ends a line at a CR.
    try (InputStream in = new BufferedInputStream(Files.newInputStream(path))) {
      if (number < 1) {
        return Optional.empty();
      }
      ByteArrayOutputStream line = new ByteArrayOutputStream();
      int n = 1;
      int b;
      while ((b = in.read()) != -1) {
        if (b != '\n') {
          if (n == number) {
            line.write(b);
          }
        } else if (n == number) {
          return Optional.of(line.toString(StandardCharsets.UTF_8));
        } else {
          n++;
        }
      }
      // A last line with no LF ends at the end of the file.
      if (n == number && line.size() > 0) {
        return Optional.of(line.toString(StandardCharsets.UTF_8));
      }
      return Optional.empty();
    }
  }
}
