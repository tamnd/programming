// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.CodingErrorAction;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

/**
 * The lines, words and bytes of a UTF-8 file, as wc counts them. {@code lines} is the number of LF
 * bytes, and a word is a run of one or more code points that are not White_Space.
 */
public record WordCount(long lines, long words, long bytes) {

  /** True for the 25 code points that have the Unicode property White_Space. */
  static boolean isWhiteSpace(int c) {
    return (c >= 0x09 && c <= 0x0d) || c == 0x20 || c == 0x85 || c == 0xa0 || c == 0x1680
        || (c >= 0x2000 && c <= 0x200a) || c == 0x2028 || c == 0x2029 || c == 0x202f
        || c == 0x205f || c == 0x3000;
  }

  /**
   * Counts the lines, words and bytes of the UTF-8 file at path.
   *
   * @throws java.nio.charset.MalformedInputException if the bytes are not valid UTF-8
   * @throws java.nio.file.NoSuchFileException if the file does not exist
   */
  public static WordCount of(Path path) throws IOException {
    long lines = 0, words = 0;
    boolean inWord = false;
    // REPORT makes the decoder throw on bad bytes. The default of InputStreamReader replaces them.
    var decoder =
        StandardCharsets.UTF_8
            .newDecoder()
            .onMalformedInput(CodingErrorAction.REPORT)
            .onUnmappableCharacter(CodingErrorAction.REPORT);
    char[] buf = new char[32 * 1024];
    try (CountingStream in = new CountingStream(Files.newInputStream(path));
        Reader reader = new InputStreamReader(in, decoder)) {
      int n;
      while ((n = reader.read(buf)) != -1) {
        for (int i = 0; i < n; i++) {
          // Each White_Space code point is one char. A surrogate is never white space.
          if (isWhiteSpace(buf[i])) {
            inWord = false;
            if (buf[i] == '\n') {
              lines++;
            }
          } else if (!inWord) {
            inWord = true;
            words++;
          }
        }
      }
      return new WordCount(lines, words, in.count);
    }
  }

  /** Counts the bytes that pass through it. */
  private static final class CountingStream extends FilterInputStream {
    long count;

    CountingStream(InputStream in) {
      super(in);
    }

    @Override
    public int read() throws IOException {
      int b = super.read();
      if (b != -1) {
        count++;
      }
      return b;
    }

    @Override
    public int read(byte[] b, int off, int len) throws IOException {
      int n = super.read(b, off, len);
      if (n > 0) {
        count += n;
      }
      return n;
    }
  }
}
