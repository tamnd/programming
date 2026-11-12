// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

import java.util.List;

final class Characters {
  private Characters() {}

  // characters returns the code points of s. A char is a UTF-16 code unit, so
  // charAt would give each half of a surrogate pair. codePoints() gives whole
  // code points.
  static List<String> characters(String s) {
    return s.codePoints().mapToObj(Character::toString).toList();
  }
}
