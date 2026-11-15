// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

/** Compares two texts line by line. */
public final class DiffTexts {
  private DiffTexts() {}

  /** Splits text at each LF. A final LF does not start a new line. */
  public static List<String> splitLines(String text) {
    List<String> lines = new ArrayList<>();
    int start = 0;
    while (start < text.length()) {
      int end = text.indexOf('\n', start);
      if (end < 0) {
        end = text.length();
      }
      lines.add(text.substring(start, end));
      start = end + 1;
    }
    return lines;
  }

  /**
   * Returns the edit script that changes the lines of oldText into the lines of newText. Each
   * line of the result is a space and a line that stays, a - and a line of oldText that goes, or
   * a + and a line of newText that comes, with an LF at the end. The script has the smallest
   * number of - and + lines. When two scripts are equally short, a - comes before a +.
   */
  public static String lines(String oldText, String newText) {
    List<String> a = splitLines(oldText);
    List<String> b = splitLines(newText);
    int n = a.size();
    int m = b.size();
    int w = m + 1;
    // lcs[i * w + j] is the length of a longest common subsequence of the lines from a[i] and
    // from b[j]. Row n and column m stay 0.
    int[] lcs = new int[Math.multiplyExact(n + 1, w)];
    for (int i = n - 1; i >= 0; i--) {
      for (int j = m - 1; j >= 0; j--) {
        lcs[i * w + j] =
            a.get(i).equals(b.get(j))
                ? lcs[(i + 1) * w + j + 1] + 1
                : Math.max(lcs[(i + 1) * w + j], lcs[i * w + j + 1]);
      }
    }
    StringBuilder out = new StringBuilder();
    int i = 0;
    int j = 0;
    while (i < n && j < m) {
      if (a.get(i).equals(b.get(j))) {
        out.append(' ').append(a.get(i++)).append('\n');
        j++;
      } else if (lcs[(i + 1) * w + j] >= lcs[i * w + j + 1]) {
        out.append('-').append(a.get(i++)).append('\n');
      } else {
        out.append('+').append(b.get(j++)).append('\n');
      }
    }
    while (i < n) {
      out.append('-').append(a.get(i++)).append('\n');
    }
    while (j < m) {
      out.append('+').append(b.get(j++)).append('\n');
    }
    return out.toString();
  }

  /**
   * Returns {@link #lines} of the two UTF-8 files.
   *
   * @throws java.nio.file.NoSuchFileException if a file does not exist
   * @throws java.nio.charset.MalformedInputException if a file is not valid UTF-8
   */
  public static String files(Path oldPath, Path newPath) throws IOException {
    return lines(Files.readString(oldPath), Files.readString(newPath));
  }
}
