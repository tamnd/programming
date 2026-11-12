// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

/** Reads all of a UTF-8 text file in one call. */
public final class ReadText {
  private ReadText() {}

  /**
   * Returns all of the text of a UTF-8 file, with no change to it. Files.readString decodes UTF-8,
   * keeps CR LF and a byte order mark, and throws MalformedInputException on bad bytes.
   */
  public static String readText(Path path) throws IOException {
    return Files.readString(path);
  }
}
