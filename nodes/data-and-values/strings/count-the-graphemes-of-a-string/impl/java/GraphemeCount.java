// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

import java.text.BreakIterator;

final class GraphemeCount {
  private GraphemeCount() {}

  // countGraphemes returns the number of extended grapheme clusters in s. The
  // character instance of BreakIterator finds the grapheme cluster boundaries.
  static int countGraphemes(String s) {
    BreakIterator it = BreakIterator.getCharacterInstance();
    it.setText(s);
    int n = 0;
    it.first();
    while (it.next() != BreakIterator.DONE) {
      n++;
    }
    return n;
  }
}
