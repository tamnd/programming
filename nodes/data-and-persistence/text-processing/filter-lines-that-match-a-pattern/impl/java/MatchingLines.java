// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

/** Selects the lines of a text file that match a regular expression. */
public final class MatchingLines {
  private MatchingLines() {}

  /**
   * Returns the lines of the UTF-8 file at path that match pattern. A line matches if the pattern
   * matches some part of it. A line ends at LF, and the LF is not part of it. A last line with no
   * LF is a line too.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   * @throws java.util.regex.PatternSyntaxException if the pattern is not valid
   */
  public static List<String> read(Path path, String pattern) throws IOException {
    // Without UNIX_LINES, a CR is a line terminator too: $ matches before a CR
    // at the end of a line, and . does not match a CR.
    Pattern regex = Pattern.compile(pattern, Pattern.UNIX_LINES);
    byte[] data = Files.readAllBytes(path);
    List<String> lines = new ArrayList<>();
    int start = 0;
    while (start < data.length) {
      int end = start;
      while (end < data.length && data[end] != '\n') {
        end++;
      }
      String line = new String(data, start, end - start, StandardCharsets.UTF_8);
      // find looks for a match in some part of the line. matches would need the
      // whole line to match.
      if (regex.matcher(line).find()) {
        lines.add(line);
      }
      start = end + 1;
    }
    return lines;
  }
}
