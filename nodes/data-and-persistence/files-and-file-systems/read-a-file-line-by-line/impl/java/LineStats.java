// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import java.io.IOException;
import java.io.UncheckedIOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.IntSummaryStatistics;
import java.util.stream.Stream;

/** The number of lines of a text file and the number of code points in its longest line. */
public record LineStats(long lines, long longest) {

  /**
   * Reads a UTF-8 text file line by line.
   *
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   * @throws java.nio.charset.MalformedInputException if the bytes are not valid UTF-8
   */
  public static LineStats of(Path path) throws IOException {
    // Files.lines decodes UTF-8 and reads lazily. Close the stream to close the file.
    try (Stream<String> lines = Files.lines(path)) {
      IntSummaryStatistics stats =
          lines.mapToInt(line -> line.codePointCount(0, line.length())).summaryStatistics();
      return new LineStats(stats.getCount(), Math.max(0, stats.getMax()));
    } catch (UncheckedIOException e) {
      // The stream wraps an error of a later read, such as a bad byte sequence.
      throw e.getCause();
    }
  }
}
