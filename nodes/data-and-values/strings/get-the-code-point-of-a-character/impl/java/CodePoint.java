// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

final class CodePoint {
  private CodePoint() {}

  // codePoint returns the code point of the one character in c. charAt(0)
  // would give only the first UTF-16 code unit of a code point above U+FFFF.
  static int codePoint(String c) {
    return c.codePointAt(0);
  }
}
