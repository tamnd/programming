// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

import java.nio.charset.StandardCharsets;

/** Finds the lengths of a string in UTF-8 bytes, in UTF-16 code units and in code points. */
public final class StringLength {
  private StringLength() {}

  /** Returns the length of s in UTF-8 bytes, in UTF-16 code units and in code points. */
  public static int[] lengths(String s) {
    // A String is a sequence of UTF-16 code units, and length() counts them.
    return new int[] {
      s.getBytes(StandardCharsets.UTF_8).length, s.length(), s.codePointCount(0, s.length())
    };
  }
}
