// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

/** Gets the first lines and the last lines of a text file. */
public final class FirstAndLastLines {
  private FirstAndLastLines() {}

  /** The first lines and the last lines of a file. */
  public record Ends(List<String> first, List<String> last) {}

  /**
   * Returns the first count lines and the last count lines of the UTF-8 file at path. If the file
   * has fewer than count lines, each list has all the lines. A line ends at LF, and the LF is not
   * part of it. A last line with no LF is a line too.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   */
  public static Ends read(Path path, int count) throws IOException {
    byte[] data = Files.readAllBytes(path);
    List<String> lines = new ArrayList<>();
    int start = 0;
    while (start < data.length) {
      int end = start;
      while (end < data.length && data[end] != '\n') {
        end++;
      }
      lines.add(new String(data, start, end - start, StandardCharsets.UTF_8));
      start = end + 1;
    }
    int take = Math.min(count, lines.size());
    // subList gives a view of the list, not a copy. List.copyOf makes a copy,
    // so the result does not keep all the lines of the file in memory.
    return new Ends(
        List.copyOf(lines.subList(0, take)),
        List.copyOf(lines.subList(lines.size() - take, lines.size())));
  }
}
