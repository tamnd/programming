// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

/** Removes the duplicate lines of a text file. */
public final class UniqueLines {
  private UniqueLines() {}

  /**
   * Returns the lines of the UTF-8 file at path, with only the first copy of each line, in the
   * order of the file. A line ends at LF, and the LF is not part of it. A last line with no LF is
   * a line too.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   */
  public static List<String> read(Path path) throws IOException {
    byte[] data = Files.readAllBytes(path);
    // A LinkedHashSet keeps the order in which the lines were added first. If
    // a line is added again, the set does not change.
    Set<String> lines = new LinkedHashSet<>();
    int start = 0;
    while (start < data.length) {
      int end = start;
      while (end < data.length && data[end] != '\n') {
        end++;
      }
      lines.add(new String(data, start, end - start, StandardCharsets.UTF_8));
      start = end + 1;
    }
    return new ArrayList<>(lines);
  }
}
