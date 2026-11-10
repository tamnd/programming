// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

/** Reads the lines of standard input. */
public final class ReadLines {
  private ReadLines() {}

  /** Reads the stream to its end and returns its lines, each without "\n" or "\r\n". */
  public static List<String> readLines(InputStream in) throws IOException {
    // The code does not close the reader, because that also closes the stream.
    BufferedReader reader = new BufferedReader(new InputStreamReader(in, StandardCharsets.UTF_8));
    List<String> lines = new ArrayList<>();
    // readLine returns null at the end of the stream.
    for (String line = reader.readLine(); line != null; line = reader.readLine()) {
      lines.add(line);
    }
    return lines;
  }

  /** Reads standard input to its end and returns its lines. */
  public static List<String> readStdinLines() throws IOException {
    return readLines(System.in);
  }
}
