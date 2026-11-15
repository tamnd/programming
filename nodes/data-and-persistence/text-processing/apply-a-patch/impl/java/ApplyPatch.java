// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Applies a unified diff to a text, with no fuzz and no offset. */
public final class ApplyPatch {
  private ApplyPatch() {}

  private static final Pattern HUNK =
      Pattern.compile("@@ -(\\d+)(?:,(\\d+))? \\+(\\d+)(?:,(\\d+))? @@");

  /** The patch is malformed, or a hunk does not match the text. */
  public static final class PatchException extends RuntimeException {
    private static final long serialVersionUID = 1L;

    /** The kinds of the error. */
    public enum Kind {
      MALFORMED,
      MISMATCH
    }

    private final Kind kind;
    private final int line;

    PatchException(Kind kind, int line) {
      super(kind.name().toLowerCase(java.util.Locale.ROOT) + " at line " + line + " of the patch");
      this.kind = kind;
      this.line = line;
    }

    public Kind kind() {
      return kind;
    }

    /** The number of the line of the patch where the error is. */
    public int line() {
      return line;
    }
  }

  private record BodyLine(char mark, String text, int number) {}

  /** Splits text after each LF. Each line keeps its LF, and a last line with no LF is a line too. */
  public static List<String> splitLines(String text) {
    List<String> lines = new ArrayList<>();
    int start = 0;
    while (start < text.length()) {
      int end = text.indexOf('\n', start);
      end = end < 0 ? text.length() : end + 1;
      lines.add(text.substring(start, end));
      start = end;
    }
    return lines;
  }

  private static PatchException malformed(int line) {
    return new PatchException(PatchException.Kind.MALFORMED, line);
  }

  private static PatchException mismatch(int line) {
    return new PatchException(PatchException.Kind.MISMATCH, line);
  }

  private static int count(String s) {
    return s == null ? 1 : Integer.parseInt(s);
  }

  /**
   * Returns the text that the unified diff patch makes from the text oldText. Each hunk must
   * match the lines of oldText at the start of its old range. Throws PatchException if the patch
   * is malformed or a hunk does not match.
   */
  public static String apply(String oldText, String patch) {
    List<String> lines = splitLines(oldText);
    // The lines of the patch, with no LF. A CR stays in its line.
    List<String> p = new ArrayList<>();
    for (String l : splitLines(patch)) {
      p.add(l.endsWith("\n") ? l.substring(0, l.length() - 1) : l);
    }
    StringBuilder out = new StringBuilder();
    int cur = 0; // the index of the next line of oldText to use
    int hunks = 0;
    int i = 0;
    while (i < p.size()) {
      if (!p.get(i).startsWith("@@")) {
        i++; // a header line, or another line outside a hunk
        continue;
      }
      Matcher m = HUNK.matcher(p.get(i));
      if (!m.lookingAt()) {
        throw malformed(i + 1);
      }
      int oldStart;
      int oldCount;
      int newCount;
      try {
        oldStart = Integer.parseInt(m.group(1));
        oldCount = count(m.group(2));
        newCount = count(m.group(4));
      } catch (NumberFormatException e) {
        throw malformed(i + 1);
      }
      hunks++;
      // An empty range starts after the line oldStart.
      int pos = oldCount > 0 ? oldStart - 1 : oldStart;
      if (pos < 0) {
        throw malformed(i + 1);
      }
      if (pos < cur || pos > lines.size()) {
        throw mismatch(i + 1);
      }
      for (; cur < pos; cur++) {
        out.append(lines.get(cur));
      }
      i++;
      List<BodyLine> body = new ArrayList<>();
      // The end of each line: an LF, or "" before a backslash line.
      List<String> ends = new ArrayList<>();
      int oldLeft = oldCount;
      int newLeft = newCount;
      while (oldLeft > 0 || newLeft > 0) {
        if (i == p.size()) {
          throw malformed(i);
        }
        String line = p.get(i);
        char mark = line.isEmpty() ? ' ' : line.charAt(0);
        if (mark == '\\') {
          if (body.isEmpty()) {
            throw malformed(i + 1);
          }
          ends.set(ends.size() - 1, ""); // the line before has no LF
          i++;
          continue;
        }
        if (mark != ' ' && mark != '-' && mark != '+') {
          throw malformed(i + 1);
        }
        if (mark != '+') {
          oldLeft--;
        }
        if (mark != '-') {
          newLeft--;
        }
        if (oldLeft < 0 || newLeft < 0) {
          throw malformed(i + 1);
        }
        body.add(new BodyLine(mark, line.isEmpty() ? "" : line.substring(1), i + 1));
        ends.add("\n");
        i++;
      }
      if (i < p.size() && p.get(i).startsWith("\\") && !body.isEmpty()) {
        ends.set(ends.size() - 1, "");
        i++;
      }
      for (int k = 0; k < body.size(); k++) {
        BodyLine b = body.get(k);
        String line = b.text() + ends.get(k);
        if (b.mark() != '+') {
          if (cur == lines.size() || !lines.get(cur).equals(line)) {
            throw mismatch(b.number());
          }
          cur++;
        }
        if (b.mark() != '-') {
          out.append(line);
        }
      }
    }
    if (hunks == 0) {
      throw malformed(p.size());
    }
    for (; cur < lines.size(); cur++) {
      out.append(lines.get(cur));
    }
    return out.toString();
  }

  /**
   * Returns apply of the two UTF-8 files. Throws NoSuchFileException if a file does not exist,
   * and PatchException.
   */
  public static String applyFile(Path path, Path patchPath) throws IOException {
    return apply(Files.readString(path), Files.readString(patchPath));
  }
}
