// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/** Sorts the lines of a text file. */
public final class SortedLines {
  private SortedLines() {}

  /**
   * Returns the lines of the UTF-8 file at path, sorted by code point. A line ends at LF, and the
   * LF is not part of it. A last line with no LF is a line too.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   */
  public static List<String> read(Path path) throws IOException {
    byte[] data = Files.readAllBytes(path);
    List<byte[]> lines = new ArrayList<>();
    int start = 0;
    while (start < data.length) {
      int end = start;
      while (end < data.length && data[end] != '\n') {
        end++;
      }
      lines.add(Arrays.copyOfRange(data, start, end));
      start = end + 1;
    }
    // String.compareTo compares UTF-16 chars, which is not the order of the
    // code points. Arrays.compareUnsigned compares the UTF-8 bytes, which is.
    lines.sort(Arrays::compareUnsigned);
    List<String> result = new ArrayList<>(lines.size());
    for (byte[] line : lines) {
      result.add(new String(line, StandardCharsets.UTF_8));
    }
    return result;
  }
}
